#include <iostream>
#include "TString.h"
#include "TChain.h"
#include "MyEvent.h"
#include <sstream>
#include <fstream>
#include <string>

string cutGLOBAL;                   // Matt changed this to mean the cut of events (1 in 3...)
string selectionGLOBAL;             // Matt changed this to mean the type of event selection

TString labelGLOBAL(20);
TString outputGLOBAL(20);

using namespace std;
int main (int argc, char **argv){

  TString infile;
  TChain *chain;

  cutGLOBAL       = argv[1];
  selectionGLOBAL = argv[2];

  TString cardfile(argv[3]);
  outputGLOBAL.Append(argv[4]);
  labelGLOBAL.Append(argv[5]);

  int fileno = 0;

  chain=new TChain("MyTree");

  TString input_filename(cardfile);
  fstream ifs(input_filename,ios::in);
  
  while (ifs >> input_filename) {
    // Ignore comments
    if ( input_filename[0] == '#' ) {
      string comment;
    } else {
      fileno++;
      stringstream ss3; ss3 << "opening " << fileno << " " << input_filename;
      chain->Add(input_filename);
    }

  }

  MyEvent *selector;

  selector= new MyEvent(chain);

  chain->Process(selector);

  return 0;

}

