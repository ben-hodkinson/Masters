#include "CamJetSelector.h"


#include <boost/algorithm/string.hpp>
#include "xAODCore/ShallowCopy.h"


CamJetSelector::CamJetSelector(const std::string& type, const std::string& name,
			 const IInterface* pParent) : CamTool(type, name, pParent), chanInfo("ChannelInfoTool/cutflow",this),m_jetCleaningTool("",this),m_jetBTaggingSelectionTools(this),m_jvtTool("",this)
{
   declareProperty("InputCollection",m_inputJets="CamJetProvider/CalibratedJets", "Name of the input jet collection");
   declareProperty("OutputDecoration",m_outputDecor="","If specified, will not filter input collection, will just decorate with a char to flag pass/fail");
   
   
   //declareProperty("IDSelections",m_IDSelections={},"will go into 'quality' decoration, tightest (smallest quality value = 0) to loosest (biggest quality value)");
   declareProperty("BTaggingWorkingPoints",m_btagWP={},"vector of strings indicating btagging working points, syntax: Tagger:WP ... will decorate is<Tagger><WP> for each WP, e.g. MV2c10:FixedCutBEff_77");
   
   declareProperty("MinPt",m_minPt=0., "pt cut");
   declareProperty("MinForwardJetPt",m_minFJPt=0., "optional (higher) pt cut for forward jets (constituent scale |eta| > 2.4)");
   declareProperty("MaxEta",m_maxEta=99., "max eta (calibrated)");
   declareProperty("CleaningCutLevel",m_cleaningCut="", "to apply cleaning, specify: LooseBad, LooseBadLLP, LooseBadTrigger, TightBad ... stick 'Ugly' on the end of any to activate Ugly");
   declareProperty("JVTWorkingPoint",m_jvtWP="","Choose from Medium or Tight");
   
   
   declareProperty("PrintCutflow",m_printCutflow=false,"Store a cutflow inside a TChannelInfo");

}



StatusCode CamJetSelector::initialize() { 
ATH_MSG_INFO("init with InputCollection = " << m_inputJets);


  if(!m_cleaningCut.empty()) {
    m_jetCleaningTool.setTypeAndName("JetCleaningTool/JetCleaningTool");
    if(m_cleaningCut.find("Ugly")==m_cleaningCut.size()-4) {
      CHECK( AAH::setProperty( m_jetCleaningTool , "CutLevel" , m_cleaningCut.substr(0,m_cleaningCut.size()-4) ) );
      CHECK( AAH::setProperty( m_jetCleaningTool , "DoUgly" , true ) );
    } else {
      CHECK( AAH::setProperty( m_jetCleaningTool , "CutLevel" , m_cleaningCut ) );
      CHECK( AAH::setProperty( m_jetCleaningTool , "DoUgly" , false ) );
    }
    CHECK( AAH::setProperty( m_jetCleaningTool , "OutputLevel" , MSG::WARNING ) );
  }
  
  //in case input collection is another CamTool, attempt to register the systematics of it
  registerSystematics(m_inputJets);
  
  if(!m_jvtWP.empty()) {
    m_jvtTool.setTypeAndName("CP::JetJvtEfficienct/jvt");
    CHECK( AAH::setProperty( m_jvtTool , "WorkingPoint" , m_jvtWP ) );
    CHECK( AAH::setProperty( m_jvtTool , "OutputLevel" , MSG::WARNING ) );
    registerSystematics( &*m_jvtTool );
  }
  
  
  
  if(m_btagWP.size()) {
      for(auto& wp : m_btagWP) {
        if(wp.find(":")==std::string::npos) {
          ATH_MSG_ERROR("BTaggingWorkingPoints must be specified in Tagger:WP syntax ... " << wp << " does not satisfy this");
          return StatusCode::FAILURE;
        }
        std::string tagger = wp.substr(0,wp.find(":"));
        std::string oPoint = wp.substr(wp.find(":")+1);
        
        std::string author = m_inputJets;
        while(author.find("CamJetProvider")==0||author.find("CamJetSelector")==0) {
          //determine author from tool properties ... keep iterating until hit raw collection
          ToolHandle<ICamTool> tool(author);
          author = dynamic_cast<AlgTool*>(&*tool)->getProperty("InputCollection").toString();
        } 
        
        ToolHandle<IAlgTool> tool("BTaggingSelectionTool/"+wp,this);
        CHECK( AAH::setProperty( tool, "MaxEta",    2.5) ); //FIXME??
        CHECK( AAH::setProperty( tool, "MinPt",     20000.) ); //FIXME??
        CHECK( AAH::setProperty( tool, "FlvTagCutDefinitionsFileName", "xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2017-07-02_v1.root") );
        CHECK( AAH::setProperty( tool, "TaggerName",     tagger) );
        CHECK( AAH::setProperty( tool, "OperatingPoint", oPoint) );
        CHECK( AAH::setProperty( tool, "JetAuthor",      author) );
        m_jetBTaggingSelectionTools.push_back( tool.typeAndName() );
        registerSystematics( &*tool );
        
        m_btagLabels.push_back("is" + tagger + oPoint);
        m_btagger.push_back(tagger); //is this right?
        
      }
  }
  




   return StatusCode::SUCCESS;

}
StatusCode CamJetSelector::finalize() { 

  if(m_printCutflow) chanInfo->Cutflow();

   return StatusCode::SUCCESS;

}




bool CamJetSelector::jetSelection(CamObject& c) {
   const xAOD::Jet* jet = c.ae<xAOD::Jet>();
   
  
   if(m_printCutflow) chanInfo->Fill("Jets",1);


   if(c.Pt()<m_minPt) return false; 
   if(m_printCutflow) chanInfo->Fill("Jets_ptCut",1);
   if(fabs(c.Eta())>m_maxEta) return false;
   if(m_printCutflow) chanInfo->Fill("Jet_etaCut",1);
   double jetEta = fabs( jet->jetP4(xAOD::JetEMScaleMomentum).eta() );
   if(jetEta>2.4) {
      //forward jet
      if(c.Pt()<m_minFJPt) return false; 
   }
   if(m_printCutflow) chanInfo->Fill("Jets_fjptCut",1);
   
   if(!m_cleaningCut.empty() && !m_jetCleaningTool->keep(*jet)) return false; 
   if(m_printCutflow) chanInfo->Fill("Jets_cleaningCut",1);
   
   if(!m_jvtTool.empty() && !m_jvtTool->passesJvtCut(*jet)) return false;
   if(m_printCutflow) chanInfo->Fill("Jets_jvtCut",1);
   
   //FIXME ... need to implement forward jvt cuts 
   
   if(!m_outputDecor.empty()) jet->auxdecor<char>(m_outputDecor) = true;
   
   int i=0;
   for(auto& tool : m_jetBTaggingSelectionTools) {
      double discrim = 0.;
      if (jet->btagging()->MVx_discriminant(m_btagger[i], discrim)){
       c.set(m_btagLabels[i])  = bool(tool->accept(c.Pt(), fabs(c.Eta()), discrim));
      }
      i++;
   }

    return true;

}


StatusCode CamJetSelector::apply(CamEvent* evt, std::string& label, const std::string&) {


    CamDeque* jets = evt->getDeque( m_inputJets , currentSystematicSet() );
    if(!jets) {
      ATH_MSG_ERROR("Could not retrieve " << m_inputJets << " (as a deque)"); 
      return StatusCode::FAILURE;
    }
    CamDeque* jets_select = 0;
    if(m_outputDecor.empty()) {
      jets_select = jets->subset( CamObject::Function(&CamJetSelector::jetSelection,this), false,false /*dont add to event, will do ourselves*/ );
    } else {
      //we only decorate the jets, 
      //ensure decoration present on at least 1 xAOD object in the container, otherwise other tools have troubles
      for(auto& jet : *jets->container() ) {jet->auxdecor<char>(m_outputDecor) = false;break;}//needed for OR tool ... must exist on at least one, so put it on all

      jets->all_call( CamObject::Function(&CamJetSelector::jetSelectionVoid,this) );
      jets_select = jets->subset( "1", false,false );
    }
    
    evt->addObject(jets_select,label); //add deque to event with required label
    
    
    return StatusCode::SUCCESS;
    
}
