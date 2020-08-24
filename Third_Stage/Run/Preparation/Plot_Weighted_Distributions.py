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

gROOT.SetStyle("ATLAS")
gROOT.ForceStyle()

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

    ## This function calculates the event weights based on heights in the True MET distribution
    def load_weights(self, weight_file_name ):

        ## Creating the full name of the weight file
        self.weight_file_name = os.path.join( self.folder, weight_file_name )

        ## Opening the weight file and reading its contents
        weight_file = open( self.weight_file_name, 'r' )
        self.w_nbins = int( weight_file.readline() )
        self.w_max   = float( weight_file.readline() )
        self.weights = [ float(s) for s in weight_file.readline().split(',') ]

    def __apply_weights(self, True_MET):
        if not hasattr(self, 'weights'):
            print( "\n\n\nError: Must call produce_weights() if you want weights added!\n\n\n" )
            return 0

        ## Calculating the bin number the Et value would fall in
        b_idx = int( self.w_nbins * True_MET / self.w_max )

        ## If the returned bin is too high, then it goes into overflow
        if b_idx >= self.w_nbins:
            b_idx = self.w_nbins - 1

        return self.weights[ b_idx ]

    def draw_distributions(self):

        canvas = TCanvas( "canvas", 'A basic canvas', 800, 600 )

        ## We chose the various distributions that we want to create
        self.unweighted_truth = TH1D( "unweighted_truth", "unweighted_truth", self.w_nbins, 0, self.w_max )
        self.weighted_truth   = TH1D( "weighted_truth", "weighted_truth", self.w_nbins, 0, self.w_max )
        self.weighted_truth_diff_binning = TH1D( "weighted_truth_diff_binning", "weighted_truth_diff_binning", 100, 0, 400 )

        ## We draw and save the first histogram directly
        data_set.var_tree.Draw( "MET_Truth_ET>>unweighted_truth" )
        canvas.Print("Unweighted.pdf")

        frac_last = 0.0
        for i in range(self.n_events):

            ## Getting event data from the TTree
            self.var_tree.GetEntry(i)
            True_MET = getattr( self.var_tree, "MET_Truth_ET" )

            ## Reweighting the data
            weight = self.__apply_weights( True_MET )

            ## Filling in the histograms
            self.weighted_truth.Fill( True_MET, weight )
            self.weighted_truth_diff_binning.Fill( True_MET, weight )

            ## Printing out progress
            if int(100*i/self.n_events)>frac_last:
                frac_last = int(100*i/self.n_events);
                print(" -- {}/{} -- {}% processed \r".format( i, self.n_events, frac_last ), end="" )
                sys.stdout.flush()

        print(" -- {}/{} -- 100% processed ".format( i+1, self.n_events ) )
        print("")


        ## Now we draw and save the other histograms in turn
        self.weighted_truth.Draw()
        canvas.Print("Weighted.pdf")

        self.weighted_truth_diff_binning.Draw()
        canvas.Print("Weighted_Diff_Binning.pdf")


    def draw_distributions_by_region(self, regions):

        ## We chose the various distributions that we want to create
        standard = {}
        weighted = {}

        for region in regions:
            standard[region] = TH1D( "standard_{}".format(region), "standard_{}".format(region), self.w_nbins, 0, self.w_max )
            weighted[region] = TH1D( "weighted_{}".format(region), "weighted_{}".format(region), self.w_nbins, 0, self.w_max )

        frac_last = 0.0
        for i in range(self.n_events):

            ## Getting event data from the TTree
            self.var_tree.GetEntry(i)
            self.alt_tree.GetEntry(i)

            True_MET = getattr( self.var_tree, "MET_Truth_ET" )
            region   = getattr( self.alt_tree, "data_label" )

            ## Reweighting the data
            weight = self.__apply_weights( True_MET )

            ## Filling in the histograms
            standard[str(region)].Fill( True_MET )
            weighted[str(region)].Fill( True_MET, weight )

            ## Printing out progress
            if int(100*i/self.n_events)>frac_last:
                frac_last = int(100*i/self.n_events);
                print(" -- {}/{} -- {}% processed \r".format( i, self.n_events, frac_last ), end="" )
                sys.stdout.flush()


        print(" -- {}/{} -- 100% processed ".format( i+1, self.n_events ) )
        print("")

        self.__draw_stack( regions, standard, "unweighted_regions.pdf" )
        self.__draw_stack( regions, weighted, "weighted_regions.pdf" )


    def __get_colour( self, region ):
        if region == "Sllvv":
            return 432-3
        elif region == "ZZ":
            return 800+7
        elif region == "WW":
            return 800-5
        elif region == "WZ":
            return 800+0
        elif region == "top":
            return 416+0
        elif region == "antitop":
            return 416-9
        elif region == "ttbar":
            return 416+3

    def __get_label( self, region ):
        if region == "Sllvv":
            return "#it{(S#scale[0.8]{HERPA}) VV #rightarrow #nu#null}"
        elif region == "ZZ":
            return "#it{ZZ #rightarrow #nu#nu}"
        elif region == "WW":
            return "#it{WW #rightarrow l#nul#nu}"
        elif region == "WZ":
            return "#it{WZ #rightarrow l#null}"
        elif region == "ttbar":
            return "#it{t#bar{t}}"
        elif region == "top":
            return "#it{Wt}"
        elif region == "antitop":
            return "#it{W#bar{t}}"

    def __draw_stack( self, regions, th1d_dict, output_name ):

        canvas = TCanvas( "canvas", 'A basic canvas', 1000, 600 )
        stack = THStack("stack", "stack")
        leg = Plotting.Create_Legend( 0.60, 0.60, 0.95, 0.95 )

        for region in regions:
            hist = th1d_dict[region]

            hist.SetLineColor(1)
            hist.SetLineWidth(1)
            hist.SetMarkerColor( self.__get_colour(region) )
            hist.SetMarkerStyle(0)
            hist.SetFillColor( self.__get_colour(region) )

            leg.AddEntry( hist, self.__get_label( region ), "f")
            stack.Add( hist )

            del hist

        stack.Draw( "HIST" )
        stack.GetYaxis().SetTitle( "Events" )
        stack.GetXaxis().SetTitle( "True #it{E_{T}^{miss}} [GeV]" )
        leg.Draw()
        canvas.Print(output_name)


    def draw_weights(self):

        ## We draw the weights using matplotlib
        space = np.linspace( 0, self.w_max, self.w_nbins )
        plt.step( space, self.weights[:-1], where = "post" )

        plt.xlabel( "True MET" )
        plt.ylabel( "Weights" )

        plt.ylim( top = np.max(self.weights) * 1.5 )
        plt.xlim( right = np.max(space)*1.2 )
        plt.savefig( "Weights" )


data_set = RootDataSet( "Learning", "combined.root" )
data_set.load_weights( "combined_weights.csv" )
# data_set.draw_distributions()

regions = [ "Sllvv", "WW", "WZ", "ZZ", "ttbar", "top", "antitop" ]
data_set.draw_distributions_by_region( regions )
# data_set.draw_weights()























