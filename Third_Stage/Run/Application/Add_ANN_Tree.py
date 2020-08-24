import os
import csv
import sys
import time
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from collections import OrderedDict
from array import array

from ROOT import ROOT, TFile, TTree, TBranch, TCanvas, TPad, gStyle, gFile, gROOT, TStyle, TH1D, TH2D, gDirectory, TProfile, TColor, TGraph, TLegend, THStack, TLatex

import torch
import torch.nn as nn
from torch.utils.data import Dataset, DataLoader

import MyPackage
import MyPackage.Network as mn



class RootDataSet(Dataset):
    def __init__( self, root_folder, data_set_name, n_events = None ):

        self.my_file_name = os.path.join( root_folder, data_set_name )
        self.my_file = TFile.Open(self.my_file_name, "update")

        self.var_tree = self.my_file.Get("var_tree")
        self.alt_tree = self.my_file.Get("alt_tree")

        self.alt_tree.SetBranchStatus("*", 0)
        self.alt_tree.SetBranchStatus("rotate_angle", 1)

        self.n_events = n_events if n_events is not None else self.var_tree.GetEntries()

        branch_names = [ b.GetName() for b in self.var_tree.GetListOfBranches() ]
        self.recon_names = branch_names[4:69]

    def __getitem__(self, idx):
        self.var_tree.GetEntry(idx)
        self.alt_tree.GetEntry(idx)

        recon = np.array( [ getattr( self.var_tree, name ) for name in self.recon_names ], dtype=np.float32  )
        theta = self.alt_tree.rotate_angle

        return recon, theta

    def __len__(self):
        return self.n_events

def main():
    ####### ADJUSTABLE PARAMETERS ########
    network_base_dir = "Saved_Networks/"

    activated_ann = mn.AMANNDA( "Swish", 5, 1000, 0.2, "XY", False )
    network_name  = "Main_Swish_5x1000_AM_SL_AA_XY_6e-07_Drop2"

    ann_tree_name = "main_ann"

    data_files = [

        # ( "Evaluation", "ttbar_1in3_noselec.root"  ),


        # ( "SR_WW",      "WW_1in3_WW_strict.root"    ),
        # ( "SR_Higgs",   "H125_all_WW_strict.root"   ),
        ( "SR_ttbar",   "ttbar_1in3_tt_strict.root"   ),


        # ( "SR_Zee", "Zee_all_Zee_strict.root"      ),
        # ( "SR_Zee", "SZee_all_Zee_strict.root"     ),

        # ( "SR_Zee", "Ztt_all_Zee_strict.root"      ),
        # ( "SR_Zee", "ZZ_1in3_Zee_strict.root"      ),
        # ( "SR_Zee", "WZ_1in3_Zee_strict.root"      ),
        # ( "SR_Zee", "WW_1in3_Zee_strict.root"      ),
        # ( "SR_Zee", "top_1in3_Zee_strict.root"     ),
        # ( "SR_Zee", "antitop_1in3_Zee_strict.root" ),
        # ( "SR_Zee", "ttbar_1in3_Zee_strict.root"   ),


        # ( "SR_Zmumu", "Zmumu_all_Zmumu_strict.root"    ),
        # ( "SR_Zmumu", "SZmumu_all_Zmumu_strict.root"   ),
        # ( "SR_Zmumu", "MGZmumu_all_Zmumu_strict.root"  ),

        # ( "SR_Zmumu", "Ztt_all_Zmumu_strict.root"      ),
        # ( "SR_Zmumu", "ZZ_1in3_Zmumu_strict.root"      ),
        # ( "SR_Zmumu", "WZ_1in3_Zmumu_strict.root"      ),
        # ( "SR_Zmumu", "WW_1in3_Zmumu_strict.root"      ),
        # ( "SR_Zmumu", "top_1in3_Zmumu_strict.root"     ),
        # ( "SR_Zmumu", "antitop_1in3_Zmumu_strict.root" ),
        # ( "SR_Zmumu", "ttbar_1in3_Zmumu_strict.root"   ),


        # ( "SR_SUSY", "SL400X250_all_susy_2l_0jets.root"   ),
        # ( "SR_SUSY", "SL550X001_all_susy_2l_0jets.root"   ),
        # ( "SR_SUSY", "SL300X200_all_susy_2l_0jets.root"   ),

        # ( "SR_SUSY", "SZee_all_susy_2l_0jets.root"       ),
        # ( "SR_SUSY", "SZmumu_all_susy_2l_0jets.root"     ),
        # ( "SR_SUSY", "Ztt_all_susy_2l_0jets.root"        ),
        # ( "SR_SUSY", "ZZ_1in3_susy_2l_0jets.root"        ),
        # ( "SR_SUSY", "WZ_1in3_susy_2l_0jets.root"        ),
        # ( "SR_SUSY", "WW_1in3_susy_2l_0jets.root"        ),
        # ( "SR_SUSY", "top_1in3_susy_2l_0jets.root"       ),
        # ( "SR_SUSY", "antitop_1in3_susy_2l_0jets.root"   ),
        # ( "SR_SUSY", "ttbar_1in3_susy_2l_0jets.root"     ),
        # ( "SR_SUSY", "Sllvv_all_susy_2l_0jets.root"      ),


        # ( "Real", "Real_all_Zee_strict.root" ),
        # ( "Real", "Real_all_Zmumu_strict.root" ),
        # ( "Real", "Real_all_susy_2l_0jets.root" ),


        ]

    n_events = None         # None = all
    mini_batch_size = 5000  #
    ######################################

    ########## Adding to the directory path ##########

    network_base_dir    =  os.path.join( os.environ["HOME_DIRECTORY"], network_base_dir )
    data_base_dir       =  os.path.join( os.environ["HOME_DIRECTORY"], "Data" )

    ########## The Neural Network is Loaded ##########
    activated_ann.load_state_dict( torch.load( os.path.join( network_base_dir, network_name, "network_model_optimal") ) )
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    activated_ann = activated_ann.to(device)
    activated_ann.eval()


    ########## Loading normalisation data ##########
    stats_data = pd.read_csv( os.path.join( network_base_dir, network_name, "data_stats.csv" ), header = None )
    means  = torch.as_tensor( stats_data.iloc[1, :].values.astype(np.float32) )
    stdevs = torch.as_tensor( stats_data.iloc[2, :].values.astype(np.float32) )
    means  = means.to(device)
    stdevs = stdevs.to(device)

    for data_spec_dir, data_set_name in data_files:
        data_full_dir = os.path.join( data_base_dir, data_spec_dir )
        print("\n\n" + data_set_name )


        ########## Getting data from the files ##########
        data_set    = RootDataSet( data_full_dir, data_set_name, n_events  )
        data_loader = DataLoader( data_set,  batch_size = mini_batch_size, num_workers = 0, pin_memory = True )


        ## We are going to add some leaves to the end of the var_tree
        if data_set.my_file.GetListOfKeys().Contains(ann_tree_name):
            print( "Found existing TTree named: {}".format(ann_tree_name)  )
            print( "Deleting this TTree and replacing it" )
            data_set.my_file.Delete( "{};0".format(ann_tree_name) )
            data_set.my_file.Delete( "{};1".format(ann_tree_name) )
            data_set.my_file.Delete( "{};2".format(ann_tree_name) )
        ann_tree = TTree( ann_tree_name, "ANN Output Tree" )


        ## Now the new branches are created
        WP_ANN_ET  = array( 'f', [0] )
        WP_ANN_X   = array( 'f', [0] )
        WP_ANN_Y   = array( 'f', [0] )
        WP_ANN_Phi = array( 'f', [0] )

        ## Unrotated output
        MET_ANN_X   = array( 'f', [0] )
        MET_ANN_Y   = array( 'f', [0] )


        br_WP_ANN_ET  = ann_tree.Branch("WP_ANN_ET",  WP_ANN_ET,  "WP_ANN_ET/f")
        br_WP_ANN_X   = ann_tree.Branch("WP_ANN_X",   WP_ANN_X,   "WP_ANN_X/f")
        br_WP_ANN_Y   = ann_tree.Branch("WP_ANN_Y",   WP_ANN_Y,   "WP_ANN_Y/f")
        br_WP_ANN_Phi = ann_tree.Branch("WP_ANN_Phi", WP_ANN_Phi, "WP_ANN_Phi/f")

        br_MET_ANN_X   = ann_tree.Branch("MET_ANN_X",   MET_ANN_X,   "MET_ANN_X/f")
        br_MET_ANN_Y   = ann_tree.Branch("MET_ANN_Y",   MET_ANN_Y,   "MET_ANN_Y/f")


        ## We begin looping over entries in the tree, adding values to the new branches
        ## Since moving data through the network takes up time, we need to do this in batches
        with torch.no_grad():
            for batch_idx, (recon, theta) in enumerate(data_loader):

                ## We first pass the batch onto the GPU
                recon = recon.to( device )

                ## The batch is then normalised together
                recon = torch.div( ( recon - means[4:69] ), stdevs[4:69] )

                ## The batch is passed through the network
                output = activated_ann( recon )

                ## Unormalised back to GeV and saved
                output = torch.mul( output, stdevs[1:3] ) + means[1:3]
                svd_output = output.cpu()

                ## We calculate the magnitude for each event
                magnitude = torch.norm( output, dim=1 )

                ## We create the batch rotation matrix
                rotation_matrix = torch.empty( len(recon), 2, 2, dtype = torch.float32, device = device )
                rotation_matrix[:,0,0] =  torch.cos(theta)
                rotation_matrix[:,1,0] =  torch.sin(theta)
                rotation_matrix[:,1,1] =  rotation_matrix[:,0,0]
                rotation_matrix[:,0,1] = -rotation_matrix[:,1,0]

                ## We apply this rotation in batch
                rotated_ouput = torch.bmm( rotation_matrix, output.unsqueeze_(1).transpose( 1, 2 ) )

                ## We calculate the angle using the arctangent
                phi = torch.atan2( rotated_ouput[:,1,0], rotated_ouput[:,0,0] )

                ## The needed variables are copied back to the cpu
                rotated_ouput = rotated_ouput.cpu()
                magnitude = magnitude.cpu()
                phi = phi.cpu()


                ## Now we cycle through the batch itself adding values to the tree
                for i in range(len(recon)):

                    ## The individual variables are saved
                    WP_ANN_X[0]   = rotated_ouput[i][0]
                    WP_ANN_Y[0]   = rotated_ouput[i][1]
                    WP_ANN_ET[0]  = magnitude[i]
                    WP_ANN_Phi[0] = phi[i]

                    MET_ANN_X[0]  = svd_output[i][0]
                    MET_ANN_Y[0]  = svd_output[i][1]

                    ## The branches are filled
                    ann_tree.Fill()

                print("Completed {}/{}          \r".format(batch_idx+1, len(data_loader)), end="" )
                sys.stdout.flush()
        print("Completed {}/{}          ".format( len(data_loader), len(data_loader) ) )

        ## Make sure that we are writing to the origional file
        gFile = data_set.my_file
        ann_tree.Write()
        data_set.my_file.Close()



if __name__ == '__main__':
    main()
