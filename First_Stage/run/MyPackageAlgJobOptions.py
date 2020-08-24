


#---- Minimal joboptions -------

theApp.EvtMax=-1                                         #says how many events to run over. Set to -1 for all events
jps.AthenaCommonFlags.FilesInput = ["../../Data/DXAODs/mc16_13TeV.395971.MGPy8EG_A14N23LO_SlepSlep_direct_300p0_200p0_2L8.deriv.DAOD_JETM3.e6710_a875_r10201_p3500/DAOD_JETM3.17654821._000001.pool.root.1"]

#../../Data/DXAODs/mc16_13TeV.395971.MGPy8EG_A14N23LO_SlepSlep_direct_300p0_200p0_2L8.deriv.DAOD_JETM3.e6710_a875_r10201_p3500/DAOD_JETM3.17654821._000001.pool.root.1
#../../Data/DXAODs/mc16_13TeV.396010.MGPy8EG_A14N23LO_SlepSlep_direct_550p0_1p0_2L8.deriv.DAOD_JETM3.e6710_a875_r10201_p3500/DAOD_JETM3.17654656._000001.pool.root.1
#../../Data/DXAODs/mc16_13TeV.395985.MGPy8EG_A14N23LO_SlepSlep_direct_400p0_250p0_2L8.deriv.DAOD_JETM3.e6710_a875_r10201_p3500/DAOD_JETM3.17654757._000001.pool.root.1

import AthenaRootComps.ReadAthenaxAODHybrid               #FAST xAOD reading!

algseq = CfgMgr.AthSequencer("AthAlgSeq")


# Adding the good run list requirements use this only for data!!!
# ToolSvc += CfgMgr.GoodRunsListSelectionTool("GRLTool",GoodRunsListVec=["data17_GRL.xml"])
# filterSeq = CfgMgr.AthSequencer("AthFilterSeq")
# filterSeq += CfgMgr.GRLSelectorAlg(Tool=ToolSvc.GRLTool)

algseq += CfgMgr.MyPackageAlg()

svcMgr += CfgMgr.CamWriter("myWriter",TreeName="/MYSTREAM/MyTree")

svcMgr += CfgMgr.THistSvc()
svcMgr.THistSvc.Output += ["MYSTREAM DATAFILE='../../Data/Simulated/SL300X200/SL300X200.root' OPT='RECREATE'"]

ToolSvc += CfgMgr.ST__SUSYObjDef_xAOD("SUSYTools")
config_file = "SUSYTools/SUSYTools_Default.conf" #look in the data directory of SUSYTools for other config files
ToolSvc.SUSYTools.ConfigFile = config_file


#2017
ToolSvc.SUSYTools.PRWLumiCalcFiles = ["data17prw_lumi.root"]
ToolSvc.SUSYTools.PRWActualMuFile = "data17prw_actualmu.root"

## When we want to do the PU reweighting by hand (only for the SUSY samples)
ToolSvc.SUSYTools.AutoconfigurePRWTool = False
ToolSvc.SUSYTools.PRWConfigFiles = [ "CustomPUWeights.root" ]


ToolSvc.SUSYTools.DataSource = 1 #configure to run on data 0, or MC 1






