

import AthenaRootComps.ReadAthenaRoot      # configure athena to read flat n-tuples
from glob import glob
svcMgr.EventSelector.InputCollections = glob("/r02/atlas/will/ZZ8TeV/NTUP_SMWZ/mc12_8TeV.126949.PowhegPythia8_AU2CT10_ZZllnunu_ee_mll4.merge.NTUP_COMMON.e1280_s1469_s1470_r3752_r3549_p1575/*")
svcMgr.EventSelector.TupleName = "physics"    # the n-tuple name in the input files to read data from
theApp.EvtMax = 5 # read all events

svcMgr.THistSvc.Output += ["MYSTREAM DATAFILE='myfile.root' OPT='RECREATE'"]
svcMgr += CfgMgr.CamWriter("myWriter",TreeName="/MYSTREAM/MyTree",OutputLevel=VERBOSE)

job = CfgMgr.AthSequencer("AthAlgSeq")
job += CfgMgr.CamExampleAlg("MyAlgInstance") # adding our analysis algorithm to the sequence of algs to run
#job.MyAlgInstance.ExampleProperty = "whatever" #example of setting your algorithms property you declared in the cxx file
