

#ifndef _CAMTAUS_
#define _CAMTAUS_

#include "AnalysisCamEvent/CamTool.h"
#include "GeneralCamTools/IChannelInfoTool.h"
#include "TauAnalysisTools/ITauSelectionTool.h"


class CamTauSelector : virtual public CamTool {

  public:
    using CamTool::apply;
  
    CamTauSelector(const std::string& type, const std::string& name, const IInterface* pParent);
  
    virtual StatusCode initialize();
    virtual StatusCode finalize();
  
    virtual StatusCode apply(CamEvent* event, std::string& label, const std::string& option);
 
  private:
    bool tauSelection(CamObject& c);
    void tauSelectionVoid(CamObject& c) {tauSelection(c);}
  
    std::string m_inputTaus;
    std::string m_outputDecor;
    bool m_printCutflow;
    double m_maxEta;
  
    ToolHandle<IChannelInfoTool> chanInfo;
    ToolHandleArray<TauAnalysisTools::ITauSelectionTool> m_tauSelectionTools;
    ToolHandle<TauAnalysisTools::ITauSelectionTool> m_tauSelectionTool;
    bool m_CreateControlPlots;
    bool m_MuonOLR;
    bool m_EleOLR;
    std::vector<size_t> m_NTracks;
    std::vector<int> m_AbsCharges;
    std::vector<std::string> m_IDWorkingPoints;
    static int convertStrToJetIDWP(std::string sJetIDWP);
    double m_minPt;
    std::string m_ConfigPath;
    
    bool m_IgnoreAODFixCheck;
    bool m_RecalcEleOLR;
  
};

#endif
