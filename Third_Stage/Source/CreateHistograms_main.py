import os
import sys
import numpy as np

from collections import OrderedDict
from ROOT import TFile, TTree, TCanvas, TPad, TMath, gStyle, gROOT, TObject, TStyle, TH1, TH1D, TH2D, TMultiGraph, TLine, TGraphErrors, gDirectory, TProfile, TColor, TGraph, TLegend, THStack, TLatex

from MyPackage import Evaluation

def main( network_name, do_weights, general_weights, electron_weights, muon_weights, data_files, no_true_comp, nbins,
         veto_all_jets, require_jets, WorkingP_List, Independant_Variable_List, Histogram_Variable_List, Profile_List, TH2D_List ):

    TH1.SetDefaultSumw2()

    ## First we check that you havent vetoed and requested jets
    if veto_all_jets and require_jets:
        print( "\n\n\nYOU CANT ASK FOR NONE YET SOME JETS\n\n\n" )
        return 0

    for data_dir, data_set_name in data_files:
        OUTPUT_dir    = os.path.join( os.environ["HOME_DIRECTORY"], "Output" )
        DATA_dir      = os.path.join( os.environ["HOME_DIRECTORY"], "Data" )
        data_base_dir = os.path.join( DATA_dir, data_dir )
        print("\n\n" + data_set_name )





        """
         ____    ____    _____   ____       _      ____       _      _____   ___    ___    _   _
        |  _ \  |  _ \  | ____| |  _ \     / \    |  _ \     / \    |_   _| |_ _|  / _ \  | \ | |
        | |_) | | |_) | |  _|   | |_) |   / _ \   | |_) |   / _ \     | |    | |  | | | | |  \| |
        |  __/  |  _ <  | |___  |  __/   / ___ \  |  _ <   / ___ \    | |    | |  | |_| | | |\  |
        |_|     |_| \_\ |_____| |_|     /_/   \_\ |_| \_\ /_/   \_\   |_|   |___|  \___/  |_| \_|

        """

        ## The list of matrices
        Indp_output = [ None for var in Independant_Variable_List ]
        TH1D_output = [ [ None for wp in WorkingP_List ] for var in Histogram_Variable_List ]
        Tail_output = [ [ None for wp in WorkingP_List ] for var in Histogram_Variable_List ]
        Prof_output = [ [ None for wp in WorkingP_List ] for var in Profile_List ]
        TH2D_output = [ [ None for wp in WorkingP_List ] for var in TH2D_List ]

        ## Open the root file
        root_file = TFile.Open(os.path.join(data_base_dir, data_set_name))

        ## Loading the tree containing the working point information
        wpt_tree = root_file.Get("wpt_tree")

        ## Making the wpt_tree and the other trees friends so that they can be compared
        wpt_tree.AddFriend( network_name, root_file )
        wpt_tree.AddFriend( "alt_tree", root_file )
        wpt_tree.AddFriend( "var_tree", root_file )

        ## Creating a string of all the weights to be applied
        event_weights = [ ]
        if "SR"   in data_dir: event_weights += general_weights
        if "ee"   in data_dir: event_weights += electron_weights
        if "mumu" in data_dir: event_weights += muon_weights

        if veto_all_jets:
            event_weights += [ "(var_tree.Jets_Loose_SumET==0)" ]
        if require_jets:
            event_weights += [ "(var_tree.Jets_Loose_SumET>0)" ]

        if do_weights: weight_string = " * ".join(event_weights)
        else: weight_string = ""

        if len(weight_string):
            print( "Events are weighted using: {}".format( weight_string ) )


        ## The strings to call up the Truth Values
        if no_true_comp:
            True_Et  = "0"
            True_Ex  = "0"
            True_Ey  = "0"
            True_Phi = "0"
        else:
            True_Et  = "WP_Truth_ET"
            True_Ex  = "WP_Truth_X"
            True_Ey  = "WP_Truth_Y"
            True_Phi = "WP_Truth_Phi"





        """
         ____    ____       _     __        __  ___   _   _    ____
        |  _ \  |  _ \     / \    \ \      / / |_ _| | \ | |  / ___|
        | | | | | |_) |   / _ \    \ \ /\ / /   | |  |  \| | | |  _
        | |_| | |  _ <   / ___ \    \ V  V /    | |  | |\  | | |_| |
        |____/  |_| \_\ /_/   \_\    \_/\_/    |___| |_| \_|  \____|

        """

        ## Before the workingpoint loop, the independant variables are drawn
        for v, var in enumerate(Independant_Variable_List):
            print( " -- {}".format( var.name ) )

            ## Creating the histogram which will be filled
            hist_name = var.name
            myhist = TH1D( hist_name, hist_name, var.nbins, var.xmin, var.xmax )
            myhist.SetStats(True)
            myhist.StatOverflows(True)

            ## Get Maths Function from variable
            maths_string = var.tree + "." + var.branch

            ## Drawing the tree and saving the hist to the matrix
            execution = "{}>>{}".format( maths_string, hist_name )
            wpt_tree.Draw( execution, weight_string, "goff" )

            ## Saving the Histogram to the Matrix
            myhist.SetDirectory( 0 )
            Indp_output[v] = myhist



        ## First we select the working point and create the correct strings
        for w, wp in enumerate(WorkingP_List):
            print( " -- {}:".format( wp.name ) )

            Rec_Et  = wp.Et
            Rec_Ex  = wp.Ex
            Rec_Ey  = wp.Ey
            Rec_Phi = wp.Phi
            if wp.tree == "ann_tree":
                Rec_Et = network_name + "." + Rec_Et
                Rec_Ex = network_name + "." + Rec_Ex
                Rec_Ey = network_name + "." + Rec_Ey

            rec_and_truth_vars = [ Rec_Et, Rec_Ex, Rec_Ey, Rec_Phi, True_Et, True_Ex, True_Ey, True_Phi ]

            ## Drawing the 1D histograms
            for v, var in enumerate(Histogram_Variable_List):
                print( " -- -- {}".format( var.name ) )

                ## Creating the histogram which will be filled
                hist_name = "{}_{}".format(var.name, wp.name)
                myhist = TH1D( hist_name, hist_name, var.nbins, var.xmin, var.xmax )
                myhist.SetStats(True)
                myhist.StatOverflows(True)

                ## Individual special plots
                if var.name == "XY":
                    ## Plot the X histogram
                    maths_string = Evaluation.TH1D_Maths_String( "X", *rec_and_truth_vars )
                    execution = "{} >> {}".format( maths_string, hist_name )
                    wpt_tree.Draw( execution, weight_string, "goff" )

                    ## Add the y histogram
                    maths_string = Evaluation.TH1D_Maths_String( "Y", *rec_and_truth_vars )
                    execution = "{} >> +{}".format( maths_string, hist_name )
                    wpt_tree.Draw( execution, weight_string, "goff" )

                else:
                    ## Get Maths Function from variable
                    maths_string = Evaluation.TH1D_Maths_String( var.name, *rec_and_truth_vars )

                    ## Drawing the tree and saving the hist to the matrix
                    execution = "{} >> {}".format( maths_string, hist_name )
                    wpt_tree.Draw( execution, weight_string, "goff" )

                ## Saving the Histogram to the Matrix
                myhist.SetDirectory( 0 )
                TH1D_output[v][w] = myhist


            ## Drawing the Profiles
            for v, (vx, vy) in enumerate(Profile_List):
                print( " -- -- {} vs {}".format( vx.name, vy.name ) )

                ## Creating the profile which will be filled
                hist_name = "{}_vs_{}_{}".format(vx.name, vy.name, wp.name)
                myhist = TProfile( hist_name, hist_name, vx.nbins, vx.xmin, vx.xmax )

                if vy.reso: myhist.SetErrorOption('s')

                ## The x variable is called from its branch in the correct tree
                x_string = vx.tree + "." + vx.branch

                ## Individual special plots
                if vy.name == "XY":
                    y_string = Evaluation.TH1D_Maths_String( "X", *rec_and_truth_vars )
                    execution = "{}:{} >> {}".format( y_string, x_string, hist_name )
                    wpt_tree.Draw( execution, weight_string, "goff prof")

                    y_string = Evaluation.TH1D_Maths_String( "Y", *rec_and_truth_vars )
                    execution = "{}:{} >>+{}".format( y_string, x_string, hist_name )
                    wpt_tree.Draw( execution, weight_string, "goff prof")

                elif vy.name == "AZ":
                    z_x  = "(alt_tree.ll_px)"
                    z_y  = "(alt_tree.ll_py)"
                    z_pt = "(alt_tree.ll_pt)"

                    x_string = z_pt
                    y_string = "({rx}*{zx} + {ry}*{zy})/{zpt}".format( rx = Rec_Ex, ry = Rec_Ey, zx = z_x, zy = z_y, zpt = z_pt )
                    execution = "{}:{} >> {}".format( y_string, x_string, hist_name )
                    wpt_tree.Draw( execution, weight_string, "goff prof")

                else:
                    y_string = Evaluation.TH1D_Maths_String( vy.name, *rec_and_truth_vars )
                    execution = "{}:{} >> {}".format( y_string, x_string, hist_name )
                    wpt_tree.Draw( execution, weight_string, "goff prof")

                ## Saving the Histogram to the Matrix
                myhist.SetDirectory( 0 )
                Prof_output[v][w] = myhist



            if wp.name not in [ "Network", "Tight"]:
                continue



            ## Drawing the TH2Ds
            for v, (vx, vy) in enumerate(TH2D_List):
                print( " -- -- {} vs {}".format( vx.name, vy.name ) )

                ## Creating the profile which will be filled
                hist_name = "2D_{}_vs_{}_{}".format(vx.name, vy.name, wp.name)
                myhist = TH2D( hist_name, hist_name, vx.nbins2d, vx.xmin2d, vx.xmax2d, vy.nbins2d, vy.xmin2d, vy.xmax2d )

                x_string = Evaluation.TH1D_Maths_String( vx.name, *rec_and_truth_vars )
                y_string = Evaluation.TH1D_Maths_String( vy.name, *rec_and_truth_vars )

                ## The x variable is called from its branch in the correct tree
                if x_string is None:
                    x_string = vx.tree + "." + vx.branch

                execution = "{}:{} >> {}".format( y_string, x_string, hist_name )
                wpt_tree.Draw( execution, weight_string, "goff" )

                ## Saving the Histogram to the Matrix
                myhist.SetDirectory( 0 )
                TH2D_output[v][w] = myhist

        root_file.Close()





        """
         _____   ____    ___   _____   ___   _   _    ____
        | ____| |  _ \  |_ _| |_   _| |_ _| | \ | |  / ___|
        |  _|   | | | |  | |    | |    | |  |  \| | | |  _
        | |___  | |_| |  | |    | |    | |  | |\  | | |_| |
        |_____| |____/  |___|   |_|   |___| |_| \_|  \____|

        """

        ## We now go through the 1D histograms and make them include overflow and underflow
        for v, var in enumerate(Histogram_Variable_List):
            for w, wp in enumerate(WorkingP_List):

                ## Include the overflow
                last_bin = TH1D_output[v][w].GetBinContent( nbins )
                overflow = TH1D_output[v][w].GetBinContent( nbins+1 )
                TH1D_output[v][w].SetBinContent( nbins, last_bin + overflow )
                TH1D_output[v][w].SetBinContent( nbins+1, 0 )

                ## Include the underflow
                first_bin = TH1D_output[v][w].GetBinContent( 1 )
                underflow = TH1D_output[v][w].GetBinContent( 0 )
                TH1D_output[v][w].SetBinContent( 1, first_bin + underflow )
                TH1D_output[v][w].SetBinContent( 0, 0 )

                ## We create a tail distrobution if it is requested
                if var.tail:
                    tail_temp = TH1D_output[v][w].GetCumulative( False )
                    tail_temp.Scale( 1 / tail_temp.GetBinContent(1) )
                    Tail_output[v][w] = tail_temp


        ## We go through the resolution profiles and replace their entries with the RMSE for each bin
        for v, (vx, vy) in enumerate(Profile_List):

            if not vy.reso:
                continue

            for w, wp in enumerate(WorkingP_List):

                old_prof = Prof_output[v][w]
                bin_width = old_prof.GetBinWidth( 1 )

                name = "{}_vs_{}_{}_res".format(vx.name, vy.name, wp.name)
                new_prof = TGraphErrors( vx.nbins )
                new_prof.SetName( name )
                new_prof.SetTitle( name )

                new_prof.SetLineWidth(2)

                for b_idx in range( vx.nbins ):
                    new_prof.SetPoint( b_idx, old_prof.GetBinCenter(b_idx+1), old_prof.GetBinError( b_idx+1 ) )
                    new_prof.SetPointError( b_idx, bin_width/2, 0 )

                Prof_output[v][w] = new_prof





        """
         ____       _     __     __  ___   _   _    ____
        / ___|     / \    \ \   / / |_ _| | \ | |  / ___|
        \___ \    / _ \    \ \ / /   | |  |  \| | | |  _
         ___) |  / ___ \    \ V /    | |  | |\  | | |_| |
        |____/  /_/   \_\    \_/    |___| |_| \_|  \____|

        """

        ## Creating the output directory
        output_dir = os.path.join( OUTPUT_dir, network_name, data_set_name[:-5] )
        if veto_all_jets:
            output_dir = output_dir + "_NOJETS"
        if require_jets:
            output_dir = output_dir + "_SOMEJETS"


        ## Check that the file can be saved
        if not os.path.exists(output_dir):
            os.system("mkdir -p " + output_dir)

        ## We create an output file for the histograms
        output_file = TFile( os.path.join( output_dir, "histograms.root" ), "update" )
        gFile = output_file

        ## We save the independants
        for v, var in enumerate(Independant_Variable_List):
            Indp_output[v].Write( "", TObject.kOverwrite )

        ## We save the TH1Ds and tails
        for v, var in enumerate(Histogram_Variable_List):
            for w in range(len(WorkingP_List)):
                TH1D_output[v][w].Write( "", TObject.kOverwrite )
                if var.tail:
                    Tail_output[v][w].Write( "", TObject.kOverwrite )

        ## We save the profiles
        for v in range(len(Profile_List)):
            for w in range(len(WorkingP_List)):
                Prof_output[v][w].Write( "", TObject.kOverwrite )

        ## We save the TH2Ds
        for v in range(len(TH2D_List)):
            for w in range(len(WorkingP_List[:2])):
                TH2D_output[v][w].Write( "", TObject.kOverwrite )

        output_file.Close()
    return 0



if __name__ == '__main__':
    main()
