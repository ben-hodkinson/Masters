#!/usr/bin/env python



def main():
   import argparse
   import subprocess
   import os
   parser = argparse.ArgumentParser(description='whatever')
   parser.add_argument( '--packageName', metavar='packageName', required=True, nargs=1, help='some help here' )
   parser.add_argument( '--algName', metavar='algName', nargs=1, required=True, help='some help here')
   parser.add_argument( '--packagePath', metavar='packagePath', nargs=1, help='some help here', default=[])
   parser.add_argument( '--jobOptionName', metavar='jobOptionName', nargs=1, help='Optional job option file creation in run folder', default=[])
   args, unknown = parser.parse_known_args()

   
   myexec = "cd $TestArea;cmt create " + args.packageName[0] + " " + args.packageName[0] + "-00-00-01 "
   if(len(args.packagePath)>0): myexec += args.packagePath[0]
   myexec += ";cd ./"
   if(len(args.packagePath)>0): myexec += args.packagePath[0] + "/"
   myexec += args.packageName[0] + ";mkdir " + args.packageName[0] + "; mkdir src/components;"
   subprocess.Popen(myexec,shell=True).wait()
   
   ppath = os.environ['TestArea'] + "/";
   if(len(args.packagePath)>0): ppath += args.packagePath[0] + "/"
   ppath += args.packageName[0] + "/"
   f = open(ppath + "src/components/" + args.packageName[0] + "_load.cxx",'w')
   f.write('#include "GaudiKernel/LoadFactoryEntries.h" \n\n')
   f.write('LOAD_FACTORY_ENTRIES( ' + args.packageName[0] + ' )\n')
   f.close()
   
   f = open(ppath + "src/components/" + args.packageName[0] + "_entries.cxx",'w')
   f.write('#include "GaudiKernel/DeclareFactoryEntries.h" \n')
   f.write('#include "../' + args.algName[0] + '.h" \n\n')
   f.write('DECLARE_ALGORITHM_FACTORY (' + args.algName[0] + ') \n\n')
   f.write('DECLARE_FACTORY_ENTRIES( ' + args.packageName[0] + ' ) { \n')
   f.write('  DECLARE_ALGORITHM(' + args.algName[0] + ') \n')
   f.write('} \n')
   f.close()

   f = open(ppath + "src/" + args.algName[0] + ".h",'w')
   f.write('#ifndef ' + args.packageName[0].upper() + "_" + args.algName[0].upper() + '_H\n')
   f.write('#define ' + args.packageName[0].upper() + "_" + args.algName[0].upper() + '_H\n\n')
   f.write('#include "AnalysisCamEvent/CamAlgorithm.h"\n\n')

   f.write('class ' + args.algName[0] + ':public CamAlgorithm {\n')
   f.write('    public:\n')
   f.write('      ' + args.algName[0] + ' (const std::string& name, ISvcLocator* pSvcLocator);\n')
   f.write('      StatusCode initialize();\n')
   f.write('      StatusCode execute();\n')
   f.write('      StatusCode finalize();\n')
   f.write('     private:\n')
   f.write('      std::string my_property; //example Property which we can set in the job options later\n')
   f.write('};\n')
   f.write('\n#endif\n')

   f.close()
   
   f = open(ppath + "src/" + args.algName[0] + ".cxx",'w')
   f.write('#include "' + args.algName[0] + '.h"\n')
   f.write('\n')
   f.write('' + args.algName[0] + '::' + args.algName[0] + '(const std::string& name, ISvcLocator* pSvcLocator) :\n')
   f.write('CamAlgorithm(name, pSvcLocator)\n')
   f.write('{\n')
   f.write('  declareProperty("ExampleProperty",my_property="hello"); //default value is "hello"\n')
   f.write('}\n')
   f.write('StatusCode ' + args.algName[0] + '::initialize(){\n')
   f.write('    ATH_MSG_INFO ("initialize()");\n')
   f.write('    return StatusCode::SUCCESS;\n')
   f.write('}\n')
   f.write('\n')
   f.write('StatusCode ' + args.algName[0] + '::execute() {\n')
   f.write('\n')
   f.write('   ATH_MSG_INFO ("Your new package and algorithm are successfully executing. ExampleProperty=" << my_property);\n')
   f.write('//ANY C++ CODE WILL WORK, ATH_MSG_INFO is just a wrapper function for cout but you could do...\n')
   f.write('  std::cout << "something" << std::endl;\n')
   f.write('    return StatusCode::SUCCESS;\n')
   f.write('}\n')
   f.write('\n')
   f.write('StatusCode ' + args.algName[0] + '::finalize() {\n')
   f.write('    ATH_MSG_INFO ("finalize()");\n')
   f.write('\n')
   f.write('    return StatusCode::SUCCESS;\n')
   f.write('}\n')
   f.close()
   
   f = open(ppath + "cmt/requirements",'w')
   f.write('package ' + args.packageName[0] + '\n')
   f.write('author  NLAA\n')
   f.write('\n')
   f.write('## for athena policies: this has to be the first use statement\n')
   f.write('use AtlasPolicy         AtlasPolicy-*\n')
   f.write('use GaudiInterface GaudiInterface-* External\n')
   f.write('private\n')
   f.write('use AnalysisCamEvent AnalysisCamEvent-* PhysicsAnalysis/AnalysisCam\n')
   f.write('end_private\n')
   f.write('branches src src/components doc python share\n')
   f.write('\n')
   f.write('private\n')
   f.write('## default is to make component library\n')
   f.write('library ' + args.packageName[0] + ' *.cxx components/*.cxx\n')
   
   f.write('apply_pattern component_library\n')
   f.write('apply_pattern declare_joboptions files="*.py"\n')
   f.write('apply_pattern declare_python_modules files="*.py"\n')
   f.write('\n')
   f.write('end_private\n')
   f.close()

   if(len(args.jobOptionName)>0):
      if not os.path.exists(os.environ['TestArea'] + "/run"):
         os.makedirs(os.environ['TestArea'] + "/run")
      f = open(os.environ['TestArea'] + "/run/" + args.jobOptionName[0] + ".py",'w')
#      f.write('import AthenaRootComps.ReadAthenaRoot      # configure athena to read flat n-tuples\n')
      f.write('import AthenaPoolCnvSvc.ReadAthenaPool\n')
      f.write('svcMgr.EventSelector.InputCollections = ["<insert pool file here>"]\n')
#      f.write('svcMgr.EventSelector.TupleName = "MyFirstD3PD"    # the n-tuple name in the input files to read data from\n')
      f.write('theApp.EvtMax = -1 # read all events\n')
      f.write('\n')
      f.write('job = CfgMgr.AthSequencer("AthAlgSeq")\n')
      f.write('job += CfgMgr.' + args.algName[0] + '("MyAlgInstance",ExampleProperty="whatever") # adding our analysis algorithm to the sequence of algs to run\n')
      f.write('job.MyAlgInstance.ExampleProperty = "anotherway to set property" #example of setting your algorithms property you declared in the cxx file\n')
      f.close()
         
if __name__ == "__main__":
   import sys
   sys.exit(main())
   
