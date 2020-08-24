import os
import array as arr
import numpy as np

from ROOT import TFile, gROOT, TCanvas, TLegend, THStack, TPad, TLine, TF1, TStyle, gStyle, TH1

from MyPackage import Plotting

def STACK( extension, OUTPUT_dir, network_name, luminosity, output_flag, output_mod, generator, extra_text, fudge, text, MC_list, Real_Data, var, wp = None ):


    is_wp = False if wp is None else True

    ## The name of the histogram to be used
    hist_name = "{}_{}".format(var.name, wp.name) if is_wp else var.name

    ## Creating the canvas
    canvas = TCanvas( "canvas" + hist_name, 'A basic canvas', 800, 800 )

    ## Creating the first pad to contain the basic histogram plots
    histo_pad = TPad( "histo_pad", "histo_pad", 0, 0.25, 1.0, 1.0 )
    histo_pad.SetBottomMargin(0)
    histo_pad.Draw()
    histo_pad.cd()
    # if is_wp:
    histo_pad.SetLogy()

    ## Adding in the legend
    leg = Plotting.Create_Legend( 0.70, 0.45, 0.95, 0.95, text_size = 0.045 )

    ## Creating a stack for the canvas
    stack = THStack("stack", var.name)

    hist_list = []
    integral_list = []
    for s, mc in enumerate(MC_list):

        rootfile_name = os.path.join( OUTPUT_dir, network_name, mc.file, "histograms.root" )

        hist = Plotting.GetGraphFromFile( rootfile_name, hist_name )
        if hist == -1:
            continue

        hist.Scale( fudge * luminosity )
        hist.SetMarkerColor(mc.colour)
        hist.SetFillColor(mc.colour)
        hist.SetLineColor(1)
        hist.SetLineWidth(1)
        hist.SetMarkerStyle(0)

        leg.AddEntry( hist, mc.label, "f")
        integral_list.append( hist.Integral(0,hist.GetNbinsX()+1) )

        hist.Scale( 1, "width" )
        hist_list.append( hist )

        if s == 0:
            MC_Total = hist.Clone()
        else:
            MC_Total.Add( hist.Clone() )

        del hist

    ## If the list is empty and no histograms were found then we skip the rest
    if not integral_list and is_wp:
        print( "No MC histograms found for working point {}\n".format( wp.name ) )
        return 0

    ## Now we use the integrals to sort the histograms in descending order
    ord_idx = np.argsort( integral_list )

    ## Adding the histograms to the stack
    for i in ord_idx:
        # Draw the object on the stack
        stack.Add( hist_list[i] )

    stack.Draw( "HIST" )
    stack.GetYaxis().SetTitleOffset(1.2)

    ## Adding the MC Statistical, lumi and xsec uncertainty
    for i in range( 1, MC_Total.GetNbinsX() + 1 ):
        if MC_Total.GetBinContent(i) > 0:
            e = MC_Total.GetBinError(i)
            h = MC_Total.GetBinContent(i)
            MC_Total.SetBinError( i, h * np.sqrt( (e/h)**2 + 0.05**2 + 0.02**2 ) )

    MC_Total.SetMarkerSize(0.)
    MC_Total.SetFillStyle(3001)
    MC_Total.SetMarkerColor(1)
    MC_Total.SetFillColor(2)
    MC_Total.Draw( "E2 SAME" )

    ## Adding the data points
    rootfile_name = os.path.join( OUTPUT_dir, network_name, Real_Data.file, "histograms.root" )

    real_hist = Plotting.GetGraphFromFile( rootfile_name, hist_name )

    real_hist.SetLineColor(1)
    real_hist.SetMarkerColor(Real_Data.colour)
    real_hist.SetMarkerStyle(20)
    real_hist.SetMarkerSize(1.2)
    real_int = real_hist.Integral(0,real_hist.GetNbinsX()+1)

    leg.AddEntry( real_hist, Real_Data.label, "p")
    real_hist.Scale( 1, "width" )
    real_hist.Draw("e1p SAME")

    print( "Real events  =  {:}".format( real_int ) )
    print( "MC events    =  {:}".format( sum(integral_list) ) )
    print( "Fudge Factor =  {:10.10}".format( real_int / sum(integral_list) ) )

    # print( "Z events     =  {:}".format( sum(integral_list[:3]) ) )
    # print( "Non Z events =  {:}".format( sum(integral_list[3:]) ) )
    # print( "Z Factor     =  {:10.10}".format( ( real_int - sum(integral_list[3:]) ) / sum(integral_list[:3]) ) )

    ## Adding in some text and the ATLAS Labels
    # if fudge != 1: text += " (#uparrow{:.2f}%)".format(fudge*100-100)
    s = 1.0
    pos = 0.88
    shift = 0.05

    Plotting.Draw_ATLASLabel(0.20, pos, "work in progress", scale=1.0 )
    pos -= shift+0.01
    Plotting.Draw_Lumi( 0.20, pos, luminosity, scale=s)
    pos -= shift
    Plotting.Draw_Text( 0.20, pos, text, scale=s )
    pos -= shift
    Plotting.Draw_Text( 0.20, pos, generator, scale=s )
    Plotting.Draw_Text( 0.55, 0.8, extra_text )
    leg.Draw()


    ## Plotting axis and limits
    stack.GetYaxis().SetTitle( "Events per GeV" )

    if var.ymin is not None: stack.SetMinimum( var.ymin )
    if var.ymax is not None: stack.SetMaximum( var.ymax )
    if var.xmin is not None and var.xmax is not None: stack.GetXaxis().SetLimits( var.xmin, var.xmax )


    ## Creating the ratio pad
    canvas.cd()
    ratio_pad = TPad( "ratio_pad", "ratio_pad", 0, 0, 1.0, 0.25 )
    ratio_pad.SetTopMargin(0.0)
    ratio_pad.SetBottomMargin(0.3)
    ratio_pad.Draw()
    ratio_pad.cd()
    ratio_pad.SetGrid( 0, 1 )

    ## Plotting the the MC error histogram
    MC_flat = MC_Total.Clone()

    for i in range( 1, MC_Total.GetNbinsX() + 1 ):
        MC_flat.SetBinContent( i, 1 )
        if MC_Total.GetBinContent(i) > 0:
            MC_flat.SetBinError( i, MC_Total.GetBinError(i) / MC_Total.GetBinContent(i) )

    MC_flat.SetMarkerSize(0.)
    MC_flat.SetFillStyle(3001)
    MC_flat.SetMarkerColor(1)
    MC_flat.SetFillColor(2)
    MC_flat.SetMaximum(1.6)
    MC_flat.SetMinimum(0.4)
    MC_flat.Draw( "E2" )
    leg.AddEntry( MC_flat, "MC stat unc", "f" )

    ## Plotting the ratio of MC to Data
    ratio_hist = real_hist.Clone()
    mcratio_hist = MC_Total.Clone()
    mcratio_hist.SetError(arr.array('d',[0]))
    ratio_hist.Divide( mcratio_hist )
    ratio_hist.Draw("ep SAME")

    ## Plotting axis and limits
    if var.xmin is not None and var.xmax is not None: ratio_hist.GetXaxis().SetRange( var.xmin, var.xmax )

    x_title = wp.name + " " + var.x_label + " " + var.units if is_wp else var.x_label + " " + var.units
    MC_flat.GetXaxis().SetTitle( x_title )
    MC_flat.GetXaxis().SetTitleSize(25)
    MC_flat.GetXaxis().SetTitleFont(43)
    MC_flat.GetXaxis().SetTitleOffset(4)
    MC_flat.GetXaxis().SetLabelFont(43)
    MC_flat.GetXaxis().SetLabelSize(20)

    MC_flat.GetYaxis().SetTitle( "Data/MC" )
    MC_flat.GetYaxis().SetNdivisions(206, False)
    MC_flat.GetYaxis().ChangeLabel( -1, -1, -1, -1, -1, -1, " " )
    MC_flat.GetYaxis().ChangeLabel(  1, -1, -1, -1, -1, -1, " " )
    MC_flat.GetYaxis().SetTitleSize(25)
    MC_flat.GetYaxis().SetTitleFont(43)
    MC_flat.GetYaxis().SetTitleOffset(1.55)
    MC_flat.GetYaxis().SetLabelFont(43)
    MC_flat.GetYaxis().SetLabelSize(20)

    ## Adding in a line on the x-axis
    line = TLine( ratio_hist.GetBinLowEdge(1) , 1, ratio_hist.GetBinLowEdge(ratio_hist.GetNbinsX()+1), 1)
    line.SetLineWidth(1)
    line.SetLineColor(1)
    line.SetLineStyle(9)
    line.Draw()


    ## Saving the canvas
    canvas.Update()
    out_folder = "{}/{}/Stack".format( OUTPUT_dir, network_name )
    if not os.path.exists( out_folder ):
        os.system( "mkdir -p {}".format(out_folder) )

    out_file = os.path.join( out_folder, "{}_{}".format( output_flag, var.name ) )
    if is_wp: out_file += "_" + wp.name
    out_file += "_{}".format( generator )
    if "0 Jets" in extra_text: out_file += "_NOJETS"
    out_file += "{}.{}".format( output_mod, extension )

    canvas.Print(out_file)




def main( extension, network_name, luminosity, output_flag, output_mod, veto_all_jets, generator, extra_text, fudge, text, Independant_Variable_List, Variable_list, WP_list, MC_list, Real_Data ):
    gROOT.SetStyle("ATLAS")
    gROOT.ForceStyle()
    gStyle.SetErrorX( 0.5 )
    TH1.SetDefaultSumw2()

    OUTPUT_dir = os.path.join( os.environ["HOME_DIRECTORY"], "Output" )

    ## Changing the name of the input file to match the generator
    if generator == "PowhegPythia":
        pass
    elif generator == "Sherpa":
        MC_list[0].file = "S" + MC_list[0].file
    elif generator == "MadGraph":
        MC_list[0].file = "MG" + MC_list[0].file
    else:
        print( "\n\n\nUnknown Generator option\n\n\n")
        return 0

    ## Changing the name of the input files to match the jet selection
    if veto_all_jets:
        for mc in MC_list:
            mc.file = mc.file + "_NOJETS"
        Real_Data.file = Real_Data.file + "_NOJETS"
        extra_text += "0 Jets"


    ########## Plotting the independant variables ##########
    for ivar in Independant_Variable_List:
        STACK( extension, OUTPUT_dir, network_name, luminosity, output_flag, output_mod, generator, extra_text, fudge, text, MC_list, Real_Data, ivar, None )

    ########## Loading and Plotting the TH1Ds ##########
    for var in Variable_list:
        for wp in WP_list:

            STACK( extension, OUTPUT_dir, network_name, luminosity, output_flag, output_mod, generator, extra_text, fudge, text, MC_list, Real_Data, var, wp )


    return 0

if __name__ == '__main__':
    main()




























