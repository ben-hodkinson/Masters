#ifndef ANALYSISCAMEVENT_CAMEXAMPLEALG_H
#define ANALYSISCAMEVENT_CAMEXAMPLEALG_H 1

#include "AnalysisCamEvent/CamAlgorithm.h"



class CamExampleAlg: public ::CamAlgorithm { 
 public: 
  CamExampleAlg( const std::string& name, ISvcLocator* pSvcLocator );
  virtual ~CamExampleAlg(); 

  virtual StatusCode  initialize(); //called before event loop
  virtual StatusCode  execute(); //called once per event
  virtual StatusCode  finalize(); //called at end of event loop
  virtual StatusCode  beginInputFile(); //called at start of input file
  virtual StatusCode  firstExecute(); //called once, for the first execution

 private: 

   void myMemberFunction(CamObject& c);

}; 

#endif //> !ANALYSISCAMEVENT_CAMEXAMPLEALG_H
