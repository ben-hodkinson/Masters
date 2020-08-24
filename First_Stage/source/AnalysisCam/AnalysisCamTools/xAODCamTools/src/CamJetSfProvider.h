#ifndef _CAMJETSFPROVIDER_
#define _CAMJETSFPROVIDER_

// includes
#include "PATInterfaces/ISystematicsTool.h"
#include "AnalysisCamEvent/CamTool.h"
#include "JetJvtEfficiency/IJetJvtEfficiency.h"
#include "FTagAnalysisInterfaces/IBTaggingEfficiencyTool.h"

class CamJetSfProvider : virtual public CamTool {

 public:
  using CamTool::apply; //stops compiler warnings
  
  // Constructors, destructor
  CamJetSfProvider(const std::string& type, const std::string& name,const IInterface* pParent);
  virtual ~CamJetSfProvider();


  virtual StatusCode initialize();
  virtual StatusCode finalize();

  virtual StatusCode apply(CamDeque* deque, std::string& label, const std::string& option);

 private:
  ToolHandle<CP::IJetJvtEfficiency> m_jetJvtEfficiencyTool;
  std::vector<std::string> m_jetBtagWorkingPoints;
  std::unordered_map<std::string, ToolHandle<IBTaggingEfficiencyTool>> m_jetBtagEfficiencyTools;
  std::string m_jetBtagTaggerName;
  std::string m_jetAuthor;
  std::string m_jetBtagCaliFile;
};

#endif
