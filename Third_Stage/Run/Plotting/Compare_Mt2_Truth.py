import os
import numpy as np
from MyPackage import Plotting
from ROOT import TFile, gROOT, TCanvas, TLegend, THStack, TH1D, gStyle, TMultiGraph, TLine, TColor

def main():

    extension = "pdf"

    luminosity = 44307.4
    ymin = 0
    ymax = 0.024

    data_set_names = [

        ( [ "SZee_all_susy_2l_0jets.root",
        "SZmumu_all_susy_2l_0jets.root"
        ],    "Z#rightarrowll",        860+0, 23 ),

        ( [ "ZZ_1in3_susy_2l_0jets.root" ],         "ZZ#rightarrowll#nu#nu", 416+3, 43 ),
        ( [ "WZ_1in3_susy_2l_0jets.root" ],         "WZ#rightarrowl#null",   416-8, 33 ),
        ( [ "WW_1in3_susy_2l_0jets.root" ],         "WW#rightarrowl#nul#nu", 416+0, 20 ),

        ( [ "ttbar_1in3_susy_2l_0jets.root",
            "top_1in3_susy_2l_0jets.root",
            "antitop_1in3_susy_2l_0jets.root"
          ],    "Top",              800+4, 21 ),


    ]

    DATA_dir      = os.path.join( os.environ["HOME_DIRECTORY"], "Data", "SR_SUSY" )

    ## Creating the plots
    gROOT.SetStyle("ATLAS")
    gROOT.ForceStyle()
    gStyle.SetErrorX( 0.5 )
    canvas = TCanvas( "canvas", 'A basic canvas', 800, 600 )
    # canvas.SetLogy()

    ## Adding in the legend
    leg = Plotting.Create_Legend( 0.60, 0.60, 0.95, 0.95, ncols = 1 )

    ## Creating the stack
    stack = THStack( "stack", "stack" )
    stack.SetMinimum( ymin )
    stack.SetMaximum( ymax )

    ## Cycling through the different datasets
    for datasets, label, colour, style in data_set_names:

        ## Creating the total histogram which will be filled
        myhist = TH1D( label, label, 50, 0, 150 )
        myhist.SetStats(True)
        myhist.StatOverflows(True)

        for dataset in datasets:

            ## The root file is opened
            rootfile_name = os.path.join( DATA_dir, dataset )
            print( rootfile_name )
            root_file = TFile.Open( rootfile_name, 'read' )
            tree = root_file.Get("mt2_Truth")

            ## Creating the current histogram which will be filled
            thishist = TH1D( label, label, 50, 0, 150 )
            thishist.SetStats(True)
            thishist.StatOverflows(True)

            ## Drawing the tree and saving the hist to the matrix
            execution = "mt2>>{}".format( label )
            tree.Draw( execution, "", "goff" )

            thishist.SetDirectory( 0 )
            myhist.Add( thishist )

            ## We are dont with the file
            root_file.Close()
            del root_file

        ## Changing the properties of the histogram
        myhist.Scale( 1 / myhist.Integral("width") )

        myhist.SetLineColor(colour)
        myhist.SetMarkerColor(colour)
        myhist.SetMarkerStyle(style)
        myhist.SetMarkerSize(1.5)

        ## Adding the legend entry
        leg.AddEntry( myhist, label, "p")

        ## Adding the object to the stack
        stack.Add( myhist )
        del myhist

    ## Drawing the stack on the currrent canvas
    stack.Draw( "NOSTACK HIST P" )
    leg.Draw()

    ## Setting axis labels
    stack.GetXaxis().SetTitle( "m_{T2} (True #it{E}_{T}^{miss}) [GeV]"  )
    stack.GetYaxis().SetTitle( "Normalised Distributions" )

    ## Moving axis tick marks
    stack.GetYaxis().SetMaxDigits(3)
    stack.GetXaxis().SetLabelOffset(0.017)

    ## Drawing all text
    left    = 0.2
    size    = 1.0
    shift   = 0.06
    pos     = 0.88
    Plotting.Draw_ATLASLabel(left, pos, "Simulation", scale=1.0 )
    pos -= shift
    Plotting.Draw_Text( left, pos, "work in progress", scale=size )
    pos -= shift
    Plotting.Draw_Lumi( left, pos, 0, scale=size)
    pos -= shift

    ## Updating the canvas
    canvas.Update()

    out_file = "true_strans.{}".format(extension)
    canvas.Print(out_file)

    del canvas

    return 0

if __name__ == '__main__':
    args = main()



