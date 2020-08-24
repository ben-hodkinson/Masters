#include "CamTauProvider.h"

#include "xAODCore/ShallowCopy.h"
#include "TauAnalysisTools/ITauSmearingTool.h"
#include "TauAnalysisTools/ITauTruthMatchingTool.h"
#include "xAODBase/IParticleHelpers.h"
#include "tauRecTools/IBuildTruthTaus.h"


CamTauProvider::CamTauProvider(const std::string& type,
                               const std::string& name,
                               const IInterface* pParent) :
                               CamTool(type, name, pParent),
                               m_tauSmearingTool(""),
			       T2MT("")
{
   declareProperty("TauCollection", m_tauCollection="TauJets", "Name of the tau collection");
   declareProperty("TauSmearingTool", m_tauSmearingTool=ToolHandle<TauAnalysisTools::ITauSmearingTool>(""), "The tau smearing tool ... if left empty, we auto configure");
   declareProperty("TauTruthMatchingTool",T2MT=ToolHandle<TauAnalysisTools::ITauTruthMatchingTool>(""), "The tau truth matching tool");
}

CamTauProvider::~CamTauProvider()
{}

StatusCode CamTauProvider::initialize() {
  ATH_MSG_INFO("Init");
  dType = getDataType();
  m_isData = (dType==CamTool::DATA);
  
  //if (AAH::toolExists( m_tauSmearingTool )) {
  if(m_tauSmearingTool.empty()) {
    //    ATH_MSG_WARNING("TauSmearingTool already exists ... will use that instance");
    //  } else {
    m_tauSmearingTool.setTypeAndName("TauAnalysisTools::TauSmearingTool/TauSmearingTool");
    //CHECK( AAH::setProperty( m_tauSmearingTool , "IsData" , m_isData ) );
    //m_tauSmearingTool.retrieve();
  }
  else  ATH_MSG_WARNING("TauSmearingTool already exists ... will use that instance");
  
  if (!AAH::toolExists(T2MT)) {
    T2MT.setTypeAndName("TauAnalysisTools::TauTruthMatchingTool/T2MT");
  }
  else ATH_MSG_WARNING("TauTruthMatchingTool already exists ... will use that instance");

  registerSystematics( m_tauCollection );
  registerSystematics( &*m_tauSmearingTool );
  registerSystematics( &*T2MT );

  return StatusCode::SUCCESS;
}

StatusCode CamTauProvider::finalize() {
  return StatusCode::SUCCESS;
}

StatusCode CamTauProvider::apply(CamEvent* evt, std::string& label, const std::string& option) {
    
  //new taus
  const xAOD::TauJetContainer* xaod_taus = 0;
  CHECK( evtStore()->retrieve( xaod_taus, m_tauCollection ) );

  CamVector* rawTaus = evt->getVector( m_tauCollection );
  
  auto cont = rawTaus->container();
  if (cont == 0) { ATH_MSG_ERROR("Failed to obtain xAOD Container for collection " << m_tauCollection); return StatusCode::FAILURE; }
  //auto calibratedTaus = xAOD::shallowCopyContainer( *static_cast<const xAOD::TauJetContainer*>(cont) );
  auto calibratedTaus = xAOD::shallowCopyContainer( *xaod_taus );
  bool setLinks = xAOD::setOriginalObjectLink(*xaod_taus, *calibratedTaus.first);
  if (!setLinks) {
    ATH_MSG_INFO("Failed to set original object link for calibrated taus");
  }
  
  if (!m_isData && !m_tauSmearingTool.empty()) {
    for (auto tau: *calibratedTaus.first) {

      T2MT->getTruth( *tau );
      m_tauSmearingTool->applyCorrection(*tau).setChecked();
    }
  }
  
  CHECK( evtStore()->record( calibratedTaus.first , label) );
  CHECK( evtStore()->record( calibratedTaus.second , label + "Aux.") );
  
  if(option.length()==0 || option=="v") evt->createVector(label,calibratedTaus.first);
  else evt->createDeque(label,calibratedTaus.first);
  
  return StatusCode::SUCCESS;
}
