#ifndef ANALYSISCAM_TCHANNELINFO_H
#define ANALYSISCAM_TCHANNELINFO_H

#include "TH1D.h"
#include <map>
#include "TString.h"

#include "TCollection.h"
#include "TTreeFormula.h"
#include "TTree.h"
#include "TChain.h"

//____________________
//
//                                                            //
// TChannelInfo                                               //
//                                                            //
// Use TChannelInfo to manage your channel meta data          //
// It functions as both a meta-data provider and a            //
// cutflow manager                                            //
//                                                            //
//

class TChannelInfo : public TNamed {

   public:
      using TNamed::Print;
      TChannelInfo();
      TChannelInfo(const char* name);
      ~TChannelInfo();

      Double_t Fill(Int_t channel, const char* name, Int_t value);
      Int_t SetInfoContent(Int_t channel, const char* name, Int_t value);

      Long64_t GetInfoEntries(Int_t channel, const char* name);
      Double_t GetInfoContent(Int_t channel, const char* name);

      Double_t Fill(Int_t channel, const char* name, Double_t value);
      Double_t SetInfoContent(Int_t channel, const char* name, Double_t value);

      Double_t Fill(Int_t channel, const char* name, Double_t value, Int_t entries, Double_t w2);

      TString SetStringContent(Int_t channel, const char* name, TString value);
      TString GetStringContent(Int_t channel, const char* name);

      TTree* GetTree(const char* channelBranchName="", bool showWarnings=false); //builds a TTree from the contained information. Indexed by channel number

      Int_t Merge(TCollection *coll);

      Int_t ReadFile(const char* filename,const char* channelBranchName="mc_channel_number");

      Int_t Scan(const char* names, const char* groups="1", const char* format="%f (%d)", const char* formulae="$value:$entries",const char* separator=":" );

      Bool_t FriendTo(TTree* chain, const char* channelBranchName="mc_channel_number", const char* fileLocation="/tmp/");

      //Int_t Scan(const char* names); //displays "all" channels automatically with format $value ($entries)

      Int_t Cutflow(const char* str="", const char* groups="channels");

      //void Print(const char* format, ... );
      void Print(Option_t* opt="") const;

      std::map<Int_t,std::map<TString,Double_t> >* GetMap() { return &m_doubles; }
      std::map<Int_t,std::map<TString,Long64_t> >* GetEntriesMap() { return &m_doublesEntries; }
      std::map<Int_t,std::map<TString,Double_t> >* GetSumw2Map() { return &m_doublesW2; }
      std::map<Int_t,std::map<TString,TString> >* GetStringMap() { return &m_strings; }
      std::map<TString,Int_t>* GetTypeMap() { return &m_types;}
      std::vector<TString>* GetCutflowOrder() { return &m_cutflow; }

      void Diff(TChannelInfo* rhs, const char* what);

      void CompareToAMI(const char* datasetField, const char* whatME,const char* whatAMI="totalEvents");

      ClassDef(TChannelInfo,3);

   private:
      std::map<Int_t,std::map<TString,Double_t > > m_doubles;
      std::map<Int_t,std::map<TString,Double_t > > m_doublesW2;
      std::map<Int_t,std::map<TString,Long64_t> > m_doublesEntries;

      std::map<Int_t,std::map<TString,TString> > m_strings;

      std::map<TString,Int_t> m_types; //0=string, -1=fixed int, -2=fixed double, 1=addable int, 2=addable double
      TTree* m_tree; //!
      TChain* m_friendChain; //! //when we add the channelInfo as a friend to a TTree/TChain, we create a TChain and friend it after building index

      std::vector<TString> m_cutflow; //stores the order in which "Info" fields are added
};

#endif
