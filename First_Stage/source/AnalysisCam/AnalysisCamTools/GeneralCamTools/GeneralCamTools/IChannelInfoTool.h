#ifndef _ICHANNELINFOTOOL_
#define _ICHANNELINFOTOOL_

#include "GaudiKernel/IAlgTool.h"

#include "AnalysisCamROOT/TChannelInfo.h"

static const InterfaceID IID_IChannelInfoTool( "IChannelInfoTool", 1, 0 );

class IChannelInfoTool : virtual public IAlgTool {

   public:
      static const InterfaceID& interfaceID() { return IID_IChannelInfoTool; };

      virtual Int_t Fill(Int_t channel, const char* name, Int_t value)=0;
      virtual Int_t Fill(const char* name, Int_t value)=0;
      virtual Int_t Fill(const char* name, UInt_t value)=0;
      virtual Int_t FillN(const char* name, UInt_t value)=0; //files "1" value times
      virtual Int_t SetInfoContent(Int_t channel, const char* name, Int_t value)=0;

      virtual Int_t GetInfoEntries(Int_t channel, const char* name)=0;
      virtual Double_t GetInfoContent(const char* name)=0;
      virtual Double_t GetInfoContent(Int_t channel, const char* name)=0;

      virtual Double_t Fill(Int_t channel, const char* name, Double_t value)=0;
      virtual Double_t Fill(const char* name, Double_t value)=0;
      virtual Double_t Fill(std::string&& name, Double_t value) { return Fill(name.c_str(),value); }
      virtual Double_t Fill(Int_t channel, const char* name, Double_t value, Int_t entries, Double_t w2) = 0;

      virtual Double_t SetInfoContent(Int_t channel, const char* name, Double_t value)=0;

      virtual Int_t ReadFile(const char* filename,const char* channelBranchName="mc_channel_number")=0;
      virtual Int_t Cutflow(const char* str="")=0;
      virtual Int_t Scan(const char* names, const char* groups="1", const char* format="%f (%d)", const char* formulae="$value:$entries",const char* separator=":" )=0;

      virtual TChannelInfo* expert() = 0;

};

#endif