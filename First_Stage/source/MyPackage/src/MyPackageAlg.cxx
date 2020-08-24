// MyPackage includes
#include "MyPackageAlg.h"

#include "xAODCore/ShallowCopy.h"
#include "xAODBase/IParticleHelpers.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/MuonAuxContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/ElectronAuxContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODJet/JetAuxContainer.h"

#include "xAODEventInfo/EventInfo.h"

#include "xAODMissingET/MissingETAuxContainer.h"

#include "xAODCutFlow/CutBookkeeperContainer.h"
#include "xAODCutFlow/CutBookkeeper.h"

MyPackageAlg::MyPackageAlg( const std::string& name, ISvcLocator* pSvcLocator ) : AthAnalysisAlgorithm( name, pSvcLocator ){

}

MyPackageAlg::~MyPackageAlg() {}



StatusCode MyPackageAlg::initialize() {

  ATH_MSG_INFO ("Initializing " << name() << "...");

  // This is called once, before the start of the event loop
  // Retrieves of tools you have configured in the joboptions go here

   m_SUSYTools.setTypeAndName("ST::SUSYObjDef_xAOD/SUSYTools");
   CHECK( m_SUSYTools.retrieve() );

  return StatusCode::SUCCESS;

}



void MyPackageAlg::selectElectrons(xAOD::ElectronContainer* electrons, CamVector* camElectrons) {

   for(auto el : *electrons) {
     if (!el->auxdata<char>("baseline")) continue;

      CamObject tmp(*el);
      bool isSignal = el->auxdata<char>("signal");

      tmp.set("signal") = isSignal;
      tmp.set("passOR") = el->auxdata<char>("passOR");

      tmp.set("scalefact")  = el->auxdata<double>("effscalefact");
      tmp.set("trig_match") = el->auxdata<char>("trigmatched");

      tmp.set("charge")     = el->auxdata<float>("charge");

      camElectrons->push_back(tmp);

   }

}



void MyPackageAlg::selectMuons(xAOD::MuonContainer* muons, CamVector* camMuons) {

   for(auto mu : *muons) {
      if (!mu->auxdata<char>("baseline")) continue;

      CamObject tmp(*mu);
      bool isSignal = mu->auxdata<char>("signal");

      tmp.set("signal") = isSignal;
      tmp.set("passOR")     = mu->auxdata<char>("passOR");
      tmp.set("charge")     = mu->auxdata<float>("charge");

      tmp.set("scalefact")  = mu->auxdata<double>("effscalefact");
      tmp.set("trig_match") = mu->auxdata<char>("trigmatched");

      tmp.set("Theta")      = mu->p4().Theta();
      tmp.set("z0sinTheta") = mu->auxdata<float>("z0sinTheta");
      tmp.set("d0sig")      = mu->auxdata<float>("d0sig");

      if (mu->muonType() == xAOD::Muon::SiliconAssociatedForwardMuon){
        tmp.set("d0") = mu->trackParticle(xAOD::Muon::CombinedTrackParticle)->d0();
      } else {
        tmp.set("d0") = mu->primaryTrackParticle()->d0();
      }

      camMuons->push_back(tmp);

	}

}




void MyPackageAlg::selectJets(xAOD::JetContainer* jets, CamVector* camJets, int p) {
  // This precycle decorates the jets with the nec values it needs before saving
  // It is here since it must be applied last, after all other selections.
  // for (auto jet : *jets) m_SUSYTools->IsSignalJet( *jet, 20000., 2.8 );


  for(auto jet : *jets) {
    if ( jet->pt() < 20000.0 ) continue;


    CamObject tmp(*jet);
    tmp.set("baseline")  = jet->auxdata<char>("baseline");
    tmp.set("signal")    = jet->auxdata<char>("signal");
    tmp.set("bad")       = jet->auxdata<char>("bad");

    tmp.set("passOR")    = jet->auxdata<char>("passOR");
    tmp.set("passJvt")   = jet->auxdata<char>("passJvt");
    tmp.set("passFJvt")  = jet->auxdata<char>("passFJvt");

    tmp.set("bjet") = jet->auxdata<char>("bjet");

    tmp.set("Jvt") = jet->auxdata<float>("Jvt");
    tmp.set("FracSamplingMax") = jet->auxdata<float>("FracSamplingMax");

    std::vector<float> JVFvec;
    float JVF = -1;
    JVFvec = jet->auxdata<std::vector<float>>("JVF");
    if ( JVFvec.size() > 0 ) JVF = JVFvec.at(p);
    tmp.set("JVF") = JVF;
    tmp.set("JvtJvfcorr") = jet->auxdata<float>("JVFCorr");

    std::vector<float> SumPtvec;
    SumPtvec = jet->auxdata<std::vector<float>>("SumPtTrkPt500");
    float Rpt = -1;
    if ( SumPtvec.size() > 0 ) Rpt = SumPtvec.at(p) / jet->pt();
    tmp.set("JvtRpt") = Rpt;

    std::vector<int> Numvec;
    Numvec = jet->auxdata<std::vector<int>>("NumTrkPt500");
    int NumTrkPt500 = -1;
    if ( Numvec.size() > 0 ) NumTrkPt500 = Numvec.at(p);
    tmp.set("NumTrkPt500") = NumTrkPt500;
    camJets->push_back(tmp);

  }

}

StatusCode MyPackageAlg::finalize() {
  ATH_MSG_INFO ("Finalizing " << name() << "...");

  // Things that happen once at the end of the event loop go here

  return StatusCode::SUCCESS;
}

StatusCode MyPackageAlg::execute() {
  ATH_MSG_DEBUG ("Executing " << name() << "...");
  setFilterPassed(false); //optional: start with algorithm not passed

   m_SUSYTools->resetSystematics();
   CHECK( m_SUSYTools->ApplyPRWTool() );

   // Collecting the primary vertex and the total number of vertices
   const xAOD::VertexContainer* pvtxs = 0;
   int pvIndex = 0;
   int PV2trk  = 0;
   int PV4trk  = 0;
   int hasPV   = 0;

   ATH_CHECK( evtStore()->retrieve( pvtxs, "PrimaryVertices" ) );

   if ( pvtxs == 0 || pvtxs->size() == 0 ) {

     ATH_MSG_WARNING(" This event has no primary vertices " );
     return StatusCode::SUCCESS;

   } else {

     int p = 0;
     for( const auto& vx : *pvtxs ) {
       if( vx->vertexType() == xAOD::VxType::PriVtx ) pvIndex = p;
       if( vx->vertexType() == xAOD::VxType::PriVtx ) hasPV = 1;
       if ( vx->nTrackParticles()>=2 ) PV2trk++;
       if ( vx->nTrackParticles()>=4 ) PV4trk++;

       p++;
     }
   }

   const xAOD::Vertex& vtx = *pvtxs->at( pvIndex );


  if ( hasPV == 1 ){

    // The objects are imported and their containers are filled
    xAOD::ElectronContainer* electrons(0);
    xAOD::ShallowAuxContainer* electrons_aux(0);
    CHECK( m_SUSYTools->GetElectrons(electrons,electrons_aux,true) );

    xAOD::MuonContainer* muons(0);
    xAOD::ShallowAuxContainer* muons_aux(0);
    CHECK( m_SUSYTools->GetMuons(muons,muons_aux,true) );

    xAOD::PhotonContainer* photons(0);
    xAOD::ShallowAuxContainer* photons_aux(0);
    CHECK( m_SUSYTools->GetPhotons(photons,photons_aux,true) );

    xAOD::JetContainer* jets(0);
    xAOD::ShallowAuxContainer* jets_aux(0);
    CHECK( m_SUSYTools->GetJets(jets,jets_aux,true) );


    // Decorate objects if they pass overlap removal and decorate jets if they are good or bad
    CHECK( m_SUSYTools->OverlapRemoval( electrons, muons, jets ) );

    // Event information is imported
    const xAOD::EventInfo* ei = 0;
    if( evtStore()->retrieve( ei, "EventInfo" ).isFailure() ) {
      ATH_MSG_WARNING( "No EventInfo object could be retrieved" );
    }
    if (!ei) {
      ATH_MSG_ERROR( "Cannot retrieve the EventInfo" );
    }

    // The object we use to write all the information to the tree
    CamEvent evt;



    // A quick event selection for data so that the file sizes arent so large.
    // This is beacuse down the line for data we only investigate in 2l regions of the same flavour
    // But for MC, which is used for training, we cant afford to be so strict
    int n_sig_elecs = 0;
    int n_sig_muons = 0;
    int n_sig_lepts = 0;

    if ( m_isData ) {

      for ( const auto& mu : *muons ) {
        if ( mu->auxdata<char>("signal") && mu->auxdata<char>("passOR") ) {
          n_sig_muons++;
        }
      }

      for ( const auto& el : *electrons ) {
        if ( el->auxdata<char>("signal") && el->auxdata<char>("passOR") ) {
          n_sig_elecs++;
        }
      }

      n_sig_lepts = n_sig_elecs + n_sig_muons;

      // We throw the event away if the total number of leptons is not 2
      // OR neither of the flavours has eaxctly two
      if ( n_sig_lepts != 2 || (n_sig_elecs!=2 && n_sig_muons!=2) ) {
        return StatusCode::SUCCESS;
      }

    }
    /////////////////////////////////////////////////////////////////////////////////



    // We store the number of events contained in the file only once, so it can be summed up later
    if ( m_totalEvents != 0 ){
      evt.set("totalEvents") = m_totalEvents;
      m_totalEvents = 0.0;
    } else {
      evt.set("totalEvents") = 0.0;
    }
    if ( m_totalWeightedEvents != 0 ){
      evt.set("totalWeightedEvents") = m_totalWeightedEvents;
      m_totalWeightedEvents = 0.0;
    } else {
      evt.set("totalWeightedEvents") = 0.0;
    }

    // We store information about the run number of all the events
    evt.set("EventNumber") = ei->eventNumber();
    evt.set("RunNumber")   = ei->runNumber();
    evt.set("lbn")         = ei->lumiBlock();
    evt.set("bcid")        = ei->bcid();
    evt.set("coreFlags")   = ei->auxdata<unsigned int>("coreFlags");

    // Storing information about weights which is dependant on of we are looking at data or MC
    if (!m_isData) {
      evt.set("EventWeight")   = ei->mcEventWeights().at(0);
      evt.set("PileUpWeight")  = m_SUSYTools->GetPileupWeight();
      evt.set("ChannelNumber") = ei->mcChannelNumber();
    } else {
      evt.set("EventWeight")   = 1.0;
      evt.set("PileUpWeight")  = 1.0;
      evt.set("ChannelNumber") = ei->runNumber();
    }


    // Quality Checking!!
    int passBadMuon = 1;
    int passBadJet  = 1;
    int passCosmic  = 1;
    int larError    = 0;
    int tileError   = 0;
    int sctError    = 0;

    for ( const auto& mu : *muons ) {
      if( mu->auxdata<char>("baseline") && mu->auxdata<char>("bad") ) {
        passBadMuon = 0;
      }
    }

    for ( auto jet : *jets ) {
      if ( jet->auxdata<char>("passOR") && jet->auxdata<char>("bad") ) {
        passBadJet = 0;
      }
    }

    for ( const auto& mu : *muons ) {
      if ( mu->auxdata<char>("baseline") && mu->auxdata<char>("passOR") && mu->auxdata<char>("cosmic") ) {
        passCosmic = 0;
      }
    }

    if ( ei->errorState(xAOD::EventInfo::LAr)  == xAOD::EventInfo::Error ) larError  = 2;
    if ( ei->errorState(xAOD::EventInfo::Tile) == xAOD::EventInfo::Error ) tileError = 2;
    if ( ei->errorState(xAOD::EventInfo::SCT)  == xAOD::EventInfo::Error ) sctError  = 2;

    evt.set("passCosmic")  = passCosmic;
    evt.set("passBadMuon") = passBadMuon;
    evt.set("passBadJet")  = passBadJet;
    evt.set("larError")    = larError;
    evt.set("tileError")   = tileError;
    evt.set("sctError")    = sctError;



    // Saving extra information about the vertices
    evt.set("pvIndex") = pvIndex;
    evt.set("PV2trk") = PV2trk;
    evt.set("PV4trk") = PV4trk;

    evt.set("vtx_z") = vtx.z();
    evt.set("vtx_nTracks") = vtx.nTrackParticles();

    evt.set("averageInteractionsPerCrossing") = ei->averageInteractionsPerCrossing();
    evt.set("actualInteractionsPerCrossing")  = ei->actualInteractionsPerCrossing();
    evt.set("CorrectedMuTrue")  = m_SUSYTools->GetCorrectedAverageInteractionsPerCrossing(true);
    evt.set("CorrectedMuFalse") = m_SUSYTools->GetCorrectedAverageInteractionsPerCrossing(false);



    // Check if event passed trigger
    evt.set("passed_elec_trigger") = m_SUSYTools->IsTrigPassed("HLT_e26_lhtight_nod0_ivarloose || HLT_e60_lhmedium_nod0 || HLT_e140_lhloose_nod0");
    evt.set("passed_muon_trigger") = m_SUSYTools->IsTrigPassed("HLT_mu26_ivarmedium || HLT_mu50");

    // Trigger matching
    for ( const auto& mu : *muons ) {
      m_SUSYTools->TrigMatch( {mu}, "HLT_mu26_ivarmedium || HLT_mu50");
    }

    for ( const auto& el : *electrons ) {
      m_SUSYTools->TrigMatch( {el}, "HLT_e26_lhtight_nod0_ivarloose || HLT_e60_lhmedium_nod0 || HLT_e140_lhloose_nod0");
    }

    // Scale Factors
    float total_elec_sf = 1.0;
    float total_muon_sf = 1.0;
    float trig_elec_sf  = 1.0;
    float trig_muon_sf  = 1.0;

    if ( !m_isData ) {
      // Calculate the global trigger scale factor of the event
      trig_muon_sf = m_SUSYTools->GetTotalMuonSF( *muons, false, false, "HLT_mu26_ivarmedium_OR_HLT_mu50", false );
      trig_elec_sf = m_SUSYTools->GetTotalEleTriggerSF( *electrons, "singleLepton" );

      // Calculating total scale factors (not including triggers), will decorate all leptons with these values as well!
      total_elec_sf = m_SUSYTools->GetTotalElectronSF( *electrons, true, true, false, true, "", false );
      total_muon_sf = m_SUSYTools->GetTotalMuonSF( *muons, true, true, "", false );
    }

    evt.set("total_elec_sf") = total_elec_sf;
    evt.set("total_muon_sf") = total_muon_sf;
    evt.set("trig_elec_sf")  = trig_elec_sf;
    evt.set("trig_muon_sf")  = trig_muon_sf;


    // SUSYTools final state determination




    // Collecting and saving the Tight TST MET and associated components
    xAOD::MissingETContainer* met = new xAOD::MissingETContainer;
    xAOD::MissingETAuxContainer* met_aux = new xAOD::MissingETAuxContainer;
    met->setStore(met_aux);
    CHECK( m_SUSYTools->GetMET(*met,jets,electrons,muons,photons,0,true,true) );

    evt.set("MET_RefFinal_TST_et")    = (*met)["Final"]->met();
    evt.set("MET_RefFinal_TST_etx")   = (*met)["Final"]->mpx();
    evt.set("MET_RefFinal_TST_ety")   = (*met)["Final"]->mpy();
    evt.set("MET_RefFinal_TST_SumET") = (*met)["Final"]->sumet();

    evt.set("MET_RefJet_TST_et")    = (*met)["RefJet"]->met();
    evt.set("MET_RefJet_TST_etx")   = (*met)["RefJet"]->mpx();
    evt.set("MET_RefJet_TST_ety")   = (*met)["RefJet"]->mpy();
    evt.set("MET_RefJet_TST_SumET") = (*met)["RefJet"]->sumet();

    evt.set("MET_RefEle_TST_et")    = (*met)["RefEle"]->met();
    evt.set("MET_RefEle_TST_etx")   = (*met)["RefEle"]->mpx();
    evt.set("MET_RefEle_TST_ety")   = (*met)["RefEle"]->mpy();
    evt.set("MET_RefEle_TST_SumET") = (*met)["RefEle"]->sumet();

    evt.set("MET_RefGamma_TST_et")    = (*met)["RefGamma"]->met();
    evt.set("MET_RefGamma_TST_etx")   = (*met)["RefGamma"]->mpx();
    evt.set("MET_RefGamma_TST_ety")   = (*met)["RefGamma"]->mpy();
    evt.set("MET_RefGamma_TST_SumET") = (*met)["RefGamma"]->sumet();

    evt.set("MET_Muons_TST_et")    = (*met)["Muons"]->met();
    evt.set("MET_Muons_TST_etx")   = (*met)["Muons"]->mpx();
    evt.set("MET_Muons_TST_ety")   = (*met)["Muons"]->mpy();
    evt.set("MET_Muons_TST_SumET") = (*met)["Muons"]->sumet();

    evt.set("MET_PVSoftTrk_TST_et")    = (*met)["PVSoftTrk"]->met();
    evt.set("MET_PVSoftTrk_TST_etx")   = (*met)["PVSoftTrk"]->mpx();
    evt.set("MET_PVSoftTrk_TST_ety")   = (*met)["PVSoftTrk"]->mpy();
    evt.set("MET_PVSoftTrk_TST_SumET") = (*met)["PVSoftTrk"]->sumet();


    // Collecting and saving the Loose TST MET and jet component
    xAOD::MissingETContainer* loose_tst_met = new xAOD::MissingETContainer;
    xAOD::MissingETAuxContainer* loose_tst_met_aux = new xAOD::MissingETAuxContainer;
    loose_tst_met->setStore(loose_tst_met_aux);
    CHECK( m_SUSYTools->GetMET_Loose(*loose_tst_met,jets,electrons,muons,photons,0,true,true) );

    evt.set("MET_RefFinal_TST_LOOSE_et")    = (*loose_tst_met)["Final"]->met();
    evt.set("MET_RefFinal_TST_LOOSE_etx")   = (*loose_tst_met)["Final"]->mpx();
    evt.set("MET_RefFinal_TST_LOOSE_ety")   = (*loose_tst_met)["Final"]->mpy();
    evt.set("MET_RefFinal_TST_LOOSE_SumET") = (*loose_tst_met)["Final"]->sumet();

    evt.set("MET_RefJet_TST_LOOSE_et")    = (*loose_tst_met)["RefJet"]->met();
    evt.set("MET_RefJet_TST_LOOSE_etx")   = (*loose_tst_met)["RefJet"]->mpx();
    evt.set("MET_RefJet_TST_LOOSE_ety")   = (*loose_tst_met)["RefJet"]->mpy();
    evt.set("MET_RefJet_TST_LOOSE_SumET") = (*loose_tst_met)["RefJet"]->sumet();


    // Collecting and saving the FJVT TST MET and jet component
    xAOD::MissingETContainer* fjvt_tst_met = new xAOD::MissingETContainer;
    xAOD::MissingETAuxContainer* fjvt_tst_met_aux = new xAOD::MissingETAuxContainer;
    fjvt_tst_met->setStore(fjvt_tst_met_aux);
    CHECK( m_SUSYTools->GetMET_fjvt(*fjvt_tst_met,jets,electrons,muons,photons,0,true,true) );

    evt.set("MET_RefFinal_TST_FJVT_et")    = (*fjvt_tst_met)["Final"]->met();
    evt.set("MET_RefFinal_TST_FJVT_etx")   = (*fjvt_tst_met)["Final"]->mpx();
    evt.set("MET_RefFinal_TST_FJVT_ety")   = (*fjvt_tst_met)["Final"]->mpy();
    evt.set("MET_RefFinal_TST_FJVT_SumET") = (*fjvt_tst_met)["Final"]->sumet();

    evt.set("MET_RefJet_TST_FJVT_et")    = (*fjvt_tst_met)["RefJet"]->met();
    evt.set("MET_RefJet_TST_FJVT_etx")   = (*fjvt_tst_met)["RefJet"]->mpx();
    evt.set("MET_RefJet_TST_FJVT_ety")   = (*fjvt_tst_met)["RefJet"]->mpy();
    evt.set("MET_RefJet_TST_FJVT_SumET") = (*fjvt_tst_met)["RefJet"]->sumet();


    // Collecting and saving the Track MET
    xAOD::MissingETContainer* trk_met = new xAOD::MissingETContainer;
    xAOD::MissingETAuxContainer* trk_met_aux = new xAOD::MissingETAuxContainer;
    trk_met->setStore(trk_met_aux);
    CHECK( m_SUSYTools->GetTrackMET(*trk_met,jets,electrons,muons) );

    evt.set("MET_Ref_Track_et")    = (*trk_met)["Track"]->met();
    evt.set("MET_Ref_Track_etx")   = (*trk_met)["Track"]->mpx();
    evt.set("MET_Ref_Track_ety")   = (*trk_met)["Track"]->mpy();
    evt.set("MET_Ref_Track_SumET") = (*trk_met)["Track"]->sumet();


    // Collecting and saving CST MET and the CST component
    xAOD::MissingETContainer* cst_met = new xAOD::MissingETContainer;
    xAOD::MissingETAuxContainer* cst_met_aux = new xAOD::MissingETAuxContainer;
    cst_met->setStore(cst_met_aux);
    CHECK( m_SUSYTools->GetMET(*cst_met,jets,electrons,muons,photons,0,false,false) );

    evt.set("MET_RefFinal_CST_et")    = (*cst_met)["Final"]->met();
    evt.set("MET_RefFinal_CST_etx")   = (*cst_met)["Final"]->mpx();
    evt.set("MET_RefFinal_CST_ety")   = (*cst_met)["Final"]->mpy();
    evt.set("MET_RefFinal_CST_SumET") = (*cst_met)["Final"]->sumet();

    evt.set("MET_SoftClus_CST_et")    = (*cst_met)["SoftClus"]->met();
    evt.set("MET_SoftClus_CST_etx")   = (*cst_met)["SoftClus"]->mpx();
    evt.set("MET_SoftClus_CST_ety")   = (*cst_met)["SoftClus"]->mpy();
    evt.set("MET_SoftClus_CST_SumET") = (*cst_met)["SoftClus"]->sumet();

    // Collecting and saving the MET Significance
    double met_significance;
    xAOD::MissingETContainer* dummy_met = new xAOD::MissingETContainer;
    xAOD::MissingETAuxContainer* dummy_met_aux = new xAOD::MissingETAuxContainer;
    dummy_met->setStore(dummy_met_aux);
    CHECK( m_SUSYTools->GetMETSig(*dummy_met,met_significance,jets,electrons,muons,photons,0,true,true));

    evt.set("MET_Significance") = met_significance;


    // If we are running on MC then we can add truth variables to the output
    if (!m_isData){

      // Truth JETS
      const xAOD::JetContainer * tr_jets = 0;
      if ( !evtStore()->retrieve(tr_jets,"AntiKt4TruthJets").isSuccess() ) {
        Error("execute()", "Failed to retrieve jet collection. Exiting." );
        return StatusCode::SUCCESS;
      }

      CamVector* trjets = evt.createVector("trjets");
      for ( auto ijet : *tr_jets ) {
        CamObject tmp(*ijet);
        if (ijet->pt()>4000.) trjets->push_back(tmp);
      }

      trjets->applyService("CamWriter/myWriter","trjet_");


      // Collecting and saving the Truth MET and associated components
      const xAOD::MissingETContainer* TruthMet =0;
      if( evtStore()->retrieve( TruthMet, "MET_Truth" ).isFailure() ) {
        ATH_MSG_WARNING( "No Truth MET object could be retrieved" );
      }

      evt.set("MET_Truth_NonInt_et")    = (*TruthMet)["NonInt"]->met();
      evt.set("MET_Truth_NonInt_etx")   = (*TruthMet)["NonInt"]->mpx();
      evt.set("MET_Truth_NonInt_ety")   = (*TruthMet)["NonInt"]->mpy();
      evt.set("MET_Truth_NonInt_SumET") = (*TruthMet)["NonInt"]->sumet();

      evt.set("MET_Truth_Int_et")    = (*TruthMet)["Int"]->met();
      evt.set("MET_Truth_Int_etx")   = (*TruthMet)["Int"]->mpx();
      evt.set("MET_Truth_Int_ety")   = (*TruthMet)["Int"]->mpy();
      evt.set("MET_Truth_Int_SumET") = (*TruthMet)["Int"]->sumet();

      evt.set("MET_Truth_IntMuons_et")    = (*TruthMet)["IntMuons"]->met();
      evt.set("MET_Truth_IntMuons_etx")   = (*TruthMet)["IntMuons"]->mpx();
      evt.set("MET_Truth_IntMuons_ety")   = (*TruthMet)["IntMuons"]->mpy();
      evt.set("MET_Truth_IntMuons_SumET") = (*TruthMet)["IntMuons"]->sumet();
    }


    // Storing information about the hard objects in the collision
    CamVector* el  = evt.createVector("el");
    CamVector* mu  = evt.createVector("mu");
    CamVector* jet = evt.createVector("jet");

    selectElectrons(electrons,el);
    selectMuons(muons,mu);
    selectJets(jets,jet,pvIndex);

    el->applyService("CamWriter/myWriter","el_");
    mu->applyService("CamWriter/myWriter","mu_");
    jet->applyService("CamWriter/myWriter","SUSYjet_");

    evt.applyService("CamWriter/myWriter");

    // Clean up things so there aren't memory leaks!
    delete met;
    delete met_aux;
    delete trk_met;
    delete trk_met_aux;
    delete cst_met;
    delete cst_met_aux;
    delete loose_tst_met;
    delete loose_tst_met_aux;
    delete fjvt_tst_met;
    delete fjvt_tst_met_aux;
    delete dummy_met;
    delete dummy_met_aux;

  } else {
    ATH_MSG_WARNING(" This event has no primary vertices " );
  } //PV cut

  return StatusCode::SUCCESS;
}

StatusCode MyPackageAlg::beginInputFile() {
  //
  //This method is called at the start of each input file, even if
  //the input file contains no events. Accumulate metadata information here
  //

  std::cout<< "m_totalEvents section " <<std::endl;

  if( inputMetaStore()->contains<xAOD::CutBookkeeperContainer>("CutBookkeepers") ) { //only found in MC15
    const xAOD::CutBookkeeperContainer* bks = 0;
    CHECK( inputMetaStore()->retrieve(bks, "CutBookkeepers") );
      const xAOD::CutBookkeeper* all = 0; int maxCycle=-1;
      for(auto cbk : *bks) { if(cbk->inputStream()=="StreamAOD" && cbk->name()=="AllExecutedEvents" && cbk->cycle()>maxCycle) { maxCycle=cbk->cycle(); all = cbk; } }
    std::cout<< "m_totalEvents: "<< all->sumOfEventWeights() <<std::endl;
    m_totalWeightedEvents = all->sumOfEventWeights();
    m_totalEvents = all->nAcceptedEvents();
  }

  std::cout<<"Autoconfigure isData and isAtlfast"<<std::endl;
  std::string projectname = "";
  ATH_CHECK( retrieveMetadata("/TagInfo", "project_name", projectname) );
  if ( projectname == "IS_SIMULATION" ) m_isData = false;
  else if (projectname.compare(0, 4, "data") == 0 ) m_isData = true;
  else {
    std::cout<<"Failed to autoconfigure -- project_name matches neither IS_SIMULATION nor data!"<<std::endl;
    return StatusCode::FAILURE;
  }

  return StatusCode::SUCCESS;
}
