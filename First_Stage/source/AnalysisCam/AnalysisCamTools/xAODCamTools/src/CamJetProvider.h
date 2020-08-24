
#ifndef _CAMJETPROVIDER_
#define _CAMJETPROVIDER_

// includes
#include "PATInterfaces/ISystematicsTool.h"
#include "AnalysisCamEvent/CamTool.h"

#include "JetCalibTools/IJetCalibrationTool.h"
#include "JetInterface/IJetModifier.h"
#include "JetCPInterfaces/ICPJetUncertaintiesTool.h"

class CamJetProvider : virtual public CamTool {

  public:
      using CamTool::apply;
      
      // Constructors, destructor
      CamJetProvider(const std::string& type, const std::string& name,const IInterface* pParent);
      virtual ~CamJetProvider();
      //StatusCode queryInterface( const InterfaceID& riid, void** ppvIf );

      virtual StatusCode initialize();
      virtual StatusCode finalize();

      //main method ... puts a CamVector into the event, containing the calibrated jets
      virtual StatusCode apply(CamEvent* event, std::string& label, const std::string& option);


   private:
      //configurable properties
      std::string m_jetCollection; 
      mutable ToolHandle<IJetCalibrationTool> m_jetCalibTool;
      mutable ToolHandle<ICPJetUncertaintiesTool> m_jetUncertsTool;
      
      
      //other
      ToolHandle<IJetModifier> m_fjvtTool;
      
      
};


#endif