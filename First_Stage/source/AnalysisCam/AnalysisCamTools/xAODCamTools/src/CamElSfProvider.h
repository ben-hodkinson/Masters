
#ifndef _CAMELSFPROVIDER_
#define _CAMELSFPROVIDER_

// includes
#include "PATInterfaces/ISystematicsTool.h"
#include "AnalysisCamEvent/CamTool.h"

#include "xAODEgamma/Electron.h"

#if ROOTCORE_RELEASE_SERIES >= 25
#include "EgammaAnalysisInterfaces/IAsgElectronEfficiencyCorrectionTool.h"
#else
#include "ElectronEfficiencyCorrection/IAsgElectronEfficiencyCorrectionTool.h"
#endif

//#include "AsgAnalysisInterfaces/IEfficiencyScaleFactorTool.h"

class CamElSfProvider : virtual public CamTool {

  public:
      using CamTool::apply; //stops compiler warnings
  
      // Constructors, destructor
      CamElSfProvider(const std::string& type, const std::string& name,const IInterface* pParent);
      virtual ~CamElSfProvider();
      //StatusCode queryInterface( const InterfaceID& riid, void** ppvIf );

      virtual StatusCode initialize();
      virtual StatusCode finalize();

      virtual StatusCode apply(CamDeque* deque, std::string& label, const std::string& option);



   private:
      std::vector<std::string> m_elSfSelections;
      ToolHandleArray<IAsgElectronEfficiencyCorrectionTool> m_elSfTools;
      
      std::string m_elSfCorrModel;
      int m_elSfNtoys;

      std::string m_elSfMapFilePath;
      std::string m_elSfIdKey;
      std::string m_elSfIsoKey;
      std::string m_elSfTriggerKey;
      std::string m_elSfChIdKey;

      int m_elSfIdCut;
};


#endif
