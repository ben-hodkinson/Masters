import os
import numpy as np
import pandas as pd
from sklearn import metrics
from ROOT import TFile, TTree, TCanvas, gROOT, TH1D, gDirectory, TGraph

from MyPackage import Plotting

gROOT.SetStyle("ATLAS")
gROOT.ForceStyle()


######## The data set class must be defined for PyTorch to be able to read the data ########
class RootDataSet(object):
    def __init__( self, infile_name, net_tree_name ):
        self.root_file     = TFile.Open( infile_name )
        self.var_tree      = self.root_file.Get("var_tree")
        self.net_tree      = self.root_file.Get(net_tree_name)
        self.n_events      = self.var_tree.GetEntries()

    def __len__(self):
        return self.n_events

    def __getitem__(self, idx):
        self.var_tree.GetEntry(idx)
        self.net_tree.GetEntry(idx)

        return [ self.var_tree.MET_TST_Tight_ET, self.var_tree.MET_SIG, self.net_tree.WP_ANN_ET ]


########################################################################

## Data location
signal_name  = os.path.join( os.environ["HOME_DIRECTORY"], "Data", "SR_Zmumu", "ZZ_1in3_Zmumu_strict.root" )
bckgnd_name  = os.path.join( os.environ["HOME_DIRECTORY"], "Data", "SR_Zmumu", "SZmumu_all_Zmumu_strict.root" )

exclusive = True
n_samples = 100000 #22976

extension = "png"

########################################################################

## Initialise the files
signal_sample = RootDataSet( signal_name, "main_ann" )
bckgnd_sample = RootDataSet( bckgnd_name, "main_ann" )

print( len(signal_sample) )
print( len(bckgnd_sample) )

## Building the arrays
label   = []
tight   = []
signi   = []
network = []
for idx in range(n_samples):

    if idx < len(signal_sample):
        signal_event = signal_sample[idx]
        if exclusive and signal_event[0] < 50:
            continue
        label.append(   1   )
        tight.append(   signal_event[0]   )
        signi.append(   signal_event[1]   )
        network.append( signal_event[2]   )

    if idx < len(bckgnd_sample):
        bckgnd_event = bckgnd_sample[idx]
        if exclusive and bckgnd_event[0] < 50:
            continue
        label.append(   0   )
        tight.append(   bckgnd_event[0]   )
        signi.append(   bckgnd_event[1]   )
        network.append( bckgnd_event[2]   )

## Calculating the area under the curve
print( "Tight:           ", metrics.roc_auc_score(label, tight) )
print( "Significance:    ", metrics.roc_auc_score(label, signi) )
print( "Network:         ", metrics.roc_auc_score(label, network) )

## Creating the ROC plot arrays (FPR, TPR)
tight_fpr, tight_tpr, _       = metrics.roc_curve(label,   tight   )
signi_fpr, signi_tpr, _       = metrics.roc_curve(label,   signi   )
network_fpr, network_tpr, _   = metrics.roc_curve(label,   network )

## Correcting the fpr into a backround rejection rate
tight_bck_rej    = 1 - tight_fpr
signi_bck_rej    = 1 - signi_fpr
network_bck_rej  = 1 - network_fpr

## Converting the lines to tgraphs
tight_gr   = TGraph( len(tight_tpr),   tight_tpr,   tight_bck_rej   )
signi_gr   = TGraph( len(signi_tpr),   signi_tpr,   signi_bck_rej   )
network_gr = TGraph( len(network_tpr), network_tpr, network_bck_rej )

## Drawing the plots using ROOT
canvas = TCanvas( "canvas", 'A basic canvas', 600, 600 )
leg = Plotting.Create_Legend( 0.2, 0.2, 0.7, 0.40, border_size=0, margin=0.1 )

tight_gr.Draw( 'AL' )
signi_gr.Draw( 'L' )
network_gr.Draw( 'L' )

leg.AddEntry( tight_gr, "Tight #it{E}_{T}^{miss}", "l")
leg.AddEntry( signi_gr, "Object-based #it{E}_{T}^{miss} Significance", "l")
leg.AddEntry( network_gr, "Network #it{E}_{T}^{miss}", "l")

## Changing the colours
tight_gr.SetLineColor(4)
signi_gr.SetLineColor(801)
network_gr.SetLineColor(2)
tight_gr.SetLineWidth(2)
signi_gr.SetLineWidth(2)
network_gr.SetLineWidth(2)

canvas.SetGrid( 1, 0 )
tight_gr.GetXaxis().SetTitle( "Signal Efficiency" )
tight_gr.GetYaxis().SetTitle( "Background Rejection" )
tight_gr.GetXaxis().SetLimits(0, 1)
tight_gr.SetMinimum(0.0)
tight_gr.SetMaximum(1.0)

## Adding in some text and the ATLAS Labels
s       = 1.0
shift   = 0.06
pos     = 0.70

Plotting.Draw_ATLASLabel(0.20, pos, "Simulation", scale=1.0 )
pos -= shift
Plotting.Draw_Text( 0.20, pos, "work in progress", scale=s )
pos -= shift
Plotting.Draw_Lumi( 0.20, pos, 0, scale=s)
pos -= shift
Plotting.Draw_Text( 0.20, pos, "Z #rightarrow #mu#mu selection", scale=0.8)
if exclusive:
    pos -= shift
    Plotting.Draw_Text( 0.20, pos, "Tight #it{E}_{T}^{miss} > 50 GeV", scale=0.8)

leg.Draw()

canvas.Update()


out_file = "roc_plot3"
if exclusive: out_file += "_ex"
out_file += "." + extension
canvas.Print(out_file)















