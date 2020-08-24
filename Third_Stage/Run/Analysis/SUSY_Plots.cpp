#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <stdio.h>

#include <TStyle.h>
#include <TSystem.h>
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

using namespace std;

float FixError( float integral, float current_error ) {
  float ratio = 0;
  float error = 0;
  if ( integral > 0 ) {
    ratio = current_error / integral;
    error = integral * sqrt( 0.02*0.02 + ratio*ratio );
  }
  return error;
}



void Print_Totals( string outfile_name, string method, THStack* MC_Stack, THStack* Signal_Stack, THStack* Real_Stack, TH1D* MC_Total ) {
  FILE*  intFile;
  double integral = 0;
  double error    = 0;
  int    n_bins   = MC_Total->GetNbinsX()+1;


  // We open the file
  intFile = fopen( (outfile_name+".txt").c_str(), "w");

  // We begin iterating through the monte-carlo stacks
  for ( auto&& hist: *MC_Stack->GetHists() ) {
    integral = ((TH1*)hist)->IntegralAndError( 0, n_bins, error);
    error = FixError( integral, error );
    fprintf(intFile, "%-25s: %-6.2f +- %6.2f \n", ((TH1*)hist)->GetName(), integral, error );
  }
  fprintf(intFile, "__________________________________________________\n" );

  // Then print the totals
  integral = MC_Total->IntegralAndError( 0, n_bins, error);
  error = FixError( integral, error );
  fprintf(intFile, "%-25s: %-6.2f +- %6.2f \n", "Total MC", integral, error );
  fprintf(intFile, "\n");

  // Then we print the observed
  for ( auto&& hist: *Real_Stack->GetHists() ) {
    integral = ((TH1*)hist)->IntegralAndError( 0, n_bins, error);
    error = FixError( integral, error );
    fprintf(intFile, "%-25s: %-6.2f +- %6.2f \n", "Observed", integral, error );
  }
  fprintf(intFile, "\n" );

  // Then we print the SUSY samples
  for ( auto&& hist: *Signal_Stack->GetHists() ) {
    integral = ((TH1*)hist)->IntegralAndError( 0, n_bins, error);
    error = FixError( integral, error );
    fprintf(intFile, "%-25s: %-6.2f +- %6.2f \n", ((TH1*)hist)->GetName(), integral, error );
  }

  fclose(intFile);
}



void Single_Bin_Output( FILE* intFile, TH1D* hist, string label = "" ) {

  int n_bins = hist->GetNbinsX()+1;
  double integral = 0;
  double error    = 0;

  if ( label == "" ) {
    label = hist->GetName();
  }

  fprintf(intFile, "%-25s:", label.c_str());

  // Iterate over each bin
  for (unsigned i = 1; i < n_bins; i++) {
    integral = hist->GetBinContent(i);
    error    = hist->GetBinError(i);
    error    = FixError( integral, error );
    fprintf(intFile, "%-6.2f+-%6.2f  |  ", integral, error );
  }

  integral = hist->IntegralAndError( 0, n_bins, error);
  error    = FixError( integral, error );
  fprintf(intFile, "| %6.2f+-%-6.2f \n", integral, error );

}



void Signal_Background( FILE* intFile, TH1D* sig_hist, TH1D* back_hist ) {

  int n_bins = sig_hist->GetNbinsX()+1;
  double signal  = 0;
  double sig_err = 0;
  double back    = 0;
  double backerr = 0;
  double sob     = 0;
  double sob_err = 0;

  fprintf(intFile, "%-25s:", "S/rB");

  // Iterate over each bin
  for (unsigned i = 1; i < n_bins; i++) {
    signal  = sig_hist->GetBinContent(i);
    sig_err = sig_hist->GetBinError(i);
    sig_err = FixError( signal, sig_err );

    back    = back_hist->GetBinContent(i);
    backerr = back_hist->GetBinError(i);
    backerr = FixError( back, backerr );

    sob     = signal / sqrt( back );
    sob_err = 0;
    if ( signal>0 && back>0 ) sob_err = sob * sqrt( (sig_err/signal)*(sig_err/signal) + (backerr/back)*(backerr/back)/4 );
    fprintf(intFile, "%-6.2f+-%6.2f  |  ", sob, sob_err );
    }

  // Then do the total
  signal  = sig_hist->IntegralAndError( 0, n_bins, sig_err);
  sig_err = FixError( signal, sig_err );

  back    = back_hist->IntegralAndError( 0, n_bins, backerr);
  backerr = FixError( back, backerr );

  sob     = signal / sqrt( back );
  sob_err = 0;
  if ( signal>0 && back>0 ) sob_err = sob * sqrt( (sig_err/signal)*(sig_err/signal) + (backerr/back)*(backerr/back)/4 );
  fprintf(intFile, "| %6.2f+-%-6.2f \n", sob, sob_err );

}



void Print_Each_Bin( string outfile_name, string method, THStack* MC_Stack, THStack* Signal_Stack, THStack* Real_Stack, TH1D* MC_Total ) {
  FILE*  intFile;
  string label;
  int n_bins = MC_Total->GetNbinsX()+1;


  // We open the file
  intFile = fopen( (outfile_name+".txt").c_str(), "w");

  // We cycle through the signal regions saving the label of each one
  fprintf(intFile, "%15s", "");
  for (unsigned i = 1; i < n_bins; i++) {
    label = string("SF-") + char(i + 96);
    fprintf(intFile, "%19s", label.c_str() );
  }
  fprintf(intFile, "        |  |   Total\n" );

  // We begin iterating through the monte-carlo stacks
  for ( auto&& hist: *MC_Stack->GetHists() ) {
    Single_Bin_Output( intFile, (TH1D*)hist );
  }
  fprintf(intFile, "\n" );

  // Then print the totals
  Single_Bin_Output( intFile, MC_Total, "Total MC" );

  // Then we print the observed
  for ( auto&& hist: *Real_Stack->GetHists() ) {
    Single_Bin_Output( intFile, (TH1D*)hist, "Observed" );
  }
  fprintf(intFile, "\n" );

  // Then we print the SUSY samples
  for ( auto&& hist: *Signal_Stack->GetHists() ) {
    Single_Bin_Output( intFile, (TH1D*)hist );
    Signal_Background( intFile, (TH1D*)hist, MC_Total );
    fprintf(intFile, "\n" );
  }
  fprintf(intFile, "\n" );



  fclose(intFile);
}




float get_bin_number( float invm, float mt2 ) {

  float idx = 0;

  if      ( 111 < invm && invm < 150  ) idx = 0.0;
  else if ( 150 < invm && invm < 200  ) idx = 1.0;
  else if ( 200 < invm && invm < 300  ) idx = 2.0;
  else if ( 300 < invm ) idx = 3.0;

  if      ( 100 < mt2 && mt2 < 150 ) idx += 0.0;
  else if ( 150 < mt2 && mt2 < 200 ) idx += 4.0;
  else if ( 200 < mt2 && mt2 < 300 ) idx += 8.0;
  else if ( 300 < mt2 ) idx = 12.0;

  return idx;


  cout << "Warning: Something seriously wrong! Cant find correct bin!" << '\n';
  return 0;

}



class Plotting_Variable {
  public:

    TH1D* MC_Total;
    TH1D* MC_Uncer;
    TH1D* Current_Hist;
    TH1D* Current_Uncer;
    THStack* MC_Stack;
    THStack* Real_Stack;
    THStack* Signal_Stack;

    string method;
    float bins;
    float min;
    float max;


    // Constuctor, initialises the variable canvas, stacks and histograms
    Plotting_Variable( const char *new_method, int new_bins, float new_min, float new_max ) {
      method = new_method;
      bins   = new_bins;
      min    = new_min;
      max    = new_max;
    }



    // Initialising the stacks and histograms, called for a new region
    void Region_Initialise( int region ) {
      MC_Stack     = new THStack( (to_string(region)+":"+method+":m_stack").c_str(), "" );
      Real_Stack   = new THStack( (to_string(region)+":"+method+":r_stack").c_str(), "" );
      Signal_Stack = new THStack( (to_string(region)+":"+method+":s_stack").c_str(), "" );
      MC_Total = new TH1D( (to_string(region)+":"+method+":total_m").c_str(), "", bins, min, max );
      MC_Uncer = new TH1D( (to_string(region)+":"+method+":total:unc").c_str(), "", bins, min, max );
      MC_Uncer->SetMarkerSize(0.);
      MC_Uncer->SetFillStyle(3256);
      MC_Uncer->SetFillColor(1);
      MC_Uncer->SetMarkerColor(1);
    }



    void Region_Descruct() {
      delete MC_Total;
      delete MC_Uncer;
      delete Current_Hist;
      delete Current_Uncer;
      delete MC_Stack;
      delete Real_Stack;
      delete Signal_Stack;
    }



    // Initialising the hist which will being filled for the current dataset
    void Current_Initialise( int cur_region, string data_set_name ){
      Current_Hist = (TH1D*)MC_Total->Clone();
      Current_Hist->SetName( (to_string(cur_region)+":"+method+":"+data_set_name).c_str() );
      Current_Hist->SetDirectory(0);
      Current_Hist->Reset();

      Current_Uncer = (TH1D*)Current_Hist->Clone();
      Current_Uncer->SetName( (to_string(cur_region)+":"+method+":"+data_set_name+":unc").c_str() );
      Current_Uncer->SetDirectory(0);
      Current_Uncer->Reset();
    }



    // Finding the x-value to fill the histogram depending on the bin
    float Apply_Method( float mt2, float wp_et, float ll_inv_mass ) {
      if ( method == "SRs"    ) return get_bin_number( ll_inv_mass, mt2 ) + 0.5;
      if ( method == "etmiss" ) return wp_et;
      if ( method == "mll"    ) return ll_inv_mass;
      if ( method == "mt2"    ) return mt2;
      return -1;
    }



    // Adding the control SF uncertainty to the current histogram
    void FixCurrentUncertainties( double sfunc ) {
      float height;
      float uncer;
      float ratio;
      for ( size_t i = 0; i < Current_Uncer->GetNbinsX()+2; i++ ) {
        height = Current_Uncer->GetBinContent(i);
        uncer  = Current_Uncer->GetBinError(i);
        if ( height > 0 ) {
          ratio  = uncer/height;
          Current_Uncer->SetBinError( i, height * sqrt( ratio*ratio + sfunc*sfunc ) );
        }
      }
    }



    // Adding the luminosity uncertainty to the error graph
    void FixTotalUncertainties() {
      float height;
      float uncer;
      float ratio;
      for ( size_t i = 0; i < MC_Uncer->GetNbinsX()+2; i++ ) {
        height = MC_Uncer->GetBinContent(i);
        uncer  = MC_Uncer->GetBinError(i);
        if ( height > 0 ) {
          ratio  = uncer/height;
          MC_Uncer->SetBinError( i, height * sqrt( ratio*ratio + 0.02*0.02 ) );
        }
      }
    }



    // Changing the current hist style and adding it to the correct stack
    void Add_To_Stacks( int type, int colour ) {

      if (type==0) {
        Current_Hist->SetMarkerColor(colour);
        Current_Hist->SetFillColor(colour);
        Current_Hist->SetMarkerStyle(0);
        Current_Hist->SetLineWidth(0);

        MC_Total->Add( Current_Hist );
        MC_Stack->Add( Current_Hist );
        MC_Uncer->Add( Current_Uncer );
      }

      if (type==1) {
        Current_Hist->SetMarkerColor(colour);
        Current_Hist->SetLineColor(colour);
        Current_Hist->SetMarkerStyle(0);
        Current_Hist->SetLineWidth(4);
        Current_Hist->SetLineStyle(7);

        Signal_Stack->Add( Current_Hist );
      }

      if (type==2) {
        Current_Hist->SetLineColor(1);
        Current_Hist->SetMarkerColor(1);
        Current_Hist->SetMarkerStyle(8);
        Current_Hist->SetMarkerSize(1.2);

        Real_Stack->Add( Current_Hist );
      }

    }


    // This function outputs the total integrals
    void Total_IandU( string outfile_name ) {
      // We print out the individual bin heights for the signal region plots
      if ( method=="SRs" ) Print_Each_Bin( outfile_name, method, MC_Stack, Signal_Stack, Real_Stack, MC_Total );
      else Print_Totals( outfile_name, method, MC_Stack, Signal_Stack, Real_Stack, MC_Total );
    }



    // Drawing the histograms and saving to file
    void Draw( TLegend* leg, TLegend* sr_leg, float luminosity, string wp_name, int region, bool do_weight, bool sherpa_VV ) {
      TCanvas* canvas = new TCanvas( string(method+ "canvas").c_str(), "", 800, 800 );
      canvas->cd();

      // Creating the histopad
      TPad* histo_pad = new TPad( "main_pad", "", 0, 0.3, 1.0, 1.0 );
      histo_pad->SetBottomMargin(0);
      histo_pad->Draw();
      histo_pad->cd();
      histo_pad->SetLogy();



      // Setting the axis limits
      if ( region == 0 ) MC_Stack->SetMaximum( 1e11 );
      if ( region == 1 ) MC_Stack->SetMaximum( 1e11 );
      if ( region == 2 ) MC_Stack->SetMaximum( 2e4 );
      if ( region == 3 ) MC_Stack->SetMaximum( 5e4 );
      if ( region == 4 ) MC_Stack->SetMaximum( 1e4 );
      if ( region == 5 ) MC_Stack->SetMaximum( 1e4 );
      if ( region == 6 ) MC_Stack->SetMaximum( 1e4 );

      MC_Stack->SetMinimum( 1E-1 );



      // Drawing the stacks and histograms
      MC_Stack->Draw( "HIST" );
      MC_Uncer->Draw( "E2 same" );
      MC_Total->Draw( "HIST same" );
      Signal_Stack->Draw( "same NOSTACK HIST" );
      Real_Stack->Draw( "same e1pX0" );


      // Drawing the legends
      sr_leg->Draw();
      leg->Draw();


      // Changing the y-axis label
      MC_Stack->GetYaxis()->SetTitle( "Events" );


      // Setting the label of the region
      string region_label = "";
      if ( region == 0 ) region_label = "No m_{T2}, m_{ll} or b-jet cut";
      if ( region == 1 ) region_label = "CR-Z";
      if ( region == 2 ) region_label = "CR-VV";
      if ( region == 3 ) region_label = "CR-top";
      if ( region == 4 ) region_label = "VR-VV";
      if ( region == 5 ) region_label = "VR-top";
      if ( region == 6 ) region_label = "2L 0J SF";


      float text_size = 0.045;

      // Adding the atlas label
      TLatex* altas = new TLatex();
      altas->SetTextSize(text_size);
      altas->SetNDC();
      altas->SetTextColor(1);
      altas->DrawLatex(0.195, 0.88, "#bf{#it{ATLAS}} work in progress");

      // Adding the luminosity flag
      TLatex* lumi = new TLatex();
      lumi->SetTextSize(text_size);
      lumi->SetNDC();
      lumi->SetTextColor(1);
      char flag [50];
      sprintf( flag, "#sqrt{s} = 13 TeV, %.2f fb^{-1}", luminosity / 1000 );
      lumi->DrawLatex( 0.195, 0.82, flag );

      // Adding the MET flag
      TLatex* met = new TLatex();
      met->SetTextSize(text_size);
      met->SetNDC();
      met->SetTextColor(1);
      met->DrawLatex( 0.195, 0.76, ( wp_name + " E_{T}^{miss}" ).c_str() );

      // Adding the signal region labels
      TLatex* l1 = new TLatex();
      l1->SetTextSize(text_size);
      l1->SetNDC();
      l1->DrawLatex( 0.195, 0.70, region_label.c_str() );



      // Creating the ratio pad
      canvas->cd();
      TPad* ratio_pad = new TPad( "ratio_pad", "", 0, 0.05, 1.0, 0.3 );
      ratio_pad->SetTopMargin(0.0);
      ratio_pad->SetBottomMargin(0.3);
      ratio_pad->Draw();
      ratio_pad->cd();


      // Creating the hist to be the denominator which does not have uncertainties
      TH1D* divider_hist = (TH1D*)MC_Total->Clone();
      for (int i = 0; i < divider_hist->GetNbinsX()+1; ++i) divider_hist->SetBinError(i, 0);

      TH1D* MC_flat = (TH1D*)MC_Uncer->Clone();
      MC_flat->Divide(divider_hist);
      MC_flat->SetMaximum(2.0);
      MC_flat->SetMinimum(0.0);
      MC_flat->Draw( "E2" );

      // Plotting the ratio of MC to Data
      TH1D* ratio_hist   = (TH1D*)Real_Stack->GetStack()->Last()->Clone();
      ratio_hist->Divide( divider_hist );
      ratio_hist->Draw("ep SAME");


      // Adding in a line on the x-axis
      TLine* line = new TLine( ratio_hist->GetBinLowEdge(1) , 1, ratio_hist->GetBinLowEdge(ratio_hist->GetNbinsX()+1), 1);
      line->SetLineWidth(1);
      line->SetLineColor(1);
      line->SetLineStyle(9);
      line->Draw();


      // Changing the labels
      if ( method == "etmiss" )  MC_flat->GetXaxis()->SetTitle( ( wp_name + " #it{E}_{T}^{miss} [GeV]" ).c_str() );
      if ( method == "mll" )     MC_flat->GetXaxis()->SetTitle( "m_{ll} [GeV]" );
      if ( method == "mt2" )     MC_flat->GetXaxis()->SetTitle( "m_{T2} [GeV]" );


      // The "SRs" plot gets its own special tick labels
      if ( method == "SRs" ) {
        TAxis* xax = MC_flat->GetXaxis();
        string label;
        for (unsigned i = 1; i < xax->GetXmax()+1; i++) {
          label = string("SF-") + char(i + 96);
          xax->SetBinLabel( i, label.c_str() );
        }
        xax->LabelsOption("v");
      }


      // Moving the titles around
      MC_flat->GetXaxis()->SetTitleSize(25);
      MC_flat->GetXaxis()->SetTitleFont(43);
      MC_flat->GetXaxis()->SetTitleOffset(4);
      MC_flat->GetXaxis()->SetLabelFont(43);
      MC_flat->GetXaxis()->SetLabelSize(20);

      MC_flat->GetYaxis()->SetTitle( "Data/MC" );
      MC_flat->GetYaxis()->SetNdivisions(204, false);
      MC_flat->GetYaxis()->ChangeLabel( -1, -1, -1, -1, -1, -1, " " );
      MC_flat->GetYaxis()->ChangeLabel(  1, -1, -1, -1, -1, -1, " " );
      MC_flat->GetYaxis()->SetTitleSize(25);
      MC_flat->GetYaxis()->SetTitleFont(43);
      MC_flat->GetYaxis()->SetTitleOffset(1.55);
      MC_flat->GetYaxis()->SetLabelFont(43);
      MC_flat->GetYaxis()->SetLabelSize(20);

      canvas->Update();
      string weighted = "";
      if ( do_weight ) {
        weighted = "_CRSF";
      }
      string output_img_name = string( "Output/"+to_string(region)+"_"+method+"_"+wp_name+weighted );
      if (sherpa_VV) output_img_name += "_ShrpVV";
      output_img_name += ".pdf";
      canvas->Print( output_img_name.c_str() );

      // Deleting all the objects created in this function
      delete canvas;
      delete divider_hist;
      delete MC_flat;
      delete ratio_hist;

    }


};



class Plotting_Dataset {
  public:

    string folder;
    string name;
    string label;
    int    colour;
    double factor;
    double sfuncer;
    bool   is_susy;

    Plotting_Dataset( const char * new_folder, const char * new_name, const char * new_label, int new_colour,
                      double new_factor = 1.0, double new_sfuncer = 0.0, bool new_is_susy = false ) {
      folder  = new_folder;
      name    = new_name;
      label   = new_label;
      colour  = new_colour;
      factor  = new_factor;
      sfuncer = new_sfuncer;
      is_susy = new_is_susy;
    }

};




int main() {
  gROOT->SetStyle("ATLAS");
  gROOT->ForceStyle();
  TH1::SetDefaultSumw2();
  gStyle->SetErrorX(0.5);

  ////////////// User input //////////////

  // The region is an int which indicates the type of preselection that will be done
  // Option 0: No selection, nothing will be cut

  // Option 1: Control Region for Z:      nbjets=0,     mll(71,111),    mt2<100,     etmiss<100
  // Option 2: Control Region for VV:     nbjets=0,     mll(71,111),    mt2>130,     etmiss>100
  // Option 3: Control Region for top:    nbjets>=1,    mll>111,        mt2(75,100)

  // Option 4: Validation for VV:         nbjets=0,     mll!((71,111),  mt2(75,100)
  // Option 5: Validation for top:        nbjets>=1,    none,           mt2>100

  // Option 6: Signal Region:             nbjets=0,     mll>111         mt2>100

  string wp_name   = "Tight";         // Tight, Network
  string wp_tree   = "wpt_tree";       // wpt_tree, main_ann
  string wp_branch = "WP_TST_Tight";      // WP_TST_Tight, WP_ANN

  double wp_et; // double, float

  float luminosity = 44307.4; // in nano

  vector<int> regions = { 6 };

  bool do_weight = true;

  bool smallset  = false; // For doing little data to increase speed.
  bool sherpa_VV = false;

  vector<Plotting_Dataset> z_datasets = {}; // The Z data is a seperate vector as it needs to be inserted after or before the others
  z_datasets.push_back( Plotting_Dataset( "SR_SUSY/",   "SZee_all",     "Z#rightarrowee",        860+0, 1.040, 0.0 ) ); // 1.040, 0.0
  z_datasets.push_back( Plotting_Dataset( "SR_SUSY/",   "SZmumu_all",   "Z#rightarrow#mu#mu",    860+1, 1.091, 0.0 ) ); // 1.091, 0.0
  z_datasets.push_back( Plotting_Dataset( "SR_SUSY/",   "Ztt_all",      "Z#rightarrow#tau#tau",  870+0, 1.000, 0.0 ) ); // 1.000, 0.0

  vector<Plotting_Dataset> mc_datasets = {};
  mc_datasets.push_back( Plotting_Dataset( "SR_SUSY/",   "ttbar_1in3",   "t#bar{t}",          800+4, 1.058, 0.045 ) ); // 1.058, 0.045    1.092, 0.055
  mc_datasets.push_back( Plotting_Dataset( "SR_SUSY/",   "top_1in3",     "Wt",                800+2, 1.058, 0.045 ) ); // 1.058, 0.045    1.092, 0.055
  mc_datasets.push_back( Plotting_Dataset( "SR_SUSY/",   "antitop_1in3", "W#bar{t}",          800+0, 1.058, 0.045 ) ); // 1.058, 0.045    1.092, 0.055
  if (sherpa_VV) {
    mc_datasets.push_back( Plotting_Dataset( "SR_SUSY/",   "Sllvv_all",  "VV#rightarrowl#nul#nu", 3,     1.0, 0.0 ) );
    mc_datasets.push_back( Plotting_Dataset( "SR_SUSY/",   "WZ_1in3",    "WZ#rightarrowl#null",   416-8, 1.0, 0.0 ) );
  } else {
    mc_datasets.push_back( Plotting_Dataset( "SR_SUSY/",   "ZZ_1in3",    "ZZ#rightarrowll#nu#nu", 416+3, 1.232, 0.064 ) ); // 1.232, 0.064     1.239, 0.064
    mc_datasets.push_back( Plotting_Dataset( "SR_SUSY/",   "WZ_1in3",    "WZ#rightarrowl#null",   416-8, 1.232, 0.064 ) ); // 1.232, 0.064     1.239, 0.064
    mc_datasets.push_back( Plotting_Dataset( "SR_SUSY/",   "WW_1in3",    "WW#rightarrowl#nul#nu", 416+2, 1.791, 0.146 ) ); // 1.791, 0.146     1.749, 0.141
  }

  vector<Plotting_Dataset> other_datasets = {};
  other_datasets.push_back( Plotting_Dataset( "SR_SUSY/",   "SL550X001_all",    "m(#tilde{l},x^{0}_{l}) = (550, 1) GeV",   213, 1.0, 0.0, true ) );
  other_datasets.push_back( Plotting_Dataset( "SR_SUSY/",   "SL400X250_all",    "m(#tilde{l},x^{0}_{l}) = (400, 250) GeV", 223, 1.0, 0.0, true ) );
  other_datasets.push_back( Plotting_Dataset( "SR_SUSY/",   "SL300X200_all",    "m(#tilde{l},x^{0}_{l}) = (300, 200) GeV", 632, 1.0, 0.0, true ) );
  other_datasets.push_back( Plotting_Dataset( "Real/",      "Real_all",         "Data", 1  ) );

  vector<Plotting_Variable> all_variables = {};
  all_variables.push_back( Plotting_Variable( "SRs",     13, 0.0,   13.0   ) );
  all_variables.push_back( Plotting_Variable( "etmiss",  20, 0.0,   400.0  ) );
  all_variables.push_back( Plotting_Variable( "mt2",     40, 0.0,   400.0  ) );
  all_variables.push_back( Plotting_Variable( "mll",     22, 60.0,  500.0  ) );

  ////////////// End of user input //////////////

  // The variables used in to fill the correct histograms
  int region;
  float  bin;
  double weight; // The holder for the combined scale factors
  int    type;   // Definine 0=bkrnd, 1=signal, 2=Real

  // The variables we will load from the trees
  double mt2;
  double lead_l_px;
  double lead_l_py;
  double subl_l_px;
  double subl_l_py;
  float  ll_inv_mass;
  double event_weight;
  double pile_up_weight;
  double tot_elec_sf;
  double tot_muon_sf;
  double trg_elec_sf;
  double trg_muon_sf;
  double cross_section;
  double kfactor;
  double filter_eff;
  double upscale;
  double twe;
  int n_cent_JetsB;

  for (int ridx = 0; ridx < regions.size(); ridx++) {
    region = regions[ridx];
    cout << "\nLOOKING AT REGION: " << region << '\n';


    // We change the order of the Z+jets sample depending on the region
    vector<Plotting_Dataset> datasets {};
    if ( region <= 1 ) datasets.insert( datasets.end(), mc_datasets.begin(), mc_datasets.end() );
    datasets.insert( datasets.end(), z_datasets.begin(), z_datasets.end() );
    if ( region >= 2 ) datasets.insert( datasets.end(), mc_datasets.begin(), mc_datasets.end() );
    datasets.insert( datasets.end(), other_datasets.begin(), other_datasets.end() );


    // We remove the individual SR plot if we arent in the signal region
    vector<Plotting_Variable> variables(all_variables);
    if ( region!=6 ) variables.erase( variables.begin() );

    // Adding the legend which is the same across all variables
    TLegend* leg = new TLegend(0.52, 0.73, 0.95, 0.95);
    leg->SetNColumns(3);
    leg->SetTextFont(42);
    leg->SetTextSize(0.043);
    leg->SetFillColor(0);
    leg->SetBorderSize(1);
    leg->SetEntrySeparation(0);
    leg->SetMargin(0.14);

    // Adding the second legend for the SUSY signals
    TLegend* sr_leg = new TLegend(0.52, 0.50, 0.95, 0.72);
    sr_leg->SetTextFont(42);
    sr_leg->SetTextSize(0.043);
    sr_leg->SetFillStyle(0);
    sr_leg->SetBorderSize(0);
    sr_leg->SetEntrySeparation(0);
    sr_leg->SetMargin(0.15);


    // We prepare the plotting variables (owners of the stacks) to get ready for a new set
    for ( auto& var: variables ) {
      var.Region_Initialise( region );
    }


    // Looping through each dataset
    for ( auto& set: datasets ) {

      cout << set.name << "\n";

      // Setting the type to tell which stacks this will contribute to
      if ( set.folder == "SR_SUSY/" && !set.is_susy ) type = 0;
      if ( set.is_susy ) type = 1;
      if ( set.folder == "Real/") type = 2;



      // Opening the the file and trees
      string full_name = getenv("HOME_DIRECTORY") + string("/Data/") + set.folder +  set.name + "_susy_2l_0jets.root";
      TFile* in_file   = new TFile(full_name.c_str(), "read");
      TTree* alt_tree  = (TTree*)in_file->Get("alt_tree");
      TTree* wpt_tree  = (TTree*)in_file->Get(wp_tree.c_str());
      TTree* mt2_tree  = (TTree*)in_file->Get( ("mt2_"+wp_name).c_str() );



      // Only enabling a few branches to increase speed
      alt_tree->SetBranchStatus( "*", 0 );
      alt_tree->SetBranchStatus( "lead_l_px",      1 );
      alt_tree->SetBranchStatus( "lead_l_py",      1 );
      alt_tree->SetBranchStatus( "subl_l_px",      1 );
      alt_tree->SetBranchStatus( "subl_l_py",      1 );
      alt_tree->SetBranchStatus( "ll_inv_mass",    1 );
      alt_tree->SetBranchStatus( "event_weight",   1 );
      alt_tree->SetBranchStatus( "pile_up_weight", 1 );
      alt_tree->SetBranchStatus( "tot_elec_sf",    1 );
      alt_tree->SetBranchStatus( "tot_muon_sf",    1 );
      alt_tree->SetBranchStatus( "trg_elec_sf",    1 );
      alt_tree->SetBranchStatus( "trg_muon_sf",    1 );
      alt_tree->SetBranchStatus( "cross_section",  1 );
      alt_tree->SetBranchStatus( "kfactor",        1 );
      alt_tree->SetBranchStatus( "filter_eff",     1 );
      alt_tree->SetBranchStatus( "upscale",        1 );
      alt_tree->SetBranchStatus( "twe",            1 );
      alt_tree->SetBranchStatus( "n_cent_JetsB",   1 );

      wpt_tree->SetBranchStatus( "*", 0 );
      wpt_tree->SetBranchStatus( ( string(wp_branch) + "_ET" ).c_str(), 1 );

      // Setting the adresses for the variables
      alt_tree->SetBranchAddress( "lead_l_px",      &lead_l_px );
      alt_tree->SetBranchAddress( "lead_l_py",      &lead_l_py );
      alt_tree->SetBranchAddress( "subl_l_px",      &subl_l_px );
      alt_tree->SetBranchAddress( "subl_l_py",      &subl_l_py );
      alt_tree->SetBranchAddress( "ll_inv_mass",    &ll_inv_mass );
      alt_tree->SetBranchAddress( "event_weight",   &event_weight );
      alt_tree->SetBranchAddress( "pile_up_weight", &pile_up_weight );
      alt_tree->SetBranchAddress( "tot_elec_sf",    &tot_elec_sf );
      alt_tree->SetBranchAddress( "tot_muon_sf",    &tot_muon_sf );
      alt_tree->SetBranchAddress( "trg_elec_sf",    &trg_elec_sf );
      alt_tree->SetBranchAddress( "trg_muon_sf",    &trg_muon_sf );
      alt_tree->SetBranchAddress( "cross_section",  &cross_section );
      alt_tree->SetBranchAddress( "kfactor",        &kfactor );
      alt_tree->SetBranchAddress( "filter_eff",     &filter_eff );
      alt_tree->SetBranchAddress( "upscale",        &upscale );
      alt_tree->SetBranchAddress( "twe",            &twe );
      alt_tree->SetBranchAddress( "n_cent_JetsB",   &n_cent_JetsB );

      wpt_tree->SetBranchAddress( ( string(wp_branch) + "_ET" ).c_str(), &wp_et );

      mt2_tree->SetBranchAddress( "mt2", &mt2 );



      // We initialise the current set of histograms for each var
      for ( auto& var: variables ) {
        var.Current_Initialise( region, set.name );
      }




      // Now we begin the event loop
      float    frac   = 0.0;
      unsigned evtmax = alt_tree->GetEntries();

      for (unsigned i = 0; i < evtmax; i++) {

        // Printing out progress
        if ( (100*i)/evtmax > frac ) {
          cout << " -- " <<  i << "/" << evtmax << " " << (100*i)/evtmax << "%                  \r";
          cout.flush();
          frac = int( (100*i)/evtmax );
        }

        // For testing smaller files
        if ( smallset && type==2 && i > 10000 ) break;

        // We load the values from each tree
        alt_tree->GetEntry( i );
        wpt_tree->GetEntry( i );
        mt2_tree->GetEntry( i );

        // We check if the region requires a veto on the number of bjets
        if ( (region == 1) && (n_cent_JetsB>0)  ) continue;
        if ( (region == 2) && (n_cent_JetsB>0)  ) continue;
        if ( (region == 3) && (n_cent_JetsB==0) ) continue;
        if ( (region == 4) && (n_cent_JetsB>0)  ) continue;
        if ( (region == 5) && (n_cent_JetsB==0)  ) continue;
        if ( (region == 6) && (n_cent_JetsB>0)  ) continue;

        // We check if the region requires a veto on mll
        if ( (region == 1) && (ll_inv_mass<71.0 || ll_inv_mass>111.0) ) continue;
        if ( (region == 2) && (ll_inv_mass<71.0 || ll_inv_mass>111.0) ) continue;
        if ( (region == 3) && (ll_inv_mass<111.0) ) continue;
        if ( (region == 4) && (ll_inv_mass>71.0 && ll_inv_mass<111.0) ) continue;
        // No mll veto in VR-top
        if ( (region == 6) && (ll_inv_mass<111.0) ) continue;

        // We check if the region requires a veto on the mt2
        if ( (region == 1) && (mt2>100.0) ) continue;
        if ( (region == 2) && (mt2<130.0) ) continue;
        if ( (region == 3) && (mt2<75.0 || mt2>100.0) ) continue;
        if ( (region == 4) && (mt2<75.0 || mt2>100.0) ) continue;
        if ( (region == 5) && (mt2<100.0) ) continue;
        if ( (region == 6) && (mt2<100.0) ) continue;

        // We check if the region cuts on the Etmiss
        if ( (region == 1) && (wp_et>100.0) ) continue;
        if ( (region == 2) && (wp_et<100.0) ) continue;


        // Calculating the weight for the event
        weight = 1.0;
        if ( type < 2 ) {
          weight *= tot_elec_sf * tot_muon_sf * trg_elec_sf * trg_muon_sf;
          weight *= event_weight * pile_up_weight * cross_section * kfactor * filter_eff * upscale * luminosity / twe;
          if (do_weight) weight *= set.factor;
        }

        // Filling the histograms of the variables
        for ( auto& var: variables ) {
          bin = var.Apply_Method( mt2, wp_et, ll_inv_mass );
          var.Current_Hist->Fill( bin, weight );
          var.Current_Uncer->Fill( bin, weight );
        }



      } // End of the event loop
      cout << "                                                \r";
      cout.flush();

      // We add the scale factor uncertainties and add the histograms to the appropriate stacks
      for ( auto& var: variables ) {
        if (do_weight) var.FixCurrentUncertainties( set.sfuncer );
        var.Add_To_Stacks( type, set.colour );
      }

      // We add the dataset to the appropriate legend (this only happens once since all variables share a legend)
      if ( type == 0 )    leg->AddEntry( variables[0].Current_Hist, set.label.c_str(), "f" );
      if ( type == 1 ) sr_leg->AddEntry( variables[0].Current_Hist, set.label.c_str(), "l" );
      if ( type == 2 )    leg->AddEntry( variables[0].Current_Hist, set.label.c_str(), "p" );

      in_file->Close();
      delete in_file;

    } // End of the dataset loop
    gSystem->cd(0);

    // Fixing the uncertainties of the total MC
    for ( auto& var: variables ) {
      var.FixTotalUncertainties();
    }

    // Outputting the uncertainty to the text files, which only needs to be done once
    string weighted = "";
    if ( do_weight ) {
      weighted = "_CRSF";
    }
    string output_txt_name = string( "Output/"+to_string(region)+"_"+wp_name+weighted );
    if (sherpa_VV) output_txt_name += "_ShrpVV";
    variables[0].Total_IandU( output_txt_name );


    // Adding one last universal item to the legend
    leg->AddEntry( variables[0].MC_Uncer, "Total SM", "lf" );

    // Drawing the objects on their canvases
    for ( auto& var: variables ) {
      var.Draw( leg, sr_leg, luminosity, wp_name, region, do_weight, sherpa_VV );
    }

    // Deleting the objects to prevent memory leaks
    for ( auto& var: variables ) {
      var.Region_Descruct();
    }

    // We reset the root space after each run
    delete sr_leg;
    delete leg;
    gROOT->Reset();

  } // End of the region loop


  return 0;

}
