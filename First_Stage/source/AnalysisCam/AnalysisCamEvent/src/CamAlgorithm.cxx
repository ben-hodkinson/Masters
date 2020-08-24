#include "AnalysisCamEvent/CamAlgorithm.h"


#include "GaudiKernel/ITHistSvc.h"
#include "TROOT.h"

#include <execinfo.h>
#include <dlfcn.h>
#include <boost/filesystem.hpp> //for checking if a file exists when printing stack traces

CamAlgorithm::CamAlgorithm(const std::string& name, ISvcLocator* pSvcLocator) : AthAnalysisAlgorithm(name, pSvcLocator), 
   m_inputMetaStore( "StoreGateSvc/InputMetaDataStore", name ),
   m_currentTree(0)
{
   declareProperty("DebugEventNumber",m_debugNumber=0,"Specify a debug event number and when it hits it, will print filename and also go verbose mode for main execute");
   m_doNotify=false; m_doneFirst=false;
}

CamAlgorithm::~CamAlgorithm() {}


TTree* CamAlgorithm::currentTree() {
   if(m_currentTree) return m_currentTree;
   TFile* f = currentFile();
   if(f) {
      //use the EventSelector to determine the tree name
      IProperty *evtSelector = 0;
      StatusCode sc = service("EventSelector",evtSelector);
      if(sc.isFailure()){ATH_MSG_ERROR("Couldn't find the EventSelectorService!!!??");return 0;}
      std::string myString;
      if(evtSelector->getProperty("TupleName",myString).isFailure()) {
         ATH_MSG_ERROR("Couldn't find the ttree name!!!??");return 0;
      } 
      TTree* t = (TTree*)f->Get(myString.c_str());
      m_currentTree=t;
      return t;
   }
   return 0;
}

ToolHandle<ICamTool> CamAlgorithm::getToolHandle(std::string toolName) {
   ToolHandle<ICamTool> tool(toolName);
   if(tool.retrieve().isFailure()) {
      ATH_MSG_ERROR("Unable to get ToolHandle for tool: " << toolName);
   }
   return tool;
}


void CamAlgorithm::handle(const Incident& incident) {
   //ATH_MSG_INFO("itype = " << incident.type());
   if(!m_doneFirst && incident.type() == IncidentType::BeginInputFile) {
     m_currentTree=0;
     m_currentChannel=0;
   } 
   
   //do incident handling in parent alg
   AthAnalysisAlgorithm::handle(incident);
}


#include "EventInfo/EventStreamInfo.h"


//this returns channel number in MC, or run number in data! 
int CamAlgorithm::currentChannelNumber() {

  if(m_currentChannel!=0) return m_currentChannel;

  m_currentChannel = -1;

  //get the channelNumber from the EventStreamInfo ...
  const EventStreamInfo* esi = 0;
  if(inputMetaStore()->retrieve(esi).isFailure()) {
    ATH_MSG_WARNING("Failed to retrieve EventStreamInfo when determining current channel number");
  }

  for(auto& type : esi->getEventTypes()) {
    if(type.test(EventType::IS_SIMULATION)) m_currentChannel = type.mc_channel_number();
    else if(esi->getRunNumbers().size()) m_currentChannel = *esi->getRunNumbers().begin(); //just uses the first run number ... there better be one because I don't test here :-(
    else m_currentChannel=0; //if data, and no run numbers, we will use runNumber==0
    break; //only check the first one
  }
  return m_currentChannel;
}

StatusCode CamAlgorithm::sysInitialize() {
   //ServiceHandle<IIncidentSvc> m_incidentSvc("IncidentSvc",name());
   //m_incidentSvc->addListener(this,IncidentType::BeginInputFile);
   //m_incidentSvc->addListener(this,IncidentType::EndInputFile);
   return AthAnalysisAlgorithm::sysInitialize();
}


void CamAlgorithm::printStackTrace() {
   std::ostringstream empty;
   CamObject::printTraceConst(empty,false);
}

StatusCode CamAlgorithm::sysExecute() {
   try {
      //std::cout << "ALG GOT EXECUTE" << std::endl;CamEvent ev; std::cout << "algeventnumber= " << ev.get<int>("EventNumber");
/*
      if(m_doNotify || !m_doneFirst) {
         if(m_doNotify) { if(beginInputFile().isFailure()) { ATH_MSG_ERROR("Failure in beginInputFile method"); return StatusCode::FAILURE; } }
         if(!m_doneFirst) CHECK( firstExecute() );
         m_doNotify=false; m_doneFirst=true;
      }
*/
      if(m_debugNumber>0) {
         CamEvent ev; 
         if(!ev.contains<int>("EventNumber") && ev.contains<unsigned long long>("eventNumber")) ev.set("EventNumber") = int(ev.get<unsigned long long>("eventNumber"));
         if(ev.get<int>("EventNumber")==int(m_debugNumber)) {
            TFile* f = currentFile();if(f) ATH_MSG_INFO("EventNumber " << m_debugNumber << " in event in file: " << f->GetName());
            //switch to verbose
            m_tmpLevel = msg().level();
            msg().setLevel(MSG::VERBOSE);
         } else if(m_tmpLevel>=0) {
            msg().setLevel(m_tmpLevel); m_tmpLevel=-1;
         }
         
      }
      

      return AthAnalysisAlgorithm::sysExecute();
   }
   catch(std::runtime_error err) {
      ATH_MSG_ERROR("=======================================================================");
      ATH_MSG_ERROR("Runtime Error: " << err.what());

/*
      TTree *tree = 0;
      myString = "/temp/TTreeStream/" + myString;
      if(m_histSvc->getTree(myString,tree).isFailure()) {
         ATH_MSG_ERROR("Couldn't find the tree in the hist svc....you must be using a new version. dying...");return StatusCode::FAILURE;
      }
*/
      //ATH_MSG_ERROR("Error in event in file: " << tree->GetCurrentFile()->GetName());

      TFile* f = currentFile(); if(f) ATH_MSG_ERROR("Error in event in file " << f->GetName());



      //try to also get the run and event numbers from the tree
      unsigned int runnum = 0;
      unsigned int eventnum = 0;
      CamEvent event;
      if(event.contains<unsigned int>("RunNumber")) runnum = event.get<unsigned int>("RunNumber");
      else if(event.contains<unsigned int>("runNumber")) runnum = event.get<unsigned int>("runNumber");
      if(event.contains<unsigned int>("EventNumber")) eventnum = event.get<unsigned int>("EventNumber");
      else if(event.contains<unsigned long long>("eventNumber")) eventnum = event.get<unsigned long long>("eventNumber");

      ATH_MSG_ERROR("[RunNumber,EventNumber] = [" << runnum << "," << eventnum << "]");


      ATH_MSG_ERROR("=================================EXITING===============================");

      abort();
   }
   catch(...) {
    // catch anything thrown within try block that derives from std::exception

      ATH_MSG_ERROR("=======================================================================");


/*
      TTree *tree = 0;
      myString = "/temp/TTreeStream/" + myString;
      if(m_histSvc->getTree(myString,tree).isFailure()) {
         ATH_MSG_ERROR("Couldn't find the tree in the hist svc....you must be using a new version. dying...");return StatusCode::FAILURE;
      }
*/
      //ATH_MSG_ERROR("Error in event in file: " << tree->GetCurrentFile()->GetName());

      TFile* f = currentFile(); if(f) ATH_MSG_ERROR("Error in event in file " << f->GetName());



      //try to also get the run and event numbers from the tree
      unsigned int runnum = 0;
      unsigned int eventnum = 0;
      CamEvent event;
      if(event.contains<unsigned int>("RunNumber")) runnum = event.get<unsigned int>("RunNumber");
      if(event.contains<unsigned int>("EventNumber")) eventnum = event.get<unsigned int>("EventNumber");

      ATH_MSG_ERROR("[RunNumber,EventNumber] = [" << runnum << "," << eventnum << "]");


      ATH_MSG_ERROR("=================================EXITING===============================");

      return StatusCode::FAILURE;
   }
   return AthAnalysisAlgorithm::sysExecute();
}
