#include "CamEleSelector.h"


#include <boost/algorithm/string.hpp>
#include "xAODCore/ShallowCopy.h"

#include "xAODTracking/TrackParticlexAODHelpers.h"

CamEleSelector::CamEleSelector(const std::string& type, const std::string& name,
			 const IInterface* pParent) : CamTool(type, name, pParent), chanInfo("ChannelInfoTool/cutflow",this), m_idSelectionTools(this)
{
   declareProperty("InputCollection",m_inputElectrons="CamElProvider/CalibratedElectrons", "Name of the input electron collection");
   declareProperty("OutputDecoration",m_outputDecor="","If specified, will not filter input collection, will just decorate with a char to flag pass/fail");
   
#if ROOTCORE_RELEASE_SERIES >= 25
   declareProperty("SelectorConfigName",m_configName="offline/mc16_20170828","The config fileset to use for the likelihood WP tools");
#else
   declareProperty("SelectorConfigName",m_configName="offline/mc15_20160512","The config fileset to use for the likelihood WP tools");
#endif
   
   declareProperty("IDWorkingPoints",m_IDSelections={},"will go into 'quality' decoration, tightest (smallest quality value = 0) to loosest (biggest quality value)");
   declareProperty("IsolationWorkingPoints",m_isoSelections={},"vector of strings indicating isolation working points ... will decorate isIsolXXXX for each selection");
   
   declareProperty("MinPt",m_minPt=0., "pt cut");
   declareProperty("MaxEta",m_maxEta=99., "max 2nd-layer calocluster eta (standard eta cut)");
   declareProperty("MaxID",m_maxID=-1, "Must pass at least IDSelection with this index .. -1 means no requirement, just decorate");
   declareProperty("MaxIsolation",m_maxIsolation=-1,"Must pass at least IsolationSelection with this index .. -1 means just decorate");
   declareProperty("MaxZ0SinTheta",m_maxZ0SinTheta=1000.,"Max z0SinTheta, in mm");
   declareProperty("MaxD0",m_maxD0=1000.,"Maximum d0, in mm");
   declareProperty("MaxD0Sig",m_maxD0Sig=1000.,"Maximim d0 significance");
   declareProperty("BadClusterCheck",m_badCluster=false,"If true, will check the BADCLUSTERELECTRON object quality bit");
   declareProperty("ElectronChargeIDWP", m_eleChargeIDWP = "", "Default is not to apply ... possible values are: Tight");
   
   declareProperty("SummaryIDLevel",m_summaryIDLevel=-1,"If specified, electrons passing this IDSelection level (quality) will get additional summary info");
   
   ///these are the expert properties ... most users shouldn't touch these
   declareProperty("IDSelectionTools",m_idSelectionTools,"Array of private IAsgSelectionTools ... order important, tightest to loosest. These go ahead of IDSelections");
   declareProperty("IsolationSelectionTools",m_isoSelectionTools,"Array of isolation tools to flag electrons with");
   
   declareProperty("PrintCutflow",m_printCutflow=false,"Store a cutflow inside a TChannelInfo");

}



StatusCode CamEleSelector::initialize() { 
ATH_MSG_INFO("init");
  //will add to m_isoSelectionTools some configured tools for the requested working points

  for(auto& wp : m_isoSelections) {
    ToolHandle<IAlgTool> tool("CP::IsolationSelectionTool/Isol"+wp); //NOTE: should we make these private tools?
    if(AAH::toolExists( tool )) {
        ATH_MSG_WARNING( tool.name() << " already exists... will use that instance");
    } else {
        CHECK( AAH::setProperty( tool, "ElectronWP", wp ) );
        CHECK( AAH::setProperty( tool, "MuonWP", wp ) );
        AAH::setProperty( tool, "OutputLevel", MSG::WARNING, false ).ignore();
    }
    m_isoSelectionTools.push_back( tool.typeAndName() );
  }
  
  std::string year = "2016";
  if(m_configName.find("mc16")!=std::string::npos) year="2017";
  
  //syntax for ID are:
  //X_Y -> ElectronLikelihoodXOfflineConfigYEAR_Y.conf
  
  m_vvlooseIdx = -1;
  int i=0;
  for(auto& id : m_IDSelections) {
    //special exception for vvlooseBL ... which uses the looseBL working point but only requires a couple of criteria to pass
    TString theID = id;
    if(theID.Index("VeryVeryLoose")==0) {
      theID = theID(8,theID.Length()); //uses the Loose tool
      m_vvlooseIdx=i;
    }
    ToolHandle<IAsgSelectionTool> tool(("AsgElectronLikelihoodTool/"+theID).Data(),this);
    
    TString part1 = (theID.Index("_")==-1) ? theID : theID(0,theID.Index("_"));
    TString part2 = (theID.Index("_")==-1) ? "" : theID(theID.Index("_"),theID.Length()).Data();
    
    
    CHECK( AAH::setProperty( tool , "ConfigFile", "ElectronPhotonSelectorTools/" + m_configName + "/ElectronLikelihood" + part1 + "OfflineConfig" + year + part2 + ".conf" ) );
    AAH::setProperty( tool , "OutputLevel", MSG::WARNING, false ).ignore();
    
    m_idSelectionTools.push_back(tool);
    i++;
  }

  //electron charge id
  if(!m_eleChargeIDWP.empty()) {
    m_elecChargeIDSelectorTool.setTypeAndName("AsgElectronChargeIDSelectorTool/ElectronChargeIDSelectorTool"); 
    //default cut value for https://twiki.cern.ch/twiki/bin/view/AtlasProtected/ElectronChargeFlipTaggerTool
    float BDTcut; 
    if( m_eleChargeIDWP == "Loose")  BDTcut = -0.288961; //97%
    else if( m_eleChargeIDWP == "Medium") BDTcut = -0.288961; //97%
    else if( m_eleChargeIDWP == "Tight") BDTcut = -0.325856; //97%
    CHECK( AAH::setProperty(m_elecChargeIDSelectorTool, "TrainingFile", "ElectronPhotonSelectorTools/ChargeID/ECIDS_20161125for2017Moriond.root" ));
    CHECK( AAH::setProperty(m_elecChargeIDSelectorTool, "CutOnBDT", BDTcut ));
  }

  //in case input collection is another CamTool, attempt to register the systematics of it
  registerSystematics(m_inputElectrons);
  
  //and the likelihood tools
  for(auto& tool : m_idSelectionTools){
    registerSystematics(&*tool);
  }


  //if(m_maxID >= m_idSelectionTools.size()) m_maxID = m_idSelectionTools.size()-1;

   return StatusCode::SUCCESS;

}
StatusCode CamEleSelector::finalize() { 

  if(m_printCutflow) chanInfo->Cutflow();

   return StatusCode::SUCCESS;

}

bool CamEleSelector::electronSelection(CamObject& c) {
   const xAOD::Electron* el = c.ae<xAOD::Electron>();
   
   if(m_printCutflow) chanInfo->Fill(name()+"_all",1);
   
   ///ELECTRON BASELINE SELECTIONS (before OR)
   //Pt cut
   if(c.Pt()<m_minPt) return false;   if(m_printCutflow) chanInfo->Fill(name()+"_ptCut",1);
   //Eta cut
   if(fabs(el->caloCluster()->etaBE(2))>m_maxEta) return false; if(m_printCutflow) chanInfo->Fill(name()+"_etaCut",1); //uses the 'first' calo cluster .. many could be associated to the electron
   
   //Quality
   if( m_badCluster && !el->isGoodOQ(xAOD::EgammaParameters::BADCLUSELECTRON) ) return false;    if(m_printCutflow) chanInfo->Fill(name()+"_OQCut",1);
   //z0 sin theta cut (0.5mm)
   if(m_maxZ0SinTheta < 999.) {
      double z0 = el->trackParticle(0)->z0() + el->trackParticle(0)->vz() - c.event()->get<double>("pvz"); //uses the 'first' track particle .. many could be associated to electron
      c.set("z0SinTheta") = float(fabs(z0*sin(el->trackParticle()->theta())));
      if( c.get("z0SinTheta") >= m_maxZ0SinTheta ) return false; if(m_printCutflow) chanInfo->Fill(name()+"_z0SinTheta",1);
   }
   
   //do loosest requirement 
   
   int quality=m_maxID;
   if(m_maxID >= int(m_idSelectionTools.size())) {
      //assumes quality is already decorated ... use that
      quality = c.get<int>("quality");
      if(quality > m_maxID) return false; //failed cut
   } else {
     if(m_maxID>=0) {
        if(m_maxID==m_vvlooseIdx) {
          Root::TAccept acc = m_idSelectionTools[m_maxID]->accept(el); 
          if(!(acc.getCutResult("NPixel") && acc.getCutResult("NSilicon"))) {
            return false; //failed
          }
        } else if(!m_idSelectionTools[m_maxID]->accept(el)) {
            return false; //failed
        }
      }
      
      //decorate electron with quality 
      //to do this, start at top and go to fail
      //only decorate a quality if we have selection tools
      if(m_idSelectionTools.size()) {
        int qualityStop = quality;
        if(quality<0) qualityStop=m_idSelectionTools.size();
        
        //now qualityStop = m_maxID or handleArray.size 
        //loop from start until we pass a selection, then stop there, that's the quality
        for(quality=0;quality<=qualityStop;quality++) {
          if(quality==qualityStop) break;
          if(quality==m_vvlooseIdx) {
            Root::TAccept acc = m_idSelectionTools[quality]->accept(el); 
            if((acc.getCutResult("NPixel") && acc.getCutResult("NSilicon"))) break;
          } else if(m_idSelectionTools[quality]->accept(el)) break;
        }

        c.set("quality")=quality;
        
      }
      
   }
   if(m_printCutflow) chanInfo->Fill(name()+"_ID",1);
   
   
   if(m_maxD0 < 999.) {
    float d0 = el->trackParticle()->d0();
    c.set("d0") = d0;
    if(std::abs(d0)>m_maxD0) return false;
    if(m_printCutflow) chanInfo->Fill(name()+"_d0",1);
   }
   
   if(m_maxD0Sig < 999.) {
     float d0_significance = xAOD::TrackingHelpers::d0significance( el->trackParticle(), c.event()->ae<xAOD::EventInfo>()->beamPosSigmaX(), c.event()->ae<xAOD::EventInfo>()->beamPosSigmaY(), c.event()->ae<xAOD::EventInfo>()->beamPosSigmaXY() );
     c.set("d0Sig") = float(std::abs(d0_significance));
     if(std::abs(d0_significance)>m_maxD0Sig) return false;
     if(m_printCutflow) chanInfo->Fill(name()+"_d0Sig",1);
   }
   
   if(!m_outputDecor.empty()) el->auxdecor<char>(m_outputDecor) = true;
   
   
   
   ///EXTRA DECORATIONS ON BASELINE ELECTRONS
   if(m_summaryIDLevel>=0) {
    if(quality<=m_summaryIDLevel) {
      
      
      //store a bunch of summary numbers too  ... to save space we only do it for loose leptons or better
      static const std::vector<xAOD::SummaryType> sumEnums = {xAOD::expectBLayerHit,
                                                              xAOD::numberOfBLayerHits,
                                                              xAOD::numberOfBLayerOutliers,
                                                              xAOD::expectInnermostPixelLayerHit,
                                                              xAOD::numberOfInnermostPixelLayerHits,
                                                              xAOD::numberOfInnermostPixelLayerOutliers,
                                                              xAOD::numberOfPixelHits,
                                                              xAOD::numberOfPixelDeadSensors,
                                                              xAOD::numberOfSCTHits,
                                                              xAOD::numberOfSCTDeadSensors};
                                                              
      static const std::vector<uint16_t> authorEnums = {xAOD::EgammaParameters::AuthorElectron,
                                                              xAOD::EgammaParameters::AuthorAmbiguous};
      c.set("summaryNum") = std::vector<int>(sumEnums.size()+authorEnums.size()+1,0);
      std::vector<int>& summaryNumbers = c.get<std::vector<int>&>("summaryNum");
      for(uint i=0;i<sumEnums.size();i++) {uint8_t v = 0; el->trackParticle()->summaryValue(v,sumEnums[i]); summaryNumbers[i]=v; }
      for(uint i=0;i<authorEnums.size();i++) summaryNumbers[sumEnums.size()+i] = el->author(authorEnums[i]);
      summaryNumbers.back() = el->nTrackParticles();
      
    } else {
      c.set("summaryNum") = std::vector<int>(0);
    }
   }

   
   for(auto& tool : m_isoSelectionTools) {
      std::string decorName="is"+tool->name().substr(8,100);
      c.set(decorName) = bool(tool->accept( *el ));
   }
   
   if(m_maxIsolation>=0 && !c.get<bool>("is"+m_isoSelectionTools[m_maxIsolation]->name().substr(8,100))) return false;

   //electron charge id
   if(!m_eleChargeIDWP.empty()) c.set("chargeId_"+m_eleChargeIDWP) = bool(m_elecChargeIDSelectorTool->accept(el));

   return true;
}



StatusCode CamEleSelector::apply(CamEvent* evt, std::string& label, const std::string&) {

    //this selection uses 'pvz' which is provided by the CamEventPreselectionTool
    if(!evt->contains<float>("pvz",CamObject::MAP|CamObject::STOREGATE)) {
      evt->applyTool("CamEventPreselectionTool","","v"); //only vertex selection needs doing
    }

    CamDeque* els = evt->getDeque( m_inputElectrons , currentSystematicSet() );
    CamDeque* els_select = 0;
    if(m_outputDecor.empty()) {
      els_select = els->subset( CamObject::Function(&CamEleSelector::electronSelection,this), false,false /*dont add to event, will do ourselves*/ );
    } else {
      //we only decorate the electrons, 
      //ensure decoration present on at least 1 xAOD object in the container, otherwise other tools have troubles
      for(auto& el : *els->container() ) {el->auxdecor<char>(m_outputDecor) = false;break;}//needed for OR tool ... must exist on at least one, so put it on all

      els->all_call( CamObject::Function(&CamEleSelector::electronSelectionVoid,this) );
      els_select = els->subset( "1", false,false );
    }
    
    evt->addObject(els_select,label); //add deque to event with required label
    
    
    return StatusCode::SUCCESS;
    
}
