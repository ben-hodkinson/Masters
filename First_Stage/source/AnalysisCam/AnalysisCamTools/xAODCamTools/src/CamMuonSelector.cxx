#include "CamMuonSelector.h"


#include <boost/algorithm/string.hpp>
#include "xAODCore/ShallowCopy.h"

#include "xAODTracking/TrackParticlexAODHelpers.h"

CamMuonSelector::CamMuonSelector(const std::string& type, const std::string& name,
			 const IInterface* pParent) : CamTool(type, name, pParent), chanInfo("ChannelInfoTool/cutflow",this), m_muSelectionTool("")
{
   declareProperty("InputCollection",m_inputMuons="CamMuonProvider/CalibratedMuons", "Name of the input muon collection");
   declareProperty("OutputDecoration",m_outputDecor="","If specified, will not filter input collection, will just decorate with a char to flag pass/fail");
   
   
   //declareProperty("IDSelections",m_IDSelections={},"will go into 'quality' decoration, tightest (smallest quality value = 0) to loosest (biggest quality value)");
   declareProperty("IsolationWorkingPoints",m_isoSelections={},"vector of strings indicating isolation working points ... will decorate isIsolXXXX for each selection");
   
   declareProperty("MinPt",m_minPt=0., "pt cut");
   declareProperty("MinCaloTagPt",m_minCaloTagPt=0., "optional (higher) pt cut for calo-tagged muons");
   declareProperty("MaxEta",m_maxEta=99., "max 2nd-layer calocluster eta (standard eta cut)");
   declareProperty("MaxID",m_maxID=-1, "Must pass at least this quality -1 means no requirement, just decorate");
   declareProperty("MaxIsolation",m_maxIsolation=-1,"Must pass at least IsolationSelection with this index .. -1 means just decorate");
   declareProperty("MaxZ0SinTheta",m_maxZ0SinTheta=1000.,"Max z0SinTheta, in mm (not applied to standalone muons)");
   declareProperty("MaxD0",m_maxD0=1000.,"Max d0 in mm (not applied to standalone muons)");
   declareProperty("MaxD0Sig",m_maxD0Sig=1000.,"Maximim d0 significance");
   declareProperty("HighPtID",m_highPtID=false,"If true, must pass highPt WP");
   declareProperty("LowPtEfficiencyID",m_lowPtEfficiencyID=false,"If true, must pass lowPt WP");
   declareProperty("PreselID",m_preselID=false,"If true, do the basic preselection cuts");
  
   ///these are the expert properties ... most users shouldn't touch these
   //declareProperty("IDSelectionTools",m_idSelectionTools,"Array of private IAsgSelectionTools ... order important, tightest to loosest. These go ahead of IDSelections");
   declareProperty("IsolationSelectionTools",m_isoSelectionTools,"Array of isolation tools to flag electrons with");
   
   
   declareProperty("PrintCutflow",m_printCutflow=false,"Store a cutflow inside a TChannelInfo");

}



StatusCode CamMuonSelector::initialize() { 
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
  
  //muon selection tool ... very generic cuts, since we call the individual methods directly
  if(m_muSelectionTool.empty()) {
    m_muSelectionTool.setTypeAndName("CP::MuonSelectionTool/MuonSelectionTool");
    if(!AAH::toolExists(m_muSelectionTool)) {
      CHECK( AAH::setProperty( m_muSelectionTool, "OutputLevel", MSG::WARNING) );
      CHECK( AAH::setProperty( m_muSelectionTool, "MaxEta", 99) );
      CHECK( AAH::setProperty( m_muSelectionTool, "MuQuality", 2) );
    }
  }



  //in case input collection is another CamTool, attempt to register the systematics of it
  registerSystematics(m_inputMuons);
  
  registerSystematics(&*m_muSelectionTool);

   return StatusCode::SUCCESS;

}
StatusCode CamMuonSelector::finalize() { 

  if(m_printCutflow) chanInfo->Cutflow();

   return StatusCode::SUCCESS;

}


bool CamMuonSelector::muonSelection(CamObject& c) {
   const xAOD::Muon* mu = c.ae<xAOD::Muon>();

   if(m_printCutflow) chanInfo->Fill("Muons",1);
   
   ///MUON BASELINE SELECTIONS (before OR)
   
   if(std::abs(mu->eta()) > m_maxEta) return false; 
   if(m_printCutflow) chanInfo->Fill("Muons_EtaCut",1); 
   
   if(m_preselID) {
    if(!m_muSelectionTool->passedIDCuts(*mu) || !m_muSelectionTool->passedMuonCuts(*mu)) return false; 
    if(m_printCutflow) chanInfo->Fill("Muons_PreselCut",1); 
   }
   
   if(m_maxID>=0) {
      int qual = m_muSelectionTool->getQuality(*mu);
      if(qual > m_maxID) return false; 
      if(m_printCutflow) chanInfo->Fill("Muons_QualityCut",1); 
      const_cast<xAOD::Muon*>(mu)->auxdata<unsigned char>("quality") = qual; //added for STDM5 derivations, missing 'quality' used by IsolHelper
      c.set("quality") = qual;
   }
   
   if(m_highPtID && !m_muSelectionTool->passedHighPtCuts(*mu)) return false;
#if ROOTCORE_RELEASE_SERIES>=25
   if(m_lowPtEfficiencyID && !m_muSelectionTool->passedLowPtEfficiencyCuts(*mu)) return false;
#endif
   
   //Pt Cut
   if(mu->pt()<m_minPt || (mu->author()==xAOD::Muon::CaloTag && mu->pt()<m_minCaloTagPt) ) return false; if(m_printCutflow) chanInfo->Fill("Muons_ptCut",1); 
   //z0 sin theta cut (0.5mm)
   if(m_maxZ0SinTheta < 999.) {
    double z0 = mu->primaryTrackParticle()->z0() + mu->primaryTrackParticle()->vz() - c.event()->get<double>("pvz"); //pvz is actually float ... but leaving as double for sake of CI tests
    c.set("z0SinTheta") = (mu->muonType() != xAOD::Muon::MuonStandAlone) ? float(fabs(z0*sin(mu->primaryTrackParticle()->theta()))) : float(0.);
    if(c.get("z0SinTheta") >= m_maxZ0SinTheta ) return false; if(m_printCutflow) chanInfo->Fill("Muons_z0SinTheta",1); //z0 0.5mm cut, except on standalone muons 
   }
   
   if(m_maxD0 < 999.) { //d0 Cut (not on SA muons) (1mm)
    double d0 = mu->primaryTrackParticle()->d0();
    c.set("d0") = float(d0);
    if( mu->muonType() != xAOD::Muon::MuonStandAlone && fabs(d0)>=m_maxD0) return false; if(m_printCutflow) chanInfo->Fill("Muons_BASELINE",1); 
   }


   if(m_maxD0Sig < 999.) {
    float d0_significance = (mu->muonType() != xAOD::Muon::MuonStandAlone) ? xAOD::TrackingHelpers::d0significance( mu->primaryTrackParticle(), c.event()->ae<xAOD::EventInfo>()->beamPosSigmaX(), c.event()->ae<xAOD::EventInfo>()->beamPosSigmaY(), c.event()->ae<xAOD::EventInfo>()->beamPosSigmaXY() ) : 0.;
    if(fabs(d0_significance)>m_maxD0Sig) return false;
    c.set("d0Sig") = float(fabs(d0_significance));
   }
   
   
   if(!m_outputDecor.empty()) mu->auxdecor<char>(m_outputDecor) = true;

   ///EXTRA DECORATIONS ON BASELINE MUONS
   
   for(auto& tool : m_isoSelectionTools) {
      std::string decorName="is"+tool->name().substr(8,100);
      c.set(decorName) = bool(tool->accept( *mu ));
   }
   if(m_maxIsolation>=0 && !c.get<bool>("is"+m_isoSelectionTools[m_maxIsolation]->name().substr(8,100))) return false;



   return true;

}



StatusCode CamMuonSelector::apply(CamEvent* evt, std::string& label, const std::string&) {

    //this selection uses 'pvz' which is provided by the CamEventPreselectionTool
    if(!evt->contains<float>("pvz",CamObject::MAP|CamObject::STOREGATE)) {
      evt->applyTool("CamEventPreselectionTool","","v"); //only vertex selection needs doing
    }

    CamDeque* mus = evt->getDeque( m_inputMuons , currentSystematicSet() );
    CamDeque* mus_select = 0;
    if(m_outputDecor.empty()) {
      mus_select = mus->subset( CamObject::Function(&CamMuonSelector::muonSelection,this), false,false /*dont add to event, will do ourselves*/ );
    } else {
      //we only decorate the muons, 
      //ensure decoration present on at least 1 xAOD object in the container, otherwise other tools have troubles
      for(auto& mu : *mus->container() ) {mu->auxdecor<char>(m_outputDecor) = false;break;}//needed for OR tool ... must exist on at least one, so put it on all

      mus->all_call( CamObject::Function(&CamMuonSelector::muonSelectionVoid,this) );
      mus_select = mus->subset( "1", false,false );
    }
    
    evt->addObject(mus_select,label); //add deque to event with required label
    
    
    return StatusCode::SUCCESS;
    
}
