import os
import numpy as np
from MyPackage import Plotting
from ROOT import TFile, gROOT, TCanvas, TLegend, THStack, TH1D, gStyle, TMultiGraph, TLine, TColor

def main():

    extension = "pdf"

    ####### File names and locations ########
    network_name = "deep_ann"
    data_set_name = "ttbar_1in3_tt_strict" #_NOJETS _SOMEJETS
    ######################################

    process    = "#it{t#bar{t}}"
    luminosity = 44307.4

    ## Initialising plotting variables
    Var_Lin   = Plotting.Variable( "Lin",   "Linear_Response",    "#Delta_{T}^{lin}",                               "",     )
    Var_Truth = Plotting.Variable( "Truth", "TrueET",             "True #it{E}_{T}^{miss}",                         "[GeV]" )

    ymin = -0.2
    ymax =  0.5

    plot_list = [
        ("main_ann", "Tight",   "",     4, 21),
        ("main_ann", "Network", "",     2, 20),
        ("flat_ann", "Network", "(A) ", 3, 22),
        ("smpl_ann", "Network", "(B) ", 6, 23),
    ]


    ## Creating the plots
    gROOT.SetStyle("ATLAS")
    gROOT.ForceStyle()
    gStyle.SetErrorX( 0.5 )
    canvas = TCanvas( "canvas_{}_{}".format(Var_Truth.name, Var_Lin.name), 'A basic canvas', 800, 600 )

    ## Adding in the legend
    leg = Plotting.Create_Legend( 0.65, 0.70, 0.95, 0.95 )

    ## Creating the stack
    stack = THStack("stack", Var_Truth.name + Var_Lin.name)
    stack.SetMinimum( ymin )
    stack.SetMaximum( ymax )

    ## Cycling through the different network directories
    for network_name, wp_name, prefix, colour, marker in plot_list:

        OUTPUT_dir    = os.path.join( os.environ["HOME_DIRECTORY"], "Output" )
        hist_dir      = os.path.join( OUTPUT_dir, network_name, data_set_name )
        rootfile_name = os.path.join( hist_dir, "histograms.root" )

        ## Creating the working point variable
        wp = Plotting.WorkingPoint( wp_name )

        ## Generating the graph name
        graph_name = "{}_vs_{}_{}".format(Var_Truth.name, Var_Lin.name, wp.name)

        ## Loading the graph using its name and file location
        graph = Plotting.GetGraphFromFile( rootfile_name, graph_name )
        if graph == -1:
            continue

        ## Setting the colors specific to the working point
        graph.SetLineColor(colour)
        graph.SetMarkerColor(colour)
        graph.SetMarkerStyle(marker)

        ## Adding the legend entry
        leg.AddEntry( graph, prefix + wp.name, "p")

        ## Adding the object to the stack
        stack.Add( graph )
        del graph

    ## Drawing the stack on the currrent canvas
    stack.Draw( "nostack" )
    leg.Draw()

    ## Setting axis labels
    stack.GetXaxis().SetTitle( Var_Truth.x_label + " " + Var_Truth.units )
    stack.GetYaxis().SetTitle( Var_Lin.x_label + " " + Var_Lin.units )

    ## Moving axis tick marks
    stack.GetYaxis().SetMaxDigits(3)
    stack.GetXaxis().SetLabelOffset(0.017)

    ## Drawing all text
    left    = 0.29
    size    = 1.0
    shift   = 0.06
    pos     = 0.88

    Plotting.Draw_ATLASLabel(left, pos, "Simulation", scale=1.0 )
    pos -= shift
    Plotting.Draw_Text( left, pos, "work in progress", scale=size )
    pos -= shift
    Plotting.Draw_Lumi( left, pos, 0, scale=size)
    pos -= shift
    Plotting.Draw_Text( left, pos, process, scale=size )
    pos -= shift

    ## Updating the canvas
    canvas.Update()

    out_file = "response_comparison_zoom.{}".format(extension)
    canvas.Print(out_file)

    del canvas

    return 0

if __name__ == '__main__':
    args = main()



