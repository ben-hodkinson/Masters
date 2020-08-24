// GeneralCamAlgs includes
#include "SumOfWeightsCamAlg.h"

#include "xAODCutFlow/CutBookkeeper.h"
#include "xAODCutFlow/CutBookkeeperContainer.h"
#include "EventInfo/EventStreamInfo.h"



SumOfWeightsCamAlg::SumOfWeightsCamAlg( const std::string& name, ISvcLocator* pSvcLocator ) : CamAlgorithm( name, pSvcLocator ),
  
  chanInfo("ChannelInfoTool/channelInfo")
{

  declareProperty( "OutputName", m_outputName = "channelInfo", "name of the output TChannelInfo" );

}

StatusCode SumOfWeightsCamAlg::initialize() { 
  chanInfo.setTypeAndName("ChannelInfoTool/" + m_outputName);
  return StatusCode::SUCCESS;
}

StatusCode SumOfWeightsCamAlg::execute() {
  if(!m_gotBookkeeper) {
    //manually adding to sum of events
    CamEvent evt;
    chanInfo->Fill(currentChannelNumber(),"Events_All",(evt.is("isSimulation")) ? double(evt.ae<xAOD::EventInfo>()->mcEventWeight()) : 1.);
  }
  return StatusCode::SUCCESS;
}



StatusCode SumOfWeightsCamAlg::beginInputFile() { 

  


  m_gotBookkeeper=false;
  if(inputMetaStore()->contains<xAOD::CutBookkeeperContainer>("CutBookkeepers")) {
    const xAOD::CutBookkeeperContainer* bks = 0;
    CHECK( inputMetaStore()->retrieve(bks, "CutBookkeepers") );
    const xAOD::CutBookkeeper* all = 0; int maxCycle=-1;
    for(auto cbk : *bks) {
        if(cbk->inputStream()=="StreamAOD" && cbk->name()=="AllExecutedEvents" && cbk->cycle()>maxCycle ) { maxCycle=cbk->cycle(); all = cbk; }
    }

    if(!all) {
      //try incomplete!
      CHECK( inputMetaStore()->retrieve(bks, "IncompleteCutBookkeepers") ); maxCycle=-1;
      for(auto cbk : *bks) {
        if(cbk->inputStream()=="StreamAOD" && cbk->name()=="AllExecutedEvents" && cbk->cycle()>maxCycle ) { maxCycle=cbk->cycle(); all = cbk; }
      }
    }
    
    if(all->nAcceptedEvents()!=0) {
    
      int chanNum = currentChannelNumber();
    
      if(chanNum==-1) {
        //use lumi blocks to see if we are in data ... if we are then we use chanNum = 0 ...
        if(inputMetaStore()->contains<SG::IAuxStore>("LumiBlocksAux.") || inputMetaStore()->contains<SG::IAuxStore>("IncompleteLumiBlocksAux.") ||  inputMetaStore()->contains<SG::IAuxStore>("SuspectLumiBlocksAux.")) {
          chanNum=0; //assuming data
        } else {
          ATH_MSG_ERROR("Failure getting channel number");
          return StatusCode::FAILURE;
        }
      }
  
      chanInfo->Fill(chanNum,"Events_All",all->sumOfEventWeights(),all->nAcceptedEvents(),all->sumOfEventWeightsSquared());
  
      
    }
    m_gotBookkeeper=true;

  }


  return StatusCode::SUCCESS;
}


