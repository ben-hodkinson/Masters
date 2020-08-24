//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jun 20 14:33:53 2018 by ROOT version 5.34/00
// from TTree MyTree/MyTree
// found on file: myfile.root
//////////////////////////////////////////////////////////

#ifndef MyEvent_h
#define MyEvent_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TLorentzVector.h>
#include <iostream>
#include <vector>
#include <string>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TProfile.h>
#include <TCanvas.h>
#include <TRandom2.h>
#include <TRandom3.h>

using namespace std;

// Fixed size dimensions of array or collections stored in the TTree if any.

class MyEvent : public TSelector {
public :

   ///////////////////////////////////
   ///////////////////////////////////

   // Variables needed throughout process
   long long int passed;
   long long int evtmax;
   long long int evtcount;
   int fraclast;
   int fracnew;
   TRandom* rnd;

   vector<vector<double>> dataset_stats;
   vector<vector<unsigned>> grl_matrix;
   double Scale;

   // Declare the file
   TFile *ofile;

   // Declare the output trees
   TTree *var_tree;
   TTree *alt_tree;
   TTree *wpt_tree;

   // Variables of the wpt_tree
   // These variables remain unrotated and are used for comparing working point performance
   // Latter on in the analysis
   double WP_Truth_ET  = 0;
   double WP_Truth_X   = 0;
   double WP_Truth_Y   = 0;
   double WP_Truth_Phi = 0;

   double WP_TST_Tight_ET;
   double WP_TST_Tight_X;
   double WP_TST_Tight_Y;
   double WP_TST_Tight_Phi;

   double WP_TST_Loose_ET;
   double WP_TST_Loose_X;
   double WP_TST_Loose_Y;
   double WP_TST_Loose_Phi;

   double WP_TST_FJVT_ET;
   double WP_TST_FJVT_X;
   double WP_TST_FJVT_Y;
   double WP_TST_FJVT_Phi;

   double WP_CST_Tight_ET;
   double WP_CST_Tight_X;
   double WP_CST_Tight_Y;
   double WP_CST_Tight_Phi;

   double WP_Track_ET;
   double WP_Track_X;
   double WP_Track_Y;
   double WP_Track_Phi;



   // Variables for the alt_tree
   // These variables are for the alt_tree and are used in MC scaling
   // And further event selection used in the SUSY analysis
   unsigned  run_number;
   ULong64_t channel_number;
   double    total_events;
   double    total_weighted_events;
   double    rotate_angle;
   TString   data_label;

   double cross_section;
   double twe;
   double kfactor;
   double filter_eff;
   double upscale;

   double  event_weight;
   double  pile_up_weight;
   double  tot_muon_sf;
   double  tot_elec_sf;
   double  trg_muon_sf;
   double  trg_elec_sf;

   double lead_l_pt;
   double lead_l_px;
   double lead_l_py;
   double lead_l_eta;
   double lead_l_mass;
   float  lead_l_charge;
   double lead_l_sf;
   int    lead_l_pdgID;

   double subl_l_pt;
   double subl_l_px;
   double subl_l_py;
   double subl_l_mass;
   double subl_l_eta;
   float  subl_l_charge;
   double subl_l_sf;
   int    subl_l_pdgID;

   float ll_inv_mass;
   float ll_pt;
   float ll_px;
   float ll_py;
   float ll_eta;

   int n_cent_Jets60;
   int n_cent_JetsB;



   // Variables for the var_tree
   // These variables are used as inputs for the neural network.
   // Every vector used is rotated such that there is no Y component in the
   // Tight MET reconstruction
   // Quickly plotting the preformances using these rotated variables is costly
   // That is why there is an extra tree for unrotated wpt above

   double MET_Truth_ET    = 0;
   double MET_Truth_X     = 0;
   double MET_Truth_Y     = 0;
   double MET_Truth_SumET = 0;

   double MET_TST_Tight_ET;
   double MET_TST_Tight_SumET;

   double MET_TST_Loose_ET;
   double MET_TST_Loose_X;
   double MET_TST_Loose_Y;
   double MET_TST_Loose_SumET;

   double MET_TST_FJVT_ET;
   double MET_TST_FJVT_X;
   double MET_TST_FJVT_Y;
   double MET_TST_FJVT_SumET;

   double MET_CST_Tight_ET;
   double MET_CST_Tight_X;
   double MET_CST_Tight_Y;
   double MET_CST_Tight_SumET;

   double MET_Track_ET;
   double MET_Track_X;
   double MET_Track_Y;
   double MET_Track_SumET;

   double Muons_ET;
   double Muons_X;
   double Muons_Y;
   double Muons_SumET;

   double Electrons_ET;
   double Electrons_X;
   double Electrons_Y;
   double Electrons_SumET;

   double Photons_ET;
   double Photons_X;
   double Photons_Y;
   double Photons_SumET;

   double Jets_Tight_ET;
   double Jets_Tight_X;
   double Jets_Tight_Y;
   double Jets_Tight_SumET;

   double Jets_Loose_ET;
   double Jets_Loose_X;
   double Jets_Loose_Y;
   double Jets_Loose_SumET;

   double Jets_FJVT_ET;
   double Jets_FJVT_X;
   double Jets_FJVT_Y;
   double Jets_FJVT_SumET;

   double PVTracks_ET;
   double PVTracks_X;
   double PVTracks_Y;
   double PVTracks_SumET;

   double SoftClus_ET;
   double SoftClus_X;
   double SoftClus_Y;
   double SoftClus_SumET;

   double FWDJets_Loose_SumET;
   double FWDJets_FJVT_SumET;

   int N_Elecs_PassOR;
   int N_Muons_PassOR;
   int N_Jets;
   int N_Loose_FWDJets;
   int N_FJVT_FWDJets;

   ULong64_t PV_NTracks;
   int NPV_2Tracks;
   int NPV_4Tracks;
   float mu_act_cross;

   double Ht;
   double MET_over_root_SumET;
   double MET_SIG;
   float  JET_PU_val;


   ///////////////////////////////////
   ///////////////////////////////////


   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Int_t           trjet_n;
   vector<double>  *trjet_tlv_pt;
   vector<double>  *trjet_tlv_eta;
   vector<double>  *trjet_tlv_phi;
   vector<double>  *trjet_tlv_m;
   Int_t           el_n = 0;
   vector<double>  *el_tlv_pt;
   vector<double>  *el_tlv_eta;
   vector<double>  *el_tlv_phi;
   vector<double>  *el_tlv_m;
   vector<float>   *el_charge;
   vector<bool>    *el_signal;
   vector<char>    *el_passOR;
   vector<double>  *el_scalefact;
   vector<char>    *el_trig_match;
   Int_t           mu_n = 0;
   Int_t           SUSYjet_n;
   vector<double>  *SUSYjet_tlv_pt;
   vector<double>  *SUSYjet_tlv_eta;
   vector<double>  *SUSYjet_tlv_phi;
   vector<double>  *SUSYjet_tlv_m;
   vector<int>     *SUSYjet_NumTrkPt500;
   vector<float>   *SUSYjet_JvtRpt;
   vector<float>   *SUSYjet_JvtJvfcorr;
   vector<char>    *SUSYjet_bad;
   vector<char>    *SUSYjet_signal;
   vector<char>    *SUSYjet_baseline;
   vector<char>    *SUSYjet_bjet;
   vector<char>    *SUSYjet_passJvt;
   vector<char>    *SUSYjet_passFJvt;
   vector<float>   *SUSYjet_Jvt;
   vector<float>   *SUSYjet_FracSamplingMax;
   vector<char>    *SUSYjet_passOR;
   vector<float>   *SUSYjet_JVF;
   Double_t        MET_Truth_IntMuons_ety;
   Double_t        MET_Truth_Int_etx;
   Double_t        MET_Truth_NonInt_SumET;
   Double_t        MET_Truth_Int_ety;
   Double_t        MET_Significance;
   Double_t        MET_SoftClus_CST_etx;
   Double_t        MET_SoftClus_CST_et;
   Double_t        MET_RefFinal_CST_SumET;
   Double_t        MET_RefFinal_CST_ety;
   Double_t        MET_RefFinal_CST_etx;
   Double_t        MET_Truth_Int_SumET;
   Double_t        MET_RefFinal_CST_et;
   Double_t        MET_Ref_Track_SumET;
   Double_t        MET_Truth_NonInt_ety;
   Double_t        MET_RefJet_TST_FJVT_SumET;
   Double_t        MET_Ref_Track_ety;
   Double_t        MET_RefJet_TST_FJVT_etx;
   Double_t        MET_RefFinal_TST_FJVT_SumET;
   Double_t        MET_RefFinal_TST_FJVT_ety;
   Double_t        MET_RefFinal_TST_FJVT_etx;
   Double_t        MET_RefJet_TST_LOOSE_SumET;
   Double_t        MET_RefJet_TST_LOOSE_ety;
   Double_t        MET_RefJet_TST_LOOSE_etx;
   Double_t        MET_RefJet_TST_LOOSE_et;
   Double_t        MET_Truth_IntMuons_SumET;
   Double_t        MET_Truth_IntMuons_et;
   Double_t        MET_Ref_Track_et;
   Double_t        MET_RefFinal_TST_LOOSE_SumET;
   Double_t        MET_RefFinal_TST_LOOSE_ety;
   Double_t        MET_RefFinal_TST_LOOSE_et;
   Double_t        MET_RefJet_TST_FJVT_et;
   Double_t        MET_PVSoftTrk_TST_ety;
   Double_t        MET_Muons_TST_et;
   Double_t        MET_RefJet_TST_ety;
   Int_t           PV2trk;
   Bool_t          isSimulation;
   Double_t        MET_PVSoftTrk_TST_etx;
   UInt_t          RunNumber;
   Double_t        MET_RefJet_TST_SumET;
   Int_t           passBadMuon;
   Int_t           pvIndex;
   Int_t           passCosmic;
   Float_t         totalWeightedEvents;
   Float_t         totalEvents;
   Double_t        MET_SoftClus_CST_SumET;
   Double_t        MET_RefJet_TST_FJVT_ety;
   Double_t        MET_RefFinal_TST_FJVT_et;
   Int_t           PV4trk;
   Double_t        MET_RefEle_TST_SumET;
   Int_t           tileError;
   Float_t         EventWeight;
   UInt_t          lbn;
   Double_t        MET_Truth_Int_et;
   Double_t        MET_Muons_TST_SumET;
   Int_t           sctError;
   Int_t           passBadJet;
   UInt_t          ChannelNumber;
   ULong64_t       EventNumber;
   Double_t        MET_SoftClus_CST_ety;
   Double_t        MET_PVSoftTrk_TST_et;
   UInt_t          coreFlags;
   Float_t         vtx_z;
   Bool_t          passed_elec_trigger;
   Float_t         PileUpWeight;
   Double_t        MET_Truth_NonInt_etx;
   Double_t        MET_Muons_TST_etx;
   Float_t         averageInteractionsPerCrossing;
   Float_t         actualInteractionsPerCrossing;
   Bool_t          passed_muon_trigger;
   Float_t         CorrectedMuTrue;
   Double_t        MET_RefFinal_TST_etx;
   Float_t         CorrectedMuFalse;
   Double_t        MET_Truth_NonInt_et;
   Double_t        MET_PVSoftTrk_TST_SumET;
   ULong64_t       vtx_nTracks;
   Double_t        MET_RefFinal_TST_ety;
   Double_t        MET_RefFinal_TST_LOOSE_etx;
   Double_t        MET_RefJet_TST_etx;
   Double_t        MET_RefFinal_TST_et;
   UInt_t          bcid;
   Double_t        MET_Truth_IntMuons_etx;
   Double_t        MET_RefJet_TST_et;
   Double_t        MET_Muons_TST_ety;
   Double_t        MET_RefFinal_TST_SumET;
   Double_t        MET_RefEle_TST_et;
   Double_t        MET_RefEle_TST_ety;
   Double_t        MET_Ref_Track_etx;
   Double_t        MET_RefEle_TST_etx;
   Double_t        MET_RefGamma_TST_et;
   Double_t        MET_RefGamma_TST_etx;
   Int_t           larError;
   Double_t        MET_RefGamma_TST_ety;
   Double_t        MET_RefGamma_TST_SumET;
   Float_t         total_elec_sf;
   Float_t         total_muon_sf;
   Float_t         trig_elec_sf;
   Float_t         trig_muon_sf;
   vector<double>  *mu_tlv_pt;
   vector<double>  *mu_tlv_eta;
   vector<double>  *mu_tlv_phi;
   vector<double>  *mu_tlv_m;
   vector<double>  *mu_Theta;
   vector<float>   *mu_z0sinTheta;
   vector<char>    *mu_trig_match;
   vector<char>    *mu_passOR;
   vector<float>   *mu_d0;
   vector<bool>    *mu_signal;
   vector<float>   *mu_charge;
   vector<float>   *mu_d0sig;
   vector<double>  *mu_scalefact;

   // List of branches
   TBranch        *b_trjet_n;   //!
   TBranch        *b_trjet_tlv_pt;   //!
   TBranch        *b_trjet_tlv_eta;   //!
   TBranch        *b_trjet_tlv_phi;   //!
   TBranch        *b_trjet_tlv_m;   //!
   TBranch        *b_el_n;   //!
   TBranch        *b_el_tlv_pt;   //!
   TBranch        *b_el_tlv_eta;   //!
   TBranch        *b_el_tlv_phi;   //!
   TBranch        *b_el_tlv_m;   //!
   TBranch        *b_el_charge;   //!
   TBranch        *b_el_signal;   //!
   TBranch        *b_el_passOR;   //!
   TBranch        *b_el_scalefact;   //!
   TBranch        *b_el_trig_match;   //!
   TBranch        *b_mu_n;   //!
   TBranch        *b_SUSYjet_n;   //!
   TBranch        *b_SUSYjet_tlv_pt;   //!
   TBranch        *b_SUSYjet_tlv_eta;   //!
   TBranch        *b_SUSYjet_tlv_phi;   //!
   TBranch        *b_SUSYjet_tlv_m;   //!
   TBranch        *b_SUSYjet_NumTrkPt500;   //!
   TBranch        *b_SUSYjet_JvtRpt;   //!
   TBranch        *b_SUSYjet_JvtJvfcorr;   //!
   TBranch        *b_SUSYjet_bad;   //!
   TBranch        *b_SUSYjet_signal;   //!
   TBranch        *b_SUSYjet_baseline;   //!
   TBranch        *b_SUSYjet_bjet;   //!
   TBranch        *b_SUSYjet_passJvt;   //!
   TBranch        *b_SUSYjet_passFJvt;   //!
   TBranch        *b_SUSYjet_Jvt;   //!
   TBranch        *b_SUSYjet_FracSamplingMax;   //!
   TBranch        *b_SUSYjet_passOR;   //!
   TBranch        *b_SUSYjet_JVF;   //!
   TBranch        *b_MET_Truth_IntMuons_ety;   //!
   TBranch        *b_MET_Truth_Int_etx;   //!
   TBranch        *b_MET_Truth_NonInt_SumET;   //!
   TBranch        *b_MET_Truth_Int_ety;   //!
   TBranch        *b_MET_Significance;   //!
   TBranch        *b_MET_SoftClus_CST_etx;   //!
   TBranch        *b_MET_SoftClus_CST_et;   //!
   TBranch        *b_MET_RefFinal_CST_SumET;   //!
   TBranch        *b_MET_RefFinal_CST_ety;   //!
   TBranch        *b_MET_RefFinal_CST_etx;   //!
   TBranch        *b_MET_Truth_Int_SumET;   //!
   TBranch        *b_MET_RefFinal_CST_et;   //!
   TBranch        *b_MET_Ref_Track_SumET;   //!
   TBranch        *b_MET_Truth_NonInt_ety;   //!
   TBranch        *b_MET_RefJet_TST_FJVT_SumET;   //!
   TBranch        *b_MET_Ref_Track_ety;   //!
   TBranch        *b_MET_RefJet_TST_FJVT_etx;   //!
   TBranch        *b_MET_RefFinal_TST_FJVT_SumET;   //!
   TBranch        *b_MET_RefFinal_TST_FJVT_ety;   //!
   TBranch        *b_MET_RefFinal_TST_FJVT_etx;   //!
   TBranch        *b_MET_RefJet_TST_LOOSE_SumET;   //!
   TBranch        *b_MET_RefJet_TST_LOOSE_ety;   //!
   TBranch        *b_MET_RefJet_TST_LOOSE_etx;   //!
   TBranch        *b_MET_RefJet_TST_LOOSE_et;   //!
   TBranch        *b_MET_Truth_IntMuons_SumET;   //!
   TBranch        *b_MET_Truth_IntMuons_et;   //!
   TBranch        *b_MET_Ref_Track_et;   //!
   TBranch        *b_MET_RefFinal_TST_LOOSE_SumET;   //!
   TBranch        *b_MET_RefFinal_TST_LOOSE_ety;   //!
   TBranch        *b_MET_RefFinal_TST_LOOSE_et;   //!
   TBranch        *b_MET_RefJet_TST_FJVT_et;   //!
   TBranch        *b_MET_PVSoftTrk_TST_ety;   //!
   TBranch        *b_MET_Muons_TST_et;   //!
   TBranch        *b_MET_RefJet_TST_ety;   //!
   TBranch        *b_PV2trk;   //!
   TBranch        *b_isSimulation;   //!
   TBranch        *b_MET_PVSoftTrk_TST_etx;   //!
   TBranch        *b_RunNumber;   //!
   TBranch        *b_MET_RefJet_TST_SumET;   //!
   TBranch        *b_passBadMuon;   //!
   TBranch        *b_pvIndex;   //!
   TBranch        *b_passCosmic;   //!
   TBranch        *b_totalWeightedEvents;   //!
   TBranch        *b_totalEvents;   //!
   TBranch        *b_MET_SoftClus_CST_SumET;   //!
   TBranch        *b_MET_RefJet_TST_FJVT_ety;   //!
   TBranch        *b_MET_RefFinal_TST_FJVT_et;   //!
   TBranch        *b_PV4trk;   //!
   TBranch        *b_MET_RefEle_TST_SumET;   //!
   TBranch        *b_tileError;   //!
   TBranch        *b_EventWeight;   //!
   TBranch        *b_lbn;   //!
   TBranch        *b_MET_Truth_Int_et;   //!
   TBranch        *b_MET_Muons_TST_SumET;   //!
   TBranch        *b_sctError;   //!
   TBranch        *b_passBadJet;   //!
   TBranch        *b_ChannelNumber;   //!
   TBranch        *b_EventNumber;   //!
   TBranch        *b_MET_SoftClus_CST_ety;   //!
   TBranch        *b_MET_PVSoftTrk_TST_et;   //!
   TBranch        *b_coreFlags;   //!
   TBranch        *b_vtx_z;   //!
   TBranch        *b_passed_elec_trigger;   //!
   TBranch        *b_PileUpWeight;   //!
   TBranch        *b_MET_Truth_NonInt_etx;   //!
   TBranch        *b_MET_Muons_TST_etx;   //!
   TBranch        *b_averageInteractionsPerCrossing;   //!
   TBranch        *b_actualInteractionsPerCrossing;   //!
   TBranch        *b_passed_muon_trigger;   //!
   TBranch        *b_CorrectedMuTrue;   //!
   TBranch        *b_MET_RefFinal_TST_etx;   //!
   TBranch        *b_CorrectedMuFalse;   //!
   TBranch        *b_MET_Truth_NonInt_et;   //!
   TBranch        *b_MET_PVSoftTrk_TST_SumET;   //!
   TBranch        *b_vtx_nTracks;   //!
   TBranch        *b_MET_RefFinal_TST_ety;   //!
   TBranch        *b_MET_RefFinal_TST_LOOSE_etx;   //!
   TBranch        *b_MET_RefJet_TST_etx;   //!
   TBranch        *b_MET_RefFinal_TST_et;   //!
   TBranch        *b_bcid;   //!
   TBranch        *b_total_muon_sf;   //!
   TBranch        *b_total_elec_sf;   //!
   TBranch        *b_trig_muon_sf;   //!
   TBranch        *b_trig_elec_sf;   //!
   TBranch        *b_MET_Truth_IntMuons_etx;   //!
   TBranch        *b_MET_RefJet_TST_et;   //!
   TBranch        *b_MET_Muons_TST_ety;   //!
   TBranch        *b_MET_RefFinal_TST_SumET;   //!
   TBranch        *b_MET_RefEle_TST_et;   //!
   TBranch        *b_MET_RefEle_TST_ety;   //!
   TBranch        *b_MET_Ref_Track_etx;   //!
   TBranch        *b_MET_RefEle_TST_etx;   //!
   TBranch        *b_MET_RefGamma_TST_et;   //!
   TBranch        *b_MET_RefGamma_TST_etx;   //!
   TBranch        *b_larError;   //!
   TBranch        *b_MET_RefGamma_TST_ety;   //!
   TBranch        *b_MET_RefGamma_TST_SumET;   //!
   TBranch        *b_mu_tlv_pt;   //!
   TBranch        *b_mu_tlv_eta;   //!
   TBranch        *b_mu_tlv_phi;   //!
   TBranch        *b_mu_tlv_m;   //!
   TBranch        *b_mu_Theta;   //!
   TBranch        *b_mu_z0sinTheta;   //!
   TBranch        *b_mu_trig_match;   //!
   TBranch        *b_mu_passOR;   //!
   TBranch        *b_mu_d0;   //!
   TBranch        *b_mu_signal;   //!
   TBranch        *b_mu_charge;   //!
   TBranch        *b_mu_d0sig;   //!
   TBranch        *b_mu_scalefact;   //!

   MyEvent(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~MyEvent() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(MyEvent,0);
};

#endif

#ifdef MyEvent_cxx
void MyEvent::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   trjet_tlv_pt = 0;
   trjet_tlv_eta = 0;
   trjet_tlv_phi = 0;
   trjet_tlv_m = 0;
   el_tlv_pt = 0;
   el_tlv_eta = 0;
   el_tlv_phi = 0;
   el_tlv_m = 0;
   el_charge = 0;
   el_signal = 0;
   el_passOR = 0;
   el_scalefact = 0;
   el_trig_match = 0;
   SUSYjet_tlv_pt = 0;
   SUSYjet_tlv_eta = 0;
   SUSYjet_tlv_phi = 0;
   SUSYjet_tlv_m = 0;
   SUSYjet_NumTrkPt500 = 0;
   SUSYjet_JvtRpt = 0;
   SUSYjet_JvtJvfcorr = 0;
   SUSYjet_bad = 0;
   SUSYjet_signal = 0;
   SUSYjet_baseline = 0;
   SUSYjet_bjet = 0;
   SUSYjet_passJvt = 0;
   SUSYjet_passFJvt = 0;
   SUSYjet_Jvt = 0;
   SUSYjet_FracSamplingMax = 0;
   SUSYjet_passOR = 0;
   SUSYjet_JVF = 0;
   mu_tlv_pt = 0;
   mu_tlv_eta = 0;
   mu_tlv_phi = 0;
   mu_tlv_m = 0;
   mu_Theta = 0;
   mu_z0sinTheta = 0;
   mu_trig_match = 0;
   mu_passOR = 0;
   mu_d0 = 0;
   mu_signal = 0;
   mu_charge = 0;
   mu_d0sig = 0;
   mu_scalefact = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   // Truth Level Stuff (Real data doesnt have these branches)
   if ( fChain->GetListOfBranches()->FindObject("trjet_n") ) {
     fChain->SetBranchAddress("trjet_n", &trjet_n, &b_trjet_n);
     fChain->SetBranchAddress("trjet_tlv_pt", &trjet_tlv_pt, &b_trjet_tlv_pt);
     fChain->SetBranchAddress("trjet_tlv_eta", &trjet_tlv_eta, &b_trjet_tlv_eta);
     fChain->SetBranchAddress("trjet_tlv_phi", &trjet_tlv_phi, &b_trjet_tlv_phi);
     fChain->SetBranchAddress("trjet_tlv_m", &trjet_tlv_m, &b_trjet_tlv_m);
     fChain->SetBranchAddress("MET_Truth_IntMuons_ety", &MET_Truth_IntMuons_ety, &b_MET_Truth_IntMuons_ety);
     fChain->SetBranchAddress("MET_Truth_Int_etx", &MET_Truth_Int_etx, &b_MET_Truth_Int_etx);
     fChain->SetBranchAddress("MET_Truth_NonInt_SumET", &MET_Truth_NonInt_SumET, &b_MET_Truth_NonInt_SumET);
     fChain->SetBranchAddress("MET_Truth_Int_ety", &MET_Truth_Int_ety, &b_MET_Truth_Int_ety);
     fChain->SetBranchAddress("MET_Truth_Int_SumET", &MET_Truth_Int_SumET, &b_MET_Truth_Int_SumET);
     fChain->SetBranchAddress("MET_Truth_NonInt_ety", &MET_Truth_NonInt_ety, &b_MET_Truth_NonInt_ety);
     fChain->SetBranchAddress("MET_Truth_IntMuons_SumET", &MET_Truth_IntMuons_SumET, &b_MET_Truth_IntMuons_SumET);
     fChain->SetBranchAddress("MET_Truth_IntMuons_et", &MET_Truth_IntMuons_et, &b_MET_Truth_IntMuons_et);
     fChain->SetBranchAddress("MET_Truth_Int_et", &MET_Truth_Int_et, &b_MET_Truth_Int_et);
     fChain->SetBranchAddress("MET_Truth_NonInt_etx", &MET_Truth_NonInt_etx, &b_MET_Truth_NonInt_etx);
     fChain->SetBranchAddress("MET_Truth_NonInt_et", &MET_Truth_NonInt_et, &b_MET_Truth_NonInt_et);
     fChain->SetBranchAddress("MET_Truth_IntMuons_etx", &MET_Truth_IntMuons_etx, &b_MET_Truth_IntMuons_etx);
   }


   if ( fChain->GetListOfBranches()->FindObject("mu_tlv_pt") ) {
     fChain->SetBranchAddress("mu_tlv_pt", &mu_tlv_pt, &b_mu_tlv_pt);
     fChain->SetBranchAddress("mu_tlv_eta", &mu_tlv_eta, &b_mu_tlv_eta);
     fChain->SetBranchAddress("mu_tlv_phi", &mu_tlv_phi, &b_mu_tlv_phi);
     fChain->SetBranchAddress("mu_tlv_m", &mu_tlv_m, &b_mu_tlv_m);
     fChain->SetBranchAddress("mu_Theta", &mu_Theta, &b_mu_Theta);
     fChain->SetBranchAddress("mu_z0sinTheta", &mu_z0sinTheta, &b_mu_z0sinTheta);
     fChain->SetBranchAddress("mu_trig_match", &mu_trig_match, &b_mu_trig_match);
     fChain->SetBranchAddress("mu_passOR", &mu_passOR, &b_mu_passOR);
     fChain->SetBranchAddress("mu_d0", &mu_d0, &b_mu_d0);
     fChain->SetBranchAddress("mu_signal", &mu_signal, &b_mu_signal);
     fChain->SetBranchAddress("mu_charge", &mu_charge, &b_mu_charge);
     fChain->SetBranchAddress("mu_d0sig", &mu_d0sig, &b_mu_d0sig);
     fChain->SetBranchAddress("mu_scalefact", &mu_scalefact, &b_mu_scalefact);
   }

   if ( fChain->GetListOfBranches()->FindObject("el_tlv_pt") ) {
     fChain->SetBranchAddress("el_tlv_pt", &el_tlv_pt, &b_el_tlv_pt);
     fChain->SetBranchAddress("el_tlv_eta", &el_tlv_eta, &b_el_tlv_eta);
     fChain->SetBranchAddress("el_tlv_phi", &el_tlv_phi, &b_el_tlv_phi);
     fChain->SetBranchAddress("el_tlv_m", &el_tlv_m, &b_el_tlv_m);
     fChain->SetBranchAddress("el_charge", &el_charge, &b_el_charge);
     fChain->SetBranchAddress("el_signal", &el_signal, &b_el_signal);
     fChain->SetBranchAddress("el_passOR", &el_passOR, &b_el_passOR);
     fChain->SetBranchAddress("el_scalefact", &el_scalefact, &b_el_scalefact);
     fChain->SetBranchAddress("el_trig_match", &el_trig_match, &b_el_trig_match);
   }

   fChain->SetBranchAddress("total_muon_sf", &total_muon_sf, &b_total_muon_sf);
   fChain->SetBranchAddress("total_elec_sf", &total_elec_sf, &b_total_elec_sf);
   if ( fChain->GetListOfBranches()->FindObject("trig_elec_sf") ) {
     fChain->SetBranchAddress("trig_elec_sf", &trig_elec_sf, &b_trig_elec_sf);
     fChain->SetBranchAddress("trig_muon_sf", &trig_muon_sf, &b_trig_muon_sf);
   }

   // The rest
   fChain->SetBranchAddress("mu_n", &mu_n, &b_mu_n);
   fChain->SetBranchAddress("el_n", &el_n, &b_el_n);
   fChain->SetBranchAddress("SUSYjet_n", &SUSYjet_n, &b_SUSYjet_n);
   fChain->SetBranchAddress("SUSYjet_tlv_pt", &SUSYjet_tlv_pt, &b_SUSYjet_tlv_pt);
   fChain->SetBranchAddress("SUSYjet_tlv_eta", &SUSYjet_tlv_eta, &b_SUSYjet_tlv_eta);
   fChain->SetBranchAddress("SUSYjet_tlv_phi", &SUSYjet_tlv_phi, &b_SUSYjet_tlv_phi);
   fChain->SetBranchAddress("SUSYjet_tlv_m", &SUSYjet_tlv_m, &b_SUSYjet_tlv_m);
   fChain->SetBranchAddress("SUSYjet_NumTrkPt500", &SUSYjet_NumTrkPt500, &b_SUSYjet_NumTrkPt500);
   fChain->SetBranchAddress("SUSYjet_JvtRpt", &SUSYjet_JvtRpt, &b_SUSYjet_JvtRpt);
   fChain->SetBranchAddress("SUSYjet_JvtJvfcorr", &SUSYjet_JvtJvfcorr, &b_SUSYjet_JvtJvfcorr);
   fChain->SetBranchAddress("SUSYjet_bad", &SUSYjet_bad, &b_SUSYjet_bad);
   fChain->SetBranchAddress("SUSYjet_signal", &SUSYjet_signal, &b_SUSYjet_signal);
   fChain->SetBranchAddress("SUSYjet_baseline", &SUSYjet_baseline, &b_SUSYjet_baseline);
   fChain->SetBranchAddress("SUSYjet_bjet", &SUSYjet_bjet, &b_SUSYjet_bjet);
   fChain->SetBranchAddress("SUSYjet_passJvt", &SUSYjet_passJvt, &b_SUSYjet_passJvt);
   fChain->SetBranchAddress("SUSYjet_passFJvt", &SUSYjet_passFJvt, &b_SUSYjet_passFJvt);
   fChain->SetBranchAddress("SUSYjet_Jvt", &SUSYjet_Jvt, &b_SUSYjet_Jvt);
   fChain->SetBranchAddress("SUSYjet_FracSamplingMax", &SUSYjet_FracSamplingMax, &b_SUSYjet_FracSamplingMax);
   fChain->SetBranchAddress("SUSYjet_passOR", &SUSYjet_passOR, &b_SUSYjet_passOR);
   fChain->SetBranchAddress("SUSYjet_JVF", &SUSYjet_JVF, &b_SUSYjet_JVF);
   fChain->SetBranchAddress("MET_Significance", &MET_Significance, &b_MET_Significance);
   fChain->SetBranchAddress("MET_SoftClus_CST_etx", &MET_SoftClus_CST_etx, &b_MET_SoftClus_CST_etx);
   fChain->SetBranchAddress("MET_SoftClus_CST_et", &MET_SoftClus_CST_et, &b_MET_SoftClus_CST_et);
   fChain->SetBranchAddress("MET_RefFinal_CST_SumET", &MET_RefFinal_CST_SumET, &b_MET_RefFinal_CST_SumET);
   fChain->SetBranchAddress("MET_RefFinal_CST_ety", &MET_RefFinal_CST_ety, &b_MET_RefFinal_CST_ety);
   fChain->SetBranchAddress("MET_RefFinal_CST_etx", &MET_RefFinal_CST_etx, &b_MET_RefFinal_CST_etx);
   fChain->SetBranchAddress("MET_RefFinal_CST_et", &MET_RefFinal_CST_et, &b_MET_RefFinal_CST_et);
   fChain->SetBranchAddress("MET_Ref_Track_SumET", &MET_Ref_Track_SumET, &b_MET_Ref_Track_SumET);
   fChain->SetBranchAddress("MET_RefJet_TST_FJVT_SumET", &MET_RefJet_TST_FJVT_SumET, &b_MET_RefJet_TST_FJVT_SumET);
   fChain->SetBranchAddress("MET_Ref_Track_ety", &MET_Ref_Track_ety, &b_MET_Ref_Track_ety);
   fChain->SetBranchAddress("MET_RefJet_TST_FJVT_etx", &MET_RefJet_TST_FJVT_etx, &b_MET_RefJet_TST_FJVT_etx);
   fChain->SetBranchAddress("MET_RefFinal_TST_FJVT_SumET", &MET_RefFinal_TST_FJVT_SumET, &b_MET_RefFinal_TST_FJVT_SumET);
   fChain->SetBranchAddress("MET_RefFinal_TST_FJVT_ety", &MET_RefFinal_TST_FJVT_ety, &b_MET_RefFinal_TST_FJVT_ety);
   fChain->SetBranchAddress("MET_RefFinal_TST_FJVT_etx", &MET_RefFinal_TST_FJVT_etx, &b_MET_RefFinal_TST_FJVT_etx);
   fChain->SetBranchAddress("MET_RefJet_TST_LOOSE_SumET", &MET_RefJet_TST_LOOSE_SumET, &b_MET_RefJet_TST_LOOSE_SumET);
   fChain->SetBranchAddress("MET_RefJet_TST_LOOSE_ety", &MET_RefJet_TST_LOOSE_ety, &b_MET_RefJet_TST_LOOSE_ety);
   fChain->SetBranchAddress("MET_RefJet_TST_LOOSE_etx", &MET_RefJet_TST_LOOSE_etx, &b_MET_RefJet_TST_LOOSE_etx);
   fChain->SetBranchAddress("MET_RefJet_TST_LOOSE_et", &MET_RefJet_TST_LOOSE_et, &b_MET_RefJet_TST_LOOSE_et);
   fChain->SetBranchAddress("MET_Ref_Track_et", &MET_Ref_Track_et, &b_MET_Ref_Track_et);
   fChain->SetBranchAddress("MET_RefFinal_TST_LOOSE_SumET", &MET_RefFinal_TST_LOOSE_SumET, &b_MET_RefFinal_TST_LOOSE_SumET);
   fChain->SetBranchAddress("MET_RefFinal_TST_LOOSE_ety", &MET_RefFinal_TST_LOOSE_ety, &b_MET_RefFinal_TST_LOOSE_ety);
   fChain->SetBranchAddress("MET_RefFinal_TST_LOOSE_et", &MET_RefFinal_TST_LOOSE_et, &b_MET_RefFinal_TST_LOOSE_et);
   fChain->SetBranchAddress("MET_RefJet_TST_FJVT_et", &MET_RefJet_TST_FJVT_et, &b_MET_RefJet_TST_FJVT_et);
   fChain->SetBranchAddress("MET_PVSoftTrk_TST_ety", &MET_PVSoftTrk_TST_ety, &b_MET_PVSoftTrk_TST_ety);
   fChain->SetBranchAddress("MET_Muons_TST_et", &MET_Muons_TST_et, &b_MET_Muons_TST_et);
   fChain->SetBranchAddress("MET_RefJet_TST_ety", &MET_RefJet_TST_ety, &b_MET_RefJet_TST_ety);
   fChain->SetBranchAddress("PV2trk", &PV2trk, &b_PV2trk);
   fChain->SetBranchAddress("isSimulation", &isSimulation, &b_isSimulation);
   fChain->SetBranchAddress("MET_PVSoftTrk_TST_etx", &MET_PVSoftTrk_TST_etx, &b_MET_PVSoftTrk_TST_etx);
   fChain->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber);
   fChain->SetBranchAddress("MET_RefJet_TST_SumET", &MET_RefJet_TST_SumET, &b_MET_RefJet_TST_SumET);
   fChain->SetBranchAddress("passBadMuon", &passBadMuon, &b_passBadMuon);
   fChain->SetBranchAddress("pvIndex", &pvIndex, &b_pvIndex);
   fChain->SetBranchAddress("passCosmic", &passCosmic, &b_passCosmic);
   fChain->SetBranchAddress("totalWeightedEvents", &totalWeightedEvents, &b_totalWeightedEvents);
   fChain->SetBranchAddress("totalEvents", &totalEvents, &b_totalEvents);
   fChain->SetBranchAddress("MET_SoftClus_CST_SumET", &MET_SoftClus_CST_SumET, &b_MET_SoftClus_CST_SumET);
   fChain->SetBranchAddress("MET_RefJet_TST_FJVT_ety", &MET_RefJet_TST_FJVT_ety, &b_MET_RefJet_TST_FJVT_ety);
   fChain->SetBranchAddress("MET_RefFinal_TST_FJVT_et", &MET_RefFinal_TST_FJVT_et, &b_MET_RefFinal_TST_FJVT_et);
   fChain->SetBranchAddress("PV4trk", &PV4trk, &b_PV4trk);
   fChain->SetBranchAddress("MET_RefEle_TST_SumET", &MET_RefEle_TST_SumET, &b_MET_RefEle_TST_SumET);
   fChain->SetBranchAddress("tileError", &tileError, &b_tileError);
   fChain->SetBranchAddress("EventWeight", &EventWeight, &b_EventWeight);
   fChain->SetBranchAddress("lbn", &lbn, &b_lbn);
   fChain->SetBranchAddress("MET_Muons_TST_SumET", &MET_Muons_TST_SumET, &b_MET_Muons_TST_SumET);
   fChain->SetBranchAddress("sctError", &sctError, &b_sctError);
   fChain->SetBranchAddress("passBadJet", &passBadJet, &b_passBadJet);
   fChain->SetBranchAddress("ChannelNumber", &ChannelNumber, &b_ChannelNumber);
   fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
   fChain->SetBranchAddress("MET_SoftClus_CST_ety", &MET_SoftClus_CST_ety, &b_MET_SoftClus_CST_ety);
   fChain->SetBranchAddress("MET_PVSoftTrk_TST_et", &MET_PVSoftTrk_TST_et, &b_MET_PVSoftTrk_TST_et);
   fChain->SetBranchAddress("coreFlags", &coreFlags, &b_coreFlags);
   fChain->SetBranchAddress("vtx_z", &vtx_z, &b_vtx_z);
   fChain->SetBranchAddress("passed_elec_trigger", &passed_elec_trigger, &b_passed_elec_trigger);
   fChain->SetBranchAddress("PileUpWeight", &PileUpWeight, &b_PileUpWeight);
   fChain->SetBranchAddress("MET_Muons_TST_etx", &MET_Muons_TST_etx, &b_MET_Muons_TST_etx);
   fChain->SetBranchAddress("averageInteractionsPerCrossing", &averageInteractionsPerCrossing, &b_averageInteractionsPerCrossing);
   fChain->SetBranchAddress("actualInteractionsPerCrossing", &actualInteractionsPerCrossing, &b_actualInteractionsPerCrossing);
   fChain->SetBranchAddress("passed_muon_trigger", &passed_muon_trigger, &b_passed_muon_trigger);
   fChain->SetBranchAddress("CorrectedMuTrue", &CorrectedMuTrue, &b_CorrectedMuTrue);
   fChain->SetBranchAddress("MET_RefFinal_TST_etx", &MET_RefFinal_TST_etx, &b_MET_RefFinal_TST_etx);
   fChain->SetBranchAddress("CorrectedMuFalse", &CorrectedMuFalse, &b_CorrectedMuFalse);
   fChain->SetBranchAddress("MET_PVSoftTrk_TST_SumET", &MET_PVSoftTrk_TST_SumET, &b_MET_PVSoftTrk_TST_SumET);
   fChain->SetBranchAddress("vtx_nTracks", &vtx_nTracks, &b_vtx_nTracks);
   fChain->SetBranchAddress("MET_RefFinal_TST_ety", &MET_RefFinal_TST_ety, &b_MET_RefFinal_TST_ety);
   fChain->SetBranchAddress("MET_RefFinal_TST_LOOSE_etx", &MET_RefFinal_TST_LOOSE_etx, &b_MET_RefFinal_TST_LOOSE_etx);
   fChain->SetBranchAddress("MET_RefJet_TST_etx", &MET_RefJet_TST_etx, &b_MET_RefJet_TST_etx);
   fChain->SetBranchAddress("MET_RefFinal_TST_et", &MET_RefFinal_TST_et, &b_MET_RefFinal_TST_et);
   fChain->SetBranchAddress("bcid", &bcid, &b_bcid);
   fChain->SetBranchAddress("MET_RefJet_TST_et", &MET_RefJet_TST_et, &b_MET_RefJet_TST_et);
   fChain->SetBranchAddress("MET_Muons_TST_ety", &MET_Muons_TST_ety, &b_MET_Muons_TST_ety);
   fChain->SetBranchAddress("MET_RefFinal_TST_SumET", &MET_RefFinal_TST_SumET, &b_MET_RefFinal_TST_SumET);
   fChain->SetBranchAddress("MET_RefEle_TST_et", &MET_RefEle_TST_et, &b_MET_RefEle_TST_et);
   fChain->SetBranchAddress("MET_RefEle_TST_ety", &MET_RefEle_TST_ety, &b_MET_RefEle_TST_ety);
   fChain->SetBranchAddress("MET_Ref_Track_etx", &MET_Ref_Track_etx, &b_MET_Ref_Track_etx);
   fChain->SetBranchAddress("MET_RefEle_TST_etx", &MET_RefEle_TST_etx, &b_MET_RefEle_TST_etx);
   fChain->SetBranchAddress("MET_RefGamma_TST_et", &MET_RefGamma_TST_et, &b_MET_RefGamma_TST_et);
   fChain->SetBranchAddress("MET_RefGamma_TST_etx", &MET_RefGamma_TST_etx, &b_MET_RefGamma_TST_etx);
   fChain->SetBranchAddress("larError", &larError, &b_larError);
   fChain->SetBranchAddress("MET_RefGamma_TST_ety", &MET_RefGamma_TST_ety, &b_MET_RefGamma_TST_ety);
   fChain->SetBranchAddress("MET_RefGamma_TST_SumET", &MET_RefGamma_TST_SumET, &b_MET_RefGamma_TST_SumET);


   Notify();
}

Bool_t MyEvent::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef MyEvent_cxx
