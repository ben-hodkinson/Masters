///Keeps track of lumi information and writes it out to any histsvc file that has a registered TTree at the end of the job
#ifndef _XAODLUMISVC_
#define _XAODLUMISVC_

#include "AnalysisCamEvent/CamService.h"
#include "GoodRunsLists/TGoodRunsListReader.h"
#include "GoodRunsLists/TGoodRunsList.h"
#include "TObjString.h"


class xAODLumiSvc : virtual public CamService {

public:
  
  // Constructors, destructor
  xAODLumiSvc(const std::string& name, ISvcLocator *svcLoc);

  ~xAODLumiSvc();
  StatusCode initialize();
  StatusCode finish();
  StatusCode beginInputFile();

  virtual void handle(const Incident& inc);

private:  
  StatusCode readLumi();

   Root::TGoodRunsListReader* m_grlR;
   Root::TGoodRunsList m_grl;
   std::vector<TObjString*> m_lumiStrings;
   std::map<TString,bool> m_doneDirs;
   bool m_requireLumi;
   bool m_writeXML;
};


#endif
