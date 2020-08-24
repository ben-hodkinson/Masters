
#ifndef _CAMMUONPROVIDER_
#define _CAMMUONPROVIDER_

// includes
#include "PATInterfaces/ISystematicsTool.h"
#include "AnalysisCamEvent/CamTool.h"

#include "xAODMuon/Muon.h"
#include "xAODMuon/MuonContainer.h"


#if ROOTCORE_RELEASE_SERIES<25
#include "MuonMomentumCorrections/IMuonCalibrationAndSmearingTool.h"
#else
#include "MuonAnalysisInterfaces/IMuonCalibrationAndSmearingTool.h"
#endif



class CamMuonProvider : virtual public CamTool {

  public:
      using CamTool::apply;
  
      // Constructors, destructor
      CamMuonProvider(const std::string& type, const std::string& name,const IInterface* pParent);
      virtual ~CamMuonProvider();
      //StatusCode queryInterface( const InterfaceID& riid, void** ppvIf );

      virtual StatusCode initialize();
      virtual StatusCode finalize();

      //main method ... puts a CamVector into the event, containing the calibrated muons
      virtual StatusCode apply(CamEvent* event, std::string& label, const std::string& option);



   private:
      //configurable properties
      std::string m_muonCollection; 
      ToolHandle<CP::IMuonCalibrationAndSmearingTool> m_muonCalibTool;
      ToolHandle<CP::IMuonCalibrationAndSmearingTool> m_muonCalibTool2017;
      
};


#endif
