
#ifndef __CAMALGORITHM__
#define __CAMALGORITHM__

// includes
#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"
//#include "GaudiKernel/IIncidentSvc.h"

#include "AnalysisCamEvent/CamEvent.h"

#include "TFile.h"
#include "TTree.h"

#include "AnalysisCamEvent/ICamTool.h"
#include "GaudiKernel/ToolHandle.h"

class CamAlgorithm : public AthAnalysisAlgorithm/*, virtual public IIncidentListener*/ {

   public:
    // These are the standard Athena public member functions.
    CamAlgorithm(const std::string& name, ISvcLocator* pSvcLocator);
    virtual ~CamAlgorithm();
	
    StatusCode sysInitialize(); 
    StatusCode sysExecute();

    virtual StatusCode initialize() { return StatusCode::SUCCESS; }
    virtual StatusCode execute() {  return StatusCode::SUCCESS; }
    virtual StatusCode finalize() { return StatusCode::SUCCESS; }
    virtual StatusCode beginInputFile() { return StatusCode::SUCCESS; }
    virtual StatusCode firstExecute() { return StatusCode::SUCCESS; }

    void handle(const Incident& incident);
   protected:
      int currentChannelNumber(); //returns runNumber in case of data, or channelNumber in case of MC (-1 indicates an error)
      virtual TTree* currentTree(); //returns a pointer to the main ttree in the current file

      virtual ToolHandle<ICamTool> getToolHandle(std::string toolName); //get a ToolHandle on the given tool and try to retrieve

      template<typename T> T* getTool(std::string toolName,std::string objName="") {
         return static_cast<T*>(getToolHandle(toolName)->get(objName));
      }

      void printStackTrace();

   private:
      mutable ServiceHandle< StoreGateSvc > m_inputMetaStore;

      unsigned int m_debugNumber;
      int m_tmpLevel=-1;
      bool m_doNotify;bool m_doneFirst;
      TTree* m_currentTree;
      int m_currentChannel=0;
};

#endif

