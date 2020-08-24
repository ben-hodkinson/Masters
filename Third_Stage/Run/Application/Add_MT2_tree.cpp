#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <algorithm>

#include <TStyle.h>
#include <TROOT.h>
#include <TTree.h>
#include <TH1.h>
#include <THStack.h>
#include <TFile.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TColor.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TLine.h>
#include <TLatex.h>

#include "lester_mt2_bisect.h"

using namespace std;



float get_mass( int pdgID ) {

  if ( pdgID == 11 ) return 0.5109989461 / 1000;

  if ( pdgID == 13 ) return 105.6583745 / 1000;

  return -1.0;

}


int main() {

  ////////////// User input //////////////

  string wp_name   = "Truth";         // Tight, Network
  string wp_tree   = "wpt_tree";      // wpt_tree, main_ann
  string wp_branch = "WP_Truth";  // WP_TST_Tight, WP_ANN

  double wp_ex; // double, float
  double wp_ey; // double, float

  vector<string>  File_List = {};
  File_List.push_back( "SR_SUSY/SZee_all_susy_2l_0jets.root"     );
  File_List.push_back( "SR_SUSY/SZmumu_all_susy_2l_0jets.root"   );
  File_List.push_back( "SR_SUSY/Ztt_all_susy_2l_0jets.root"      );
  File_List.push_back( "SR_SUSY/ZZ_1in3_susy_2l_0jets.root"      );
  File_List.push_back( "SR_SUSY/WZ_1in3_susy_2l_0jets.root"      );
  File_List.push_back( "SR_SUSY/WW_1in3_susy_2l_0jets.root"      );
  File_List.push_back( "SR_SUSY/top_1in3_susy_2l_0jets.root"     );
  File_List.push_back( "SR_SUSY/antitop_1in3_susy_2l_0jets.root" );
  File_List.push_back( "SR_SUSY/ttbar_1in3_susy_2l_0jets.root"   );

  // File_List.push_back( "SR_SUSY/Sllvv_all_susy_2l_0jets.root" );

  File_List.push_back( "SR_SUSY/SL400X250_all_susy_2l_0jets.root" );
  File_List.push_back( "SR_SUSY/SL550X001_all_susy_2l_0jets.root" );
  File_List.push_back( "SR_SUSY/SL300X200_all_susy_2l_0jets.root" );

  // File_List.push_back( "Real/Real_all_susy_2l_0jets.root" );


  ////////////// End of user input //////////////

  // We create the new tree name
  string tree_name = "mt2_" + wp_name;

  // The needed variables we will use for event selection
  // We dont load mass from the TTree as the PGD masses are more accurate
  double lead_l_mass;
  double subl_l_mass;
  double mt2;

  // The variables we will load from the trees
  double lead_l_px;
  double lead_l_py;
  int    lead_l_pdgID;
  double subl_l_px;
  double subl_l_py;
  int    subl_l_pdgID;
  float  ll_inv_mass;



  // Looping through each dataset
  for ( auto& dataset: File_List ) {

    cout << "\n" << dataset << "\n";

    // Opening the the file and trees
    string full_name = getenv("HOME_DIRECTORY") + string("/Data/") + dataset;
    TFile* file      = new TFile(full_name.c_str(), "update");
    TTree* alt_tree  = (TTree*)file->Get("alt_tree");
    TTree* wpt_tree  = (TTree*)file->Get(wp_tree.c_str());

    // Only enabling a few branches to increase speed
    alt_tree->SetBranchStatus( "*", 0 );
    alt_tree->SetBranchStatus( "lead_l_px",      1 );
    alt_tree->SetBranchStatus( "lead_l_py",      1 );
    alt_tree->SetBranchStatus( "lead_l_pdgID",   1 );
    alt_tree->SetBranchStatus( "subl_l_px",      1 );
    alt_tree->SetBranchStatus( "subl_l_py",      1 );
    alt_tree->SetBranchStatus( "subl_l_pdgID",   1 );
    alt_tree->SetBranchStatus( "ll_inv_mass",    1 );

    wpt_tree->SetBranchStatus( "*", 0 );
    wpt_tree->SetBranchStatus( ( string(wp_branch) + "_X" ).c_str(),  1 );
    wpt_tree->SetBranchStatus( ( string(wp_branch) + "_Y" ).c_str(),  1 );

    // Setting the adresses for the variables
    alt_tree->SetBranchAddress( "lead_l_px",      &lead_l_px );
    alt_tree->SetBranchAddress( "lead_l_py",      &lead_l_py );
    alt_tree->SetBranchAddress( "lead_l_pdgID",   &lead_l_pdgID );
    alt_tree->SetBranchAddress( "subl_l_px",      &subl_l_px );
    alt_tree->SetBranchAddress( "subl_l_py",      &subl_l_py );
    alt_tree->SetBranchAddress( "subl_l_pdgID",   &subl_l_pdgID );
    alt_tree->SetBranchAddress( "ll_inv_mass",    &ll_inv_mass );

    wpt_tree->SetBranchAddress( ( string(wp_branch) + "_X" ).c_str(),  &wp_ex );
    wpt_tree->SetBranchAddress( ( string(wp_branch) + "_Y" ).c_str(),  &wp_ey );



    // We check if the ttree already exists and declare a new one
    if ( file->GetListOfKeys()->Contains( tree_name.c_str() ) ) {
        cout << "Found existing TTree named: " << tree_name << '\n';
        cout << "Deleting this TTree and replacing it" << '\n';
        file->Delete( (tree_name + ";0").c_str() );
        file->Delete( (tree_name + ";1").c_str() );
        file->Delete( (tree_name + ";2").c_str() );
    }



    TTree* new_tree = new TTree( tree_name.c_str(), tree_name.c_str() );

    // We add a new branch to this tree
    TBranch *b_mt2_tight   = new_tree->Branch("mt2", &mt2, "mt2/d");

    // Now we begin the event loop
    float fraclast   = 0.0;
    unsigned percent = 0;
    unsigned evtmax  = alt_tree->GetEntries();

    for (unsigned i = 0; i < evtmax; i++) {

      // if ( i > 1000 ) break;

      // We load the values from each tree
      alt_tree->GetEntry( i );
      wpt_tree->GetEntry( i );

      // We used the IDs to get the masses and flavour
      lead_l_mass = get_mass( lead_l_pdgID );
      subl_l_mass = get_mass( subl_l_pdgID );

      // Calculate the stransverse mass
      mt2 = asymm_mt2_lester_bisect::get_mT2( lead_l_mass, lead_l_px, lead_l_py, subl_l_mass, subl_l_px, subl_l_py, wp_ex, wp_ey, 0.0, 0.0, 0.0 );

      // Fill the ttree
      new_tree->Fill();

      // Printing out progress
      percent = int(100*i/evtmax);
      if ( percent > fraclast ) {
        cout << " -- " <<  i << "/" << evtmax << " " << percent << "%                  \r";
        cout.flush();
        fraclast = percent;
      }

    }

    cout << " -- " <<  evtmax << "/" << evtmax << " " << 100 << "%                  \n";
    cout.flush();

    gFile = file;
    new_tree->Write();
    file->Close();
    delete file;

  }


  return 0;

}
