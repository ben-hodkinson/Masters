
filelist = os.environ['xAODTESTFILE'] #replace with your own list of files

import AthenaPoolCnvSvc.ReadAthenaPool      # configure athena to read pool
from glob import glob
svcMgr.EventSelector.InputCollections = [filelist]
theApp.EvtMax = 5 # read all events

#example output setup
svcMgr += CfgMgr.THistSvc()
svcMgr.THistSvc.Output += ["MYSTREAM DATAFILE='myfile.root' OPT='RECREATE'"]
svcMgr += CfgMgr.CamWriter("myWriter",TreeName="/MYSTREAM/MyTree",Branches=["mcChannelNumber/unsigned int"])

#example tool setup
ToolSvc += CfgMgr.CamMetaData(ReadFiles=["metadata.tsv"])

job = CfgMgr.AthSequencer("AthAlgSeq")
job += CfgMgr.CamxAODExampleAlg("MyAlgInstance") # adding our analysis algorithm to the sequence of algs to run
#job.MyAlgInstance.ExampleProperty = "whatever" #example of setting your algorithms property you declared in the cxx file

job += CfgMgr.CamxAODExampleAlg("SecondAlg")

