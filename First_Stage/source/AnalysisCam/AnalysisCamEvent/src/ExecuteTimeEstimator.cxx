#include "AnalysisCamEvent/ExecuteTimeEstimator.h"

//#include "AthenaKernel/errorcheck.h"
#include <ctime>

#include "TChain.h"

// Constructor
ExecuteTimeEstimator::ExecuteTimeEstimator(const std::string& name, ISvcLocator *svcLoc) :
  CamService(name, svcLoc),
  m_previousTime(0),m_eventsSincePreviousTime(0),m_totalEntries(0),m_entriesLeft(0),m_firstEvent(true)
{
      declareProperty("checkInterval", m_checkInterval=60.);
}

// Destructor
ExecuteTimeEstimator::~ExecuteTimeEstimator()
{

}


#include "GaudiKernel/IAppMgrUI.h"
#include "GaudiKernel/IJobOptionsSvc.h"

StatusCode ExecuteTimeEstimator::initialize() {
   //form chain from list of input files and get the total number of entries
   
   //ask the joboptions svc for the properties, do not ask the selector directly
   //this is because this invokes the selector's existence, which fires the beginInputFile for first file
   ServiceHandle<IJobOptionsSvc> joSvc("JobOptionsSvc","ExecuteTimeEstimator");
   auto props = joSvc->getProperties("EventSelector");
   if(!props) { ATH_MSG_ERROR("Couldn't find the EventSelector properties!!!??");return StatusCode::FAILURE; }
   std::string myString = "CollectionTree"; //assume POOL file format
   
   std::vector<std::string> m_inputCollectionsName;
   
   for(auto prop : *props) {
      if(prop->name()=="TupleName") myString = prop->toString();
      else if(prop->name()=="InputCollections") CHECK( Gaudi::Parsers::parse(m_inputCollectionsName,prop->toString()) );
   }
   if(!m_inputCollectionsName.size()) { ATH_MSG_ERROR("Couldn't find the input files!!!??");return StatusCode::FAILURE; }

   //only determine if necessary (theApp.EvtMax = -1) ...
   
   SmartIF<IProperty> theApp( Gaudi::createApplicationMgr() );
   std::string evtMax; CHECK( theApp->getProperty("EvtMax",evtMax) );
   CHECK(Gaudi::Parsers::parse( m_totalEntries, evtMax));
   if(m_totalEntries == -1) {
    ATH_MSG_INFO("  ===>>> Determining total number of entries .... <<<<===");
  
    TChain c(myString.c_str());
    for(unsigned int j=0;j<m_inputCollectionsName.size();j++) {
        c.Add(m_inputCollectionsName[j].c_str());
    }
    m_totalEntries = c.GetEntries();
   }
   ATH_MSG_INFO("  ===>>>  Entries to be processed = " << m_totalEntries << "  <<<===");
   m_entriesLeft = m_totalEntries;

  return StatusCode::SUCCESS;

}

StatusCode ExecuteTimeEstimator::finalize(){
    return StatusCode::SUCCESS;
}


StatusCode ExecuteTimeEstimator::execute() {

   time_t seconds;
   seconds = time(NULL);

   if(m_eventsSincePreviousTime==0) {
      m_previousTime = seconds;
   }

   m_eventsSincePreviousTime++;

   if((seconds - m_previousTime)>m_checkInterval) {
      m_previousTime = seconds;
      m_entriesLeft -= m_eventsSincePreviousTime; 
      double rateEst = ((double)m_eventsSincePreviousTime)/m_checkInterval;
      std::cout << "ExecuteTimeEstimator:  ===>>>  " << 100.*((double)(m_totalEntries-m_entriesLeft)/(m_totalEntries)) << "% complete (" << (m_totalEntries-m_entriesLeft) << " entries processed). Estimated Processing Rate = " << rateEst << " Hz. Estimated time remaining = " << (double)m_entriesLeft/((double)m_eventsSincePreviousTime) << " mins  <<<===" << std::endl;
      m_eventsSincePreviousTime=0;
   }

   return StatusCode::SUCCESS;

}
