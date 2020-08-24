#define MyEvent_cxx
// The class definition in MyEvent.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("MyEvent.C")
// Root > T->Process("MyEvent.C","some options")
// Root > T->Process("MyEvent.C+")
//

#include "MyEvent.h"
#include <TSelector.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TStyle.h>
#include <TRandom2.h>
#include <TMatrixD.h>
#include <TLegend.h>
#include <TVectorD.h>
#include <fstream>
#include <iostream>
#include <string>
#include <valarray>
#include <math.h>
#include <TApplication.h>
#include <iomanip>

extern string cutGLOBAL;
extern string selectionGLOBAL;

extern TString outputGLOBAL;
extern TString labelGLOBAL;

// Reading a list of dataset stats from a specific datafile
vector<vector<double>> Read_Dataset_Stats_From_File( bool print_matrix = false ){
  vector<vector<double>> dataset_stats;

  ifstream file( "/home/mleigh/workdir/Second_Stage/Data_Regions.csv" );
  string line;
  string title;

  vector<double> linedata;

  if ( !file.is_open() ) {
    cout << "\n\n\nCant open the Dataset-Stats file!!! \nCheck name and if it exists!!!\n\n\n";
  }

  getline( file, title, '\n');
  // printf( "%s \n", title.c_str() );

  while ( getline( file, line, '\n') ) {
    // printf( "%s \n", line.c_str() );

    if ( (line[0]==' ') || (line[0]=='|') || line.size()==0 ) continue;

    stringstream sep(line);
    string cell;

    unsigned e = 0; // Skipping the name line
    while ( getline( sep, cell, ',' ) ) {
      if ( e != 1 ) linedata.push_back( atof( cell.c_str() ) );
      e++;
    }

    dataset_stats.push_back( linedata );
    linedata.clear();

  }

  file.close();

  return dataset_stats;

}



int Get_DSID_index( int dsid, vector<vector<double>> dataset_stats ) {
  int this_id = 0;

  for (unsigned i = 0; i < dataset_stats.size(); i++) {
    this_id = int( dataset_stats[i][0] );

    if ( this_id == dsid ) {
      return i;
    }


  }

  return -1;

}



// Readning in the list of requirements for the GRL
vector<vector<unsigned>> GRL_Parser(){

  vector<vector<unsigned>> grl;

  ifstream file( "/home/mleigh/workdir/Second_Stage/GRL.csv" );
  string line;
  string title;

  vector<unsigned> linedata;

  if ( !file.is_open() ) {
    cout << "\n\n\nCant open the GRL file!!! \nCheck name and if it exists!!!\n\n\n";
  }

  while ( getline( file, line, '\n') ) {
    if ( line.size() == 0 ) continue;

    stringstream sep(line);
    string cell;

    while ( getline( sep, cell, ',' ) ) {
      linedata.push_back( atoi( cell.c_str() ) );
    }

    grl.push_back( linedata );
    linedata.clear();

  }

  file.close();

  return grl;

}



// Checking that the real data passed the GRL requirements
bool Is_GRL( vector<vector<unsigned>> matrix, unsigned run_number, unsigned lb_number ) {
  bool pass = false;

  for ( auto& tuple: matrix ) {

    if ( run_number == tuple[0] ) {
      if ( tuple[1] <= lb_number && lb_number <= tuple[2] ) {
        pass = true;
        break;
      }
    }

  }

  return pass;

}


class Analysis_Particle {
  public:

    TLorentzVector kins;
    float  charge;
    char   passedOR;
    bool   isSignal;
    double sf;
    char   tmatched;
    int    pdgID;

    Analysis_Particle() {
      kins.SetPxPyPzE( 0, 0, 0, 0 );
      charge   = 0.0;
      passedOR = 0;
      isSignal = 0;
      sf       = 1.0;
      tmatched = 0;
      pdgID    = 0;
    }

    void set_base_attributes( double new_pt, double new_eta, double new_phi, double new_mass, float new_charge,
                              char new_passedOR, bool new_isSignal, double new_sf, char new_tmatched,
                              int new_pdgID ) {

      kins.SetPtEtaPhiM( new_pt, new_eta, new_phi, new_mass );
      charge   = new_charge;
      passedOR = new_passedOR;
      isSignal = new_isSignal;
      sf       = new_sf;
      tmatched = new_tmatched;
      pdgID    = new_pdgID;

    }

    void copy_base_attributes( Analysis_Particle other_partcle ) {
      kins     = other_partcle.kins;
      charge   = other_partcle.charge;
      passedOR = other_partcle.passedOR;
      isSignal = other_partcle.isSignal;
      sf       = other_partcle.sf;
      tmatched = other_partcle.tmatched;
      pdgID    = other_partcle.pdgID;
    }

    void print_info() {
      cout << " - PT:           " << kins.Pt() << '\n';
      cout << " - mass:         " << kins.M()  << '\n';
      cout << " - charge:       " << charge    << '\n';
      cout << " - passedOR:     " << passedOR  << '\n';
      cout << " - isSignal:     " << isSignal  << '\n';
      cout << " - pdgID:        " << pdgID     << '\n';
      cout << " - sf:           " << sf        << '\n';
      cout << " - tmatched:     " << tmatched  << '\n';
      cout << '\n';
    }


};



class Reconstructed_Z: public Analysis_Particle {
  public:

    bool good_reconstruction;
    float this_mass;
    float z_mass;

    Reconstructed_Z() {
      good_reconstruction = false;
      this_mass = 0.0;
      z_mass    = 91.18;
    }

    void create_from_parents( Analysis_Particle lepton1, Analysis_Particle lepton2, float tolerance ) {

      if ( !lepton1.passedOR || !lepton2.passedOR ) return;
      if ( !lepton1.isSignal || !lepton2.isSignal ) return;
      if ( lepton1.charge * lepton2.charge >= 0 )   return;

      // Now we check the kinematics of the z_boson
      kins = ( lepton1.kins + lepton2.kins );

      // Now we calculate its mass
      this_mass = kins.M();

      // And we check if the mass is acceptable
      if ( fabs( this_mass - z_mass*1000 ) < tolerance*1000.0 ) {
        good_reconstruction = true;
      }

    }

};



void find_leading_leptons_in_event( vector<double> *lept_pt, vector<double> *lept_eta, vector<double> *lept_phi, vector<double> *lept_m, vector<float> *lept_charge,
                                    vector<char> *lept_passOR, vector<bool> *lept_signal, vector<double> *lept_scalefact, vector<char> *lept_trig_match,
                                    int lept_id,
                                    Analysis_Particle &leading_lepton, Analysis_Particle &subleading_lepton ) {

  // We cycle through the leptons
  for( unsigned i = 0; i < lept_pt->size(); i++ ) {

    if ( !lept_passOR->at(i) || !lept_signal->at(i) ) continue;

    // We check if it is greater than the leading lepton
    if ( lept_pt->at(i) > leading_lepton.kins.Pt() ) {

      subleading_lepton.copy_base_attributes( leading_lepton );
      leading_lepton.set_base_attributes( lept_pt->at(i), lept_eta->at(i), lept_phi->at(i), lept_m->at(i), lept_charge->at(i),
                                          lept_passOR->at(i), lept_signal->at(i), lept_scalefact->at(i), lept_trig_match->at(i),
                                          lept_id );

    // If it at least bigger than the subleading
    } else if ( lept_pt->at(i) > subleading_lepton.kins.Pt() ) {

      subleading_lepton.set_base_attributes( lept_pt->at(i), lept_eta->at(i), lept_phi->at(i), lept_m->at(i), lept_charge->at(i),
                                             lept_passOR->at(i), lept_signal->at(i), lept_scalefact->at(i), lept_trig_match->at(i),
                                             lept_id );

    }

  }

}



int total_baseline_over( vector<double> *lepton_pt, float pt_cut ){
  int total_over_cut = 0;

  for ( unsigned i = 0; i < lepton_pt->size(); i++ ){
    if ( lepton_pt->at(i) > pt_cut*1000.0 ) total_over_cut++;
  }

  return total_over_cut;

}



int total_signal_over( vector<double> *lepton_pt, vector<bool> *lepton_sig, vector<char> *lepton_passOR, float pt_cut ){
  int total_over_cut = 0;

  for ( unsigned i = 0; i < lepton_pt->size(); i++ ){
    if ( lepton_passOR->at(i) && ( lepton_pt->at(i) > pt_cut*1000.0 ) && lepton_sig->at(i) ) total_over_cut++;
  }

  return total_over_cut;

}



int clockwise_rotate( Double_t &x, Double_t &y, double angle){
  float new_x =   x*cos(angle) + y*sin(angle);
  float new_y = - x*sin(angle) + y*cos(angle);

  x = new_x;
  y = new_y;

  return 0;
}



float jet_prob_pileup( double pt, float jvt, double eta ){
  if (pt<20) {
    return 0.00;
  }

  if (fabs(eta)>2.7) {
    if (pt>40)           return 0.00;
    if (pt<40 && pt>30)  return 0.07;
    if (pt<30)           return 0.20;
  }

  if (jvt<0.05) {
    if (pt>150)          return 0.00;
    if (pt<150)          return 0.95;
  }

  if (jvt<0.55) {
    if (pt>100)          return 0.00;
    if (pt<100)          return 0.40;
  }

  return 0.00;

}



void MyEvent::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   evtcount = 0;
   passed   = 0;
   fraclast = 0;
   fracnew  = 0;

   // One of the first things we do is read the dataset stats
   dataset_stats = Read_Dataset_Stats_From_File();
   grl_matrix = GRL_Parser();

   rnd = new TRandom2(0);
   rnd->SetSeed(0);

   //Initialize Output File
   ofile = new TFile(outputGLOBAL,"RECREATE");

   // Initialize the new TTree which will hold all variables to go into the ANN
   var_tree = new TTree("var_tree", "ANN Variable Tree");

   var_tree->Branch("MET_Truth_ET", &MET_Truth_ET);
   var_tree->Branch("MET_Truth_X", &MET_Truth_X);
   var_tree->Branch("MET_Truth_Y", &MET_Truth_Y);
   var_tree->Branch("MET_Truth_SumET", &MET_Truth_SumET);

   var_tree->Branch("MET_TST_Tight_ET", &MET_TST_Tight_ET);
   var_tree->Branch("MET_TST_Tight_SumET", &MET_TST_Tight_SumET);

   var_tree->Branch("MET_TST_Loose_ET", &MET_TST_Loose_ET);
   var_tree->Branch("MET_TST_Loose_X", &MET_TST_Loose_X);
   var_tree->Branch("MET_TST_Loose_Y", &MET_TST_Loose_Y);
   var_tree->Branch("MET_TST_Loose_SumET", &MET_TST_Loose_SumET);

   var_tree->Branch("MET_TST_FJVT_ET", &MET_TST_FJVT_ET);
   var_tree->Branch("MET_TST_FJVT_X", &MET_TST_FJVT_X);
   var_tree->Branch("MET_TST_FJVT_Y", &MET_TST_FJVT_Y);
   var_tree->Branch("MET_TST_FJVT_SumET", &MET_TST_FJVT_SumET);

   var_tree->Branch("MET_CST_Tight_ET", &MET_CST_Tight_ET);
   var_tree->Branch("MET_CST_Tight_X", &MET_CST_Tight_X);
   var_tree->Branch("MET_CST_Tight_Y", &MET_CST_Tight_Y);
   var_tree->Branch("MET_CST_Tight_SumET", &MET_CST_Tight_SumET);

   var_tree->Branch("MET_Track_ET", &MET_Track_ET);
   var_tree->Branch("MET_Track_X", &MET_Track_X);
   var_tree->Branch("MET_Track_Y", &MET_Track_Y);
   var_tree->Branch("MET_Track_SumET", &MET_Track_SumET);

   var_tree->Branch("MET_over_root_SumET", &MET_over_root_SumET);
   var_tree->Branch("MET_SIG", &MET_SIG);

   var_tree->Branch("Muons_ET", &Muons_ET);
   var_tree->Branch("Muons_X", &Muons_X);
   var_tree->Branch("Muons_Y", &Muons_Y);
   var_tree->Branch("Muons_SumET", &Muons_SumET);

   var_tree->Branch("Electrons_ET", &Electrons_ET);
   var_tree->Branch("Electrons_X", &Electrons_X);
   var_tree->Branch("Electrons_Y", &Electrons_Y);
   var_tree->Branch("Electrons_SumET", &Electrons_SumET);

   var_tree->Branch("Photons_ET", &Photons_ET);
   var_tree->Branch("Photons_X", &Photons_X);
   var_tree->Branch("Photons_Y", &Photons_Y);
   var_tree->Branch("Photons_SumET", &Photons_SumET);

   var_tree->Branch("Jets_Tight_ET", &Jets_Tight_ET);
   var_tree->Branch("Jets_Tight_X", &Jets_Tight_X);
   var_tree->Branch("Jets_Tight_Y", &Jets_Tight_Y);
   var_tree->Branch("Jets_Tight_SumET", &Jets_Tight_SumET);

   var_tree->Branch("Jets_Loose_ET", &Jets_Loose_ET);
   var_tree->Branch("Jets_Loose_X", &Jets_Loose_X);
   var_tree->Branch("Jets_Loose_Y", &Jets_Loose_Y);
   var_tree->Branch("Jets_Loose_SumET", &Jets_Loose_SumET);

   var_tree->Branch("Jets_FJVT_ET", &Jets_FJVT_ET);
   var_tree->Branch("Jets_FJVT_X", &Jets_FJVT_X);
   var_tree->Branch("Jets_FJVT_Y", &Jets_FJVT_Y);
   var_tree->Branch("Jets_FJVT_SumET", &Jets_FJVT_SumET);

   var_tree->Branch("PVTracks_ET", &PVTracks_ET);
   var_tree->Branch("PVTracks_X", &PVTracks_X);
   var_tree->Branch("PVTracks_Y", &PVTracks_Y);
   var_tree->Branch("PVTracks_SumET", &PVTracks_SumET);

   var_tree->Branch("SoftClus_ET", &SoftClus_ET);
   var_tree->Branch("SoftClus_X", &SoftClus_X);
   var_tree->Branch("SoftClus_Y", &SoftClus_Y);
   var_tree->Branch("SoftClus_SumET", &SoftClus_SumET);

   var_tree->Branch("FWDJets_Loose_SumET", &FWDJets_Loose_SumET);
   var_tree->Branch("FWDJets_FJVT_SumET", &FWDJets_FJVT_SumET);

   var_tree->Branch("N_Elecs_PassOR", &N_Elecs_PassOR);
   var_tree->Branch("N_Muons_PassOR", &N_Muons_PassOR);
   var_tree->Branch("N_Jets", &N_Jets);
   var_tree->Branch("N_Loose_FWDJets", &N_Loose_FWDJets);
   var_tree->Branch("N_FJVT_FWDJets", &N_FJVT_FWDJets);

   var_tree->Branch("PV_NTracks", &PV_NTracks);
   var_tree->Branch("NPV_2Tracks", &NPV_2Tracks);
   var_tree->Branch("NPV_4Tracks", &NPV_4Tracks);
   var_tree->Branch("mu_act_cross", &mu_act_cross);
   var_tree->Branch("Ht", &Ht);
   var_tree->Branch("JET_PU_val", &JET_PU_val);




   // Initialize the new TTree which will hold the alternative variables
   alt_tree = new TTree("alt_tree", "Alternative Variable Tree");

   alt_tree->Branch("run_number", &run_number);
   alt_tree->Branch("channel_number", &channel_number);
   alt_tree->Branch("total_weighted_events", &total_weighted_events);
   alt_tree->Branch("total_events", &total_events);
   alt_tree->Branch("rotate_angle", &rotate_angle);
   alt_tree->Branch("data_label", &data_label);

   alt_tree->Branch("cross_section", &cross_section );
   alt_tree->Branch("twe", &twe );
   alt_tree->Branch("kfactor", &kfactor );
   alt_tree->Branch("filter_eff", &filter_eff );
   alt_tree->Branch("upscale", &upscale );

   alt_tree->Branch("event_weight", &event_weight);
   alt_tree->Branch("pile_up_weight", &pile_up_weight);
   alt_tree->Branch("tot_muon_sf", &tot_muon_sf);
   alt_tree->Branch("tot_elec_sf", &tot_elec_sf);
   alt_tree->Branch("trg_muon_sf", &trg_muon_sf);
   alt_tree->Branch("trg_elec_sf", &trg_elec_sf);

   alt_tree->Branch("lead_l_pt", &lead_l_pt);
   alt_tree->Branch("lead_l_px", &lead_l_px);
   alt_tree->Branch("lead_l_py", &lead_l_py);
   alt_tree->Branch("lead_l_mass", &lead_l_mass);
   alt_tree->Branch("lead_l_eta", &lead_l_eta);
   alt_tree->Branch("lead_l_charge", &lead_l_charge);
   alt_tree->Branch("lead_l_sf", &lead_l_sf);
   alt_tree->Branch("lead_l_pdgID", &lead_l_pdgID);

   alt_tree->Branch("subl_l_pt", &subl_l_pt);
   alt_tree->Branch("subl_l_px", &subl_l_px);
   alt_tree->Branch("subl_l_py", &subl_l_py);
   alt_tree->Branch("subl_l_mass", &subl_l_mass);
   alt_tree->Branch("subl_l_eta", &subl_l_eta);
   alt_tree->Branch("subl_l_charge", &subl_l_charge);
   alt_tree->Branch("subl_l_sf", &subl_l_sf);
   alt_tree->Branch("subl_l_pdgID", &subl_l_pdgID);

   alt_tree->Branch("ll_inv_mass", &ll_inv_mass);
   alt_tree->Branch("ll_pt", &ll_pt);
   alt_tree->Branch("ll_px", &ll_px);
   alt_tree->Branch("ll_py", &ll_py);
   alt_tree->Branch("ll_eta", &ll_eta);

   alt_tree->Branch("n_cent_Jets60", &n_cent_Jets60);
   alt_tree->Branch("n_cent_JetsB", &n_cent_JetsB);




   // Initialize the new TTree which will hold the unrotated working point variables
   wpt_tree = new TTree("wpt_tree", "Unrotated Working Point Tree");

   wpt_tree->Branch("WP_Truth_ET",      &WP_Truth_ET);
   wpt_tree->Branch("WP_Truth_X",       &WP_Truth_X);
   wpt_tree->Branch("WP_Truth_Y",       &WP_Truth_Y);
   wpt_tree->Branch("WP_Truth_Phi",     &WP_Truth_Phi);

   wpt_tree->Branch("WP_TST_Tight_ET",  &WP_TST_Tight_ET);
   wpt_tree->Branch("WP_TST_Tight_X",   &WP_TST_Tight_X);
   wpt_tree->Branch("WP_TST_Tight_Y",   &WP_TST_Tight_Y);
   wpt_tree->Branch("WP_TST_Tight_Phi", &WP_TST_Tight_Phi);

   wpt_tree->Branch("WP_TST_Loose_ET",  &WP_TST_Loose_ET);
   wpt_tree->Branch("WP_TST_Loose_X",   &WP_TST_Loose_X);
   wpt_tree->Branch("WP_TST_Loose_Y",   &WP_TST_Loose_Y);
   wpt_tree->Branch("WP_TST_Loose_Phi", &WP_TST_Loose_Phi);

   wpt_tree->Branch("WP_TST_FJVT_ET",   &WP_TST_FJVT_ET);
   wpt_tree->Branch("WP_TST_FJVT_X",    &WP_TST_FJVT_X);
   wpt_tree->Branch("WP_TST_FJVT_Y",    &WP_TST_FJVT_Y);
   wpt_tree->Branch("WP_TST_FJVT_Phi",  &WP_TST_FJVT_Phi);

   wpt_tree->Branch("WP_CST_Tight_ET",  &WP_CST_Tight_ET);
   wpt_tree->Branch("WP_CST_Tight_X",   &WP_CST_Tight_X);
   wpt_tree->Branch("WP_CST_Tight_Y",   &WP_CST_Tight_Y);
   wpt_tree->Branch("WP_CST_Tight_Phi", &WP_CST_Tight_Phi);

   wpt_tree->Branch("WP_Track_ET",      &WP_Track_ET);
   wpt_tree->Branch("WP_Track_X",       &WP_Track_X);
   wpt_tree->Branch("WP_Track_Y",       &WP_Track_Y);
   wpt_tree->Branch("WP_Track_Phi",     &WP_Track_Phi);




}

void MyEvent::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t MyEvent::Process(Long64_t entry){

  // For running on a small number of events.
  // if ( evtcount < 2 ) cout << "YOU ARE NOT RUNNING OVER THE WHOLE DATASET!!!\n";
  // if ( evtcount >= 100000 ) {
    // return kTRUE;
  // }

  // This loads a single event into the work directory
  MyEvent::GetEntry(entry);
  evtmax=fChain->GetEntries();

  // Printing out progress
  fracnew = int( float(evtcount) / evtmax * 100 );
  if ( fracnew > fraclast ) {
    printf( " -- %10lld / %-10lld completed (%d%%)                                \r", evtcount, evtmax, fracnew );
    fraclast = fracnew;
    cout.flush();
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////
  // This is seriously important!!!
  // It determines whether or not to save dataset for training or evaluation.
  // Every third event should be used for evaluation and signal: (entry%3 == 0) pass
  bool keep = false;

  if ( cutGLOBAL == "1in3" ) {
    if ( evtcount < 1 )    cout << "Only every third event will be saved!!!\n";
    if ( evtcount%3 == 0 ) keep = true;
    Scale = 3.0;
  }

  else if ( cutGLOBAL == "2in3") {
    if ( evtcount < 1 )    cout << "Only every first and second event will be saved!!!\n";
    if ( evtcount%3 != 0 ) keep = true;
    Scale = 3.0 / 2.0;
  }

  else if ( cutGLOBAL == "all" ) {
    if ( evtcount < 1 )   cout << "All events will be saved!!!\n";
    keep = true;
    Scale = 1.0;
  }

  else {
    if ( evtcount < 5 )   cout << "Unrecognized cut!!!\n";
  }

  evtcount++;
  if (!keep) return kTRUE;
  ///////////////////////////////////////////////////////////////////////////////////////////////



  // Quality Cuts
  if ( !passBadJet || !passBadMuon || !passCosmic ){
    return kTRUE;
  }

  if ( !isSimulation ) {
    if ( larError == 2 || tileError == 2 || sctError == 2 ){
      return kTRUE;
    }
  }

  // Commented out since we added the good run checker in stage 1
  // if ( !isSimulation ) {
  //   if ( !Is_GRL( grl_matrix, RunNumber, lbn ) ) {
  //     cout << RunNumber << '\n';
  //     cout << lbn << '\n';
  //     cout << Is_GRL( grl_matrix, RunNumber, lbn ) << '\n';
  //     cout << '\n';
  //   }
  // }
  


  ///////////////////////////////////////////////////////////////////////////////////////////////
  ///////// This section collects important variables of the data not already in MyTree /////////

  // Resets these values
  cross_section = 0;
  kfactor       = 0;
  filter_eff    = 0;
  twe           = 0;

  int index = -1;
  if ( isSimulation ) index = Get_DSID_index( int(ChannelNumber), dataset_stats );

  // Here we record the number of baseline leptons over certain pt
  int n_base_elecs_over_10GeV = total_baseline_over( el_tlv_pt, 10.0 );
  int n_base_muons_over_10GeV = total_baseline_over( mu_tlv_pt, 10.0 );

  // Here we record the number of signal leptons that pass OR over a certain pt
  int n_sig_elecs_over_20GeV = total_signal_over( el_tlv_pt, el_signal, el_passOR, 20.0 );
  int n_sig_muons_over_20GeV = total_signal_over( mu_tlv_pt, mu_signal, mu_passOR, 20.0 );

  int n_sig_elecs_over_25GeV = total_signal_over( el_tlv_pt, el_signal, el_passOR, 25.0 );
  int n_sig_muons_over_25GeV = total_signal_over( mu_tlv_pt, mu_signal, mu_passOR, 25.0 );


  // These are needed values for inputs to the neural network
  // The number of signal jets and FWD jet activity in the event
  // (all jets at this point are already above 20 GeV)
  int n_Jets               = 0;
  int n_Loose_FWDJets      = 0;
  int n_FJVT_FWDJets       = 0;
  float sum_Loose_FWDJets  = 0.0;
  float sum_FJVT_FWDJets   = 0.0;


  // These are values needed for the SUSY event selection
  int n_cent_Jets_above_60 = 0;
  int n_cent_bJets         = 0;

  for ( unsigned i = 0; i < SUSYjet_tlv_pt->size(); i++ ) {

    // First we discard all jets if they are not baseline, failed overlap removal, or too high eta
    if ( !SUSYjet_baseline->at(i) || !SUSYjet_passOR->at(i) || fabs(SUSYjet_tlv_eta->at(i))>2.8 ) continue;

    // From SUSY tools: "Retain the jet if JVT > 0.59 OR pt > 60 GeV OR abs(η)>2.5"
    if ( SUSYjet_Jvt->at(i)>0.59 || SUSYjet_tlv_pt->at(i)>60000.0 || fabs(SUSYjet_tlv_eta->at(i))>2.5 ) {

      n_Jets++;

      // If it is a forward jet, then we make note of it and its Pt
      if ( fabs(SUSYjet_tlv_eta->at(i)) > 2.5 ) {

        n_Loose_FWDJets++;
        sum_Loose_FWDJets += SUSYjet_tlv_pt->at(i);

        // We also check if it passes the forward JVT tagger
        if ( SUSYjet_passFJvt->at(i) ) {
          n_FJVT_FWDJets++;
          sum_FJVT_FWDJets += SUSYjet_tlv_pt->at(i);
        }

      }

      // For the SUSY analysis we need to pass a central jet veto
      // So we count the jets that fall within this window
      // We also include the passJvt method, which should have been used from
      // the beggining, but the network has been trained like this so :/
      if ( fabs(SUSYjet_tlv_eta->at(i)) < 2.5 && SUSYjet_passJvt->at(i) ) {

        if ( SUSYjet_tlv_pt->at(i)>60000.0 ) n_cent_Jets_above_60++;
        if ( SUSYjet_bjet->at(i) ) n_cent_bJets++;

      }

    }

  }



  // Now we find the two highest PT signal leptons in the event, the constructor resets them between events (which is good)
  // The leading and subleading will only be selected if they are signal and passOR
  Reconstructed_Z z_boson;
  Analysis_Particle leading_lepton;
  Analysis_Particle subleading_lepton;

  TLorentzVector lepton_pair_kins;
  float lepton_pair_mass = 0;
  float lepton_pair_pt   = 0;
  float lepton_pair_px   = 0;
  float lepton_pair_py   = 0;
  float lepton_pair_eta  = 0;

  // Fist we cycle through the electrons, then the muons, checking if they can make it to the top two spots
  find_leading_leptons_in_event( el_tlv_pt, el_tlv_eta, el_tlv_phi, el_tlv_m, el_charge, el_passOR, el_signal, el_scalefact, el_trig_match, 11, leading_lepton, subleading_lepton );
  find_leading_leptons_in_event( mu_tlv_pt, mu_tlv_eta, mu_tlv_phi, mu_tlv_m, mu_charge, mu_passOR, mu_signal, mu_scalefact, mu_trig_match, 13, leading_lepton, subleading_lepton );

  lepton_pair_kins = leading_lepton.kins + subleading_lepton.kins;
  lepton_pair_mass = lepton_pair_kins.M();
  lepton_pair_pt   = lepton_pair_kins.Pt();
  lepton_pair_px   = lepton_pair_kins.Px();
  lepton_pair_py   = lepton_pair_kins.Py();
  lepton_pair_eta  = lepton_pair_kins.Eta();



  ///////////////////////////////////////////////////////////////////////////////////////////////



  ///////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////// Starting here are the different event selections! //////////////////////


  ////////////////// Z->ee //////////////////
  ///////////////// STRICT //////////////////
  if ( selectionGLOBAL == "Zee_strict" ) {
    if ( evtcount <= 2 ) cout << "Applying Z->ee strict event selection\n";

    // 1) Exactly two signal electrons passing OR with no additional baseline electrons above 10GeV
    if ( n_sig_elecs_over_20GeV != 2 ||  n_base_elecs_over_10GeV != 2  || n_base_muons_over_10GeV != 0 ) {
      return kTRUE;
    }

    // 2) Lowest unprescaled trigger is fired
    if ( !passed_elec_trigger ) {
      return kTRUE;
    }

    // 3) One of the electrons matches the trigger
    if ( leading_lepton.tmatched == 0 && subleading_lepton.tmatched == 0 ) {
      return kTRUE;
    }

    // 4) Leading and subleading leptons have PT greater than 30, 20
    if ( leading_lepton.kins.Pt() < 30000.0 || subleading_lepton.kins.Pt() < 20000.0 ) {
      return kTRUE;
    }

    // 5) They create a valid Z boson candidate
    z_boson.create_from_parents( leading_lepton, subleading_lepton, 15.0 );
    if ( !z_boson.good_reconstruction ) {
      return kTRUE;
    }

  }
  //////////////////////////////////////////////


  ////////////////// Z->mm //////////////////
  ///////////////// STRICT //////////////////
  else if ( selectionGLOBAL == "Zmumu_strict" ) {
    if ( evtcount <= 2 ) cout << "Applying Z->mumu strict event selection\n";

    // 1) Exactly muons passing OR with no additional leptons above 10GeV
    if ( n_sig_muons_over_20GeV != 2 ||  n_base_muons_over_10GeV != 2  || n_base_elecs_over_10GeV != 0 ) {
      return kTRUE;
    }

    // 2) Lowest unprescaled muon trigger is fired
    if ( !passed_muon_trigger ) {
      return kTRUE;
    }

    // 3) One of the muons matches the trigger
    if ( leading_lepton.tmatched == 0 && subleading_lepton.tmatched == 0 ) {
      return kTRUE;
    }

    // 4) Leading and subleading leptons have PT greater than 30, 20
    if ( leading_lepton.kins.Pt() < 30000.0 || subleading_lepton.kins.Pt() < 20000.0 ) {
      return kTRUE;
    }

    // 5) They create a valid Z boson candidate
    z_boson.create_from_parents( leading_lepton, subleading_lepton, 15.0 );
    if ( !z_boson.good_reconstruction ) {
      return kTRUE;
    }

  }
  //////////////////////////////////////////////


  ////////////////// WW->lvlv //////////////////
  /////////////////// STRICT ///////////////////
  else if ( selectionGLOBAL == "WW_strict" ) {
    if ( evtcount <= 2 ) cout << "Applying WW->lvlv strict event selection\n";

    // 1) Exactly two signal leptons above 25 GeV passing OR
    if ( n_sig_muons_over_25GeV + n_sig_elecs_over_25GeV != 2 ){
      return kTRUE;
    }

    // 2) No other leptons above 10 GeV
    if ( n_base_muons_over_10GeV + n_base_elecs_over_10GeV != 2 ){
      return kTRUE;
    }

    // 3) One of the lowest unprescaled triggers is fired
    if ( !passed_elec_trigger && !passed_muon_trigger ) {
      return kTRUE;
    }

    // 4) One of the leptons matches the trigger
    if ( leading_lepton.tmatched == 0 && subleading_lepton.tmatched == 0 ) {
      return kTRUE;
    }

    // 5) Leading and subleading are of opposite charge
    if ( leading_lepton.charge * subleading_lepton.charge >= 0 ) {
      return kTRUE;
    }

  }
  //////////////////////////////////////////////
  
  
  ////////////////// ttbar //////////////////
  else if ( selectionGLOBAL == "tt_strict" ) {
    if ( evtcount <= 2 ) cout << "Applying ttbar strict event selection\n";

    // 1) At least four jets
    if ( n_Jets < 4 ) {
      return kTRUE;
    }
    
    // 2) At least one b-tagged jet
    if ( n_cent_bJets == 0 ) {
      return kTRUE;
    }

  }
  //////////////////////////////////////////////
  
  
  ////////////////// ZZ->llvv //////////////////
  else if ( selectionGLOBAL == "ZZ_lax" ) {
    if ( evtcount <= 2 ) cout << "Applying ZZ->llvv lax event selection\n";

    // 1) At least two baseline leptons of the same flavour were detected
    if ( n_base_muons_over_10GeV < 2 && n_base_elecs_over_10GeV < 2 ) {
      return kTRUE;
    }

  }
  //////////////////////////////////////////////


  ////////////////// WW->lvlv //////////////////
  else if ( selectionGLOBAL == "WW_lax" ) {
    if ( evtcount <= 2 ) cout << "Applying WW->lvlv lax event selection\n";
    
    // 1) At least two baseline leptons were detected
      if ( n_base_muons_over_10GeV + n_base_elecs_over_10GeV < 2 ){
        return kTRUE;
      }
      
    }
  //////////////////////////////////////////////


  ////////////////// WZ->lvll //////////////////
  else if ( selectionGLOBAL == "WZ_lax" ) {
    if ( evtcount <= 2 ) cout << "Applying WZ->lvll lax event selection\n";
    
    // 1) At least three baseline leptons were detected
    if ( n_base_muons_over_10GeV + n_base_elecs_over_10GeV < 3 ){
      return kTRUE;
    }
    
  }
  //////////////////////////////////////////////


  ////////////////// ttbar and single top //////////////////
  else if ( selectionGLOBAL == "top_lax" ) {
    if ( evtcount <= 2 ) cout << "Applying ttbar lax event selection\n";

    // 1) Two signal jets were detected
    if ( n_Jets < 2 ) {
     return kTRUE;
    }

  }
  //////////////////////////////////////////////


  ////////////////// susy analysis //////////////////
  // Here the event selection is based on cutflow //
  else if ( selectionGLOBAL == "susy_2l_0jets" ) {
    if ( evtcount <= 2 ) cout << "Applying the susy analysis event selection for 2l SF 0Jets\n";

    // 1) Exactly two signal leptons above 25 GeV passing OR
    if ( n_sig_muons_over_25GeV + n_sig_elecs_over_25GeV != 2 ){
      return kTRUE;
    }

    // 2) No other leptons above 10 GeV
    if ( n_base_muons_over_10GeV + n_base_elecs_over_10GeV != 2 ){
      return kTRUE;
    }

    // 3) One of the lowest unprescaled triggers is fired
    if ( !passed_elec_trigger && !passed_muon_trigger ) {
      return kTRUE;
    }

    // 4) One of the leptons matches the trigger
    if ( leading_lepton.tmatched == 0 && subleading_lepton.tmatched == 0 ) {
      return kTRUE;
    }

    // 5) Leading pair is the same flavour
    if ( leading_lepton.pdgID != subleading_lepton.pdgID ) {
      return kTRUE;
    }

    // 6) Leading lepton pair is opposite charge
    if ( leading_lepton.charge * subleading_lepton.charge >= 0 ) {
      return kTRUE;
    }

    // 7) Leading lepton pair pt is above 25, 25
    if ( leading_lepton.kins.Pt() < 25000.0 || subleading_lepton.kins.Pt() < 25000.0 ) {
      return kTRUE;
    }

    // 8) Dilepton invariant mass is greater than 60 GeV ( lowered to see Z peak )
    if ( lepton_pair_mass < 60000.0 ) {
      return kTRUE;
    }

    // 9) No central jets above 60 GeV (b-jet veto happens later but we need it for control regions!)
    if ( n_cent_Jets_above_60 > 0 ) {
      return kTRUE;
    }

  }
  //////////////////////////////////////////////

  else if ( selectionGLOBAL == "noselec" ) {
    if ( evtcount <= 2 ) cout << "No event selection applied!!!\n";
  }

  else {
    if ( evtcount < 5 ) cout << "Unrecognized event selection. Will now die!!!\n";
      return kTRUE;
  }



  ///////////////////////////////////////////////////////////////////////////////////////////////
  passed++;



  // First we fill in all of the variables needed in the wpt_tree (everything is saved in GeV)
  if ( isSimulation ){
    WP_Truth_ET        =   MET_Truth_NonInt_et    / 1000;
    WP_Truth_X         =   MET_Truth_NonInt_etx   / 1000;
    WP_Truth_Y         =   MET_Truth_NonInt_ety   / 1000;
    WP_Truth_Phi       =   atan2( MET_Truth_NonInt_ety, MET_Truth_NonInt_etx );
  }

  WP_TST_Tight_ET     =    MET_RefFinal_TST_et    / 1000;
  WP_TST_Tight_X      =    MET_RefFinal_TST_etx   / 1000;
  WP_TST_Tight_Y      =    MET_RefFinal_TST_ety   / 1000;
  WP_TST_Tight_Phi    =    atan2( MET_RefFinal_TST_ety, MET_RefFinal_TST_etx );

  WP_TST_Loose_ET     =    MET_RefFinal_TST_LOOSE_et    / 1000;
  WP_TST_Loose_X      =    MET_RefFinal_TST_LOOSE_etx   / 1000;
  WP_TST_Loose_Y      =    MET_RefFinal_TST_LOOSE_ety   / 1000;
  WP_TST_Loose_Phi    =    atan2( MET_RefFinal_TST_LOOSE_ety, MET_RefFinal_TST_LOOSE_etx );

  WP_TST_FJVT_ET      =    MET_RefFinal_TST_FJVT_et    / 1000;
  WP_TST_FJVT_X       =    MET_RefFinal_TST_FJVT_etx   / 1000;
  WP_TST_FJVT_Y       =    MET_RefFinal_TST_FJVT_ety   / 1000;
  WP_TST_FJVT_Phi     =    atan2( MET_RefFinal_TST_FJVT_ety, MET_RefFinal_TST_FJVT_etx );

  WP_CST_Tight_ET     =    MET_RefFinal_CST_et    / 1000;
  WP_CST_Tight_X      =    MET_RefFinal_CST_etx   / 1000;
  WP_CST_Tight_Y      =    MET_RefFinal_CST_ety   / 1000;
  WP_CST_Tight_Phi    =    atan2( MET_RefFinal_CST_ety, MET_RefFinal_CST_etx );

  WP_Track_ET         =    MET_Ref_Track_et    / 1000;
  WP_Track_X          =    MET_Ref_Track_etx   / 1000;
  WP_Track_Y          =    MET_Ref_Track_ety   / 1000;
  WP_Track_Phi        =    atan2( MET_Ref_Track_ety, MET_Ref_Track_etx );


  // Now we can begin rotating all of the vectors and filling the var_tree
  // Now we need to find all the appropriate values for the var_tree
  if ( isSimulation ){
    clockwise_rotate( MET_Truth_NonInt_etx, MET_Truth_NonInt_ety, WP_TST_Tight_Phi );
    MET_Truth_ET        =   MET_Truth_NonInt_et    / 1000;
    MET_Truth_X         =   MET_Truth_NonInt_etx   / 1000;
    MET_Truth_Y         =   MET_Truth_NonInt_ety   / 1000;
    MET_Truth_SumET     =   MET_Truth_NonInt_SumET / 1000;
  }

  MET_TST_Tight_ET    =   MET_RefFinal_TST_et    / 1000;
  MET_TST_Tight_SumET =   MET_RefFinal_TST_SumET / 1000;

  clockwise_rotate( MET_RefFinal_TST_LOOSE_etx, MET_RefFinal_TST_LOOSE_ety, WP_TST_Tight_Phi );
  MET_TST_Loose_ET    =   MET_RefFinal_TST_LOOSE_et    / 1000;
  MET_TST_Loose_X     =   MET_RefFinal_TST_LOOSE_etx   / 1000;
  MET_TST_Loose_Y     =   MET_RefFinal_TST_LOOSE_ety   / 1000;
  MET_TST_Loose_SumET =   MET_RefFinal_TST_LOOSE_SumET / 1000;

  clockwise_rotate( MET_RefFinal_TST_FJVT_etx, MET_RefFinal_TST_FJVT_ety, WP_TST_Tight_Phi );
  MET_TST_FJVT_ET     =   MET_RefFinal_TST_FJVT_et    / 1000;
  MET_TST_FJVT_X      =   MET_RefFinal_TST_FJVT_etx   / 1000;
  MET_TST_FJVT_Y      =   MET_RefFinal_TST_FJVT_ety   / 1000;
  MET_TST_FJVT_SumET  =   MET_RefFinal_TST_FJVT_SumET / 1000;

  clockwise_rotate( MET_RefFinal_CST_etx, MET_RefFinal_CST_ety, WP_TST_Tight_Phi );
  MET_CST_Tight_ET    =   MET_RefFinal_CST_et    / 1000;
  MET_CST_Tight_X     =   MET_RefFinal_CST_etx   / 1000;
  MET_CST_Tight_Y     =   MET_RefFinal_CST_ety   / 1000;
  MET_CST_Tight_SumET =   MET_RefFinal_CST_SumET / 1000;

  clockwise_rotate( MET_Ref_Track_etx, MET_Ref_Track_ety, WP_TST_Tight_Phi );
  MET_Track_ET        =   MET_Ref_Track_et    / 1000;
  MET_Track_X         =   MET_Ref_Track_etx   / 1000;
  MET_Track_Y         =   MET_Ref_Track_ety   / 1000;
  MET_Track_SumET     =   MET_Ref_Track_SumET / 1000;

  clockwise_rotate( MET_Muons_TST_etx, MET_Muons_TST_ety, WP_TST_Tight_Phi );
  Muons_ET            =   MET_Muons_TST_et    / 1000;
  Muons_X             =   MET_Muons_TST_etx   / 1000;
  Muons_Y             =   MET_Muons_TST_ety   / 1000;
  Muons_SumET         =   MET_Muons_TST_SumET / 1000;

  clockwise_rotate( MET_RefEle_TST_etx, MET_RefEle_TST_ety, WP_TST_Tight_Phi );
  Electrons_ET        =   MET_RefEle_TST_et    / 1000;
  Electrons_X         =   MET_RefEle_TST_etx   / 1000;
  Electrons_Y         =   MET_RefEle_TST_ety   / 1000;
  Electrons_SumET     =   MET_RefEle_TST_SumET / 1000;

  clockwise_rotate( MET_RefGamma_TST_etx, MET_RefGamma_TST_ety, WP_TST_Tight_Phi );
  Photons_ET          =   MET_RefGamma_TST_et    / 1000;
  Photons_X           =   MET_RefGamma_TST_etx   / 1000;
  Photons_Y           =   MET_RefGamma_TST_ety   / 1000;
  Photons_SumET       =   MET_RefGamma_TST_SumET / 1000;

  clockwise_rotate( MET_RefJet_TST_etx, MET_RefJet_TST_ety, WP_TST_Tight_Phi );
  Jets_Tight_ET       =   MET_RefJet_TST_et    / 1000;
  Jets_Tight_X        =   MET_RefJet_TST_etx   / 1000;
  Jets_Tight_Y        =   MET_RefJet_TST_ety   / 1000;
  Jets_Tight_SumET    =   MET_RefJet_TST_SumET / 1000;

  clockwise_rotate( MET_RefJet_TST_LOOSE_etx, MET_RefJet_TST_LOOSE_ety, WP_TST_Tight_Phi );
  Jets_Loose_ET       =   MET_RefJet_TST_LOOSE_et    / 1000;
  Jets_Loose_X        =   MET_RefJet_TST_LOOSE_etx   / 1000;
  Jets_Loose_Y        =   MET_RefJet_TST_LOOSE_ety   / 1000;
  Jets_Loose_SumET    =   MET_RefJet_TST_LOOSE_SumET / 1000;

  clockwise_rotate( MET_RefJet_TST_FJVT_etx, MET_RefJet_TST_FJVT_ety, WP_TST_Tight_Phi );
  Jets_FJVT_ET        =   MET_RefJet_TST_FJVT_et    / 1000;
  Jets_FJVT_X         =   MET_RefJet_TST_FJVT_etx   / 1000;
  Jets_FJVT_Y         =   MET_RefJet_TST_FJVT_ety   / 1000;
  Jets_FJVT_SumET     =   MET_RefJet_TST_FJVT_SumET / 1000;

  clockwise_rotate( MET_PVSoftTrk_TST_etx, MET_PVSoftTrk_TST_ety, WP_TST_Tight_Phi );
  PVTracks_ET         =   MET_PVSoftTrk_TST_et    / 1000;
  PVTracks_X          =   MET_PVSoftTrk_TST_etx   / 1000;
  PVTracks_Y          =   MET_PVSoftTrk_TST_ety   / 1000;
  PVTracks_SumET      =   MET_PVSoftTrk_TST_SumET / 1000;

  clockwise_rotate( MET_SoftClus_CST_etx, MET_SoftClus_CST_ety, WP_TST_Tight_Phi );
  SoftClus_ET         =   MET_SoftClus_CST_et     / 1000;
  SoftClus_X          =   MET_SoftClus_CST_etx    / 1000;
  SoftClus_Y          =   MET_SoftClus_CST_ety    / 1000;
  SoftClus_SumET      =   MET_SoftClus_CST_SumET  / 1000;

  FWDJets_Loose_SumET =   sum_Loose_FWDJets / 1000;
  FWDJets_FJVT_SumET  =   sum_FJVT_FWDJets  / 1000;

  N_Elecs_PassOR      = n_sig_elecs_over_20GeV;
  N_Muons_PassOR      = n_sig_muons_over_20GeV;
  N_Jets              = n_Jets;
  N_Loose_FWDJets     = n_Loose_FWDJets;
  N_FJVT_FWDJets      = n_FJVT_FWDJets;

  PV_NTracks          =   vtx_nTracks;
  NPV_2Tracks         =   PV2trk;
  NPV_4Tracks         =   PV4trk;
  mu_act_cross        =   actualInteractionsPerCrossing;

  if ( !isSimulation ) mu_act_cross = actualInteractionsPerCrossing / 1.03;

  Ht                  =   Muons_SumET + Electrons_SumET + Photons_SumET + Jets_Tight_SumET;
  MET_over_root_SumET =   MET_TST_Tight_ET / sqrt(MET_TST_Tight_SumET);
  MET_SIG             =   MET_Significance;

  JET_PU_val = 0;
  float current_val = 0;
  for (int i = 0; i < SUSYjet_n; i++) {
    current_val = jet_prob_pileup ( SUSYjet_tlv_pt->at(i) / 1000, SUSYjet_Jvt->at(i), SUSYjet_tlv_eta->at(i) ) * ( SUSYjet_tlv_pt->at(i) / 1000 );
    JET_PU_val += current_val * current_val;
  }
  JET_PU_val = sqrt(JET_PU_val);



  // Filling the alternative variables
  run_number            = RunNumber;
  channel_number        = ChannelNumber;
  total_events          = totalEvents;
  total_weighted_events = totalWeightedEvents;
  rotate_angle          = WP_TST_Tight_Phi;
  data_label            = labelGLOBAL;


  if ( index != -1 ) {
    cross_section         = dataset_stats[index][1];
    kfactor               = dataset_stats[index][2];
    filter_eff            = dataset_stats[index][3];
    twe                   = dataset_stats[index][4];
  }
  upscale               = Scale;

  event_weight          = EventWeight;
  pile_up_weight        = PileUpWeight;
  tot_muon_sf           = total_muon_sf;
  tot_elec_sf           = total_elec_sf;
  trg_muon_sf           = trig_muon_sf;
  trg_elec_sf           = trig_elec_sf;

  lead_l_pt             = leading_lepton.kins.Pt() / 1000;
  lead_l_px             = leading_lepton.kins.Px() / 1000;
  lead_l_py             = leading_lepton.kins.Py() / 1000;
  lead_l_eta            = leading_lepton.kins.Eta();
  lead_l_mass           = leading_lepton.kins.M()  / 1000;
  lead_l_charge         = leading_lepton.charge;
  lead_l_sf             = leading_lepton.sf;
  lead_l_pdgID          = leading_lepton.pdgID;

  subl_l_pt             = subleading_lepton.kins.Pt() / 1000;
  subl_l_px             = subleading_lepton.kins.Px() / 1000;
  subl_l_py             = subleading_lepton.kins.Py() / 1000;
  subl_l_eta            = subleading_lepton.kins.Eta();
  subl_l_mass           = subleading_lepton.kins.M()  / 1000;
  subl_l_charge         = subleading_lepton.charge;
  subl_l_sf             = subleading_lepton.sf;
  subl_l_pdgID          = subleading_lepton.pdgID;

  ll_inv_mass           = lepton_pair_mass / 1000;
  ll_pt                 = lepton_pair_pt   / 1000;
  ll_px                 = lepton_pair_px   / 1000;
  ll_py                 = lepton_pair_py   / 1000;
  ll_eta                = lepton_pair_eta;

  n_cent_Jets60         = n_cent_Jets_above_60;
  n_cent_JetsB          = n_cent_bJets;


  // This is just a final discriminant as some (very few, like 2 or 3) events were coming through with several TeV momenta.
  // They may not be unphysical but they sure do *&^% up the network's ability to learn. So we discard them anyway!
  vector<double> components = { MET_TST_Tight_ET, Muons_ET, Electrons_ET, Photons_ET, Jets_Loose_ET };
  for (unsigned i = 0; i < components.size(); i++) {
    if ( components[i] > 3000.0 ) {
      return kTRUE;
    }
  }

  if ( MET_TST_Tight_SumET > 4000.0 || MET_TST_Tight_SumET == 0 ) {
    return kTRUE;
  }

  var_tree->Fill();
  alt_tree->Fill();
  wpt_tree->Fill();

  return kTRUE;

}

void MyEvent::SlaveTerminate(){
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.
}

void MyEvent::Terminate(){
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.
  std::cout<<"MyEvent::Terminate()                                   " << std::endl;
  ofile->Write();
  ofile->Close();
}
