import os
import commands

this_directory = "/home/mleigh/workdir/Second_Stage/"

class AnalysisControl( object ):
    def __init__( self, base_dir, file_locations, input_commands, extra = "" ):
        self.base_dir       = base_dir
        self.file_locations = file_locations
        self.input_commands = input_commands
        self.extra = extra

        os.chdir( this_directory )
        os.system( "source setup.sh" )
        os.chdir( "src/" )

    def run_analysis( self ):

        for ( folder, label, cut, selection ) in ( self.input_commands ):
            print("\n\n")
            print( "################## {l:^15} - {c:^15} - {s:^15} ##################".format( l = label, c = cut, s = selection ) )

            input_names  = self.__MakeInputName( label )
            self.__RewriteInputFiles( input_names )

            output_name  = self.__MakeOutputName( folder, label, cut, selection )
            self.__CheckIndicateOutput( output_name )


            ## ./Analysis, Cut, Selection, Files, Output, label
            command =  "./Analysis {c} {s} launcher_temp.files {on} {l}".format( c = cut, s = selection, on = output_name, l = label )
            print( "EXECUTING" )
            print( command )
            os.system( command )

            print("#############################################################################")
            print("\n\n")


    def __MakeInputName( self, label ):
        names = [ self.base_dir + n for n in self.file_locations[label] ]
        print( "Using input files:")
        for n in names:
            print( " -- " + n )
        return names


    def __RewriteInputFiles( self, input_names ):
        print( "Rewriting launcher_temp.files with new input" )
        print( "" )

        input_list = open("launcher_temp.files", "w")
        for n in input_names:
            input_list.write( n + "*\n" )
        input_list.close()


    def __MakeOutputName( self, folder, label, cut, selection ):

        ## Check that the file can be saved
        output_dir = "../Results/{f}/".format( f = folder )
        if not os.path.exists( output_dir ):
            os.system("mkdir -p " + output_dir)

        name = output_dir + "{l}_{c}_{s}{e}.root".format( l = label, c = cut, s = selection, e = self.extra )
        print( "Making output file:" )
        print( " -- " + name )
        return name


    def __CheckIndicateOutput( self, output_name ):
        if os.path.isfile( output_name ):
            print( " -- will be overwriting previous!" )
        print( "" )





file_locations = {}

# No True Met
file_locations["Zee"]     = [ "user.mleigh.Zee.361106.26_07_19.Fixed_MYSTREAM/" ]
file_locations["SZee"]    = [ "user.mleigh.Sherpa_Zee.26_07_19.Fixed_MYSTREAM/" ]
file_locations["Zmumu"]   = [ "user.mleigh.Zmumu.361107.26_07_19.Fixed_MYSTREAM/" ]
file_locations["SZmumu"]  = [ "user.mleigh.Sherpa_Zmumu.26_07_19.Fixed_MYSTREAM/" ]
file_locations["MGZmumu"] = [ "user.mleigh.MadGraph_Zmumu.26_07_19.Fixed_MYSTREAM/" ]
# True Met for Learning
file_locations["WW"]      = [ "user.mleigh.WWlvlv.361600.26_07_19.Fixed_MYSTREAM/" ]
file_locations["WZ"]      = [ "user.mleigh.WZlvll.361601.26_07_19.Fixed_MYSTREAM/" ]
file_locations["ZZ"]      = [ "user.mleigh.ZZvvll.361604.26_07_19.Fixed_MYSTREAM/" ]
file_locations["ttbar"]   = [ "user.mleigh.ttbar.410470.26_07_19.Fixed_MYSTREAM/" ]
file_locations["top"]     = [ "user.mleigh.Wt_top.410646.26_07_19.Fixed_MYSTREAM/" ]
file_locations["antitop"] = [ "user.mleigh.Wt_antitop.410647.26_07_19.Fixed_MYSTREAM/" ]
# Only for Learning
file_locations["Sllvv"]   = [ "user.mleigh.Sherpa_llvv.364254.26_07_19.Fixed_MYSTREAM/" ]
# Only for signal regions
file_locations["H125"]    = [ "user.mleigh.VBFH125.341080.26_07_19.Fixed_MYSTREAM/" ]
file_locations["Ztt"]     = [ "user.mleigh.Ztautau.361108.26_07_19.Fixed_MYSTREAM/" ]

# SUSY_SAMPLES
file_locations["SL550X001"] = [ "SL550X001/" ]
file_locations["SL400X250"] = [ "SL400X250/" ]
file_locations["SL300X200"] = [ "SL300X200/" ]


file_locations["Real"] = [
                "user.mleigh.real_dilepton_B.15_08_19.GRL_2LSF_MYSTREAM/",
                "user.mleigh.real_dilepton_C.15_08_19.GRL_2LSF_MYSTREAM/",
                "user.mleigh.real_dilepton_D.15_08_19.GRL_2LSF_MYSTREAM/",
                "user.mleigh.real_dilepton_E.15_08_19.GRL_2LSF_MYSTREAM/",
                "user.mleigh.real_dilepton_F.15_08_19.GRL_2LSF_MYSTREAM/",
                "user.mleigh.real_dilepton_H.15_08_19.GRL_2LSF_MYSTREAM/",
                "user.mleigh.real_dilepton_I.15_08_19.GRL_2LSF_MYSTREAM/",
                "user.mleigh.real_dilepton_K.15_08_19.GRL_2LSF_MYSTREAM/",
                "user.mleigh.real_dilepton_K2.15_08_19.GRL_2LSF_MYSTREAM/",
                ]

mc_base_dir   = "/atlas/DATAMC/Matt_Data/Simulated/"
real_base_dir = "/atlas/DATAMC/Matt_Data/Real/"


## folder, inptlabel, cut, selection
mc_input_commands = [

    ## For Chris
    # ( "Chris", "Zee",   "1in3", "ZZ_lax" ),
    # ( "Chris", "Zee",   "2in3", "ZZ_lax" ),
    # ( "Chris", "Zmumu", "1in3", "ZZ_lax" ),
    # ( "Chris", "Zmumu", "2in3", "ZZ_lax" ),

    ## For Learning
    # ( "Learning", "Sllvv",   "all",  "noselec" ),
    # ( "Learning", "WW",      "2in3", "noselec" ),
    # ( "Learning", "WZ",      "2in3", "noselec" ),
    # ( "Learning", "ZZ",      "2in3", "noselec" ),
    # ( "Learning", "ttbar",   "2in3", "noselec" ),
    # ( "Learning", "top",     "2in3", "noselec" ),
    # ( "Learning", "antitop", "2in3", "noselec" ),

    ## For evaluation
    # ( "Evaluation", "WW",      "1in3", "noselec" ),
    # ( "Evaluation", "WZ",      "1in3", "noselec" ),
    # ( "Evaluation", "ZZ",      "1in3", "noselec" ),
    # ( "Evaluation", "ttbar",   "1in3", "noselec" ),
    # ( "Evaluation", "top",     "1in3", "noselec" ),
    # ( "Evaluation", "antitop", "1in3", "noselec" ),

    ## For Z->ee
    # ( "SR_Zee", "Zee",     "all",  "Zee_strict" ),
    # ( "SR_Zee", "SZee",    "all",  "Zee_strict" ),
    # ( "SR_Zee", "Ztt",     "all",  "Zee_strict" ),
    # ( "SR_Zee", "WW",      "1in3", "Zee_strict" ),
    # ( "SR_Zee", "WZ",      "1in3", "Zee_strict" ),
    # ( "SR_Zee", "ZZ",      "1in3", "Zee_strict" ),
    # ( "SR_Zee", "ttbar",   "1in3", "Zee_strict" ),
    # ( "SR_Zee", "top",     "1in3", "Zee_strict" ),
    # ( "SR_Zee", "antitop", "1in3", "Zee_strict" ),

    ## For Z->mumu
    # ( "SR_Zmumu", "Zmumu",   "all",  "Zmumu_strict" ),
    # ( "SR_Zmumu", "MGZmumu", "all",  "Zmumu_strict" ),
    # ( "SR_Zmumu", "SZmumu",  "all",  "Zmumu_strict" ),
    # ( "SR_Zmumu", "Ztt",     "all",  "Zmumu_strict" ),
    # ( "SR_Zmumu", "WW",      "1in3", "Zmumu_strict" ),
    # ( "SR_Zmumu", "WZ",      "1in3", "Zmumu_strict" ),
    # ( "SR_Zmumu", "ZZ",      "1in3", "Zmumu_strict" ),
    # ( "SR_Zmumu", "ttbar",   "1in3", "Zmumu_strict" ),
    # ( "SR_Zmumu", "top",     "1in3", "Zmumu_strict" ),
    # ( "SR_Zmumu", "antitop", "1in3", "Zmumu_strict" ),

    ## For evaluation signal regions
    # ( "SR_WW",    "WW",    "1in3",  "WW_strict" ),
    # ( "SR_Higgs", "H125",  "all",   "WW_strict" ),
    # ( "SR_ttbar", "ttbar", "1in3",  "tt_strict" ),

    ## SUSY Regions
    # ( "SR_SUSY", "Sllvv",   "all",  "susy_2l_0jets" ),
    # ( "SR_SUSY", "SZee",    "all",  "susy_2l_0jets" ),
    # ( "SR_SUSY", "SZmumu",  "all",  "susy_2l_0jets" ),
    # ( "SR_SUSY", "Ztt",     "all",  "susy_2l_0jets" ),
    # ( "SR_SUSY", "WW",      "1in3", "susy_2l_0jets" ),
    # ( "SR_SUSY", "WZ",      "1in3", "susy_2l_0jets" ),
    # ( "SR_SUSY", "ZZ",      "1in3", "susy_2l_0jets" ),
    # ( "SR_SUSY", "ttbar",   "1in3", "susy_2l_0jets" ),
    # ( "SR_SUSY", "top",     "1in3", "susy_2l_0jets" ),
    # ( "SR_SUSY", "antitop", "1in3", "susy_2l_0jets" ),

    # ( "SR_SUSY", "SL550X001", "all", "susy_2l_0jets" ),
    # ( "SR_SUSY", "SL400X250", "all", "susy_2l_0jets" ),
    # ( "SR_SUSY", "SL300X200", "all", "susy_2l_0jets" ),


    ]

real_input_commands = [

    # ( "Real", "Real",   "all",  "Zee_strict" ),
    # ( "Real", "Real",   "all",  "Zmumu_strict" ),
    # ( "Real", "Real",   "all",  "susy_2l_0jets" ),

    ]







mc_controller = AnalysisControl( mc_base_dir, file_locations, mc_input_commands )
mc_controller.run_analysis()

real_controller = AnalysisControl( real_base_dir, file_locations, real_input_commands )
real_controller.run_analysis()
