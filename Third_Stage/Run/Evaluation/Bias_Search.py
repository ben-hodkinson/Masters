import os
import csv
import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

import graphviz
from sklearn import tree

from ROOT import TFile, TTree, TBranch, TCanvas, TPad, gStyle, gFile, gROOT, TStyle, TH1D, TH2D, gDirectory, TProfile, TColor, TGraph, TLegend, THStack, TLatex

######## The data set class must be defined for PyTorch to be able to read the data ########
class RootDataSet(object):
    def __init__( self, root_folder, data_set_name, ann_tree_name, n_events = None, real = False ):
        
        self.my_file_name = os.path.join( root_folder, data_set_name )
        self.my_file = TFile.Open(self.my_file_name)
                
        self.my_tree = self.my_file.Get("var_tree")
        self.ann_tree = self.my_file.Get(ann_tree_name)
        
        self.branch_names = [ b.GetName() for b in self.my_tree.GetListOfBranches() ][:67]
        self.ann_names    = [ b.GetName() for b in self.ann_tree.GetListOfBranches() ]
                
        self.n_events = n_events if n_events is not None else self.my_tree.GetEntries()
        
    def __getitem__(self, idx):
        self.my_tree.GetEntry(idx)
        self.ann_tree.GetEntry(idx)
        
        truth   = np.array( [ getattr( self.my_tree, name ) for name in self.branch_names[1:3] ] )
        recon   = np.array( [ getattr( self.my_tree, name ) for name in self.branch_names[4:67] ] )
        ann_met = np.array( [ getattr( self.ann_tree, name ) for name in self.ann_names[1:] ] )
        
        return recon, truth, ann_met
        
    def __len__(self):
        return self.n_events
        
class WorkingP(object):
    def __init__(self, name, branch, tree):
        
        self.name = name
        self.tree = tree
        
        self.Et = branch + "_ET"
        self.Ex = branch + "_X"
        self.Ey = branch + "_Y"

def main():     
       
    ####### ADJUSTABLE PARAMETERS ########
    data_base_dir = "/home/matthew/Documents/Masters/Met_network/Data/Evaluation/"
    data_set_name = "combined_evaluation.root"
    
    network_name =  "Final" # All, NonZero
    ann_tree_name = "final_ann_tree" # ann_tree, ann_tree_Non0

    real = 0
    n_events = None     # None = all
    ######################################

    ########## Getting data from the files ##########
    data_set = RootDataSet( data_base_dir, data_set_name, ann_tree_name, n_events, real  )
    n_events = len(data_set)

    ########## File management ##########
    
    ## Open the root file
    root_file = TFile.Open(os.path.join(data_base_dir, data_set_name))
    if root_file.IsZombie():
        print( "\n\n\nWarning: Cant open the file --- {}\n\n\n".format(root_file) )

    ## Generating the matrices
    features = []
    classes = []
    
    ## Now we need to go through the root file,     
    frac_last = 0             
    for idx in range(n_events):
        recon, truth, ann_met = data_set[idx]
        
        ## Add the recon data to the matrix
        features.append( recon.tolist() )
                        
        ## Now we need to calculate the euclidean distance truth and the recs
        tight_met = np.array( [ recon[0], 0.0 ] )
        ann_dist = np.linalg.norm( ann_met   - truth )
        rec_dist = np.linalg.norm( tight_met - truth )
        
        cat = 0
        if ann_dist < rec_dist:
            cat = 1
            
        classes.append( cat )
        
        # Printing out progress 
        if int(100*idx/n_events)>frac_last:
            frac_last = int(100*idx/n_events);
            print(" -- {}/{} -- {}% processed \r".format( idx, n_events, frac_last ), end="" )
            sys.stdout.flush()
    print(" -- {}/{} -- 100% processed ".format( idx+1, n_events ) )
    
    print("")
    print( "Accuracy: {}/{}".format( sum(classes), len(classes) ) ) 
    
    ## Growing the desicion tree
    
    dec_tree = tree.DecisionTreeClassifier( min_samples_leaf = int(n_events/20) )
    dec_tree.fit(features, classes )
    
    dot_data = tree.export_graphviz(dec_tree, out_file = None, feature_names = data_set.branch_names[4:], filled = True, rounded = True, proportion = True) 
    graph = graphviz.Source(dot_data) 
    
    
    OUTPUT_dir = os.path.join( os.environ["HOME_DIRECTORY"], "Output" )
    output_dir = os.path.join( OUTPUT_dir, network_name, data_set_name[:-5] )
    ## Check that the file can be saved
    if not os.path.exists(output_dir):
        os.system("mkdir -p " + output_dir)
    
    graph.render( filename = "tree", directory=output_dir, cleanup=True) 
    


if __name__ == '__main__':
    main()






