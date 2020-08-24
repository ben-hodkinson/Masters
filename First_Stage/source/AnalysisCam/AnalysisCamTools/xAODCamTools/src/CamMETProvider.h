#ifndef _CAMMETPROVIDER_
#define _CAMMETPROVIDER_

// includes
#include "PATInterfaces/ISystematicsTool.h"
#include "AnalysisCamEvent/CamTool.h"

#include "xAODMissingET/MissingETContainer.h"
#include "xAODMissingET/MissingETAuxContainer.h"
#include "xAODMissingET/MissingETAssociationMap.h"

#include "METInterface/IMETMaker.h"

class CamMETProvider : virtual public CamTool {

  public:
      using CamTool::apply;
  
      // Constructors, destructor
      CamMETProvider(const std::string& type, const std::string& name,const IInterface* pParent);
      virtual ~CamMETProvider();
      //StatusCode queryInterface( const InterfaceID& riid, void** ppvIf );

      virtual StatusCode initialize();
      virtual StatusCode execute(); //to reset every event
      virtual StatusCode finalize();

      //main method ... puts a CamVector into the event, containing the calibrated muons
      virtual StatusCode apply(CamEvent* event, std::string& label, const std::string& option);


      
   private:
      StatusCode addHardObject(CamEvent* evt, const std::string& collection, const std::string& selection, const std::string& metPart, xAOD::Type::ObjectType type, xAOD::MissingETContainer* metCont);
   
      //configurable properties
      std::string m_muonCollection, m_eleCollection, m_tauCollection, m_muonSelection, m_eleSelection, m_tauSelection, m_jetCollection, m_inputMETContainer, m_inputMETMap; 
      
      bool m_eventLoaded = false;
      
      ToolHandle<IMETMaker> m_metMaker;
      
      const xAOD::MissingETContainer* m_metCont;
      const xAOD::MissingETAssociationMap* m_metMap;
      
};


#endif
