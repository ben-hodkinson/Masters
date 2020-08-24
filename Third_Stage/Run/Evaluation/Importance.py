import os
import csv
import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from ROOT import TFile, TTree, TBranch, TCanvas, TPad, gStyle, gFile, gROOT, TStyle, TH1D, TH2D, gDirectory, TProfile, TColor, TGraph, TLegend, THStack, TLatex

import torch
import torch.nn as nn
from torch.utils.data import Dataset, DataLoader

sys.path.append('/home/matthew/Documents/Masters/Met_network/Networks')
import Network as mn

######## The data set class must be defined for PyTorch to be able to read the data ########
class RootDataSet(Dataset):
    def __init__( self, root_folder, data_set_name, n_events = None, real = False ):
        
        self.my_file_name = os.path.join( root_folder, data_set_name )
        self.my_file = TFile.Open(self.my_file_name)
                
        self.my_tree = self.my_file.Get("var_tree")
        self.n_events = n_events if n_events is not None else self.my_tree.GetEntries()

        self.branch_names = [ b.GetName() for b in self.my_tree.GetListOfBranches() ]
        
    def __getitem__(self, idx):
        self.my_tree.GetEntry(idx)
        truth = torch.tensor( [ getattr( self.my_tree, name ) for name in self.branch_names[1:3] ], dtype=torch.float32  )
        recon = torch.tensor( [ getattr( self.my_tree, name ) for name in self.branch_names[4:] ], dtype=torch.float32  )
        return recon, truth
        
    def __len__(self):
        return self.n_events

def main():        
    
    ####### ADJUSTABLE PARAMETERS ########
    network_base_dir = "/home/matthew/Documents/Masters/Met_network/Networks/Saved_Networks/"
    stats_base_dir   = "/home/matthew/Documents/Masters/Met_network/Data/Training/"
    data_base_dir    = "/home/matthew/Documents/Masters/Met_network/Data/Evaluation/"
    
    network_name  = "NonZero_Swish_5x1000_AM_L1_AA_XY_5e-06_Drop2"
    activated_ann = mn.AMANNDA( "Swish", 5, 1000, 0.2, "XY", False ) # ( act, depth, width, dropout_p, out_type, btchnorm )
    stats_file    = "nonzero_data_stats.csv"
    
    data_set_name = "combined_evaluation.root"
        
    real = 0
    n_events = 1         # None = all
    mini_batch_size = 1
    loss_fn = nn.MSELoss( reduction = "mean" )
    
    output_dir = "Plots/{}/{}".format(network_name,data_set_name[:-5])
    ######################################


    ########## The Neural Network is Loaded ##########
    activated_ann.load_state_dict(torch.load( network_base_dir + network_name + "/network_model_optimal" ) )
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    activated_ann = activated_ann.to(device)
    activated_ann.eval()


    ########## Loading normalisation data ##########
    stats_data = pd.read_csv( os.path.join( stats_base_dir, stats_file ), header = None ) 
    means  = torch.as_tensor( stats_data.iloc[1, 4:].values.astype(np.float32) )
    stdevs = torch.as_tensor( stats_data.iloc[2, 4:].values.astype(np.float32) )
    means  = means.to(device)
    stdevs = stdevs.to(device)
    

    ########## Getting data from the files ##########
    data_set    = RootDataSet( data_base_dir, data_set_name, n_events, real  )
    data_loader = DataLoader( data_set,  batch_size = mini_batch_size, num_workers = 1 )


    ######## Creating gradient containers which will convey importance ########
    average_gradients = torch.zeros( 63 )
    average_gradients = average_gradients.to( device )
    
    for batch_idx, (recon, truth) in enumerate( data_loader ):
    
        ## We first pass the batch onto the GPU
        recon = recon.to( device )
        truth = truth.to( device )
    
        ## The batch is then normalised together
        norm_recon = torch.div( ( recon - means ), stdevs )
        norm_recon.requires_grad_(True)
    
        ## The batch is passed through the network
        output = activated_ann( norm_recon )
        dummy = torch.zeros( output.size(), device = device )
    
        ## We split it into x and y values
        output_split = output.split( 1, dim=1 )
        output_x_values = output_split[0]
        output_y_values = output_split[1]
        
        truth_split = dummy.split( 1, dim=1 )
        truth_x_values = truth_split[0]
        truth_y_values = truth_split[1]
    
        ## Now we calculate the x gradients
        x_loss = loss_fn( output_x_values, truth_x_values )
        x_loss.backward( retain_graph=True )
        x_gradients = norm_recon.grad.abs()
        x_batch_gradients = x_gradients.mean(0)
        average_gradients += x_batch_gradients
        norm_recon.grad.data.zero_()
        
        ## Now we calculate the y gradients
        y_loss = loss_fn( output_y_values, truth_y_values )
        y_loss.backward( retain_graph=True )
        y_gradients = norm_recon.grad.abs()
        y_batch_gradients = y_gradients.mean(0)
        average_gradients += y_batch_gradients
        norm_recon.grad.data.zero_()
    
        print("Completed {}/{} \r".format(batch_idx, len(data_loader)), end="" )
        sys.stdout.flush()
    print("Completed {}/{} \n".format(len(data_loader), len(data_loader)) )
    
    ## Copying data back to cpu
    average_gradients = average_gradients.cpu().numpy()
    average_gradients = average_gradients / average_gradients.max()
    
    ## Calculating the most important names
    indicies = average_gradients.argsort()[::-1]
    recon_names = data_set.branch_names[4:]
    ordered_names = [ recon_names[i] for i in indicies ]
    ordered_gradients = [ average_gradients[i] for i in indicies ]
    
    ## Plotting the bar graph of the top ten
    fig = plt.figure( figsize = (6,6) )
    ax_imprt = fig.add_subplot( 1 , 1 , 1 )
    
    number_to_show = 10
    x = np.arange(number_to_show)[::-1]
    
    ax_imprt.set_xlabel('Relative Importance')
    ax_imprt.barh( x, ordered_gradients[:number_to_show] )
    ax_imprt.set_yticks( x )
    ax_imprt.set_yticklabels( ordered_names[:number_to_show] )
        
    if not os.path.exists(output_dir):
        os.system("mkdir -p " + output_dir)
        
    output_file = "{}/Importance_def.svg".format(output_dir)
    plt.title("Top 10 Variables")
    plt.tight_layout()
    plt.savefig(output_file)
    
    


if __name__ == '__main__':
    main()






