from __future__ import print_function

import os
import csv
import sys
import random
import numpy as np
import pandas as pd

import matplotlib.pyplot as plt

from ROOT import TFile, TTree, TCanvas, gROOT, TH1D, THStack, gDirectory

from MyPackage import Plotting

######## The data set class must be defined for PyTorch to be able to read the data ########
class RootDataSet(object):
    def __init__( self, folder, input_file ):
        print("")

        ## Creates the full name of the output file
        self.folder = os.path.join( os.environ["HOME_DIRECTORY"], "Data", folder )

        ## Opens the root file
        print ("Opening ROOT file")
        self.root_file = TFile.Open(os.path.join(self.folder, input_file))

        ## Loads the TTree
        print ("Opening variable TTree")
        self.var_tree  = self.root_file.Get("var_tree")
        self.alt_tree  = self.root_file.Get("alt_tree")
        self.n_events  = self.var_tree.GetEntries()
        self.var_names = [ branch.GetName() for branch in self.var_tree.GetListOfBranches() ]
        print("")

data_set = RootDataSet( "Learning", "combined.root" )

labels = []

frac_last = 0.0
for i in range(data_set.n_events):

    ## Getting event data from the TTree
    data_set.alt_tree.GetEntry(i)
    label = str( getattr( data_set.alt_tree, "data_label" ) )

    if label not in labels:
        labels.append( label )

    ## Printing out progress
    if int(100*i/data_set.n_events)>frac_last:
        frac_last = int(100*i/data_set.n_events);
        print(" -- {}/{} -- {}% processed \r".format( i, data_set.n_events, frac_last ), end="" )
        sys.stdout.flush()

print( "" )
print( labels )










