#include "CamTauSelector.h"
#include <cstdlib>

#include <boost/algorithm/string.hpp>
#include "xAODCore/ShallowCopy.h"
#include "TauAnalysisTools/TauSelectionTool.h"

CamTauSelector::CamTauSelector(const std::string& type,
                               const std::string& name,
                               const IInterface* pParent) :
                               CamTool(type, name, pParent),
                               chanInfo("ChannelInfoTool/cutflow",this),
                               m_tauSelectionTool("",this)
{
  declareProperty("InputCollection", m_inputTaus="CamTauProvider/CalibratedTaus", "Name of the input tau collection");
  declareProperty("OutputDecoration", m_outputDecor="","If specified, will not filter input collection, will just decorate with a char to flag pass/fail");
  declareProperty("PrintCutflow",m_printCutflow=false,"Store a cutflow inside a TChannelInfo");
 
  declareProperty("MaxEta", m_maxEta=999., "eta cut");
  
  declareProperty("CreateControlPlots", m_CreateControlPlots=false);
  declareProperty("MinPt", m_minPt=20000.); //default value in TauSelectionTool
  declareProperty("ConfigPath", m_ConfigPath="");
  declareProperty("IgnoreAODFixCheck",m_IgnoreAODFixCheck=true);
  declareProperty("RecalcEleOLR",m_RecalcEleOLR=false);
  declareProperty("AbsCharges", m_AbsCharges={},"Array of intereted abs charges");
  declareProperty("NTracks",m_NTracks={},"Array of intereted track numbers");
  declareProperty("EleOLR", m_EleOLR=true);
  declareProperty("MuonOLR", m_MuonOLR=false);
}

StatusCode CamTauSelector::initialize() {
  ATH_MSG_INFO("init");
  if ( m_tauSelectionTool.empty() ) {
    m_tauSelectionTool.setTypeAndName("TauAnalysisTools::TauSelectionTool/TauSelectionTool");
    if(!AAH::toolExists(m_tauSelectionTool)) {

      ATH_MSG_INFO("Initializing " << m_tauSelectionTool.name());
      
      CHECK( AAH::setProperty(m_tauSelectionTool, "SelectionCuts", (int)TauAnalysisTools::SelectionCuts(TauAnalysisTools::CutPt |
													TauAnalysisTools::CutAbsEta |
													TauAnalysisTools::CutAbsCharge |
													TauAnalysisTools::CutNTrack |
													TauAnalysisTools::CutJetIDWP /*|
													TauAnalysisTools::CutEleOLR |
													TauAnalysisTools::CutMuonOLR*/)));
      CHECK( AAH::setProperty(m_tauSelectionTool, "PtMin", m_minPt/1000));
      CHECK( AAH::setProperty(m_tauSelectionTool, "AbsEtaMax", m_maxEta));
      CHECK( AAH::setProperty(m_tauSelectionTool, "AbsCharges", m_AbsCharges));
      CHECK( AAH::setProperty(m_tauSelectionTool, "NTracks", m_NTracks));
      CHECK( AAH::setProperty(m_tauSelectionTool, "JetIDWP", int(TauAnalysisTools::JETIDNONE)));
      CHECK( AAH::setProperty(m_tauSelectionTool, "EleOLR", m_EleOLR));
      CHECK( AAH::setProperty(m_tauSelectionTool, "MuonOLR", m_MuonOLR));
      CHECK( AAH::setProperty(m_tauSelectionTool, "ConfigPath", m_ConfigPath));
      //These two lines are for release checks:
      //CHECK( AAH::setProperty(m_tauSelectionTool, "IgnoreAODFixCheck", m_IgnoreAODFixCheck));
      //CHECK( AAH::setProperty(m_tauSelectionTool, "RecalcEleOLR", m_RecalcEleOLR));
      //CHECK( AAH::setProperty(m_tauSelectionTool, "CreateControlPlots", m_CreateControlPlots));

      m_tauSelectionTool.retrieve();
    }
  }
  registerSystematics(&*m_tauSelectionTool);
  return StatusCode::SUCCESS;
}

StatusCode CamTauSelector::finalize() {
  if(m_printCutflow) chanInfo->Cutflow();
  return StatusCode::SUCCESS;
}

bool CamTauSelector::tauSelection(CamObject& c){
  const xAOD::TauJet* tauJet = c.ae<xAOD::TauJet>();
  
  if (m_printCutflow) chanInfo->Fill("Taus", 1);

  if (tauJet->pt()<m_minPt) return false;  
  if (fabs(c.Eta())>m_maxEta) return false;
  if (m_printCutflow) chanInfo->Fill("Taus_etaCut",1);
  
  if (!m_tauSelectionTool->accept(*tauJet)) return false;
  if (m_printCutflow) chanInfo->Fill("Taus_toolCut", 1);

  if(!m_outputDecor.empty()) tauJet->auxdecor<char>(m_outputDecor)=true;

  c.set("IsTauTight") = (bool)tauJet->isTau( xAOD::TauJetParameters::JetBDTSigTight );
  c.set("IsTauMedium") =(bool)tauJet->isTau( xAOD::TauJetParameters::JetBDTSigMedium );
  c.set("IsTauLoose") =(bool)tauJet->isTau( xAOD::TauJetParameters::JetBDTSigLoose );

  return true;
  
}

StatusCode CamTauSelector::apply(CamEvent* evt, std::string& label, const std::string&){
  CamDeque* taus = evt->getDeque( m_inputTaus, currentSystematicSet() );
  if (!taus) {
    ATH_MSG_ERROR("Could not retreive " << m_inputTaus << " (as a deque)");
    return StatusCode::FAILURE;
  }
  CamDeque* taus_select = 0;
  if (m_outputDecor.empty()) {
    taus_select = taus->subset( CamObject::Function(&CamTauSelector::tauSelection, this), false, false/*dont add to event, will do ourselves*/);
  } else {
    // only decorate taus
    for (auto& tau : *taus->container() ) {
      tau->auxdecor<char>(m_outputDecor) = false;
      break;
    }
    taus->all_call( CamObject::Function(&CamTauSelector::tauSelectionVoid, this) );
    taus_select = taus->subset("1", false, false); //output all tau .. the selected tau are decorated in the tauSelectionMethod
  }
  
  evt->addObject(taus_select, label);
  
  return StatusCode::SUCCESS;
  
}

