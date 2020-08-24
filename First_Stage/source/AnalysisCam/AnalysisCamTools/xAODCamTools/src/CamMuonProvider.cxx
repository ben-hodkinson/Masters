#include "CamMuonProvider.h"


#include <boost/algorithm/string.hpp>
#include "xAODCore/ShallowCopy.h"
#include "xAODBase/IParticleHelpers.h"

CamMuonProvider::CamMuonProvider(const std::string& type, const std::string& name,
			 const IInterface* pParent) : CamTool(type, name, pParent), 
	
    m_muonCalibTool(""), m_muonCalibTool2017("")
{
   declareProperty("MuonCollection",m_muonCollection="Muons", "Name of the muons collection");
   declareProperty("MuonCalibTool",m_muonCalibTool=ToolHandle<CP::IMuonCalibrationAndSmearingTool>(""), "The muon calibration tool ... if left empty, we auto configure");

}

// Destructor
CamMuonProvider::~CamMuonProvider()
{}

StatusCode CamMuonProvider::initialize() { 
ATH_MSG_INFO("Init");



  if(m_muonCalibTool.empty()) {
    //configure calib tool ourselves
    m_muonCalibTool.setTypeAndName("CP::MuonCalibrationAndSmearingTool/MuonCalibTool");
    CHECK( AAH::setProperty( m_muonCalibTool , "Year", "Data16" ) );
#if ROOTCORE_RELEASE_SERIES < 25
    //explicitly set release and year for the 20.7 code, for preservation!
    CHECK( AAH::setProperty( m_muonCalibTool , "Release", "Recs2016_15_07" ) );
#endif
    CHECK( AAH::setProperty( m_muonCalibTool , "SagittaCorr" , 0) );
    CHECK( AAH::setProperty( m_muonCalibTool , "doSagittaMCDistortion" , 0) );
    CHECK( AAH::setProperty( m_muonCalibTool , "OutputLevel" , msg().level()+1 ) );
    
#if ROOTCORE_RELEASE_SERIES >= 25
    //we will also create a tool for 2017 analysis ...
    m_muonCalibTool2017.setTypeAndName("CP::MuonCalibrationAndSmearingTool/MuonCalibTool2017");
    CHECK( AAH::setProperty( m_muonCalibTool2017 , "Year" , "Data17") );
    CHECK( AAH::setProperty( m_muonCalibTool2017 , "SagittaCorr" , 0) );
    CHECK( AAH::setProperty( m_muonCalibTool2017 , "doSagittaMCDistortion" , 0) );
    CHECK( AAH::setProperty( m_muonCalibTool2017 , "OutputLevel" , msg().level()+1 ) );
#endif
    
  }

  //register the systematics of the tool (if any) ...
  registerSystematics( &*m_muonCalibTool );
  if(!m_muonCalibTool2017.empty()) registerSystematics( &*m_muonCalibTool2017 );

  return StatusCode::SUCCESS;

}
StatusCode CamMuonProvider::finalize() { 

   return StatusCode::SUCCESS;

}



StatusCode CamMuonProvider::apply(CamEvent* evt, std::string& label, const std::string& option) {


    //new muons
    const xAOD::MuonContainer* xaod_muons = 0;  
    CHECK( evtStore()->retrieve( xaod_muons, m_muonCollection ) );
    //chronoSvc()->chronoStart( "CamMuonProvider::calibrate_muons" );
    auto calibratedMuons = xAOD::shallowCopyContainer( *xaod_muons );
    bool setLinks = xAOD::setOriginalObjectLink(*xaod_muons, *calibratedMuons.first);
    if (!setLinks) {
        ATH_MSG_INFO("Failed to set original object link for calibrated muons");
    }
    
    if(evt->get<unsigned int>("runNumber") >= 324300 || (evt->get<bool>("isSimulation")&&evt->ae<xAOD::EventInfo>()->runNumber()==300000) ) {
      //for data17 or mc16c, we use a different tool 
      if(!m_muonCalibTool2017.empty()) {
          for(auto muon : *calibratedMuons.first) {
            m_muonCalibTool2017->applyCorrection(*muon).setChecked(); //FIXME should check return code
          }
      }
    } else {
    
      if(!m_muonCalibTool.empty()) {
          for(auto muon : *calibratedMuons.first) {
            m_muonCalibTool->applyCorrection(*muon).setChecked(); //FIXME should check return code
          }
      }
    }
    //chronoSvc()->chronoStop( "CamMuonProvider::calibrate_muons" );
    
    //put in the storegate so that they are cleaned up automatically
    CHECK( evtStore()->record( calibratedMuons.first , label) ); 
    CHECK( evtStore()->record( calibratedMuons.second , label + "Aux.") );
    
    //put in the event
    //default will be to create a vector
    if(option.length()==0 || option=="v") evt->createVector(label,calibratedMuons.first);
    else evt->createDeque(label,calibratedMuons.first);
    
    return StatusCode::SUCCESS;
    
}
