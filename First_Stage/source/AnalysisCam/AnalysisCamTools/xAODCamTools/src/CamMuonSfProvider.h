
#ifndef _CAMMUONSFPROVIDER_
#define _CAMMUONSFPROVIDER_

// includes
#include "PATInterfaces/ISystematicsTool.h"
#include "AnalysisCamEvent/CamTool.h"

#include "xAODMuon/Muon.h"

#if ROOTCORE_RELEASE_SERIES<25
#include "MuonEfficiencyCorrections/IMuonEfficiencyScaleFactors.h"
#else
#include "MuonAnalysisInterfaces/IMuonEfficiencyScaleFactors.h"
#endif

class CamMuonSfProvider : virtual public CamTool {

  public:
      using CamTool::apply; //stops compiler warnings
  
      // Constructors, destructor
      CamMuonSfProvider(const std::string& type, const std::string& name,const IInterface* pParent);
      virtual ~CamMuonSfProvider();
      //StatusCode queryInterface( const InterfaceID& riid, void** ppvIf );

      virtual StatusCode initialize();
      virtual StatusCode finalize();

      virtual StatusCode apply(CamDeque* deque, std::string& label, const std::string& option);



   private:
      std::vector<std::string> m_muonSfSelections;
      ToolHandleArray<CP::IMuonEfficiencyScaleFactors> m_muonSfTools;

      std::string m_muonSfRecoWp;

      std::string m_muonSfIsoWp;

      std::string m_muonSfTrigMuonQual;
      std::string m_muonSfTrigYear;
      std::string m_muonSfTrigMc;

      
};


#endif
