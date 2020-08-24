
#ifndef _CAMEVENTPRESELECTION_
#define _CAMEVENTPRESELECTION_

// includes
#include "AnalysisCamEvent/CamTool.h"

#include "AsgAnalysisInterfaces/IGoodRunsListSelectionTool.h"
#include "AsgAnalysisInterfaces/IPileupReweightingTool.h"

#include "TrigDecisionTool/TrigDecisionTool.h"
#include "GeneralCamTools/IChannelInfoTool.h"

#include "GoodRunsLists/TGoodRunsListReader.h"

class CamEventPreselectionTool : virtual public CamTool {

  public:
      using CamTool::apply;
      
      // Constructors, destructor
      CamEventPreselectionTool(const std::string& type, const std::string& name,const IInterface* pParent);
      virtual ~CamEventPreselectionTool();

      virtual StatusCode initialize();
      virtual StatusCode finalize();

      //main method ... puts "label" bool property in CamEvent if passes preselection
      virtual StatusCode apply(CamEvent* event, std::string& label, const std::string& option);


   private:
      //configurable properties
      std::vector<std::string> m_grl;
      ToolHandle<CP::IPileupReweightingTool> m_prwTool;
      bool m_printCutflow;
      
      ToolHandle<Trig::TrigDecisionTool> m_tdtTool;
      ToolHandle<IChannelInfoTool> chanInfo; //used to store a cutflow, if requested
      ToolHandle<IGoodRunsListSelectionTool> m_grlTool;
      
      bool m_allowSkipPRW = true;
      bool m_hasPRWConfigFiles = false;
      
      std::string m_shortName;
      
      std::vector<std::string> m_triggers;
      std::string m_triggerDir;
      
      struct Trigger {
        Trigger(unsigned int id, std::string t1, std::string t2="", Root::TGoodRunsList* g=0) : idx(id), trigger(t1) , dataTrigger((t2=="")?t1:t2), grl(g) { }
        unsigned int idx = 0;
        std::string trigger; //the actual trigger name
        std::string dataTrigger; //if different trigger for data
        Root::TGoodRunsList* grl = 0; //used to restrict trigger to certain runs
        bool disabled = false; //will skip this trigger if disabled
        const std::string& getTrigger(bool data) const { if(data) return dataTrigger; return trigger; }
        bool isActive(int runNumber, int lbNumber) const { 
          if(disabled) return false;
          if(!grl) return true;
          return (grl->HasRunLumiBlock(runNumber,0) || grl->HasRunLumiBlock(runNumber,lbNumber));
        }
      };
      
      std::vector<Trigger> m_triggerConf;
      std::vector<std::unique_ptr<Root::TGoodRunsListReader>> m_grlReaders;
      
      
};


#endif