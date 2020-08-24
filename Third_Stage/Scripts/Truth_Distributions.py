import os
import numpy as np

from ROOT import TFile, gROOT, TCanvas, TLegend, THStack, TH1D, gStyle, TMultiGraph, TLine, TH2D, gPad, TPad

def get_mean_and_dev( file_name, tree_name, var_name ):
    file = TFile.Open( file_name )
    tree = file.Get( tree_name )

    hist = TH1D( "holder", "holder", 1, 1, 2 )
    hist.SetStats(True)
    hist.StatOverflows(True)

    tree.Draw( var_name + " >> holder ", "", "goff" )

    mean = hist.GetMean()
    dev  = hist.GetStdDev()

    file.Close()
    del tree
    del hist

    return mean, dev

## Setting up the graph styles
gROOT.SetStyle("ATLAS")
gROOT.ForceStyle()
gStyle.SetPalette(54)

## Histogram specifics
nbinsx = 270
nbinsy = 120

large_x_range = [ -200, 1000 ]
large_y_range = [ -200, 200 ]

small_x_range = [ -5, 7 ]
small_y_range = [ -5, 5 ]

## Calling the file and the TTree
file_name = os.path.join( os.environ["HOME_DIRECTORY"], "Data", "Learning", "combined.root" )
file      = TFile.Open( file_name )
tree      = file.Get("var_tree")

## Collecting the means and variances
# x_mean, x_var = get_mean_and_dev( file_name, "var_tree", "MET_Truth_X" )
# y_mean, y_var = get_mean_and_dev( file_name, "var_tree", "MET_Truth_Y" )

## Creating the two 2D histograms which will be filled
hist_1 = TH2D( "large_hist", "large_hist", nbinsx, large_x_range[0], large_x_range[1], nbinsy, large_y_range[0], large_y_range[1] )
# hist_2 = TH2D( "small_hist", "small_hist", nbinsx, small_x_range[0], small_x_range[1], nbinsy, small_y_range[0], small_y_range[1] )

## Filling the two histograms
tree.Draw( "MET_Truth_Y : MET_Truth_X >> large_hist", "", "goff" )
# tree.Draw( " ( MET_Truth_Y - {:f} ) / {:f} : ( MET_Truth_X - {:f} ) / {:f} >> small_hist".format( y_mean , y_var, x_mean, x_var) , "", "goff" )

# hist_1.SetMinimum( 2.0 )
# hist_2.SetMinimum( 2.0 )

## Initialising the canvases
canvas_1 = TCanvas( "large_canvas", 'A basic canvas', 950, 450 )
histo_pad = TPad( "histo_pad", "histo_pad", 0, 0.0, 1.0, 1.0 )
histo_pad.Draw()
histo_pad.cd()
histo_pad.SetLogz()
histo_pad.SetRightMargin(0.1)

# canvas_2 = TCanvas( "small_canvas", 'A basic canvas', 950, 450 )

## Adding the distograms to the canvases
# canvas_1.cd()
# canvas_1.SetLogz()
hist_1.Draw("colz")

# canvas_2.cd()
# canvas_2.SetLogz()
# hist_2.Draw("col1")

## Changing the axis titles
hist_1.GetXaxis().SetTitle( "#it{True E_{//}^{miss} [GeV]}" )
hist_1.GetYaxis().SetTitle( "#it{True E_{#perp}^{miss} [GeV]}" )

# hist_2.GetXaxis().SetTitle( "Standardised True E_{//}^{miss}" )
# hist_2.GetYaxis().SetTitle( "Standardised True E_{#perp}^{miss}" )

## Printing the histograms
canvas_1.Update()
canvas_1.Print("1.pdf")

# canvas_2.Update()
# canvas_2.Print("2.pdf")
#






