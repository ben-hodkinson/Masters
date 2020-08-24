// GeneralCamAlgs includes
#include "NtupleCutAlg.h"

#include "AnalysisCamEvent/CamEvent.h"

#include "TROOT.h"

NtupleCutAlg::NtupleCutAlg( const std::string& name, ISvcLocator* pSvcLocator ) : CamAlgorithm( name, pSvcLocator )
{

  declareProperty( "Cut", m_expression = "", "TTree expression to select on" );

}

StatusCode NtupleCutAlg::initialize() { 
  return StatusCode::SUCCESS;
}

StatusCode NtupleCutAlg::beginInputFile() {

  if(m_formula) delete m_formula;
  
  TTree* t = currentTree();
  
  //temporarily save the state of all the branches in the tree, because we need to activate all branches to compile formula successfully ..
  std::map<TString,bool> branchStatus;
  for(int i=0;i<t->GetListOfBranches()->GetEntries();i++) {
    TBranch* b = (TBranch*)t->GetListOfBranches()->At(i);
    branchStatus[b->GetName()] = !b->TestBit(kDoNotProcess);
  }
  
  //temporarily enable all branches to compile formula ... 
  t->SetBranchStatus("*",1);
  
  m_formula = new TTreeFormula(name().c_str(),m_expression.c_str(), t);
  
  m_formula->GetNdata();

  int i=0;
  std::vector<TString> branchNames;
  while( TLeaf* l = m_formula->GetLeaf(i) ) {
    branchStatus[ l->GetBranch()->GetName() ] = true;
    i++;
  }
  
  t->SetBranchStatus("*",0); //disable all again ..
  
  
  //but enable my branches
  for(auto& b : branchStatus) {
    if(b.second) t->SetBranchStatus(b.first,1);
  }
  

  return StatusCode::SUCCESS;
}



StatusCode NtupleCutAlg::execute() {

  //first ensure our branches have been read in!
  int i=0;
  while( TLeaf* l = m_formula->GetLeaf(i) ) {
    l->GetBranch()->GetEntry( m_formula->GetTree()->GetReadEntry() );
    i++;
  }
  
  bool decision = m_formula->EvalInstance();
  
  m_total++;
  m_pass += decision;

  setFilterPassed( decision );
  return StatusCode::SUCCESS;
}

StatusCode NtupleCutAlg::finalize() {
  ATH_MSG_INFO("Pass rate: " << m_pass << " / " << m_total);
  return StatusCode::SUCCESS;
}
