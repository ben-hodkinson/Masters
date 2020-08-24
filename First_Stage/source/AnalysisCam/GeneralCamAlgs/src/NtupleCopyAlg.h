#ifndef GENERALCAMALGS_NTUPLECOPYALG_H
#define GENERALCAMALGS_NTUPLECOPYALG_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"

//Example ROOT Includes
//#include "TTree.h"
//#include "TH1D.h"


class NtupleCopyAlg: public ::AthAnalysisAlgorithm { 
 public: 
  NtupleCopyAlg( const std::string& name, ISvcLocator* pSvcLocator );
  virtual ~NtupleCopyAlg() {}

  ///uncomment and implement methods as required

                                        //IS EXECUTED:
  virtual StatusCode  initialize();     //once, before any input is loaded
  virtual StatusCode  beginInputFile(); //start of each input file, only metadata loaded
  virtual StatusCode execute();

 private: 
  std::string m_outputStream;
  std::vector<std::string> m_metaItems;
  
  std::map<TString, std::pair<TTree*,std::string>> m_outputTrees; //first tree is output, second is file name of last input tree

}; 

#endif //> !GENERALCAMALGS_GENERALCAMALGSALG_H
