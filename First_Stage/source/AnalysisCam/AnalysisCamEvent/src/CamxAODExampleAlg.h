#ifndef ANALYSISCAMEVENT_CAMXAODEXAMPLEALG_H
#define ANALYSISCAMEVENT_CAMXAODEXAMPLEALG_H 1

#include "AnalysisCamEvent/CamAlgorithm.h"



class CamxAODExampleAlg: public ::CamAlgorithm { 
 public: 
  CamxAODExampleAlg( const std::string& name, ISvcLocator* pSvcLocator );
  virtual ~CamxAODExampleAlg(); 

  virtual StatusCode  initialize(); //called before event loop
  virtual StatusCode  execute(); //called once per event
  virtual StatusCode  finalize(); //called at end of event loop
  virtual StatusCode  beginInputFile(); //called at start of input file

 private: 

   void myMemberFunction(CamObject& c);

}; 

#endif //> !ANALYSISCAMEVENT_CAMXAODEXAMPLEALG_H
