#include "AnalysisCamEvent/CamService.h"
#include "AnalysisCamEvent/CamEvent.h"


#include "TROOT.h"

CamService::CamService(const std::string& name, ISvcLocator *svcLoc) :
  AthService(name, svcLoc),m_evtStore("StoreGateSvc/StoreGateSvc",name), m_doNotify(false), m_doneFirst(false), m_currentFile(0), m_currentTree(0)
{
   declareProperty("EvtStore",m_evtStore = ServiceHandle<StoreGateSvc>("StoreGateSvc",name));
}

// Destructor
CamService::~CamService() {}

///listen for events and new files
StatusCode CamService::sysInitialize() {
   ServiceHandle<IIncidentSvc> incsvc("IncidentSvc",name());
   int priority = 0; //must ensure listeners are low enough priorty that are not fired before the metadata aux setups are cleared properly (IOVDbMetaDataTools priority is 60)
   incsvc->addListener( this, IncidentType::BeginEvent, priority, true);
   incsvc->addListener( this, IncidentType::BeginInputFile, priority, true);
   incsvc->addListener(this,IncidentType::EndInputFile);

   return AthService::sysInitialize();
}




void CamService::handle(const Incident& inc) {
   if(inc.type()==IncidentType::BeginInputFile) {
      if(this->beginInputFile().isFailure()) { ATH_MSG_ERROR("Failure in beginInputFile method"); return; }
      m_doNotify=true; //wait for next BeginEvent, so that branches are ready in storegate
      m_doneFirst=true; //from now on listen for files closing instead
      //std::cout << "GOT BEGIN FILE" << std::endl;
   } else if(inc.type()==IncidentType::EndInputFile) {
      //immediately fire the endFileNotify() method, which should NOT require looking at any of the branch information!!
      if(this->endInputFile().isFailure()) { ATH_MSG_ERROR("Failure in endInputFile method"); return; }
      //std::cout << "GOT END FILE" << std::endl;
      m_doneFirst=true; //in case the BeginInputFile never fired - likely since tools get be loaded after the start of loop
      m_doNotify=true; //wait for next begin event
      //clear the currentFile and currentTree
      m_currentFile=0; m_currentTree=0;
   } else if (inc.type() == IncidentType::BeginEvent) {
      if(this->execute().isFailure()) {
         return; //FIXME: should really try to exit
      }
      m_doneFirst=true;
   }
}


TFile* CamService::currentFile() {
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

TTree* CamService::currentTree() {
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