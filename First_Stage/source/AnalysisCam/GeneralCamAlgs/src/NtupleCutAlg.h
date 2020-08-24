#ifndef GENERALCAMALGS_NTUPLECUTALG_H
#define GENERALCAMALGS_NTUPLECUTALG_H 1

#include "AnalysisCamEvent/CamAlgorithm.h"

//Example ROOT Includes
//#include "TTree.h"
//#include "TH1D.h"

#include "TTreeFormula.h"

class NtupleCutAlg: public ::CamAlgorithm { 
 public: 
  NtupleCutAlg( const std::string& name, ISvcLocator* pSvcLocator );
  virtual ~NtupleCutAlg() {}

  ///uncomment and implement methods as required

                                        //IS EXECUTED:
  virtual StatusCode  initialize();     //once, before any input is loaded
  virtual StatusCode  beginInputFile(); //start of each input file, only metadata loaded
  virtual StatusCode execute();
  virtual StatusCode finalize();

 private: 
  std::string m_expression;

  TTreeFormula* m_formula = 0;

  long m_total=0;
  long m_pass=0;

}; 

#endif //> !GENERALCAMALGS_GENERALCAMALGSALG_H
