import numpy as np

from MyPackage import Plotting
import MultiPlot_main

def main():

    extension = "pdf"

    ####### File names and locations ########
    network_name = "smpl_ann"
    data_set_name = "ttbar_1in3_tt_strict" #_NOJETS _SOMEJETS
    ######################################

    process    = "#it{t#bar{t}}"
    luminosity = 44307.4

    ## Initialising plotting variables
    Var_X     = Plotting.Variable( "X",     "X_Resolution",       "#it{E}_{x}^{miss} Resolution",                  "[GeV]" )
    Var_Y     = Plotting.Variable( "Y",     "Y_Resolution",       "#it{E}_{y}^{miss} Resolution",                  "[GeV]" )
    Var_R     = Plotting.Variable( "R",     "Scalar_Distance",    "#it{E}_{T}^{miss} - True #it{E}_{T}^{miss}",    "[GeV]" )
    Var_D     = Plotting.Variable( "D",     "Euclidean_Distance", "|#bf{E}_{T}^{miss} - True #bf{E}_{T}^{miss}|",  "[GeV]" )
    Var_Phi   = Plotting.Variable( "Phi",   "Angular_Resolution", "#it{E}_{#phi}^{miss} Resolution",               "[rad]" )
    Var_Lin   = Plotting.Variable( "Lin",   "Linear_Response",    "#Delta_{T}^{lin}",                               "",     )
    Var_XY    = Plotting.Variable( "XY",    "Resolution",         "#it{E}_{x,y}^{miss} Resolution",                 "[GeV]" )
    Var_PU    = Plotting.Variable( "PU",    "PileUp",             "Number of vertices (2 tracks)",                  "",     )
    Var_MU    = Plotting.Variable( "MU",    "ActCross",           "Number of interactions #mu",                     "",     )
    Var_Truth = Plotting.Variable( "Truth", "TrueET",             "True #it{E}_{T}^{miss}",                         "[GeV]" )
    Var_Tight = Plotting.Variable( "Tghtx", "TightET",            "Tight #it{E}_{T}^{miss}",                        "[GeV]" )
    Var_Sum   = Plotting.Variable( "Sum",   "SumET",              "#Sigma E_{T}",                                   "[GeV]" )
    Var_Az    = Plotting.Variable( "AZ",    "ZProjec",            "#LT #bf{E}_{T}^{miss} #upoint #bf{A}_{Z} #GT",   "[GeV]" )
    Var_ZPt   = Plotting.Variable( "ZPt",   "ZPt",                "#it{p}_{T}^{Z}",                                 "[GeV]" )
    Var_WP    = Plotting.Variable( "WPmag", "WPmag",              "#it{E}_{T}^{miss}",                              "[GeV]" )

    ## A list of all the working points used
    WP_list = [
        Plotting.WorkingPoint( "Track" ),
        Plotting.WorkingPoint( "Calo" ),
        Plotting.WorkingPoint( "FJVT" ),
        Plotting.WorkingPoint( "Loose" ),
        Plotting.WorkingPoint( "Tight" ),
        Plotting.WorkingPoint( "Network" ),
    ]

    ## A list of all the plots that are created
    H1D_hist_list = [
        # (Var_XY, 0, 1.0e7),
        # (Var_R, 0, 9e3),
        # (Var_D, 0, 2e6),
        ]

    H1D_tail_list = [
        # (Var_D, 1e-4, 100),
        ]

    profile_list = [
        # ( Var_Truth, Var_Lin, -0.6, 1 ),
        # ( Var_Tight, Var_Lin, -0.6, 1 ),
        # ( Var_ZPt, Var_Az, -100, 50 ),
        ]

    tgraph_list = [
        # ( Var_Truth, Var_Phi, 0, 2 ),

        ( Var_Truth, Var_XY, 15, 50 ),
        ( Var_PU,    Var_XY, 20, 85 ),
        ( Var_MU,    Var_XY, 20, 85 ),
        ( Var_Sum,   Var_XY, 15, 85 ),
        ]

    H2D_hist_list = [
        # ( Var_Truth, Var_WP ),
        # ( Var_Tight, Var_WP ),

        # ( Var_Truth, Var_R ),

        # ( Var_X, Var_Y ),
        ]

    return extension, network_name, data_set_name, process, luminosity, WP_list, H1D_hist_list, H1D_tail_list, profile_list, tgraph_list, H2D_hist_list

if __name__ == '__main__':
    args = main()
    MultiPlot_main.main( *args )
