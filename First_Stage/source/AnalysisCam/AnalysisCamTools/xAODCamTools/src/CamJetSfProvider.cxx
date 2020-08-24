#include "CamJetSfProvider.h"
#include "xAODCore/ShallowCopy.h"
#include <boost/algorithm/string.hpp>
#include "JetJvtEfficiency/IJetJvtEfficiency.h"
#include "PathResolver/PathResolver.h"
#include "xAODBTaggingEfficiency/BTaggingEfficiencyTool.h"
#include "xAODBTagging/BTaggingAuxContainer.h"
#include "xAODBTagging/BTagging.h"
#include "xAODJet/JetContainer.h"
 

CamJetSfProvider::CamJetSfProvider(const std::string& type, const std::string& name,
				   const IInterface* pParent) : CamTool(type, name, pParent), m_jetJvtEfficiencyTool(this)
{
  //declareProperty("JetSfSelections", m_jetSfSelections = {"reco", "iso", "TTVA"}, "Vector of jet scale factors to consider");
  //declareProperty("JetSfTools", m_jetSfTools, "Array of jet scale factor tools");  
  //declareProperty("JetSfRecoWp", m_jetSfRecoWp = "Loose", "Jet scale factor reco working point");
  declareProperty("JetJvtEfficiency", m_jetJvtEfficiencyTool=ToolHandle<CP::IJetJvtEfficiency>(""));
  declareProperty("JetBtagWoringPoints", m_jetBtagWorkingPoints = {"FixedCutBEff_77"}, "vector of b-tagging working point");
  declareProperty("JetBtagTaggerName", m_jetBtagTaggerName = "MV2c10");
  declareProperty("JetAuthor", m_jetAuthor = "AntiKt4EMTopoJets");
  declareProperty("JetBtagCaliFile", m_jetBtagCaliFile = PathResolverFindCalibFile("xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2017-07-02_v1.root"));
}

CamJetSfProvider::~CamJetSfProvider()
{}

StatusCode CamJetSfProvider::initialize(){

  //initializing jet jvt efficiency tool
  m_jetJvtEfficiencyTool.setTypeAndName("CP::JetJvtEfficiency/JetJvtEfficiency");
  if (AAH::toolExists(m_jetJvtEfficiencyTool)) {
    ATH_MSG_WARNING( m_jetJvtEfficiencyTool.name() << " already exists... will use that instance");
  }
  else {
    StatusCode code = m_jetJvtEfficiencyTool.retrieve();
    if (code != StatusCode::SUCCESS){
      std::cout << "Initialization of tool " << m_jetJvtEfficiencyTool.name() << " failed! Subsequent results may not make sense." << std::endl;
    }
    else {
      std::cout << "Initialization of tool " << m_jetJvtEfficiencyTool.name() << " finished." << std::endl;
    }
  }
  
  //initializing jet b-tagging efficeincy tool
  if (m_jetBtagCaliFile == "")
  {
    ATH_MSG_WARNING( "Calibration File not Found!!" );
  }
  else
    ATH_MSG_WARNING( "Calibration File is found at " << m_jetBtagCaliFile );
  
  for (auto& WP : m_jetBtagWorkingPoints) {
    m_jetBtagEfficiencyTools[WP] = "BTaggingEfficiencyTool_" + WP;
  }
  

  registerSystematics(&*m_jetJvtEfficiencyTool);

  return StatusCode::SUCCESS;
}

StatusCode CamJetSfProvider::finalize() { 
  return StatusCode::SUCCESS;
}

StatusCode CamJetSfProvider::apply(CamDeque* jetDeque, std::string&, const std::string& option){

  auto jetSysts = this->getSystematics();
  bool onlyNominal(option == "n");
  CP::SystematicSet tmpSet;
  
  if(currentSystematicSet().size() > 0) {
    tmpSet = currentSystematicSet();
    onlyNominal = true;
  }

  //CamDeque* truth_jets = jetDeque->event()->getDeque("AntiKt4TruthJets");
  const xAOD::JetContainer *truthJets = nullptr;
  CHECK( evtStore()->retrieve( truthJets, "AntiKt4TruthJets"));
  

 
  for (auto jet = jetDeque->begin(); jet != jetDeque->end(); ++jet){
    const xAOD::Jet* jetAuxElement = (*jet)->ae<xAOD::Jet>();
    
    jetAuxElement->auxdecor<char>("isJvtPU") = true;
    jetAuxElement->auxdecor<char>("isJvtHS") = false;
    
    for (const auto& tjet : *truthJets)
    {
      if (tjet->p4().DeltaR(jetAuxElement->p4())<0.3 && tjet->pt()>10e3)
        jetAuxElement->auxdecor<char>("isJvtHS") = true;
      if (tjet->p4().DeltaR(jetAuxElement->p4())<0.6)
        jetAuxElement->auxdecor<char>("isJvtPU") = false;
    }
    
    
    float jvtSF(1.);
    bool passes_jvt = jetAuxElement->auxdataConst<char>("passFJVT");

    if (passes_jvt) {
      m_jetJvtEfficiencyTool->getEfficiencyScaleFactor(*jetAuxElement, jvtSF).ignore();     
    }
    else {
      m_jetJvtEfficiencyTool->getInefficiencyScaleFactor(*jetAuxElement, jvtSF).ignore();
    }
    (*jet)->set("jvt_scaleFactor") = jvtSF;
    
    if(onlyNominal) continue; //go to next
    
    // do variations on SF as well ( only if css is empty (i.e. no kin var) )
    for (auto sv : jetSysts) {
      tmpSet.insert(sv);
      float jvtSF(1.);
      if (passes_jvt) {
        m_jetJvtEfficiencyTool->applySystematicVariation(tmpSet).ignore();
        m_jetJvtEfficiencyTool->getEfficiencyScaleFactor(*jetAuxElement, jvtSF).ignore();
      }
      (*jet)->set("jvt_scaleFactor" + sv.name()) = jvtSF;
      tmpSet.clear();
    }
  }
  return StatusCode::SUCCESS;
}
