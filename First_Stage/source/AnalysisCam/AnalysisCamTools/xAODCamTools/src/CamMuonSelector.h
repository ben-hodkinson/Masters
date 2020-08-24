



#ifndef _CAMMUONS_
#define _CAMMUONS_

// includes
#include "AnalysisCamEvent/CamTool.h"


#if ROOTCORE_RELEASE_SERIES<25
#include "MuonSelectorTools/IMuonSelectionTool.h"
#else
#include "MuonAnalysisInterfaces/IMuonSelectionTool.h"
#endif



#include "IsolationSelection/IIsolationSelectionTool.h"
#include "GeneralCamTools/IChannelInfoTool.h"

class CamMuonSelector : virtual public CamTool {

   public:
      using CamTool::apply;
       
      // Constructors, destructor
      CamMuonSelector(const std::string& type, const std::string& name,const IInterface* pParent);


      virtual StatusCode initialize();
      virtual StatusCode finalize();

      //main method ... puts a CamDeque into the event, containing the selected electrons
      virtual StatusCode apply(CamEvent* event, std::string& label, const std::string& option);

      

   private:
      bool muonSelection(CamObject& c); //selection method used to form the subset
      void muonSelectionVoid(CamObject& c) { muonSelection(c); } //used when just decorating
   
      //configurable properties
      std::string m_inputMuons;  //the input collection name (may be another CamTool)
      std::string m_outputDecor;
      bool m_printCutflow; //should tool track a cutflow or not?
      std::string m_configName;
      
      ToolHandle<IChannelInfoTool> chanInfo; //used to store a cutflow, if requested
      
      
      
      ToolHandle<CP::IMuonSelectionTool> m_muSelectionTool;
      ToolHandleArray<CP::IIsolationSelectionTool> m_isoSelectionTools;
      
      std::vector<std::string> m_isoSelections;
      double m_minPt;double m_minCaloTagPt;
      double m_maxEta;
      int m_maxID;
      int m_maxIsolation;
      double m_maxZ0SinTheta;
      double m_maxD0;
      double m_maxD0Sig;
      
      
      bool m_highPtID; bool m_lowPtEfficiencyID; bool m_preselID; 
      
      
};


#endif
