#include "CamJetProvider.h"



#include "xAODCore/ShallowCopy.h"
#include "xAODBase/IParticleHelpers.h"

CamJetProvider::CamJetProvider(const std::string& type, const std::string& name,
			 const IInterface* pParent) : CamTool(type, name, pParent), 
	
	m_jetCalibTool(""),
	m_fjvtTool("JetForwardJvtTool/ForwardJVT")
{
   declareProperty("InputCollection",m_jetCollection="AntiKt4EMTopoJets", "Name of the jet collection");
   declareProperty("JetCalibTool",m_jetCalibTool=ToolHandle<IJetCalibrationTool>(""), "The calibration tool ... if left empty, we auto configure");
   declareProperty("JetUncertaintiesTool",m_jetUncertsTool=ToolHandle<ICPJetUncertaintiesTool>(""), "The calibration tool ... if left empty, we auto configure");

}

// Destructor
CamJetProvider::~CamJetProvider()
{}

StatusCode CamJetProvider::initialize() { 
ATH_MSG_INFO("Init");
   
   if(m_jetCalibTool.empty() || m_jetUncertsTool.empty()) {

   CamTool::DataType dType = getDataType();
   bool isData = (dType==CamTool::DATA);
   bool isFastSim = (dType==CamTool::FASTSIM);
      
    
    if(m_jetCalibTool.empty()) {
  
      //calibrate tool ourselves
      m_jetCalibTool.setTypeAndName("JetCalibrationTool/"+name()+"_calibtool");
      CHECK( AAH::setProperty( m_jetCalibTool , "IsData" , isData ) );
      std::string jetColl = m_jetCollection;
      CHECK( AAH::setProperty( m_jetCalibTool , "JetCollection", jetColl.erase(jetColl.find("Jets"),4) ) ); //FIXME: infer from JetCollection!!
      
      CHECK( AAH::setProperty( m_jetCalibTool , "OutputLevel", msg().level()+1 ) );
#if ROOTCORE_RELEASE_SERIES >= 25
      if (isData) {
          CHECK( AAH::setProperty( m_jetCalibTool , "CalibSequence", "JetArea_Residual_EtaJES_GSC_Insitu" ) );
      } else {
          CHECK( AAH::setProperty( m_jetCalibTool , "CalibSequence", isFastSim ? "JetArea_Residual_EtaJES_GSC" : "JetArea_Residual_EtaJES_GSC_Smear" ) );
      }
      CHECK( AAH::setProperty( m_jetCalibTool , "ConfigFile" , (jetColl.find("PFlow")!=std::string::npos) ? "JES_data2017_2016_2015_Recommendation_PFlow_Aug2018_rel21.config" : "JES_data2017_2016_2015_Recommendation_Aug2018_rel21.config" ) );
      CHECK( AAH::setProperty( m_jetCalibTool , "CalibArea" , "00-04-81" ) );
#else
      CHECK( AAH::setProperty( m_jetCalibTool , "CalibSequence", "JetArea_Residual_Origin_EtaJES_GSC" ) );
      CHECK( AAH::setProperty( m_jetCalibTool , "ConfigFile" , isFastSim ? "JES_MC15Prerecommendation_AFII_June2015.config" : "JES_MC15cRecommendation_May2016.config" ) );
#endif
      
      if(msgLevel()>=MSG::INFO) CHECK( AAH::setProperty( m_jetCalibTool, "OutputLevel", MSG::WARNING) ); //because tools are too noisy!
      
    }
    
    if(m_jetUncertsTool.empty()) {
      std::string jetColl = m_jetCollection;
      jetColl.erase(jetColl.find("Jets"),4);
      if(jetColl!="AntiKt4EMPFlow") {
        //calibrate ourselves
        m_jetUncertsTool.setTypeAndName("JetUncertaintiesTool/"+name()+"_systtool");
        CHECK( AAH::setProperty( m_jetUncertsTool , "JetDefinition" ,  jetColl) ); //FIXME: infer
        CHECK( AAH::setProperty( m_jetUncertsTool , "IsData" , isData ) ); //FIXME: infer
        CHECK( AAH::setProperty( m_jetUncertsTool , "MCType" , isFastSim ? "AFII" : "MC16" ) );
        CHECK( AAH::setProperty( m_jetUncertsTool , "ConfigFile" , "rel21/Summer2018/R4_GlobalReduction_FullJER.config" ) );
        CHECK( AAH::setProperty( m_jetUncertsTool , "CalibArea" , "CalibArea-05" ) );
        if(msgLevel()>=MSG::INFO) CHECK( AAH::setProperty( m_jetUncertsTool, "OutputLevel", MSG::WARNING) ); //because tools are too noisy!
      }
      
    }
  }

   //register the CP tools for systematics, as well as the requested jet collection,
   //because even that might be a CamTool-provided collection!
   if(!m_jetUncertsTool.empty()) registerSystematics( &*m_jetUncertsTool );
   registerSystematics( &*m_jetCalibTool );
   registerSystematics( m_jetCollection );
   
   return StatusCode::SUCCESS;

}
StatusCode CamJetProvider::finalize() { 

   return StatusCode::SUCCESS;

}


StatusCode CamJetProvider::apply(CamEvent* evt, std::string& label, const std::string& option) {

    

    //old way of getting jet collection
    //const xAOD::JetContainer* xaod_jets = 0;  
    //CHECK( evtStore()->retrieve( xaod_jets , m_jetCollection ) );
    
    //retrieve raw jets using getVector because JetCollection might be 
    //from a CamJet itself!
    CamVector* rawJets = evt->getVector( m_jetCollection );
    

    auto cont = rawJets->container(); //new way to get container
    if(cont==0) { ATH_MSG_ERROR("Failed to obtain xAOD Container for collection " << m_jetCollection); return StatusCode::FAILURE; } 
    auto calibratedJets = xAOD::shallowCopyContainer( *static_cast<const xAOD::JetContainer*>(cont) );
    xAOD::setOriginalObjectLink(*cont, *calibratedJets.first);
    if(!m_jetCalibTool.empty()) {
        for(auto jet : *calibratedJets.first) {
          m_jetCalibTool->applyCorrection(*jet).setChecked();
          if(!m_jetUncertsTool.empty()) m_jetUncertsTool->applyCorrection(*jet).setChecked(); //FIXME should check return code
        }
    }
    m_fjvtTool->modify(*calibratedJets.first); //decorates with passFJVT char
    //chronoSvc()->chronoStop( "CamJetProvider::calibrate_jets" );
    

    //put in the storegate so that they are cleaned up automatically
    CHECK( evtStore()->record( calibratedJets.first , label) ); 
    CHECK( evtStore()->record( calibratedJets.second , label + "Aux.") );
    
    //put in the event
    ATH_MSG_VERBOSE("Creating jets = " << label);
    if(option.length()==0 || option=="v") evt->createVector(label,calibratedJets.first);
    else evt->createDeque(label,calibratedJets.first);
    
    return StatusCode::SUCCESS;
    
}
