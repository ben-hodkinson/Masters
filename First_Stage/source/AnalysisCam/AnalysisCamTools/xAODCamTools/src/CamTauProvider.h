
#ifndef _CAMTAUPROVIDER_
#define _CAMTAUPROVIDER_

// includes
#include "PATInterfaces/ISystematicsTool.h"
#include "AnalysisCamEvent/CamTool.h"

#include "xAODTau/TauJetContainer.h"
#include "xAODTau/DiTauJetContainer.h"
#include "xAODTau/TauJetAuxContainer.h"

#include "TauAnalysisTools/ITauSmearingTool.h"
#include "TauAnalysisTools/ITauTruthMatchingTool.h"
#include "tauRecTools/IBuildTruthTaus.h"


class CamTauProvider : virtual public CamTool {
  public:
    using CamTool::apply;
  
    CamTauProvider(const std::string& type, const std::string& name,const IInterface* pParent);
    virtual ~CamTauProvider();
    virtual StatusCode initialize();
    virtual StatusCode finalize();
    virtual StatusCode apply(CamEvent* event, std::string& label, const std::string& option);
  
  private:
    CamTool::DataType dType;
    bool m_isData;
    std::string m_tauCollection;
    ToolHandle< TauAnalysisTools::ITauSmearingTool > m_tauSmearingTool;
    ToolHandle<TauAnalysisTools::ITauTruthMatchingTool> T2MT;
    ToolHandle<tauRecTools::IBuildTruthTaus> m_buildTruthTaus;
  
};

#endif

