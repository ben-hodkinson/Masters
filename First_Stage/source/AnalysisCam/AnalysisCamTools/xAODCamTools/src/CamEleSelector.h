



#ifndef _CAMELECTRONS_
#define _CAMELECTRONS_

// includes
#include "AnalysisCamEvent/CamTool.h"


#include "PATCore/IAsgSelectionTool.h"
#include "IsolationSelection/IIsolationSelectionTool.h"
#include "GeneralCamTools/IChannelInfoTool.h"
#include "ElectronPhotonSelectorTools/AsgElectronLikelihoodTool.h"

class CamEleSelector : virtual public CamTool {

   public:
      using CamTool::apply;
       
      // Constructors, destructor
      CamEleSelector(const std::string& type, const std::string& name,const IInterface* pParent);


      virtual StatusCode initialize();
      virtual StatusCode finalize();

      //main method ... puts a CamDeque into the event, containing the selected electrons
      virtual StatusCode apply(CamEvent* event, std::string& label, const std::string& option);

      

   private:
      bool electronSelection(CamObject& c); //selection method used to form the subset
      void electronSelectionVoid(CamObject& c) { electronSelection(c); } //used when just decorating
   
      //configurable properties
      std::string m_inputElectrons;  //the input collection name (may be another CamTool)
      std::string m_outputDecor;
      bool m_printCutflow; //should tool track a cutflow or not?
      std::string m_configName;
      
      ToolHandle<IChannelInfoTool> chanInfo; //used to store a cutflow, if requested
      
      
      
      
      ToolHandleArray<CP::IIsolationSelectionTool> m_isoSelectionTools;
      ToolHandleArray<IAsgSelectionTool> m_idSelectionTools; 
      ToolHandle<IAsgElectronLikelihoodTool> m_elecChargeIDSelectorTool;
      std::string m_eleChargeIDWP;
      std::vector<std::string> m_isoSelections;
      std::vector<std::string> m_IDSelections;
      double m_minPt;
      double m_maxEta;
      int m_maxID;
      int m_maxIsolation;
      double m_maxZ0SinTheta;
      double m_maxD0Sig;
      double m_maxD0;
      bool m_badCluster;
      int m_summaryIDLevel;
      
      int m_vvlooseIdx=-1; //used to do something slightly different
      
};


#endif
