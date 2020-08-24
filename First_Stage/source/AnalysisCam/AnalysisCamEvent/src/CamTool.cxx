#include "AnalysisCamEvent/CamTool.h"
#include "AnalysisCamEvent/CamEvent.h"
#include "AnalysisCamEvent/CamDeque.h"
#include "AnalysisCamEvent/CamObject.h"

#include "TROOT.h"

#include <boost/algorithm/string.hpp>

CamTool::CamTool(const std::string& type, const std::string& name,
			 const IInterface* pParent) : ::AthAlgTool(type, name, pParent), m_doNotify(false), m_doneFirst(false), m_currentFile(0), m_currentTree(0)
{
  declareInterface< ICamTool >(this);
  
  m_thisAsSystematicsTool = dynamic_cast<CP::ISystematicsTool*>(this);
  
}

// Destructor
CamTool::~CamTool() {}

StatusCode CamTool::sysInitialize() {
   
   ServiceHandle<IIncidentSvc> incsvc("IncidentSvc",name());
   int priority = 100;

   incsvc->addListener( this, IncidentType::BeginEvent, priority);
   incsvc->addListener( this, IncidentType::BeginInputFile, priority);
   incsvc->addListener( this, IncidentType::EndInputFile, priority);


   return AthAlgTool::sysInitialize();
}


void CamTool::handle(const Incident& inc) {
   if(!m_doneFirst && inc.type()==IncidentType::BeginInputFile) {
      m_doNotify=false;//Decided user must call notify() from the camInitialize() the first time //wait for next BeginEvent, so that branches are ready in storegate
      m_doneFirst=true; //from now on listen for files closing instead
      //std::cout << "GOT BEGIN FILE" << std::endl;
   } else if(inc.type()==IncidentType::EndInputFile) {
      //std::cout << "GOT END FILE" << std::endl;
      m_doneFirst=true; //in case the BeginInputFile never fired - likely since tools get be loaded after the start of loop
      m_doNotify=true; //wait for next begin event
      //clear the currentFile and currentTree
      m_currentFile=0; m_currentTree=0;
   } else if (inc.type() == IncidentType::BeginEvent) {
      //std::cout << "GOT BEGIN EVENT" << std::endl;
      if(m_doNotify) {
      //CamEvent ev; std::cout << "eventnumber= " << ev.get<int>("EventNumber");
         if(this->beginInputFile().isFailure()) { ATH_MSG_ERROR("Failure in Notify method"); return; }
         m_doNotify=false;
      }
      if(this->execute().isFailure()) {
         return; //FIXME: should really try to exit
      }
   }
}

TFile* CamTool::currentFile() {
   if(m_currentFile) return m_currentFile;
   IProperty *evtSelector = 0;
   StatusCode sc = service("EventSelector",evtSelector);
   if(sc.isFailure()){ATH_MSG_ERROR("Couldn't find the EventSelectorService!!!??");return 0;}
   //get the list of input files - use this to determine which open file is the current input file
   const StringArrayProperty& m_inputCollectionsName = dynamic_cast<const StringArrayProperty&>(evtSelector->getProperty("InputCollections"));
   ATH_MSG_VERBOSE("nOpenFile=" << gROOT->GetListOfFiles()->GetSize() << ". nFilesInInputCollection=" << m_inputCollectionsName.value().size());
/*
   for(int i=0;i<gROOT->GetListOfFiles()->GetSize();i++) {
      std::cout << "Open file: " << gROOT->GetListOfFiles()->At(i)->GetName() << std::endl;
   }
*/
   for(int i=0;i<gROOT->GetListOfFiles()->GetSize();i++) {
         TFile *g = (TFile*)gROOT->GetListOfFiles()->At(i);
         //see if this file is in the input file list
         //strip everything except stuff either side of last /
         TString s(g->GetName());
         TObjArray* tokens = s.Tokenize("/");
         TObjString* lastToken = dynamic_cast<TObjString*>(tokens->Last());
         TString sToCompare("");
         bool shortComparison(false);
         if(tokens->GetEntries()>1) {
            TString beforeSlash((dynamic_cast<TObjString*>(tokens->At(tokens->GetEntries()-2)))->GetString());
            if(beforeSlash.Length()>0) sToCompare += beforeSlash; 
            sToCompare += "/";
         } else {
            shortComparison=true;
         }
         sToCompare += lastToken->GetString();
         TString sToCompare_short(lastToken->GetString()); //short versions search
         delete tokens;
         ATH_MSG_VERBOSE("Look at " << sToCompare);
         for(unsigned int j=0;j<m_inputCollectionsName.value().size();j++) {
            TString t(m_inputCollectionsName.value()[j].c_str());
            //try perfect match first
            if(s.EqualTo(t)) {
               ATH_MSG_DEBUG("Current File is: " << m_inputCollectionsName.value()[j]);
               m_currentFile = g;
               return g;
            }
            TObjArray* tokens = t.Tokenize("/");
            TObjString* lastToken = dynamic_cast<TObjString*>(tokens->Last());
            TString tToCompare = "";
            bool shortComparison2(false);
            if(tokens->GetEntries()>1) {
               TString beforeSlash((dynamic_cast<TObjString*>(tokens->At(tokens->GetEntries()-2)))->GetString());
               if(beforeSlash.Length()>0) tToCompare += beforeSlash; 
               tToCompare += "/";
            } else {
               shortComparison2=true;
            }
            tToCompare += lastToken->GetString();
            TString tToCompare_short(lastToken->GetString());
            delete tokens;
            ATH_MSG_VERBOSE("cf with : " << m_inputCollectionsName.value()[j]);
            if(shortComparison || shortComparison2) { //doing short version search, no directories to distinguish files!
               if(sToCompare_short.EqualTo(tToCompare_short)) {
                  ATH_MSG_DEBUG("Current File is: " << m_inputCollectionsName.value()[j]);
                  m_currentFile = g;
                  return g;
               }
            } else
            if(sToCompare.EqualTo(tToCompare)) {
               ATH_MSG_DEBUG("Current File is: " << m_inputCollectionsName.value()[j]);
               m_currentFile=g;
               return g;
            }
         }
   } 
   ATH_MSG_ERROR("Could not find the current file!");
   return 0; //something went wrong :-(
}

TTree* CamTool::currentTree() {
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

ToolHandle<ICamTool> CamTool::getToolHandle(const std::string& toolName) {
   ToolHandle<ICamTool> tool(toolName);
   if(tool.retrieve().isFailure()) {
      ATH_MSG_ERROR("Unable to get ToolHandle for tool: " << toolName);
   }
   return tool;
}


void  CamTool::applySystematicSet(const CP::SystematicSet& syst) {
  m_currentSyst = syst;
  if(m_thisAsSystematicsTool) m_thisAsSystematicsTool->applySystematicVariation(syst).ignore();
  
  for(auto& tool : m_subCamTools) tool->applySystematicSet(syst);
  for(auto& tool : m_systTools) tool->applySystematicVariation(syst).ignore();
  
}

void  CamTool::registerSystematics(const std::string& toolName) {
  //check if toolName is a CamTool valid CamTool
  ToolHandle<ICamTool> tool(toolName);
  //first check the tool type is ok .. to avoid printing an error
  auto checkTool = AlgTool::Factory::create(tool.type(), tool.type(), tool.name(), this);
  if(checkTool) delete checkTool;
  else { return; } //not a valid tool, so don't do anything more
  if(tool.isValid()) registerSystematics( &*tool ); 
}

void CamTool::registerSystematics(IAlgTool* tool) {
  //if tool is a CamTool, call getSystematics on it to register ...
  if(dynamic_cast<ICamTool*>(tool)) {
    m_registeredSysts.insert( dynamic_cast<ICamTool*>(tool)->getSystematics() );
    m_subCamTools.push_back(dynamic_cast<ICamTool*>(tool));
  } else if(dynamic_cast<CP::ISystematicsTool*>(tool)) {
    m_registeredSysts.insert( dynamic_cast<CP::ISystematicsTool*>(tool)->recommendedSystematics() );
    m_systTools.push_back(dynamic_cast<CP::ISystematicsTool*>(tool));
  }
}

CamTool::DataType CamTool::getDataType() {
  CamEvent event;
  if(!event.is("isSimulation")) return DATA;

  std::string simType("");
  ServiceHandle<StoreGateSvc> metaStore("StoreGateSvc/InputMetaDataStore",name());
  if(AAH::retrieveMetadata("/Simulation/Parameters","SimulationFlavour",simType,metaStore).isFailure()) {
    ATH_MSG_WARNING("getDataType(): Could not infer fast/full sim ... will assume full sim");
    return FULLSIM;
  }
  boost::to_upper(simType);
  if(simType.find("ATLFASTII")!=std::string::npos) return FASTSIM;
  return FULLSIM;
  
}

