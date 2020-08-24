#ifndef __CAMSERVICE__
#define __CAMSERVICE__

// includes
#include "AnalysisCamEvent/CamEvent.h"

#include "AthenaBaseComps/AthService.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"

#include "AnalysisCamEvent/ICamService.h"

#include "StoreGate/StoreGateSvc.h"
#include "GaudiKernel/ServiceHandle.h"

#include "TTree.h"
#include "TFile.h"

class CamService : public AthService, virtual public ICamService, 
                             virtual public IIncidentListener {

public:
  
  // Constructors, destructor
  CamService(const std::string& name, ISvcLocator *svcLoc);

  virtual ~CamService();
  virtual StatusCode sysInitialize();

  StatusCode queryInterface( const InterfaceID& riid, void** ppvIf ) {
      if(IID_ICamService == riid) {
         *ppvIf = dynamic_cast<ICamService*>(this);
         return StatusCode::SUCCESS;
      }
      return AthService::queryInterface(riid,ppvIf);
  }

  virtual void handle(const Incident& inc);
    

    virtual StatusCode initialize() { return StatusCode::SUCCESS; } //called on initialize
    virtual StatusCode execute() { return StatusCode::SUCCESS; } //called each event
    virtual StatusCode finalize() { return StatusCode::SUCCESS; } //called on finalize
    virtual StatusCode beginInputFile() { return StatusCode::SUCCESS; } //called when new file opened and the first event is loaded
    virtual StatusCode endInputFile() { return StatusCode::SUCCESS; } //called when a file is closing - may not have had any events

   //for now, decided services cannot be "applied" to objects - as would have to add CamService support into CamObjects apply() method
   virtual StatusCode apply(CamEvent*,std::string&,const std::string&)  {
         ATH_MSG_ERROR("This CamService cannot be applied to a CamEvent");
         return StatusCode::FAILURE;
      }
      virtual StatusCode apply(CamDeque*,std::string&,const std::string&) {
         ATH_MSG_ERROR("This CamService cannot be applied to a CamDeque/CamVector");
         return StatusCode::FAILURE;
      }
      virtual StatusCode apply(CamObject*,std::string&,const std::string&) {
         ATH_MSG_ERROR("This CamService cannot be applied to a CamObject");
         return StatusCode::FAILURE;
      }

      virtual void* get(std::string) {
         return 0;
      }

      ///CamVectors are converted to a CamDeque and applied to that. The CamDeque points to the CamVector members
      StatusCode apply(CamVector* v,std::string& label,const std::string& option) {
         //ATH_MSG_ERROR("This CamTool cannot be applied to a CamVector");
         //create a temporary deque to pass to the other apply method to apply to the vector elements
         if(v==0) {ATH_MSG_ERROR("CamVector* is zero"); return StatusCode::FAILURE;}
         CamDeque* tmpDeque = v->subsetDeque("1",true,false); //let the temporary deque adopt, but don't add to event... we'll destroy it
         StatusCode s = apply(tmpDeque,label,option);
         v->setPropertyMap(std::move(*(tmpDeque->getPropertyMap())));
         //take ownership of children again
         v->adopt();
         delete tmpDeque;
         return s;
      }

      virtual StatusCode apply(CamEvent* event,const std::string& option)  {
         std::string dummyLabel = "";
         return apply(event,dummyLabel,option);
      }
      virtual StatusCode apply(CamDeque* deque,const std::string& option) {
         std::string dummyLabel = "";
         return apply(deque,dummyLabel,option);
      }
      virtual StatusCode apply(CamObject* object,const std::string& option) {
         std::string dummyLabel = "";
         return apply(object,dummyLabel,option);
      }
      StatusCode apply(CamVector* vector,const std::string& option) {
         std::string dummyLabel = "";
         return apply(vector,dummyLabel,option);
      }

      ServiceHandle<StoreGateSvc>& evtStore() const {
         return m_evtStore;
      }


protected:
      virtual TFile* currentFile(); //returns a pointer to the current file
      virtual TTree* currentTree(); //returns a pointer to the main ttree in the current file

private:  
       mutable ServiceHandle<StoreGateSvc> m_evtStore;
      
   bool m_doNotify;
   bool m_doneFirst;
   TFile* m_currentFile;
   TTree* m_currentTree;
};


#endif

