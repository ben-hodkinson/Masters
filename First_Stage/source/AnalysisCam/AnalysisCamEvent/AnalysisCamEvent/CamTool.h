
#ifndef __CAMTOOL__
#define __CAMTOOL__

// includes
#include "AnalysisCamEvent/CamEvent.h"


#include "AnalysisCamEvent/ICamTool.h"
#include "AthenaBaseComps/AthAlgTool.h"

#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"

#include "TTree.h"
#include "TFile.h"

#include "GaudiKernel/ToolHandle.h"

#include "AthAnalysisBaseComps/AthAnalysisHelper.h"

class CamTool : public ::AthAlgTool, virtual public ICamTool, virtual public IIncidentListener {

  public:
      // Constructors, destructor
      CamTool(const std::string& type, const std::string& name,const IInterface* pParent);
   
      virtual ~CamTool();
      //StatusCode queryInterface( const InterfaceID& riid, void** ppvIf );

      virtual StatusCode sysInitialize();

      virtual void handle(const Incident& inc);

      virtual StatusCode initialize() { return StatusCode::SUCCESS; } //called on initialize
      virtual StatusCode execute() { return StatusCode::SUCCESS; } //called each event
      virtual StatusCode finalize() { return StatusCode::SUCCESS; } //called on finalize
      virtual StatusCode beginInputFile() { return StatusCode::SUCCESS; } //called when new file opened



      virtual StatusCode apply(CamEvent*,std::string&,const std::string&)  {
         ATH_MSG_ERROR("This CamTool cannot be applied to a CamEvent");
         return StatusCode::FAILURE;
      }
      virtual StatusCode apply(CamDeque*,std::string&,const std::string&) {
         ATH_MSG_ERROR("This CamTool cannot be applied to a CamDeque/CamVector");
         return StatusCode::FAILURE;
      }
      virtual StatusCode apply(CamObject*,std::string&,const std::string&) {
         ATH_MSG_ERROR("This CamTool cannot be applied to a CamObject");
         return StatusCode::FAILURE;
      }

      virtual void* get(const std::string&) {
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
         if(CamObject::Documentation) { v->setDocuMap(std::move(*(tmpDeque->getDocuMap()))); }
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

      virtual void applySystematicSet(const CP::SystematicSet& syst) final;

      virtual const CP::SystematicSet& getSystematics() const { return m_registeredSysts; }

   protected:
      virtual TFile* currentFile(); //returns a pointer to the current file
      virtual TTree* currentTree(); //returns a pointer to the main ttree in the current file
      virtual const CP::SystematicSet& currentSystematicSet() const { return m_currentSyst; }
      
      //register systematics associated to a given tool
      void registerSystematics(const std::string& toolName);
      void registerSystematics(IAlgTool* tool);

      virtual ToolHandle<ICamTool> getToolHandle(const std::string& toolName); //get a ToolHandle on the given tool and try to retrieve

      template<typename T> T* getTool(const std::string& toolName,const std::string& objName="") {
         return static_cast<T*>(getToolHandle(toolName)->get(objName));
      }

      //helper method to determine if something is data, fullSim, or fastSim
      //at the moment it doesn't cache the result, so user should assume this is 'expensive'
      enum DataType { DATA , FULLSIM , FASTSIM };
      DataType getDataType();

   private:
      bool m_doNotify;bool m_doneFirst;
      TFile* m_currentFile;
      TTree* m_currentTree;
      
      
      CP::SystematicSet m_registeredSysts; //the registered systematics - done in initialize
      CP::SystematicSet m_currentSyst; //the currently affecting systematics 
      
      CP::ISystematicsTool* m_thisAsSystematicsTool = 0;
      std::vector<ICamTool*> m_subCamTools; //vector of tools that have been registeredSystematics 
      std::vector<CP::ISystematicsTool*> m_systTools; //tools that have been registeredSystematics;
};


#endif