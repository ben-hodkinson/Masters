#ifndef ANALYSISCAMEXAMPLE_ANALYSISCAMEXAMPLEALG_H
#define ANALYSISCAMEXAMPLE_ANALYSISCAMEXAMPLEALG_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"


#include "AnalysisCamEvent/CamEvent.h"

class AnalysisCamExampleAlg: public ::AthAnalysisAlgorithm { 
 public: 
  AnalysisCamExampleAlg( const std::string& name, ISvcLocator* pSvcLocator ) : AthAnalysisAlgorithm( name, pSvcLocator ){ }
  
                                        //IS EXECUTED:
  //virtual StatusCode  initialize();     //once, before any input is loaded
  //virtual StatusCode  beginInputFile(); //start of each input file, only metadata loaded
  virtual StatusCode  firstExecute();   //once, after first eventdata is loaded (not per file)
  virtual StatusCode  execute();        //per event
  //virtual StatusCode  endInputFile();   //end of each input file
  //virtual StatusCode  metaDataStop();   //when outputMetaStore is populated by MetaDataTools
  //virtual StatusCode  finalize();       //once, after all events processed
  

  ///Other useful methods provided by base class are:
  ///evtStore()        : ServiceHandle to main event data storegate
  ///inputMetaStore()  : ServiceHandle to input metadata storegate
  ///outputMetaStore() : ServiceHandle to output metadata storegate
  ///histSvc()         : ServiceHandle to output ROOT service (writing TObjects)
  ///currentFile()     : TFile* to the currently open input file
  ///retrieveMetadata(...): See twiki.cern.ch/twiki/bin/view/AtlasProtected/AthAnalysisBase#ReadingMetaDataInCpp


 private: 
    CP::SystematicSet m_elSysts, m_muSysts;

}; 

#endif //> !ANALYSISCAMEXAMPLE_ANALYSISCAMEXAMPLEALG_H
