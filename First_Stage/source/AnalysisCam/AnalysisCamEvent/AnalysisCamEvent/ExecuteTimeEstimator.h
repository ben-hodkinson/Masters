///Estimates the processing rate and, if has access to number of events, can estimate how long left to process
#ifndef _EXECUTETIMEESTIMATOR_
#define _EXECUTETIMEESTIMATOR_

#include "AnalysisCamEvent/CamService.h"

class ExecuteTimeEstimator : virtual public CamService {

public:
  using CamService::apply;
  // Constructors, destructor
  ExecuteTimeEstimator(const std::string& name, ISvcLocator *svcLoc);

  virtual ~ExecuteTimeEstimator();
  virtual StatusCode initialize();
  virtual StatusCode finalize();
  virtual StatusCode execute();
  
   virtual StatusCode apply(CamObject* obj,std::string&,std::string) {
         obj->set("TotalNumberOfEntries") = m_totalEntries;
         return StatusCode::SUCCESS;
      }

private:  
   long m_previousTime;
   long m_eventsSincePreviousTime;
   long m_totalEntries;
   long m_entriesLeft;
   bool m_firstEvent;

   double m_checkInterval;
};


#endif
