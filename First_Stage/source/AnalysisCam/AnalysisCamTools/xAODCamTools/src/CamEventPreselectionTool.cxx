#include "CamEventPreselectionTool.h"

#include "xAODTracking/VertexContainer.h"

#include "xAODCore/ShallowCopy.h"

#include "PathResolver/PathResolver.h"

#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TPRegexp.h"

#include "PileupReweighting/TPileupReweighting.h"

CamEventPreselectionTool::CamEventPreselectionTool(const std::string& type, const std::string& name,
			 const IInterface* pParent) : CamTool(type, name, pParent),
	m_tdtTool("Trig::TrigDecisionTool/TrigDecisionTool"), chanInfo("ChannelInfoTool/cutflow",this),
	m_grlTool("GoodRunsListSelectionTool",this) /* private tool */
{
#if ROOTCORE_RELEASE_SERIES >= 25
   declareProperty("GRL",m_grl={"GoodRunsLists/data15_13TeV/20170619/physics_25ns_21.0.19.xml",
                                "GoodRunsLists/data16_13TeV/20180129/physics_25ns_21.0.19.xml",
                                "GoodRunsLists/data17_13TeV/20180309/physics_25ns_Triggerno17e33prim.xml"});
#else
   declareProperty("GRL",m_grl={"GoodRunsLists/data15_13TeV/20160720/physics_25ns_20.7.xml",
                             "GoodRunsLists/data16_13TeV/20170215/physics_25ns_20.7.xml"},"List of GoodRunsLists to use for selecting data (and for PRW)");
#endif

   declareProperty("PRWTool",m_prwTool=ToolHandle<CP::IPileupReweightingTool>(""), "The PRW tool ... if left empty, we auto configure");
   
   declareProperty("AllowSkipPRW",m_allowSkipPRW=true, "If true, will check if PRW is supported for the channel before calling the tool .. unsupported channels get weight=1");
   
   declareProperty("Triggers",m_triggers=std::vector<std::string>(),"List of triggers in format: trigger[<grl>][<DataTrigger>], where the last parts are optional");
   //further explanation:
   // A valid trigger could be:
   //   HLT_mu26   #assumes valid for all runs and for both mc and data
   //   HLT_mu26[MyPackage/trigGRL/HLT_mu26.xml] #valid for runs given in grl, for both mc and data
   //   HLT_mu26[][HLT_mu26_L1MU15] #valid for all runs, but for data will use the alternative trigger
   
   declareProperty("TriggerDir",m_triggerDir="","If specified, will use each xml file in the directory as a separate trigger config");
   
   declareProperty("PrintCutflow",m_printCutflow=false);


}

// Destructor
CamEventPreselectionTool::~CamEventPreselectionTool()
{}

StatusCode CamEventPreselectionTool::initialize() { 

   m_shortName = name();
   if(m_shortName.find("ToolSvc.")==0) m_shortName = m_shortName.substr(8,100); //strips off "ToolSvc."
   
   //pass on the GRL to our private GRL tool
   CHECK( AAH::setProperty(m_grlTool,"GoodRunsListVec",m_grl) );
   
   
   if(m_prwTool.empty()) {
      m_prwTool.setTypeAndName("CP::PileupReweightingTool/"+m_shortName+"_prwtool");
      if(AAH::toolExists(m_prwTool)) {
        ATH_MSG_WARNING(m_prwTool.name() << " already exists. Using that instance");
      } else {
        //FIXME: infer lumicalc files from GRL list 
#if ROOTCORE_RELEASE_SERIES >= 25
        std::vector<std::string> listOfLumicalcFiles = {"GoodRunsLists/data15_13TeV/20170619/PHYS_StandardGRL_All_Good_25ns_276262-284484_OflLumi-13TeV-008.root",
                                                        "GoodRunsLists/data16_13TeV/20180129/PHYS_StandardGRL_All_Good_25ns_297730-311481_OflLumi-13TeV-009.root",
                                                        "GoodRunsLists/data17_13TeV/20180309/physics_25ns_Triggerno17e33prim.lumicalc.OflLumi-13TeV-010.root"};
        
#else
        std::vector<std::string> listOfLumicalcFiles = {"GoodRunsLists/data15_13TeV/20160720/physics_25ns_20.7.lumicalc.OflLumi-13TeV-005.root",
                                                        "GoodRunsLists/data16_13TeV/20170215/physics_25ns_20.7.lumicalc.OflLumi-13TeV-008.root"};
        CHECK( AAH::setProperty( m_prwTool, "UsePeriodConfig", "MC15" ) ); 
#endif
        CHECK( AAH::setProperty( m_prwTool, "LumiCalcFiles", listOfLumicalcFiles ) );
        CHECK( AAH::setProperty( m_prwTool, "ConfigFiles", "[]" ) ); //no config files by default
        
      }
   }
    
    //check if the prwTool has config files ... will use to decide if we obtain pileup weight 
    //if no config files, we will just do the random run number 

    std::vector<std::string> cFiles; 
    Gaudi::Parsers::parse(cFiles,(dynamic_cast<IProperty*>(&*m_prwTool))->getProperty("ConfigFiles").toString()).ignore();
    if(cFiles.size()) {
      m_hasPRWConfigFiles = true;
      ATH_MSG_INFO("PRW has config files ... will do DATA_SF variations!");
    }



   if(!m_triggerDir.empty()) {
      m_triggerDir = PathResolverFindCalibDirectory(m_triggerDir);
      if(m_triggerDir.empty()) return StatusCode::FAILURE;

      //put the triggers on the front of the list 
      std::vector<std::string> tmpTriggers = m_triggers;
      m_triggers.clear();

      //get list of xml files in that directory ... each one we will use a separate trigger 
      TSystemDirectory dir("triggerDir",m_triggerDir.c_str());
      TList* files = dir.GetListOfFiles();
      if(files) {
        TSystemFile* file;
        TString fName;
        TIter next(files);
        while( (file=(TSystemFile*)next()) ) {
          if(file->IsDirectory()) continue; //ignore subdirectories 
          fName = file->GetName();
          if(!fName.EndsWith(".xml")) continue;
          m_triggers.push_back( Form("%s[%s/%s]",TString(fName(0,fName.Index("."))).Data(),file->GetTitle(),fName.Data()) );
          ATH_MSG_INFO("Loaded " << m_triggers.back());
        }
      }
      
      for(auto& t : tmpTriggers) m_triggers.push_back(t);
      
   }
   
   //need to read all the runs for trigger range configuration
   Root::TGoodRunsListReader allRunsReader;
   for(auto& grl : m_grl) {
    allRunsReader.AddXMLFile(PathResolverFindCalibFile(grl));
   }
   allRunsReader.Interpret();
   
   //need to parse the trigger list, populating GRL for each, as required 
   int counter=0; int maxCounter=0;
   for(auto& trig : m_triggers) {
    std::string mcTrig=trig;
    std::string grlFile="";
    std::string dataTrig="";
    
    if(trig.find("[")!=std::string::npos) {
      mcTrig = trig.substr(0,trig.find("["));
      grlFile = trig.substr(trig.find("[")+1,trig.find("]")-trig.find("[")-1);
      if(trig.find("[",trig.find("[")+1)!=std::string::npos) {
        auto startPos = trig.find("[",trig.find("[")+1);
        dataTrig = trig.substr(startPos+1,trig.find("]",startPos)-startPos-1);
      }
    }
    
    Root::TGoodRunsList* myGrl = 0;
    if(!grlFile.empty()) {
      //first locate the file
      grlFile = PathResolverFindCalibFile( grlFile );
      if(grlFile.empty()) return StatusCode::FAILURE;
      //need to load the GRL ...
      m_grlReaders.push_back( std::unique_ptr<Root::TGoodRunsListReader>(new Root::TGoodRunsListReader) );
      m_grlReaders.back()->AddXMLFile( grlFile );
      m_grlReaders.back()->Interpret();
      myGrl = new Root::TGoodRunsList( m_grlReaders.back()->GetGoodRunsList(0) );
      //check for RunList, will use that to 'add runs' 
      if(myGrl->GetMetaData().find("RunList")!=myGrl->GetMetaData().end()) {
        std::vector<std::string> runs; 
        TStringToken st(myGrl->GetMetaData().find("RunList")->second.Data(),",");
        while(st.NextToken()) {
          std::string r = ((TString)st).Data();
          if(r.find("-")!=std::string::npos) {
            //is a run range, need to parse
            int minRun=0; int maxRun=999999;
            if(r.find("-")==0) {
              //open-ended lower range
              maxRun=std::atoi(r.substr(1).c_str());
            } else if(r.find("-")==r.length()-1) {
              //open-ended upper range
              minRun=std::atoi(r.substr(0,r.length()-1).c_str());
            } else {
              //regular range
              minRun=std::atoi(r.substr(0,r.find("-")).c_str());
              maxRun=std::atoi(r.substr(r.find("-")+1).c_str());
            }
            for(auto& rr : allRunsReader.GetMergedGoodRunsList().GetRunlist()) {
              if(rr > maxRun) continue;
              if(rr < minRun) continue;
              if(myGrl->HasRun(rr)) continue;
              myGrl->AddRunLumiBlock(rr,0); //add a dummy lb ... actually has special meaning: 0 will mean "accept all events in this run"
            }
            
          } else {
            if(myGrl->HasRun(std::atoi(r.c_str()))) continue;
            myGrl->AddRunLumiBlock(std::atoi(r.c_str()),0); //add a dummy lb ... actually has special meaning: 0 will mean "accept all events in this run"
          }
        }
      }
      
      
      //also check for metadata with name 'Trigger' or 'DataTrigger'
      if(myGrl->GetMetaData().find("Trigger")!=myGrl->GetMetaData().end()) {
        mcTrig = myGrl->GetMetaData().find("Trigger")->second;
      }
      if(myGrl->GetMetaData().find("DataTrigger")!=myGrl->GetMetaData().end()) {
        dataTrig = myGrl->GetMetaData().find("DataTrigger")->second;
      }
      
      if(myGrl->GetMetaData().find("TriggerIndex")!=myGrl->GetMetaData().end()) {
        counter = myGrl->GetMetaData().find("TriggerIndex")->second.Atoi();
      } else if(!m_triggerDir.empty()) {
        ATH_MSG_ERROR("When specifying a TriggerDir the GRL must have a TriggerIndex metadata in them");
        return StatusCode::FAILURE;
      }
      
      
    }
    m_triggerConf.push_back(Trigger(counter,mcTrig,dataTrig,myGrl));
    if(counter >= maxCounter) maxCounter=counter+1;
    counter = m_triggerConf.size(); //next counter will be end of triggerConf by default 
   }
   
   
   //reorganize m_triggers to match the index order
   //any missing indices will just be left with blanks!
   m_triggers.clear(); m_triggers.resize(maxCounter,"");
   for(auto& trig : m_triggerConf) {
     if(m_triggers[trig.idx]!="") {
        ATH_MSG_ERROR("CONFLICT with TriggerIndices ... " << m_triggers[trig.idx] << " and " << trig.trigger << " both have index " << trig.idx);
        return StatusCode::FAILURE;
     }
     m_triggers[trig.idx] = trig.trigger;
     if(!trig.dataTrigger.empty()) m_triggers[trig.idx] += "[][" + trig.dataTrigger + "]";
   }
   
   //just check for blanks ...
   for(unsigned int i=0;i<m_triggers.size();i++) {
      if(m_triggers[i].empty()) ATH_MSG_WARNING("Trigger index " << i << " is empty ... will never fire");
   }
   

   return StatusCode::SUCCESS;

}
StatusCode CamEventPreselectionTool::finalize() { 

   return StatusCode::SUCCESS;

}

//option can contain: c (cleaning), p (pileup reweighting), t (trigger), v (vertex), a (all trig decisions)
//if it contains none, then do all
StatusCode CamEventPreselectionTool::apply(CamEvent* evt, std::string& label, const std::string& option) {

   const xAOD::EventInfo* ei = evt->ae<xAOD::EventInfo>();

   bool passThrough = !(option.empty() || option.find("s")==std::string::npos); //flag if should let us go 
   
   bool isMC = evt->is("isSimulation");
   
   bool eventOK=true;bool doneAll=true;
   
   if(label.length()==0) label=m_shortName; //ensures label is valid
   
   evt->set(label) = false; //does not pass

   ///evtWeight property
   evt->set("evtWeight") = (isMC) ? double(ei->mcEventWeight()) : 1.;

   if(option.empty() || option.find("c")!=std::string::npos) {
      ///standard cleaning cuts on data
      if(!isMC) {
          if(!m_grlTool->passRunLB(*evt/*exploits auto casting to eventinfo*/)) return StatusCode::SUCCESS;
          if(m_printCutflow) chanInfo->Fill("passGRL",1);
          if(evt->ae<xAOD::EventInfo>()->eventFlags(xAOD::EventInfo::LAr)==xAOD::EventInfo::Error) return StatusCode::SUCCESS;
          if(m_printCutflow) chanInfo->Fill("passLArFlag",1);
          if(evt->ae<xAOD::EventInfo>()->eventFlags(xAOD::EventInfo::Tile)==xAOD::EventInfo::Error) return StatusCode::SUCCESS;
          if(m_printCutflow) chanInfo->Fill("passTileFlag",1);
          if( (evt->ae<xAOD::EventInfo>()->eventFlags(xAOD::EventInfo::Core)&0x40000) != 0) return StatusCode::SUCCESS; //rejects incomplete events
          if(m_printCutflow) chanInfo->Fill("passCoreFlag",1);
          if( (evt->ae<xAOD::EventInfo>()->eventFlags(xAOD::EventInfo::SCT) == xAOD::EventInfo::Error) ) return StatusCode::SUCCESS; //new for 2016
          if(m_printCutflow) chanInfo->Fill("passSCTFlag",1);
          evt->set("mcChannelNumber") = 0; //0 for data
          evt->set("RunNumber") = uint(ei->runNumber()); //store the run number
      } 
      evt->set("passCleaning") = true;
      if(m_printCutflow) chanInfo->Fill("passCleaning",evt->get("evtWeight"));
   } else {
      doneAll=false;
   }
   
   if(option.empty() || option.find("p")!=std::string::npos) {
      
      //copy RandomRunNumber into CamEvent if in MC
      if(isMC) {
        evt->set("corrected_averageInteractionsPerCrossing") = ei->averageInteractionsPerCrossing();
        evt->set("correctedAndScaled_averageInteractionsPerCrossing") = ei->averageInteractionsPerCrossing();
        
        uint rrn = m_prwTool->getRandomRunNumber(*ei);
        if(rrn==0) rrn = m_prwTool->getRandomRunNumber(*ei, false); //generate a mu-independent rrn 
        uint rln = m_prwTool->expert()->GetRandomLumiBlockNumber( rrn );
        ei->auxdecor<unsigned int>("RandomRunNumber") = rrn;
        ei->auxdecor<unsigned int>("RandomLumiBlockNumber") = rln;
        evt->set("RunNumber") = rrn;
        evt->set("runNumber") = rrn;
        evt->set("lumiBlock") = rln; 
        
        
        bool canPRW = (!m_allowSkipPRW) || (m_prwTool->expert()->GetPrimaryDistribution(ei->mcChannelNumber(),ei->runNumber()));
        
        if(canPRW) {
          ei->auxdecor<float>("PileupWeight") = m_prwTool->getCombinedWeight(*ei);
          ei->auxdecor<float>("PileupPrimaryWeight") = m_prwTool->expert()->GetPrimaryWeight( ei->runNumber(), ei->mcChannelNumber(), ei->averageInteractionsPerCrossing() );
          //get the PRW uncertainty weights
          if(m_hasPRWConfigFiles) {
            CP::SystematicSet tmpSet;tmpSet.insert(CP::SystematicVariation("PRW_DATASF",1));
            m_prwTool->applySystematicVariation( tmpSet ).ignore();
            evt->set("PileupWeight_UP") = m_prwTool->getCombinedWeight( *evt );
            tmpSet.clear();tmpSet.insert( CP::SystematicVariation("PRW_DATASF",-1) );
            m_prwTool->applySystematicVariation( tmpSet ).ignore();
            evt->set("PileupWeight_DOWN") = m_prwTool->getCombinedWeight( *evt );
            m_prwTool->applySystematicVariation( CP::SystematicSet() ).ignore();
          }
          
        } else {
          ei->auxdecor<float>("PileupWeight") = 1.;
          ei->auxdecor<float>("PileupPrimaryWeight") = 1.;
          //wont decorate pileup weight variations, to increase chance of spotting mistake! 
        }
        
      } else {
        //with data, just decorate correctedAverageInteractionsPerCrossing and correctedAndScaledAverageInteractionsPerCrossing 
        evt->set("corrected_averageInteractionsPerCrossing") = m_prwTool->getCorrectedAverageInteractionsPerCrossing(*ei,false);
        evt->set("correctedAndScaled_averageInteractionsPerCrossing") = m_prwTool->getCorrectedAverageInteractionsPerCrossing(*ei,true);
      }
   }


   if(option.empty() || option.find("t")!=std::string::npos) {
      //chronoStatService()->chronoStart( "ZdZdAnalysis::preExecute_trigDecision" );
      bool passTrigger = (m_triggerConf.size()==0); //if no triggers then we just pass!
      
      if(!passTrigger) {
        int trigDecisions(0);
        int allTrigDecisions(0); //filled if "a" option specified 
        int activityMask(0);
        bool ignoreRunDMC = (option.find("a")!=std::string::npos);
        
        unsigned int runNum = evt->get<unsigned int>("runNumber");
        unsigned int lbNum = evt->get<unsigned int>("lumiBlock");
        //loop over triggers, and test the active ones
        for(auto& trig : m_triggerConf) {
          if(!trig.isActive( runNum, lbNum )) {
            if(ignoreRunDMC) {
              bool res = m_tdtTool->isPassed(trig.getTrigger(!isMC));
              allTrigDecisions += (res << trig.idx);
            }
            continue;
          } else if(ignoreRunDMC) {
            activityMask += (1 << trig.idx);
          }
          bool res = m_tdtTool->isPassed(trig.getTrigger(!isMC));
          trigDecisions += (res << trig.idx);
          if(res) passTrigger=true;
        }
        
        
        evt->set("passTriggers") = trigDecisions;
        
        if(ignoreRunDMC) {
          evt->set("passTriggers_noMask") = trigDecisions+allTrigDecisions;
          evt->set("passTriggers_activityMask") = activityMask;
        }
      }
      
      if(!passTrigger) {
        eventOK=false; //do not record any more events in cutflow
        if(!passThrough) return StatusCode::SUCCESS;
      }
      if(m_printCutflow && eventOK && doneAll) chanInfo->Fill("passTrigger",evt->get("evtWeight"));
      //chronoStatService()->chronoStop( "ZdZdAnalysis::preExecute_trigDecision" );
   } else {
      doneAll=false;
   }

   if(option.empty() || option.find("v")!=std::string::npos) {
      //decorate primary vertex (used in selection methods)
      const xAOD::VertexContainer* vtxs = 0;
      CHECK( evtStore()->retrieve(vtxs,"PrimaryVertices") );
      evt->set("nPV") = int(vtxs->size());
      //require at least one to be 'THE' primary vertex 
      const xAOD::Vertex* priVtx = 0;
      for(auto v : *vtxs) if(v->vertexType()==xAOD::VxType::PriVtx) { priVtx=v; break; }
    
      if(!priVtx) {
          eventOK=false;
          evt->set("pvz") = 0.;
          if(!passThrough) return StatusCode::SUCCESS;
      } else {
          evt->set("npvTracks") = uint(priVtx->nTrackParticles());
          evt->set("passNPV") = true;
          evt->set("pvz") = priVtx->z(); //used in the object selection (z0 cut)
      }
    
      if(m_printCutflow && eventOK && doneAll) chanInfo->Fill("passNPV",evt->get("evtWeight"));
   } else {
      doneAll=false;
   }
   
   
   if(eventOK && doneAll) evt->set(label) = true; //passed all
   
   return StatusCode::SUCCESS;
    
}
