import numpy as np

from MyPackage import Evaluation
import CreateHistograms_main

def main():

    ####### File names and locations ########

    network_name = "smpl_ann"

    do_weights = True
    general_weights  = [ "event_weight", "pile_up_weight", "cross_section", "kfactor", "filter_eff", "1.0/twe", "upscale" ]
    electron_weights = [ "tot_elec_sf", "trg_elec_sf" ] # "tot_elec_sf", "trg_elec_sf"
    muon_weights     = [ "tot_muon_sf", "trg_muon_sf" ] # "tot_muon_sf", "trg_muon_sf"

    data_files = [

        # ( "Evaluation", "ttbar_1in3_top_lax.root"  ),

        # ( "SR_WW",      "WW_1in3_WW_strict.root"    ),
        # ( "SR_Higgs",   "H125_all_WW_strict.root"   ),
        ( "SR_ttbar",   "ttbar_1in3_tt_strict.root"   ),

        # ( "Real", "Real_all_Zee_strict.root" ),

        # ( "SR_Zee", "Zee_all_Zee_strict.root"      ),
        # ( "SR_Zee", "SZee_all_Zee_strict.root"     ),

        # ( "SR_Zee", "Ztt_all_Zee_strict.root"      ),
        # ( "SR_Zee", "ZZ_1in3_Zee_strict.root"      ),
        # ( "SR_Zee", "WZ_1in3_Zee_strict.root"      ),
        # ( "SR_Zee", "WW_1in3_Zee_strict.root"      ),
        # ( "SR_Zee", "top_1in3_Zee_strict.root"     ),
        # ( "SR_Zee", "antitop_1in3_Zee_strict.root" ),
        # ( "SR_Zee", "ttbar_1in3_Zee_strict.root"   ),

        # ( "Real", "Real_all_Zmumu_strict.root" ),

        # ( "SR_Zmumu", "Zmumu_all_Zmumu_strict.root"    ),
        # ( "SR_Zmumu", "SZmumu_all_Zmumu_strict.root"   ),
        # ( "SR_Zmumu", "MGZmumu_all_Zmumu_strict.root"  ),

        # ( "SR_Zmumu", "Ztt_all_Zmumu_strict.root"      ),
        # ( "SR_Zmumu", "ZZ_1in3_Zmumu_strict.root"      ),
        # ( "SR_Zmumu", "WZ_1in3_Zmumu_strict.root"      ),
        # ( "SR_Zmumu", "WW_1in3_Zmumu_strict.root"      ),
        # ( "SR_Zmumu", "top_1in3_Zmumu_strict.root"     ),
        # ( "SR_Zmumu", "antitop_1in3_Zmumu_strict.root" ),
        # ( "SR_Zmumu", "ttbar_1in3_Zmumu_strict.root"   ),

        ]

    ## Histogram parameters
    no_true_comp = False
    nbins        = 41

    ## Further selections
    veto_all_jets = False
    require_jets  = False

    ######################################


    ## Initialising evaluation variables
    ## Etmiss variables
    Var_X     = Evaluation.Variable( "X",    nbins, -200.0, 200.0, reso = True )
    Var_Y     = Evaluation.Variable( "Y",    nbins, -200.0, 200.0, reso = True )
    Var_D     = Evaluation.Variable( "D",    80,  0.0,   250.0, tail = True )
    Var_R     = Evaluation.Variable( "R",    nbins, -200.0, 200.0 )
    Var_Az    = Evaluation.Variable( "AZ",   nbins, -10.0, 10.0 )

    Var_Lin   = Evaluation.Variable( "Lin",  nbins, -1.0,   1.0 )
    Var_Phi   = Evaluation.Variable( "Phi",  nbins, -np.pi, np.pi, reso = True )
    Var_XY    = Evaluation.Variable( "XY",   nbins, -200.0, 200.0, reso = True )

    ## Independant Variables
    Var_PU    = Evaluation.Variable( "PU",    20,   20.0,  40.0, tree = "var_tree", branch = "NPV_2Tracks" )
    Var_MU    = Evaluation.Variable( "MU",    30,   10.0,  80.0, tree = "var_tree", branch = "mu_act_cross" )
    Var_Sum   = Evaluation.Variable( "Sum",   30,   200.0, 1200.0, tree = "var_tree", branch = "MET_TST_Tight_SumET" )
    Var_Truth = Evaluation.Variable( "Truth", 30,   0.0,   300.0, tree = "var_tree", branch = "MET_Truth_ET" )
    Var_Tight = Evaluation.Variable( "Tghtx", 50,   0.0,   300.0, tree = "var_tree", branch = "MET_TST_Tight_ET" )

    Var_NJ    = Evaluation.Variable( "NJ",    16,    0.0,  16.0, tree = "var_tree", branch = "N_Jets" )

    Var_LPt   = Evaluation.Variable( "LPt",   nbins, 15.0, 300.0, tree = "alt_tree", branch = "lead_l_pt")
    Var_LEta  = Evaluation.Variable( "LEta",  nbins, -2.8, 2.8,   tree = "alt_tree", branch = "lead_l_eta" )
    Var_SPt   = Evaluation.Variable( "SPt",   nbins, 15.0, 300.0, tree = "alt_tree", branch = "lead_l_pt" )
    Var_SEta  = Evaluation.Variable( "SEta",  nbins, -2.8, 2.8,   tree = "alt_tree", branch = "lead_l_eta" )

    Var_ZM    = Evaluation.Variable( "ZM",   nbins, 71.0,  111.0, tree = "alt_tree", branch = "ll_inv_mass" )
    Var_ZPt   = Evaluation.Variable( "ZPt",  30,     0.0,  300.0, tree = "alt_tree", branch = "ll_pt" )
    Var_ZEta  = Evaluation.Variable( "ZEta", nbins, -10.0, 10.0,  tree = "alt_tree", branch = "ll_eta" )

    ## Placeholder for working point
    Var_WP    = Evaluation.Variable( "WPmag", nbins, 0.0, 250.0, tail = True )

    Var_X.TH2D_params( 81, -70, 70 )
    Var_Y.TH2D_params( 81, -70, 70 )
    Var_R.TH2D_params( 120, -120, 120 )
    Var_WP.TH2D_params( 100, 0, 200 )
    Var_Truth.TH2D_params( 100, 0, 200 )
    Var_Tight.TH2D_params( 100, 0, 200 )
    Var_Lin.TH2D_params( 100, -1, 2 )

    ## A list of all the working points used and which branch/tree they belong to
    WorkingP_List = [
        Evaluation.WorkingPoint( "Network" ),
        Evaluation.WorkingPoint( "Tight" ),
        Evaluation.WorkingPoint( "Loose" ),
        Evaluation.WorkingPoint( "FJVT" ),
        Evaluation.WorkingPoint( "Calo" ),
        Evaluation.WorkingPoint( "Track" ),
        # Evaluation.WorkingPoint( "Truth" ),
        ]

    Independant_Variable_List = [
        # Var_NJ,
        # Var_PU,
        # Var_MU,
        # Var_LPt,
        # Var_LEta,
        # Var_SPt,
        # Var_SEta,
        # Var_ZM,
        # Var_ZEta,
        # Var_ZPt
    ]

    Histogram_Variable_List = [
        # Var_XY,
        # Var_WP,
        # Var_R,
        # Var_D,
        ]

    Profile_List = [
        # ( Var_Truth, Var_Lin ),
        # ( Var_Tight, Var_Lin ),

        # ( Var_Truth, Var_Phi ),

        ( Var_Truth, Var_XY ),
        ( Var_PU,    Var_XY ),
        ( Var_MU,    Var_XY ),
        ( Var_Sum,   Var_XY ),

        # ( Var_ZPt,   Var_Az ),
        ]

    TH2D_List = [
        # ( Var_Truth, Var_WP ),
        # ( Var_Tight, Var_WP ),

        # ( Var_Truth, Var_R ),
        # ( Var_Tight, Var_R ),

        # ( Var_X, Var_Y ),
        ]


    return ( network_name, do_weights, general_weights, electron_weights, muon_weights, data_files, no_true_comp, nbins,
             veto_all_jets, require_jets, WorkingP_List, Independant_Variable_List, Histogram_Variable_List, Profile_List, TH2D_List )


if __name__ == '__main__':
    args = main()
    CreateHistograms_main.main( *args )
