



#ifndef _CAMJETS_
#define _CAMJETS_

// includes
#include "AnalysisCamEvent/CamTool.h"

#include "JetInterface/IJetSelector.h"
#include "JetJvtEfficiency/IJetJvtEfficiency.h"
#include "FTagAnalysisInterfaces/IBTaggingSelectionTool.h"


#include "GeneralCamTools/IChannelInfoTool.h"

class CamJetSelector : virtual public CamTool {

   public:
      using CamTool::apply;
       
      // Constructors, destructor
      CamJetSelector(const std::string& type, const std::string& name,const IInterface* pParent);


      virtual StatusCode initialize();
      virtual StatusCode finalize();

      //main method ... puts a CamDeque into the event, containing the selected electrons
      virtual StatusCode apply(CamEvent* event, std::string& label, const std::string& option);

      

   private:
      bool jetSelection(CamObject& c); //selection method used to form the subset
      void jetSelectionVoid(CamObject& c) { jetSelection(c); } //used when just decorating
   
      //configurable properties
      std::string m_inputJets;  //the input collection name (may be another CamTool)
      std::string m_outputDecor;
      bool m_printCutflow; //should tool track a cutflow or not?
      std::string m_configName;
      
      ToolHandle<IChannelInfoTool> chanInfo; //used to store a cutflow, if requested
      
      ToolHandle<IJetSelector> m_jetCleaningTool;
      ToolHandleArray<IBTaggingSelectionTool> m_jetBTaggingSelectionTools; //one for each WP
      ToolHandle<CP::IJetJvtEfficiency> m_jvtTool;
      
      double m_minPt;double m_minFJPt;
      double m_maxEta;
      
      std::string m_cleaningCut;
      bool m_passJvt;
      
      std::vector<std::string> m_btagWP; //btag working points ... just decorates
      std::string m_jvtWP;
      
      std::vector<std::string> m_btagLabels,m_btagger;
      
      
      
};


#endif
