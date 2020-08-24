#! /usr/bin/env python

import os
import commands

# Put the list of datasets you want to run over here (remove the '/' from the end).
date = "15_08_19"
flag = "GRL_2LSF"

names = [

    # ( "Zee.361106",           "mc16_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.deriv.DAOD_JETM3.e3601_s3126_r10201_p3495" ),
    # ( "Zmumu.361107",         "mc16_13TeV.361107.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zmumu.deriv.DAOD_JETM3.e3601_s3126_r10201_p3495" ),
    # ( "Ztautau.361108",       "mc16_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.deriv.DAOD_JETM3.e3601_s3126_r10201_p3749" ),
    # #
    # ( "WWlvlv.361600",        "mc16_13TeV.361600.PowhegPy8EG_CT10nloME_AZNLOCTEQ6L1_WWlvlv.deriv.DAOD_JETM3.e4616_s3126_r10201_p3495" ),
    # ( "WZlvll.361601",        "mc16_13TeV.361601.PowhegPy8EG_CT10nloME_AZNLOCTEQ6L1_WZlvll_mll4.deriv.DAOD_JETM3.e4475_s3126_r10201_p3495" ),
    # ( "ZZvvll.361604",        "mc16_13TeV.361604.PowhegPy8EG_CT10nloME_AZNLOCTEQ6L1_ZZvvll_mll4.deriv.DAOD_JETM3.e4475_s3126_r10201_p3495" ),
    # #
    # ( "ttbar.410470",         "mc16_13TeV.410470.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad.deriv.DAOD_JETM3.e6337_s3126_r10201_p3495" ),
    # ( "Wt_top.410646",        "mc16_13TeV.410646.PowhegPythia8EvtGen_A14_Wt_DR_inclusive_top.deriv.DAOD_JETM3.e6552_s3126_r10201_p3495" ),
    # ( "Wt_antitop.410647",    "mc16_13TeV.410647.PowhegPythia8EvtGen_A14_Wt_DR_inclusive_antitop.deriv.DAOD_JETM3.e6552_s3126_r10201_p3495" ),

    # ( "VBFH125.341080",       "mc16_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.deriv.DAOD_JETM3.e3871_s3126_r10201_p3495" ),
    # ( "Sherpa_llvv.364254",   "mc16_13TeV.364254.Sherpa_222_NNPDF30NNLO_llvv.deriv.DAOD_JETM3.e5916_s3126_r10201_p3495" ),

    # ( "SlSl_550_1.396010",    "mc16_13TeV.396010.MGPy8EG_A14N23LO_SlepSlep_direct_550p0_1p0_2L8.deriv.DAOD_JETM3.e6710_a875_r10201_p3500" ),
    # ( "SlSl_500_300.395999",  "mc16_13TeV.395999.MGPy8EG_A14N23LO_SlepSlep_direct_500p0_300p0_2L8.deriv.DAOD_JETM3.e6710_a875_r10201_p3500" ),


    ( "real_dilepton_B",  "data17_13TeV.periodB.physics_Main.PhysCont.DAOD_JETM3.grp17_v01_p3372" ),
    ( "real_dilepton_C",  "data17_13TeV.periodC.physics_Main.PhysCont.DAOD_JETM3.grp17_v01_p3372" ),
    ( "real_dilepton_D",  "data17_13TeV.periodD.physics_Main.PhysCont.DAOD_JETM3.grp17_v01_p3372" ),
    ( "real_dilepton_E",  "data17_13TeV.periodE.physics_Main.PhysCont.DAOD_JETM3.grp17_v01_p3372" ),
    ( "real_dilepton_F",  "data17_13TeV.periodF.physics_Main.PhysCont.DAOD_JETM3.grp17_v01_p3372" ),
    ( "real_dilepton_H",  "data17_13TeV.periodH.physics_Main.PhysCont.DAOD_JETM3.grp17_v01_p3372" ),
    ( "real_dilepton_I",  "data17_13TeV.periodI.physics_Main.PhysCont.DAOD_JETM3.grp17_v01_p3372" ),
    ( "real_dilepton_K",  "data17_13TeV.periodK.physics_Main.PhysCont.DAOD_JETM3.grp17_v01_p3372" ),
    ( "real_dilepton_K2", "data17_13TeV.periodK.physics_Main.PhysCont.DAOD_JETM3.grp17_v01_p3402" ),


    ]

txt_files = [

    # ( "MadGraph_Zee",    "InputScripts/MadGraph_Zee.txt" ),
    # ( "MadGraph_Zmumu",  "InputScripts/MadGraph_Zmumu.txt" ),

    # ( "Sherpa_Zee",      "InputScripts/Sherpa_Zee.txt" ),
    # ( "Sherpa_Zmumu",    "InputScripts/Sherpa_Zmumu.txt" ),
    # ( "Sherpa_Ztautau",  "InputScripts/Sherpa_Ztautau.txt" ),

    # ( "Sherpa_Wlnu",     "InputScripts/Sherpa_Wlnu.txt" ),

    ]

extra_files = "data17prw_lumi.root,data17prw_actualmu.root,data17_GRL.xml"


for (output, input) in names:

    command = "pathena --inDS=%s --outDS=user.mleigh.%s.%s.%s --nGBPerJob=15 MyPackageAlgJobOptions.py --extFile=%s" % (input, output, date, flag, extra_files)

    print ""
    print command
    os.system(command)

    # break

for (output, txt_name) in txt_files:

    command = "pathena --inDsTxt=%s --outDS=user.mleigh.%s.%s.%s --nGBPerJob=20 MyPackageAlgJobOptions.py --extFile=%s" % (txt_name, output, date, flag, extra_files)

    print "\n"
    print command
    os.system(command)
