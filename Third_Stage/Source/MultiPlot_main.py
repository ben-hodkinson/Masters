import os
import numpy as np

from ROOT import TFile, gROOT, TCanvas, TLegend, THStack, TH1D, gStyle, TMultiGraph, TLine, TColor

from MyPackage import Plotting

def draw_all_text( data_set_name, luminosity, process ):

    ## Adding in some text and the ATLAS Labels
    left    = 0.29
    size    = 1.0
    shift   = 0.06
    pos     = 0.88

    if "Real" in data_set_name:
        Plotting.Draw_ATLASLabel(left, pos, "work in progress", scale=1.0 )
        pos -= shift
        Plotting.Draw_Lumi( left, pos, luminosity, scale=size)
        pos -= shift
        Plotting.Draw_Text( left, pos, process, scale=size)
        pos -= shift
    else:
        Plotting.Draw_ATLASLabel(left, pos, "Simulation", scale=1.0 )
        pos -= shift
        Plotting.Draw_Text( left, pos, "work in progress", scale=size )
        pos -= shift
        Plotting.Draw_Lumi( left, pos, 0, scale=size)
        pos -= shift
        Plotting.Draw_Text( left, pos, process, scale=size )
        pos -= shift

    if "NOJETS"   in data_set_name: Plotting.Draw_Text(left, pos, text="0 Jets", scale=s)
    if "SOMEJETS" in data_set_name: Plotting.Draw_Text(left, pos, text="#geq 1 Jet", scale=s)




def main( extension, network_name, data_set_name, process, luminosity, WP_list, H1D_hist_list, H1D_tail_list, profile_list, tgraph_list, H2D_hist_list ):
    gROOT.SetStyle("ATLAS")
    gROOT.ForceStyle()
    gStyle.SetErrorX( 0.5 )

    OUTPUT_dir    = os.path.join( os.environ["HOME_DIRECTORY"], "Output" )
    hist_dir      = os.path.join( OUTPUT_dir, network_name, data_set_name )
    rootfile_name = os.path.join( hist_dir, "histograms.root" )


    ########## Loading and Plotting the TH1Ds ##########
    for (var, min, max) in H1D_hist_list:

        ## Creating the canvas object
        canvas = TCanvas( "canvas_{}".format( var.name ), 'A basic canvas', 800, 600 )

        ## Adding a legend
        leg = Plotting.Create_Legend( 0.72, 0.60, 0.95, 0.95, show_stats = True )

        ## Superimposing all of the graphs into a stack
        stack = Plotting.Draw_1D_Comp( rootfile_name, var, min, max, WP_list, leg )

        ## If the stack fails then we skip
        if stack == -1:
            continue

        ## Drawing all text
        draw_all_text( data_set_name, luminosity, process )

        ## Updating the canvas
        canvas.Update()

        ## Saving the file
        out_file = "{}/{}_{}.{}".format(hist_dir, data_set_name, var.plotname, extension)
        canvas.Print(out_file)

        del canvas



    ########## Loading and Plotting the Tails ##########
    for (var, min, max) in H1D_tail_list:

        ## Creating the canvas object
        canvas = TCanvas( "canvas_tail_{}".format( var.name ), 'A basic canvas', 800, 600 )

        ## Adding in the legend
        leg = Plotting.Create_Legend( 0.75, 0.65, 0.95, 0.95 )

        ## Superimposing all of the graphs into a stack
        filled_stack = Plotting.Draw_1D_Comp( rootfile_name, var, min, max, WP_list, leg, isTail = True )

        ## If the stack fails then we skip
        if filled_stack == -1:
            continue

        ## Drawing all text
        draw_all_text( data_set_name, luminosity, process )

        ## Adding the legend
        leg.Draw()
        canvas.Update()
        canvas.SetLogy()

        out_file = "{}/{}_{}_tail.{}".format(hist_dir, data_set_name, var.plotname, extension)
        canvas.Print(out_file)

        del canvas



    ########## Profiles and TGraphs ##########
    for (var_x, var_y, min, max) in profile_list + tgraph_list:

        ## Are we looking at a TGraph or a TProfile
        isTgraph = True if (var_x, var_y, min, max) in tgraph_list else False

        ## Creating the canvas object
        canvas = TCanvas( "canvas_{}_{}".format(var_x.name, var_y.name), 'A basic canvas', 800, 600 )

        ## Adding in the legend
        leg = Plotting.Create_Legend( 0.75, 0.65, 0.95, 0.95 )

        ## Superimposing all of the graphs into a stack
        filled_stack = Plotting.Draw_2D_Comp( rootfile_name, var_x, var_y, min, max, WP_list, leg, isTgraph = isTgraph )

        ## If the stack fails then we skip
        if filled_stack == -1:
            continue

        ## Drawing all text
        draw_all_text( data_set_name, luminosity, process )

        ## Updating the canvas
        canvas.Update()

        out_file = "{}/{}_{}_{}.{}".format(hist_dir, data_set_name, var_x.plotname, var_y.plotname, extension)
        canvas.Print(out_file)

        del canvas




    ########## 2D histograms (each as their own) ##########
    for (vx, vy) in H2D_hist_list:
        for wp in WP_list:

            if wp.name not in ["Tight","Network"]:
                continue

            canvas = TCanvas( "{}_{}_{}_2D".format(vx.plotname, vy.plotname, wp.name), 'A basic canvas', 800, 800 )
            # gStyle.SetPalette(53)
            # TColor.InvertPalette()

            graph_name = "2D_{}_vs_{}_{}".format(vx.name, vy.name, wp.name)
            hist = Plotting.GetGraphFromFile( rootfile_name, graph_name )

            ylabel = wp.name + " " + vy.x_label + " " + vy.units

            if wp.name == "Network":
                if network_name == "flat_ann":
                    ylabel = "(A) " + ylabel
                if network_name == "smpl_ann":
                    ylabel = "(B) " + ylabel
                if network_name == "deep_ann":
                    ylabel = "(Z) " + ylabel


            hist.GetXaxis().SetTitle( vx.x_label + " " + vx.units )
            hist.GetYaxis().SetTitle( ylabel )

            # canvas.SetLogz()
            hist.Draw("col")

            if vx.name == "Truth" or "Tghtx":

                xline = TLine(0,0,200,200)
                xline.SetLineColor(1)
                xline.SetLineWidth(2)
                xline.Draw()

                # dline = TLine(0,0,80,-80)
                # dline.SetLineColor(1)
                # dline.SetLineWidth(2)
                # dline.Draw()

            else:

                hist.GetXaxis().SetTitle( "E_{x}^{miss} - E_{x}^{miss, true} " + vx.units )
                hist.GetYaxis().SetTitle( "E_{y}^{miss} - E_{y}^{miss, true} " + vy.units )

                xline = TLine(-50,0,50,0)
                xline.SetLineColor(1)
                xline.SetLineWidth(2)
                xline.Draw()

                yline = TLine(0,-50,0,50)
                yline.SetLineColor(1)
                yline.SetLineWidth(2)
                yline.Draw()


            hist.GetYaxis().SetMaxDigits(3)
            hist.GetXaxis().SetLabelOffset(0.017)

            # Adding in some text and the ATLAS Labels
            leg = Plotting.Create_Legend( 0.19, 0.78, 0.58, 0.94 )
            leg.Draw()

            Plotting.Draw_ATLASLabel(0.2, 0.89, text="Simulation")
            Plotting.Draw_Text(0.2, 0.84, text="work in progress", scale=1)
            Plotting.Draw_Text(0.2, 0.79, text=process, scale=1)

            canvas.Update()

            out_file = "{}/{}_{}.{}".format(hist_dir, data_set_name, graph_name, extension)
            canvas.Print(out_file)

    return 0

if __name__ == '__main__':
    main()














































