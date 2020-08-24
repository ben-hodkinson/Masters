
#ifndef _CAMELPROVIDER_
#define _CAMELPROVIDER_

// includes
#include "PATInterfaces/ISystematicsTool.h"
#include "AnalysisCamEvent/CamTool.h"

#include "xAODEgamma/Electron.h"
#include "xAODEgamma/ElectronContainer.h"

#if ROOTCORE_RELEASE_SERIES >= 25
#include "EgammaAnalysisInterfaces/IEgammaCalibrationAndSmearingTool.h"
#else
#include "ElectronPhotonFourMomentumCorrection/IEgammaCalibrationAndSmearingTool.h"
#endif

#include "IsolationCorrections/IIsolationCorrectionTool.h"

class CamElProvider : virtual public CamTool {

  public:
      using CamTool::apply; //stops compiler warnings
  
      // Constructors, destructor
      CamElProvider(const std::string& type, const std::string& name,const IInterface* pParent);
      virtual ~CamElProvider();
      //StatusCode queryInterface( const InterfaceID& riid, void** ppvIf );

      virtual StatusCode initialize();
      virtual StatusCode finalize();

      //main method ... puts a CamVector into the event, containing the calibrated electrons
      virtual StatusCode apply(CamEvent* event, std::string& label, const std::string& option);



   private:
      //configurable properties
      std::string m_elCollection; 
      std::string m_esModel; std::string m_decorrModel;
      bool m_doIsoCorrection;
    
      ToolHandle<CP::IEgammaCalibrationAndSmearingTool> m_elCalibTool;
      ToolHandle<CP::IIsolationCorrectionTool> m_isoCorrTool;
     
};


#endif
