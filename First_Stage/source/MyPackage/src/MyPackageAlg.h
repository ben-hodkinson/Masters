#ifndef MYPACKAGE_MYPACKAGEALG_H
#define MYPACKAGE_MYPACKAGEALG_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"

#include "SUSYTools/ISUSYObjDef_xAODTool.h"

// Trigger Tool
#include "TrigDecisionTool/TrigDecisionTool.h"

#include "PATInterfaces/SystematicSet.h"
#include "AnalysisCamEvent/CamEvent.h"

class MyPackageAlg: public ::AthAnalysisAlgorithm { 
 public: 
  MyPackageAlg( const std::string& name, ISvcLocator* pSvcLocator );
  virtual ~MyPackageAlg(); 
                                        //IS EXECUTED:
  virtual StatusCode  initialize();     //once, before any input is loaded
  virtual StatusCode  execute();        //per event
  virtual StatusCode  finalize();       //once, after all events processed
  
  virtual StatusCode  beginInputFile(); //start of each input file, only metadata loaded

 private: 

   void selectElectrons(xAOD::ElectronContainer* electrons, CamVector* camElectrons);

   void selectMuons(xAOD::MuonContainer* muons, CamVector* camMuons);

   void selectJets(xAOD::JetContainer* jets, CamVector* camJets, int p);

   StatusCode setupSystematics();
   bool m_firstEvent = true;
   float m_totalEvents = 0.0;
   float m_totalWeightedEvents = 0.0;

   float m_isData = false;

   ToolHandle<ST::ISUSYObjDef_xAODTool> m_SUSYTools;

   std::vector<CP::SystematicSet> sysList;

}; 

#endif //> !MYPACKAGE_MYPACKAGEALG_H
