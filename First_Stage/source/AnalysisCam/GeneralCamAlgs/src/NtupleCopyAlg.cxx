// GeneralCamAlgs includes
#include "NtupleCopyAlg.h"

#include "AnalysisCamEvent/CamEvent.h"

#include "TROOT.h"
#include "TMethodCall.h"

NtupleCopyAlg::NtupleCopyAlg( const std::string& name, ISvcLocator* pSvcLocator ) : AthAnalysisAlgorithm( name, pSvcLocator )
{

  declareProperty( "OutputStream", m_outputStream = "MYSTREAM", "name of the output stream" );
  declareProperty( "MetaItems", m_metaItems = {"channelInfo","MetaData"}, "names of metadata items that should be merged from the input, if they exist");

}

StatusCode NtupleCopyAlg::initialize() { 

  if(m_metaItems.size()>0) {
    std::vector<std::string> inputFiles;
    Gaudi::Parsers::parse(inputFiles,AAH::getProperty("EventSelector","InputCollections")).ignore();
    

    std::map<std::string,TObject*> mergedObjects;
    
    for(auto& file : inputFiles) {
      TFile* f = TFile::Open(file.c_str());
      if(!f) {
        ATH_MSG_FATAL("Could not open " << file);
        return StatusCode::FAILURE;
      }
      for(auto& objName : m_metaItems) {
        TObject* obj = f->Get(objName.c_str());
        if(!obj) continue;
        if(mergedObjects[objName]==0) mergedObjects[objName] = obj->Clone(objName.c_str());
        else {
          TMethodCall fMergeCall;
          fMergeCall.InitWithPrototype(obj->IsA(),"Merge","TCollection*");

          TList l; l.Add(obj);
          fMergeCall.ResetParam();
          fMergeCall.SetParam((Long_t)&l);
          fMergeCall.Execute(mergedObjects[objName]);
          l.Remove(obj);
        }
      }
      f->Close();
      delete f;
    }
  
  
    for(auto& objs : mergedObjects) {
      ATH_MSG_INFO("Registering " << objs.first << " in output stream " << m_outputStream);
      CHECK( histSvc()->regGraph( "/" + m_outputStream + "/" + objs.first, (TGraph*)objs.second ) );
    }
  
  }


  return StatusCode::SUCCESS;
}

StatusCode NtupleCopyAlg::beginInputFile() {

  return StatusCode::SUCCESS;
}

#include "TBranchElement.h"

StatusCode NtupleCopyAlg::execute() {
  //
  CamEvent e;
  std::string fileName = e.get<std::string>("collectionName");
  std::string treeName = e.get<std::string>("tupleName");

  //get the tree ...
  TFile* f = static_cast<TFile*>(gROOT->GetListOfFiles()->FindObject(fileName.c_str()));
  if(!f) {
    ATH_MSG_FATAL("Could not find file " << fileName);
    return StatusCode::FAILURE;
  }
  
  TTree* inputTree = static_cast<TTree*>(f->Get(treeName.c_str()));
  if(!inputTree) {
    ATH_MSG_FATAL("Could not find tree " << treeName);
    return StatusCode::FAILURE;
  }
  
  
  auto treeIter = m_outputTrees.find(treeName);
  
  //have we created this tree already?
  if(treeIter ==m_outputTrees.end() ) {
    //need to create tree ..
    
    //we must first activate all branches!
    inputTree->SetBranchStatus("*",1);
    //we done use CloneTree because that calls LoadTree(0) which resets the ReadEntry to 0!!!
    m_outputTrees[treeName] = std::make_pair((TTree*)inputTree->Clone(inputTree->GetName()),"");
    
    // The clone should not delete any objects allocated by SetAddress().
    TObjArray* branches = m_outputTrees[treeName].first->GetListOfBranches();
    Int_t nb = branches->GetEntriesFast();
    for (Int_t i = 0; i < nb; ++i) {
      TBranch* br = (TBranch*) branches->UncheckedAt(i);
      if (br->InheritsFrom(TBranchElement::Class())) {
        ((TBranchElement*) br)->ResetDeleteObject();
      }
    }
    
    m_outputTrees[treeName].first->Reset(); //resets the entry counter
    CHECK( histSvc()->regTree("/"+m_outputStream+"/"+treeName,m_outputTrees[treeName].first) );
    
    treeIter = m_outputTrees.find(treeName);
  }
  
  TTree* outputTree = treeIter->second.first;
  
  if(fileName != treeIter->second.second) {
    ATH_MSG_DEBUG("Switching to new tree for " << treeIter->first << " entry number " << inputTree->GetReadEntry());
    //need to ensure addresses of output tree are up to date
    inputTree->SetBranchStatus("*",1);
    inputTree->GetEntry( inputTree->GetReadEntry() );
    outputTree->ResetBranchAddresses();
    inputTree->CopyAddresses(outputTree);
    m_outputTrees[treeName].second = fileName;
  }
  
  //load all the branches
  inputTree->GetEntry( inputTree->GetReadEntry() );
  
  outputTree->Fill();
  
  
  return StatusCode::SUCCESS;
}

