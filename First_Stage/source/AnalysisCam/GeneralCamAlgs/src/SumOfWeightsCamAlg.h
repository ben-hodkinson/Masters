
#ifndef GENERALCAMALGS_GENERALCAMALGSALG_H
#define GENERALCAMALGS_GENERALCAMALGSALG_H 1

#include "AnalysisCamEvent/CamAlgorithm.h"
//Example ROOT Includes
//#include "TTree.h"
//#include "TH1D.h"

#include "GeneralCamTools/IChannelInfoTool.h"


class SumOfWeightsCamAlg: public ::CamAlgorithm { 
 public: 
  SumOfWeightsCamAlg( const std::string& name, ISvcLocator* pSvcLocator );
  virtual ~SumOfWeightsCamAlg() {}

  ///uncomment and implement methods as required

                                        //IS EXECUTED:
  virtual StatusCode  initialize();     //once, before any input is loaded
  virtual StatusCode  beginInputFile(); //start of each input file, only metadata loaded
  virtual StatusCode execute();

 private: 
  std::string m_outputName;
  ToolHandle<IChannelInfoTool> chanInfo;

  bool m_gotBookkeeper = false;

}; 

#endif //> !GENERALCAMALGS_GENERALCAMALGSALG_H
