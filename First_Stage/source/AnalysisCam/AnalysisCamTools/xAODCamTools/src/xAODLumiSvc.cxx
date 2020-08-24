#include "xAODLumiSvc.h"

#include "GoodRunsLists/TGoodRunsListWriter.h"
#include "GaudiKernel/ITHistSvc.h"

#include "TKey.h"

#include "xAODLuminosity/LumiBlockRangeContainer.h"

#include "AthAnalysisBaseComps/AthAnalysisHelper.h"

// Constructor
xAODLumiSvc::xAODLumiSvc(const std::string& name, ISvcLocator *svcLoc) :
  CamService(name, svcLoc), m_grlR(0)
{
  declareProperty("RequireLumi",m_requireLumi=false,"Data must have at least one lumi string");
   declareProperty("WriteXML",m_writeXML=false,"If true, will write an XML file too");
}

// Destructor
xAODLumiSvc::~xAODLumiSvc()
{

}

StatusCode xAODLumiSvc::beginInputFile() {
   //use metadata to see if this is MC ... if it is, we do nothing. If in doubt, we do readLumi anyways
   std::string project_name;
   if(AthAnalysisHelper::retrieveMetadata("/TagInfo","project_name",project_name).isSuccess()) {
      if(project_name.find("IS_SIMULATION") != std::string::npos) return StatusCode::SUCCESS; //was simulation!
   }
   
   return readLumi();
}

StatusCode xAODLumiSvc::readLumi() {

   //get the xAODLuminosity lumiblocks and add to the list 

   ServiceHandle<StoreGateSvc> inputMetaStore("StoreGateSvc/InputMetaDataStore",name());

   bool hasLumi=false;

   if(inputMetaStore->contains<xAOD::LumiBlockRangeContainer>("LumiBlocks")) {
      const xAOD::LumiBlockRangeContainer* lbrs = 0;
      CHECK( inputMetaStore->retrieve(lbrs,"LumiBlocks") );
      //loop over lumiblocks and fill
      for(auto lbr : *lbrs) {
         for(uint runNum = lbr->startRunNumber(); runNum <= lbr->stopRunNumber(); runNum++) {
//std::cout << runNum << ": ";
            for(uint lb = lbr->startLumiBlockNumber(); lb <= lbr->stopLumiBlockNumber(); lb++) {
//std::cout << lb << ",";
               m_grl.AddRunLumiBlock(runNum,lb);
            }
//std::cout << std::endl;
         }
      }
      hasLumi=true;
   }

   //also include the incomplete lumiblocks ... we assume that you run over enough files to complete the incomplete ones!
   if(inputMetaStore->contains<xAOD::LumiBlockRangeContainer>("IncompleteLumiBlocks")) {
      const xAOD::LumiBlockRangeContainer* lbrs = 0;
      CHECK( inputMetaStore->retrieve(lbrs,"IncompleteLumiBlocks") );
      //loop over lumiblocks and fill
      for(auto lbr : *lbrs) {
         for(uint runNum = lbr->startRunNumber(); runNum <= lbr->stopRunNumber(); runNum++) {
            for(uint lb = lbr->startLumiBlockNumber(); lb <= lbr->stopLumiBlockNumber(); lb++) {
               m_grl.AddRunLumiBlock(runNum,lb);
            }
         }
      }
      hasLumi=true;
   }

   //also include the suspect ... really should event count these to check they're ok though!
   if(inputMetaStore->contains<xAOD::LumiBlockRangeContainer>("SuspectLumiBlocks")) {
      const xAOD::LumiBlockRangeContainer* lbrs = 0;
      CHECK( inputMetaStore->retrieve(lbrs,"SuspectLumiBlocks") );
      //loop over lumiblocks and fill
      for(auto lbr : *lbrs) {
         for(uint runNum = lbr->startRunNumber(); runNum <= lbr->stopRunNumber(); runNum++) {
            for(uint lb = lbr->startLumiBlockNumber(); lb <= lbr->stopLumiBlockNumber(); lb++) {
               m_grl.AddRunLumiBlock(runNum,lb);
            }
         }
      }
      hasLumi=true;
   }

   if(hasLumi){
      ATH_MSG_INFO("Loaded Lumi info for file: " << currentFile()->GetName());
      return StatusCode::SUCCESS;
   }

   TDirectoryFile* dir = dynamic_cast<TDirectoryFile*>( currentFile()->GetDirectory( "Lumi" ) );
   if( ! dir ) {
     if(m_requireLumi) { ATH_MSG_ERROR("File: " << currentFile()->GetName() << " has no Lumi directory"); return StatusCode::FAILURE; }
     else { ATH_MSG_WARNING("File: " << currentFile()->GetName() << " has no Lumi directory"); return StatusCode::SUCCESS; }
   }
   
   TList* keys = dir->GetListOfKeys();
   //loop over all that are objstrings
   for( Int_t j = 0; j < keys->GetEntries(); ++j ) {
     TKey *key = dynamic_cast<TKey*>( keys->At( j ) );
     TObjString* ostring = (TObjString*) key->ReadObjectAny( TObjString::Class() );
     if( ! ostring )  continue;
     // Check that it has the right format:
     if( ostring->GetString().BeginsWith( "<?xml version=" ) && ostring->GetString().Contains( "DOCTYPE LumiRangeCollection" ) ) {
       TObjString* s = new TObjString(ostring->GetString().Data());
       m_lumiStrings.push_back(s);
       hasLumi=true;
     }
   }

   if(!hasLumi) {
     //final effort, try to find the lumiTree
     TTree* lumiTree = dynamic_cast<TTree*>(currentFile()->Get("Lumi/lumiTree"));
     if(lumiTree) {
       //read all the lumi strings in
       TString *in = 0;
       lumiTree->SetBranchAddress("lumi",&in);
       for(int j=0;j<lumiTree->GetEntries();j++) {
	 lumiTree->GetEntry(j);
	 TObjString* s = new TObjString(in->Data());
	 m_lumiStrings.push_back(s);
       }
     } else {
       if(m_requireLumi) { ATH_MSG_ERROR("File: " << currentFile()->GetName() << " has no Lumi"); return StatusCode::FAILURE; }
       else { ATH_MSG_WARNING("File: " << currentFile()->GetName() << " has no Lumi"); return StatusCode::SUCCESS; }
     }   
   }

   ATH_MSG_DEBUG("Loaded Lumi info for file: " << currentFile()->GetName());

   return StatusCode::SUCCESS;

}


StatusCode xAODLumiSvc::initialize() {

  ServiceHandle<IIncidentSvc> isvc("IncidentSvc",name());
  isvc->addListener( this, "MetaDataStop", 0, true);

  return StatusCode::SUCCESS;

}

#include "xAODLuminosity/LumiBlockRangeContainer.h"
#include "xAODLuminosity/LumiBlockRangeAuxContainer.h"
#include "xAODRootAccess/TEvent.h"

#include <boost/filesystem.hpp>


void xAODLumiSvc::handle(const Incident& inc) {
  if (inc.type() == "MetaDataStop") {
      finish().ignore();
  }
  CamService::handle(inc);
}

StatusCode xAODLumiSvc::finish(){
    //do we need to make a processed GRL?
   if(m_lumiStrings.size()>0) {
      m_grlR = new Root::TGoodRunsListReader();
      for(unsigned int i=0;i<m_lumiStrings.size();i++) {
         const TString b = (m_lumiStrings[i])->GetString();
         m_grlR->AddXMLString(b);
         m_grlR->Interpret();
      }
      m_grl.AddGRL(m_grlR->GetMergedGoodRunsList());
   }

   //only write xml if there's something in the grl 
   if(m_grl.IsEmpty()) return StatusCode::SUCCESS;

   TString filename = this->name(); filename += ".xml";
   Root::TGoodRunsListWriter* writer = new Root::TGoodRunsListWriter(m_grl,filename);
   if(m_writeXML) writer->WriteXMLFile();
   
   //put this into a TTree, for safe file merging
   ITHistSvc* m_histSvc = 0;
   service("THistSvc",m_histSvc).ignore(); //or check the statuscode
   std::vector<std::string> histsvcTrees = m_histSvc->getTrees(); 


   ServiceHandle<StoreGateSvc> metastore("StoreGateSvc/MetaDataStore",name());

   //use xAOD::RootAccess to create a metadata file
   xAOD::TEvent evt;
   TFile* outFile = new TFile("metadata.out.root","RECREATE");
   if(evt.writeTo( outFile ).isFailure()) return StatusCode::FAILURE;
   
   std::vector<std::string> keys;
   metastore->keys<xAOD::LumiBlockRangeContainer>(keys);
   std::vector<std::string> allKeys = {"LumiBlocks","IncompleteLumiBlocks","SuspectLumiBlocks"};
   for(auto& key : allKeys) {
    bool foundKey(false);for(uint i=0;i<keys.size();i++) {if(keys[i]==key) {foundKey=true;break;}}
    if(foundKey) {
      auto lbs = metastore->retrieveUniquePrivateCopy<xAOD::LumiBlockRangeContainer>(key); //returns unique_ptr
      auto lbsAux = metastore->retrieveUniquePrivateCopy<xAOD::LumiBlockRangeAuxContainer>(key+"Aux.");  //returns unique_ptr
      if( evt.recordMeta( lbs.release() , key ).isFailure() ) return StatusCode::FAILURE; //technically we wont need to store the interface container, but doesnt harm us
      if( evt.recordMeta( lbsAux.release() , key+"Aux.").isFailure() ) return StatusCode::FAILURE;
    } else {
      //record a dummy interface and aux container
      xAOD::LumiBlockRangeContainer* dummy = new xAOD::LumiBlockRangeContainer;
      xAOD::LumiBlockRangeAuxContainer* dummyAux = new xAOD::LumiBlockRangeAuxContainer;
      dummy->setStore(dummyAux);
      if( evt.recordMeta( dummy , key).isFailure() ) return StatusCode::FAILURE;
      if( evt.recordMeta( dummyAux , key+"Aux.").isFailure() ) return StatusCode::FAILURE;
    }
   }
   
   if( evt.finishWritingTo( outFile ).isFailure() ) return StatusCode::FAILURE;;
   
   TTree* metatree = dynamic_cast<TTree*>(outFile->Get("MetaData"))->CloneTree(); 
   metatree->SetDirectory(0);

   for(unsigned int i=0;i<histsvcTrees.size();i++) {
      TString* out = new TString(writer->GetXMLString());
      //strip the tree name (all after last /)
      TString ss(histsvcTrees[i].c_str());
      //TString ssPath = ss(0,ss.Last('/'));
      //path is up to second slash: 
      int secondSlash = TString(ss(1,ss.Length())).First('/');
      TString ssPath = ss(0,secondSlash+1);
      ssPath += "/Lumi/lumiTree";
      if(m_doneDirs[ssPath]) continue;
      TTree* outTree = new TTree("lumiTree","lumiTree");
      outTree->Branch("lumi","TString",&out);
      outTree->Fill();
      std::string sPath(ssPath.Data());
      ATH_MSG_INFO("Writing GRL Info to file: " << ssPath);
      m_histSvc->regTree(sPath,outTree);
      
      if(metatree) {
          TString sss = ss(0,secondSlash+1);sss += "/MetaData";
          m_histSvc->regTree(sss.Data(),metatree->CloneTree());
      }
      
      m_doneDirs[ssPath]=true;
      delete out;
      
      
      //fake the xAOD lumiblock metadata
      //TTree* mTree = new TTree("MetaData","MetaData");
      
      
      
   }
   
   
   
   
   

   if(histsvcTrees.size()==0) {
      //write out to a file anyways!
      TFile f1("xAODLumiSvc.result.root","RECREATE");
      f1.mkdir("Lumi")->cd();
      TString* out = new TString(writer->GetXMLString());
      TTree* outTree = new TTree("lumiTree","lumiTree");
      outTree->Branch("lumi","TString",&out);
      outTree->Fill();
      outTree->Write();
      f1.Close();
      delete out;
   }


   delete writer;



   return StatusCode::SUCCESS;
}

