#include "CamElProvider.h"


#include <boost/algorithm/string.hpp>
#include "xAODCore/ShallowCopy.h"
#include "xAODBase/IParticleHelpers.h"

CamElProvider::CamElProvider(const std::string& type, const std::string& name,
			 const IInterface* pParent) : CamTool(type, name, pParent), 
	
						      m_elCalibTool("",this),
    m_isoCorrTool("")
{
   declareProperty("ElCollection",m_elCollection="Electrons", "Name of the electrons collection");

   declareProperty("ESModel",m_esModel="","Specify an ESModel, or leave blank for default");
   declareProperty("decorrelationModel",m_decorrModel="1NP_v1","Specify a different decorrelation model, or set blank to use default");

   declareProperty("DoIsoCorrection",m_doIsoCorrection=true,"can disable isolation corrections");

   declareProperty("ElCalibTool",m_elCalibTool=ToolHandle<CP::IEgammaCalibrationAndSmearingTool>(""), "The electron calibration tool ... if left empty, we auto configure");
   declareProperty("IsoCorrTool",m_isoCorrTool=ToolHandle<CP::IIsolationCorrectionTool>(""), "The isolation correction tool ... if left empty, we auto configure");

}

// Destructor
CamElProvider::~CamElProvider()
{}

StatusCode CamElProvider::initialize() { 
ATH_MSG_INFO("Init");
    
   CamTool::DataType dType = getDataType();
   bool isData = (dType==CamTool::DATA);
   bool isFastSim = (dType==CamTool::FASTSIM);
  
  if(m_elCalibTool.empty()) {
    //configure calib tool ourselves
    m_elCalibTool.setTypeAndName("CP::EgammaCalibrationAndSmearingTool/ElCalibTool");
    if(AAH::toolExists( m_elCalibTool )) {
      ATH_MSG_WARNING("ElCalibTool already exists ... will use that instance");
    } else {

    #if ROOTCORE_RELEASE_SERIES >= 25
      CHECK( AAH::setProperty( m_elCalibTool , "ESModel", (m_esModel.empty()) ? "es2017_R21_v1" : m_esModel ) );
    #else
      CHECK( AAH::setProperty( m_elCalibTool , "ESModel", (m_esModel.empty()) ? "es2016data_mc15c" : m_esModel ) );
    #endif
      if(!m_decorrModel.empty()) CHECK( AAH::setProperty( m_elCalibTool , "decorrelationModel", "1NP_v1" ) );
      CHECK( AAH::setProperty( m_elCalibTool , "useAFII" , isFastSim ? 1 : 0 ) );
      AAH::setProperty( m_elCalibTool , "OutputLevel" , msg().level()+1, false ).ignore(); //example of setting property without overriding existing value, if specified
    }
    
  }
  
  if(m_doIsoCorrection) {
    if(m_isoCorrTool.empty()) {
      //configure calib tool ourselves
      m_isoCorrTool.setTypeAndName("CP::IsolationCorrectionTool/IsoCorrTool");
      CHECK( AAH::setProperty( m_isoCorrTool , "IsMC" , !isData ) );
      CHECK( AAH::setProperty( m_isoCorrTool , "AFII_corr" , isFastSim ? 1 : 0 ) );
    }
  }
  
  //register systematics of tools (if any)
  registerSystematics( &*m_elCalibTool );
  if(m_doIsoCorrection) registerSystematics( &*m_isoCorrTool );

  return StatusCode::SUCCESS;

}
StatusCode CamElProvider::finalize() { 

   return StatusCode::SUCCESS;

}


StatusCode CamElProvider::apply(CamEvent* evt, std::string& label, const std::string& option) {

    //first check if calibrated electrons already present in storegate ... if so then we do nothing 
    if( evtStore()->contains<xAOD::ElectronContainer>( label ) ) {
      ATH_MSG_DEBUG(label << " already present in storegate, so will use that collection");
      
      xAOD::ElectronContainer* electrons = evtStore()->retrieve<xAOD::ElectronContainer>(label);
      
      //put in the event
      //default will be to create a vector
      if(option.length()==0 || option=="v") evt->createVector(label,electrons);
      else evt->createDeque(label,electrons);
      
      return StatusCode::SUCCESS;
    }


    //tool needs a RandomRunNumber ... can get by running the CamEventPreselectionTool (with option 'p') 
    if(!evt->contains<uint>("RandomRunNumber",CamObject::AUXELEMENT)) {
      evt->applyTool("CamEventPreselectionTool","","p");
    }


    //new electrons
    const xAOD::ElectronContainer* xaod_electrons = 0;  
    CHECK( evtStore()->retrieve( xaod_electrons, m_elCollection ) );
    //chronoSvc()->chronoStart( "CamElProvider::calibrate_electrons" );
    auto calibratedElectrons = xAOD::shallowCopyContainer( *xaod_electrons );
    bool setLinks = xAOD::setOriginalObjectLink(*xaod_electrons, *calibratedElectrons.first);
    if (!setLinks) {
        ATH_MSG_INFO("Failed to set original object link for calibrated muons");
    }
    
        for(auto el : *calibratedElectrons.first) {
          m_elCalibTool->applyCorrection(*el).setChecked();
          if(m_doIsoCorrection) m_isoCorrTool->applyCorrection(*el).setChecked(); //FIXME should check return code
        }
    
    //chronoSvc()->chronoStop( "CamElProvider::calibrate_electrons" );
    
    //put in the storegate so that they are cleaned up automatically
    CHECK( evtStore()->record( calibratedElectrons.first , label) ); 
    CHECK( evtStore()->record( calibratedElectrons.second , label + "Aux.") );
    
    //put in the event
    //default will be to create a vector
    if(option.length()==0 || option=="v") evt->createVector(label,calibratedElectrons.first);
    else evt->createDeque(label,calibratedElectrons.first);
    
    return StatusCode::SUCCESS;
    
}
