import numpy as np

from MyPackage import Plotting
import StackPlot_main

def main():

    ####### ADJUSTABLE PARAMETERS ########

    extension = "png"

    network_name = "main_ann"
    output_flag  = "Zee" #Zee Zmumu
    output_mod   = ""

    ## Further selections
    veto_all_jets = False

    generator = "PowhegPythia" # PowhegPythia MadGraph Sherpa
    extra_text = ""
    fudge = 1.000          # 1.03997034
    luminosity = 44307.4

    ## Really shitty code to quickly call Z factor
    if output_flag == "Zmumu":
        if generator == "PowhegPythia":
            fudge = 1.100412665
        if generator == "MadGraph":
            fudge = 1.056480382
        if generator == "Sherpa":
            fudge = 1.090844259
    if output_flag == "Zee":
        if generator == "PowhegPythia":
            fudge = 1.043956735
        if generator == "Sherpa":
            fudge = 1.03997034

    ## A list of all the working point variables which will be used for plotting
    Independant_Variable_List = [
        # Plotting.Variable( "NJ", "",    "Number of jets",                "",       ymin = 1.0, ymax = 5.0e9 ),
        # Plotting.Variable( "MU", "",    "Interactions per crossing",   "",       ymin = 10.0, ymax = 1.0e9 ),
        # Plotting.Variable( "LPt", "",   "Leading Lepton Pt",           "[GeV]",  ymin = 1.0, ymax = 1.0e9 ),
        # Plotting.Variable( "LEta", "",  "Leading Lepton #eta",         "[GeV]",  ymin = 1.0, ymax = 1.0e9 ),
        # Plotting.Variable( "SPt", "",   "Subeading Lepton Pt",         "[GeV]",  ymin = 1.0, ymax = 1.0e9 ),
        # Plotting.Variable( "SEta", "",  "Subeading Lepton #eta",       "[GeV]",  ymin = 1.0, ymax = 1.0e9 ),
        # Plotting.Variable( "ZM", "",    "#it{ll} invariant mass",        "[GeV]",  ymin = 10.0, ymax = 1.0e9 ),
        # Plotting.Variable( "ZPt", "",   "Z boson p_{t}",                 "[GeV]",  ymin = 1.0, ymax = 1.0e9 ),
        # Plotting.Variable( "ZEta", "",  "Z boson #eta",                 "",      ymin = 10.0, ymax = 1.0e9 ),
    ]

    Variable_list = [
        # Plotting.Variable( "WPmag",  "Magnitude",  "#it{E_{T}^{miss}}",   "[GeV]", ymin = 1.0, ymax = 1e8),
        Plotting.Variable( "XY",     "Resolution", "#it{E_{x,y}^{miss}}", "[GeV]", ymin = 1.0, ymax = 1e8),
    ]

    ## A list of all the working points used
    WP_list = [
        # Plotting.WorkingPoint( "Truth" ),
        # Plotting.WorkingPoint( "Network" ),
        Plotting.WorkingPoint( "Tight" ),
    ]

    ## A list of all the monte_carlo samples to stack
    if output_flag == "Zee":
        text = "Z #rightarrow ee"

        MC_list = [
            Plotting.Region( "Zee_all_Zee_strict",      432-3, "Z #rightarrow ee",         ),
            Plotting.Region( "Ztt_all_Zee_strict",      616+0, "Z #rightarrow #tau#tau",  ),
            Plotting.Region( "WW_1in3_Zee_strict",      800-5, "WW #rightarrow l#nul#nu", ),
            Plotting.Region( "WZ_1in3_Zee_strict",      800+0, "WZ #rightarrow l#null",   ),
            Plotting.Region( "ZZ_1in3_Zee_strict",      800+7, "ZZ #rightarrow ll#nu#nu", ),
            Plotting.Region( "ttbar_1in3_Zee_strict",   416+3, "t#bar{t}",                ),
            Plotting.Region( "top_1in3_Zee_strict",     416+0, "Single-t",                ),
            Plotting.Region( "antitop_1in3_Zee_strict", 416-9, "Single-#bar{t}",          ),
            ]

        Real_Data = Plotting.Region( "Real_all_Zee_strict", 1, "Data" )

    if output_flag == "Zmumu":
        text = "Z #rightarrow #mu#mu"

        MC_list = [
            Plotting.Region( "Zmumu_all_Zmumu_strict",    432+1, "Z #rightarrow #mu#mu",    ),
            Plotting.Region( "Ztt_all_Zmumu_strict",      616+0, "Z #rightarrow #tau#tau",  ),
            Plotting.Region( "WW_1in3_Zmumu_strict",      800-5, "WW #rightarrow l#nul#nu", ),
            Plotting.Region( "WZ_1in3_Zmumu_strict",      800+0, "WZ #rightarrow l#null",   ),
            Plotting.Region( "ZZ_1in3_Zmumu_strict",      800+7, "ZZ #rightarrow ll#nu#nu", ),
            Plotting.Region( "ttbar_1in3_Zmumu_strict",   416+3, "t#bar{t}",                ),
            Plotting.Region( "top_1in3_Zmumu_strict",     416+0, "Wt",                ),
            Plotting.Region( "antitop_1in3_Zmumu_strict", 416-9, "W#bar{t}",          ),
            ]

        Real_Data = Plotting.Region( "Real_all_Zmumu_strict", 1, "Data" )


    return extension, network_name, luminosity, output_flag, output_mod, veto_all_jets, generator, extra_text, fudge, text, Independant_Variable_List, Variable_list, WP_list, MC_list, Real_Data

if __name__ == '__main__':
    args = main()
    StackPlot_main.main( *args )



