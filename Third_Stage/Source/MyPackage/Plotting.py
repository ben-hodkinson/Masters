import os
import numpy as np

from ROOT import TFile, TGraphErrors, TLatex, TLegend, THStack, TH1D, TMultiGraph, gStyle

OUTPUT_dir = os.path.join( os.environ["HOME_DIRECTORY"], "Output" )

class Variable(object):
    def __init__(self, name, plotname, x_label, units, xmin = None, xmax = None, ymin = None, ymax = None ):

        self.name = name
        self.plotname = plotname
        self.x_label = x_label
        self.units = units

        self.xmin = xmin
        self.xmax = xmax
        self.ymin = ymin
        self.ymax = ymax



class Region(object):
    def __init__(self, file, colour, label, xsec = 1.0, twe = 1.0, kfac = 1.0, filter_eff = 1.0, upscale = 1.0, fudge = 1.0 ):

        self.file = file
        self.colour = colour
        self.label = label

        self.xsec = xsec
        self.twe = twe

        self.upscale = upscale
        self.kfac = kfac
        self.filter_eff = filter_eff

        self.fudge = fudge



class WorkingPoint(object):
    def __init__(self, name, colour = None, marker = None):

        self.name = name

        if colour == None:
            colour = 0
            if name == "Network": colour = 2
            if name == "Tight":   colour = 4
            if name == "Loose":   colour = 3
            if name == "FJVT":    colour = 6
            if name == "Calo":    colour = 7
            if name == "Track":   colour = 28
            self.colour = colour

        if marker == None:
            marker = 0
            if name == "Network": marker = 20
            if name == "Tight":   marker = 21
            if name == "Loose":   marker = 29
            if name == "FJVT":    marker = 22
            if name == "Calo":    marker = 23
            if name == "Track":   marker = 33
            self.marker = marker



## This function opens a histogram from a root file and copies it to the active workspace
def GetGraphFromFile( file_name, graph_name ):

    ## The root file is opened
    root_file = TFile.Open( file_name, 'read' )
    if not root_file:
        print( "\n\n\nNo file named '{}'\n\n\n".format(file_name) )
        return -1

    ## The graph is loaded
    graph = root_file.Get( graph_name )
    if not graph:
        print( "\n\n\nNo object named   '{}'   in file   '{}'\n\n\n".format(graph_name, file_name) )
        return -1

    ## If it is part of a root histogram class it must manually be moved to the current directory
    if not graph.InheritsFrom(TGraphErrors.Class()):
        graph.SetDirectory(0)

    root_file.Close()

    return graph



def Draw_ATLASLabel(x, y, text=None, color=1, scale=1):

    l = TLatex()
    l.SetNDC()
    l.SetTextColor(color)
    string = "#scale[{s}]{{#bf{{#it{{ATLAS}}}} {t}}}".format( s=scale, t=text )
    l.DrawLatex(x, y, string)



def Draw_Text(x, y, text, color=1, scale=1):

    l = TLatex()
    l.SetNDC()
    l.SetTextColor(color)
    string = "#scale[{s}]{{{t}}}".format( s=scale, t=text )
    l.DrawLatex(x, y, string )



def Draw_Lumi(x, y, lumi, unit = "fb^{-1}", **kwargs):
    if lumi == 0:
        lumi_text = "#sqrt{s} = 13 TeV"
    else:
        lumi_text = "#sqrt{{s}} = 13 TeV, {:.1f} {}".format( lumi/1000, unit )
    Draw_Text(x, y, lumi_text, **kwargs )



def Create_Legend( x1, y1, x2, y2, font = 42, text_size = 0.045, fill = 0, border_size = 1, entry_sep = 0, margin = 0.3, show_stats = False, ncols = 1 ):

    leg = TLegend(x1, y1, x2, y2)
    leg.SetTextFont(font)
    leg.SetTextSize(text_size)
    leg.SetFillColor(fill)
    leg.SetBorderSize(border_size)
    leg.SetEntrySeparation(entry_sep)
    leg.SetMargin(margin)
    leg.SetNColumns(ncols)

    if show_stats:
        leg.SetNColumns(2)

        ## Creating headers for the legend
        leg.AddEntry( 0, "", "")
        # leg.AddEntry( 0, "#bf{Mean}", "")
        leg.AddEntry( 0, "#bf{RMSE}", "")

    return leg



def Draw_1D_Comp( rootfile_name, var, min, max, WP_list, leg, isTail = False ):
    gStyle.SetErrorX( 0.5 )

    ## The TStack is created with min and max set
    stack = THStack( "stack", var.name )
    stack.SetMinimum( min )
    stack.SetMaximum( max )

    for wp in WP_list:

        ## Loading the graph using its name and file location
        graph_name = "{}_{}".format(var.name, wp.name)
        if isTail:
            graph_name += "_cumulative"

        graph = GetGraphFromFile( rootfile_name, graph_name )
        if graph == -1:
            continue

        ## Setting the colors specific to the working point
        graph.SetLineColor(wp.colour)
        graph.SetMarkerColor(wp.colour)
        graph.SetMarkerStyle((wp.marker))

        leg.AddEntry( graph, wp.name, "p")

        width = graph.GetBinWidth( 0 )

        ## We check if the legend requires values for statistics
        if leg.GetNColumns() == 2:

            ## Calculate the MEAN and RMSE and add to the legend
            mean  = graph.GetMean()
            stdev = graph.GetStdDev()
            rms   = np.sqrt( mean*mean + stdev*stdev )

            ## Adding the legend entry
            # leg.AddEntry( 0, "{:5.2f}".format(mean), "")
            leg.AddEntry( 0, "{:5.2f}".format(rms), "")

        ## Adding the object to the stack
        stack.Add( graph )
        del graph

    ## Checking to see if any graphs were found for this variable
    if stack.GetNhists() == 0:
        print( "\n\n\nNo graphs found for working point {}\n\n\n".format( wp.name ) )
        return -1

    ## Drawing the stack on the currrent canvas
    stack.Draw( "nostack EP" )
    leg.Draw()

    ## Setting axis labels
    if isTail:
        stack.GetXaxis().SetTitle( var.x_label + " threshold " + var.units )
        stack.GetYaxis().SetTitle( "#it{f}_{tail}" )
    else:
        stack_y_label = "Events per {:.0f} GeV".format(width) if width > 1 else "Events per GeV"

        stack.GetXaxis().SetTitle( var.x_label + " " + var.units )
        stack.GetYaxis().SetTitle( stack_y_label )

    ## Moving axis tick marks
    stack.GetYaxis().SetMaxDigits(3)
    stack.GetXaxis().SetLabelOffset(0.017)

    return stack




def Draw_2D_Comp( rootfile_name, var_x, var_y, min, max, WP_list, leg, isTgraph = False ):
    gStyle.SetErrorX(0.5)

    ## Creating a multigraph for the canvas if we are looking at TGraphs
    if isTgraph:
        stack = TMultiGraph()
        args = "AP"

    ## Creating a stack for the canvas if we are looking at TPofiles
    else:
        stack = THStack("stack", var_x.name + var_y.name)
        args = "nostack "

    stack.SetMinimum( min )
    stack.SetMaximum( max )

    for wp in WP_list:

        ## Generating the graph name
        graph_name = "{}_vs_{}_{}".format(var_x.name, var_y.name, wp.name)
        if isTgraph:
            graph_name += "_res"

        ## Loading the graph using its name and file location
        graph = GetGraphFromFile( rootfile_name, graph_name )
        if graph == -1:
            continue

        ## Setting the colors specific to the working point
        graph.SetLineColor(wp.colour)
        graph.SetMarkerColor(wp.colour)
        graph.SetMarkerStyle(wp.marker)

        ## Adding the legend entry
        leg.AddEntry( graph, wp.name, "p")

        ## Adding the object to the stack
        stack.Add( graph )
        del graph

    ## Checking to see if any graphs were found for this variable
    nhists = stack.GetListOfGraphs().GetSize() if isTgraph else stack.GetNhists()
    if nhists == 0:
        print( "\n\n\nNo graphs found for working point {}\n\n\n".format( wp.name ) )
        return -1

    ## Drawing the stack on the currrent canvas
    stack.Draw( args )
    leg.Draw()

    ## Setting axis labels
    stack.GetXaxis().SetTitle( var_x.x_label + " " + var_x.units )
    stack.GetYaxis().SetTitle( var_y.x_label + " " + var_y.units )

    ## Moving axis tick marks
    stack.GetYaxis().SetMaxDigits(3)
    stack.GetXaxis().SetLabelOffset(0.017)

    return stack




































