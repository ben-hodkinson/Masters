from __future__ import print_function

import os
import csv
import sys
import time
import random
import numpy as np
import pandas as pd
from ROOT import TFile, TTree, TCanvas, gROOT, TH1D, gDirectory

######## The data set class must be defined for PyTorch to be able to read the data ########
class RootDataSet(object):
    def __init__( self, folder, input_file, inp_tree, output_file ):
        print("")
        ## Creates the full name of the output file
        self.folder = os.path.join( os.environ["HOME_DIRECTORY"], "Data", folder )
        self.outfile_name = os.path.join( self.folder, output_file )

        ## Opens the root file
        print ("Opening ROOT file")
        self.root_file = TFile.Open(os.path.join(self.folder, input_file))

        ## Loads the TTree
        print ("Opening variable TTree")
        self.tree      = self.root_file.Get(inp_tree)
        self.n_events  = self.tree.GetEntries()

        ## Creates a list of all the variable names in the tree
        self.var_names = [ branch.GetName() for branch in self.tree.GetListOfBranches() ]
        print("")


    ## This is a function which returns the total number of events the tree
    def __len__(self):
        return sum( self.n_events )


    ## This function calculates the mean and stdev of each of the leaves on the tree
    def produce_stats(self, load_others = False):
        canvas = TCanvas( "dummy_stat_canvas", 'dummy_stat_canvas', 1000, 800 )

        ## If we need to generate our own stats and save to csv file
        if load_others is False:

            ## The stats are held in arrays
            data_means = []
            data_stddevs = []

            ## Each stat is saved by making a histogram and appending its stats to the array
            print( "Calculating means and deviations" )
            for i, var in enumerate(self.var_names):
                sys.stdout.flush()

                hist_name = "hist" + var
                hist = TH1D( hist_name, hist_name, 1, 1, 2 )
                hist.SetStats(True)
                hist.StatOverflows(True)

                self.tree.Draw( var + ">>" + hist_name, "", "goff" )
                data_means.append( hist.GetMean() )
                data_stddevs.append( hist.GetStdDev() )
                print( " {} - {} -> ( {:6.4f}, {:6.4f} )                      ".format( i, var, hist.GetMean(), hist.GetStdDev() ) )


            ## The stats are saved to a file along with the column names
            print( " -- writing                       " )
            mns_outfile_name = self.outfile_name[:-4] + "_stats.csv"
            with open(mns_outfile_name, 'w') as outfile:
                wr = csv.writer(outfile)
                wr.writerow( self.var_names )
                wr.writerow( [ "{:.8e}".format(x) for x in data_means ] )
                wr.writerow( [ "{:.8e}".format(x) for x in data_stddevs ] )


        ## If we are loading old statistics in an already produced txt file
        else:
            print( "Loading old means and deviations" )
            file_name = os.path.join(self.folder, load_others)
            old_stat_file = open( file_name, 'r' )
            old_stat_file.readline()
            data_means   = [ float(s) for s in old_stat_file.readline().split(',') ]
            data_stddevs = [ float(s) for s in old_stat_file.readline().split(',') ]
            old_stat_file.close()

        self.means   = np.array( data_means, dtype = np.float64 )
        self.stddevs = np.array( data_stddevs, dtype = np.float64 )
        print("")


    ## This function calculates the event weights based on heights in the True MET distribution
    def produce_weights(self, w_nbins = 50, w_max = 300 ):
        print( "Creating weight distribution" )

        canvas = TCanvas( "dummy_weight_canvas", 'dummy_weight_canvas', 1000, 800 )

        ## The bins to be used in the truth histogram
        self.w_nbins = w_nbins
        self.w_max   = w_max

        ## Drawing the truth histogram
        true_hist = TH1D( "true_hist", "true_hist", self.w_nbins, 0, self.w_max )
        data_set.tree.Draw( "MET_Truth_ET>>true_hist", "", "goff" )

        ## Root histograms start at 0 (underflow) and go to max+1 (overflow)
        ## Thus they actually contain (w_nbins+2) bins

        ## The height array of the true histogram
        heights = np.zeros( self.w_nbins )

        ## Filling in the heights based on histogram content
        for idx in range(0, self.w_nbins ):
            heights[idx] = true_hist.GetBinContent( idx + 1 )

        ## Calculate the max height of the histogram
        max_height = np.amax( heights )

        ## Weight array is created by the ratio between that height and the max
        weights = max_height / heights

        ## The stats are saved to a file along with the column names
        print( " -- writing" )
        weight_outfile_name = self.outfile_name[:-4] + "_weights.csv"
        with open( weight_outfile_name, 'w' ) as outfile:
            wr = csv.writer(outfile)
            wr.writerow( [self.w_nbins] )
            wr.writerow( [self.w_max] )
            wr.writerow( [ "{:.8e}".format(x) for x in weights ] )

        self.weights = weights.astype(np.float64)
        print("")



    ## A function used to return normalised versions of a data array
    def __apply_stats(self, data, normalise_lvl ):
        if not hasattr(self, 'means'):
            print( "\n\n\nError: Must call produce_stats() if you want to normalise!\n\n\n" )
            return 0

        if len(data) != len(self.means) and len(data) != len(self.stddevs):
            print( "\n\n\nError: The data and the stats arrays are of different length! Cant normalise!\n\n\n" )
            return 0

        if normalise_lvl == 1:
            truth_data = data[0:4]
            recon_data = data[4:]
            recon_data = np.divide( ( recon_data - self.means[4:] ), self.stddevs[4:] )
            normed_data = np.concatenate((truth_data,recon_data))
            return normed_data.astype(np.float64).tolist()

        if normalise_lvl == 2:
            normed_data = np.divide( ( data - self.means ), self.stddevs )
            return normed_data.astype(np.float64).tolist()



    ## A function used to calculate the specific weight given the event
    def __apply_weights(self, True_MET):
        if not hasattr(self, 'weights'):
            print( "\n\n\nError: Must call produce_weights() if you want weights added!\n\n\n" )
            return 0

        ## Calculating the bin number the Et value would fall in
        b_idx = int( self.w_nbins * True_MET / self.w_max )

        ## If the returned bin is too high, then it goes into overflow
        if b_idx > (self.w_nbins-1):
            b_idx = (self.w_nbins-1)

        return self.weights[ b_idx ]



    ## This is the main function used to create the CSV file by iterating through the root file
    def create_csv(self, max_n = 0, reweight = True, normalise_lvl = 2 ):
        print("Converting dataset to CSV")

        ## Calculating how many events to convert
        if max_n != 0:
            self.n_events = max_n

        ## Opening the outfile
        out_file = open(self.outfile_name, 'w')
        writ = csv.writer(out_file)

        ## Writing the column names as the first line
        # columns = self.var_names.copy()
        # columns.append( "Weights" )
        # writ.writerow( columns )

        ## Iterating over every event in the tree
        frac_last = 0
        for i in range(self.n_events):

            ## Getting event data from the TTree
            self.tree.GetEntry(i)
            data = [ getattr( self.tree, var ) for var in self.var_names ]

            ## Applying a weight to the data
            weight = 1.0
            True_MET = data[0]
            if reweight == True:
                weight = self.__apply_weights( True_MET )

            ## Normalising the event
            if normalise_lvl > 0:
                data = self.__apply_stats( data, normalise_lvl )

            ## Writing the event information
            data.append( weight )
            writ.writerow( [ "{:.10e}".format(x) for x in data ] )

            ## Printing out progress
            if int(100*i/self.n_events)>frac_last:
                frac_last = int(100*i/self.n_events);
                print(" -- {}/{} -- {}% processed \r".format( i, self.n_events, frac_last ), end="" )
                sys.stdout.flush()

        print(" -- {}/{} -- 100% processed ".format( i+1, self.n_events ) )
        print("")

        out_file.close()



    def shuffle_split_csv(self, output_1, output_2, split):

        print( "Splitting the CSV file into {} and {}".format(output_1, output_2) )
        print( " ----> {} will take first {} rows".format(output_1, split ) )
        print( " ----> {} will take the remaining {}".format(output_2, self.n_events - split) )

        ## The names of the files are created
        output_1_name = self.outfile_name[:-4] + "_" + output_1 + ".csv"
        output_2_name = self.outfile_name[:-4] + "_" + output_2 + ".csv"
                
        print( " -- Loading... " )
        df0 = pd.read_csv( self.outfile_name, header=None, dtype=np.float32 )

        print( " -- Selecting First Sample..." )
        df1 = df0.sample( n=split, random_state=200 )
        
        print( " -- Selecting Second Sample..." )
        df0.drop(df1.index, inplace = True)
        
        print( " -- Saving 1..." )
        df1.to_csv(output_1_name, mode='w', index=None, header=None, float_format="%.10e")
        
        print( " -- Saving 2..." )
        df0.to_csv(output_2_name, mode='w', index=None, header=None, float_format="%.10e")



data_set = RootDataSet( "Learning", "combined.root", "var_tree", "combined.csv" )

data_set.produce_stats( load_others = "combined_stats.csv" ) # False will create own from root file
data_set.produce_weights( w_nbins = 80, w_max = 300 )

data_set.create_csv( max_n = 0, reweight = True, normalise_lvl = 2 ) ## Normalise level 0 = No Normalisation, 1 = Rec Only, 2 = Rec and Truth

data_set.shuffle_split_csv( "test", "train", 500000 )












