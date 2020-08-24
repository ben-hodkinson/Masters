#ifndef _CAMMETADATA_
#define _CAMMETADATA_

///Adds dynamic properties that can attempt to infer D3PD MetaData
///This CamTool is considered "useful" enough by other CamTools to be included in the core package

// includes
#include "AnalysisCamEvent/CamTool.h"
#include "GaudiKernel/IIncidentSvc.h"



class CamMetaData : virtual public CamTool {

  public:
      using CamTool::apply;
      // Constructors, destructor
      CamMetaData(const std::string& type, const std::string& name,const IInterface* pParent);
   
      virtual ~CamMetaData();
      //StatusCode queryInterface( const InterfaceID& riid, void** ppvIf );


      virtual StatusCode initialize();
      virtual StatusCode finalize();

      virtual StatusCode beginInputFile(); //set file quantities invalid

      virtual StatusCode apply(CamEvent* event,std::string& label, const std::string& option) { return apply(dynamic_cast<CamObject*>(event),label,option);}
      virtual StatusCode apply(CamObject* event,std::string& label, const std::string& option);

      std::string getMetaString(CamObject& event, va_list args); ///generic metadata string getter
      double getMetaDouble(CamObject& event, va_list args); ///generic metadata double getter
      int getMetaInt(CamObject& event, va_list args); ///generic metadata int getter

   private:
      Int_t ReadFile(const char* filename);

      TTree* m_metaTree; //read in from files

      std::vector<std::string> m_readFiles;
      std::vector<std::string> m_indexBranches;
      bool m_dump;
      

};


#endif
