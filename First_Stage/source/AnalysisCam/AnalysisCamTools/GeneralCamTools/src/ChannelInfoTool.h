
#ifndef _CHANNELINFOTOOL_
#define _CHANNELINFOTOOL_

// includes
#include "GeneralCamTools/IChannelInfoTool.h"
#include "AnalysisCamEvent/CamTool.h"


class ChannelInfoTool : virtual public CamTool, virtual public IChannelInfoTool, virtual public TChannelInfo {

  public:
      using IChannelInfoTool::Fill;
      // Constructors, destructor
      ChannelInfoTool(const std::string& type, const std::string& name,const IInterface* pParent);
      virtual ~ChannelInfoTool();
      //StatusCode queryInterface( const InterfaceID& riid, void** ppvIf );

      virtual StatusCode initialize();
      virtual StatusCode finalize();

      virtual void* get(const std::string&) { return this; }

      //void fill(CamObject& c, va_list args);

      //virtual StatusCode apply(CamEvent* evt,std::string& label,std::string option);

      virtual TChannelInfo* expert() { return this; }

      virtual Int_t Fill(const char* name, Int_t value) {
         CamEvent evt; int chan = (!evt.contains<uint>("mcChannelNumber")) ? evt.get<uint>("runNumber") : evt.get<uint>("mcChannelNumber");
         if(!chan) chan = evt.get<uint>("runNumber");
         return Fill(chan,name,value);
      }
      virtual Double_t Fill(const char* name, Double_t value) {
         CamEvent evt; int chan = (!evt.contains<uint>("mcChannelNumber")) ? evt.get<uint>("runNumber") : evt.get<uint>("mcChannelNumber");
         if(!chan) chan = evt.get<uint>("runNumber");
         return Fill(chan,name,value);
      }
      inline virtual Int_t Fill(const char* name, UInt_t value) { return Fill(name,int(value)); }
      inline virtual Int_t FillN(const char* name, UInt_t value) {
         Int_t out = 0;
         for(uint i=0;i<value;i++) { out += Fill(name,1); } 
         return out;
      }

      virtual Double_t GetInfoContent(const char* name) {
         CamEvent evt; int chan = (!evt.contains<uint>("mcChannelNumber")) ? evt.get<uint>("runNumber") : evt.get<uint>("mcChannelNumber");
         if(!chan) chan = evt.get<uint>("runNumber");
         return GetInfoContent(chan,name);
      }


      virtual Int_t Fill(Int_t channel, const char* name, Int_t value) { 
         return TChannelInfo::Fill(channel,name,value);
      }

      virtual Int_t SetInfoContent(Int_t channel, const char* name, Int_t value) {
         return TChannelInfo::SetInfoContent(channel,name,value);
      }

      virtual Int_t GetInfoEntries(Int_t channel, const char* name) {
         return TChannelInfo::GetInfoEntries(channel,name);
      }
      virtual Double_t GetInfoContent(Int_t channel, const char* name) {
         return TChannelInfo::GetInfoContent(channel,name);
      }

      virtual Double_t Fill(Int_t channel, const char* name, Double_t value, Int_t entries, Double_t w2) {
         return TChannelInfo::Fill(channel,name,value,entries,w2);
      }

      virtual Double_t Fill(Int_t channel, const char* name, Double_t value) {
         return TChannelInfo::Fill(channel,name,value);
      }
      virtual Double_t SetInfoContent(Int_t channel, const char* name, Double_t value) {
         return TChannelInfo::SetInfoContent(channel,name,value);
      }

      virtual Int_t ReadFile(const char* filename,const char* channelBranchName="mc_channel_number") {
         return TChannelInfo::ReadFile(filename,channelBranchName);
      }
      virtual Int_t Cutflow(const char* str="") { 
         return TChannelInfo::Cutflow(str);
      }

      virtual Int_t Scan(const char* names, const char* groups="1", const char* format="%f (%d)", const char* formulae="$value:$entries",const char* separator=":" ) {
         return TChannelInfo::Scan(names,groups,format,formulae,separator);
      }

   private:
      bool m_printCutflow;
      //TChannelInfo* m_channelInfo;
      std::string m_outFile;
      std::map<TString,bool> m_doneDirs;


      StringArrayProperty m_writeToStreams;
};


#endif