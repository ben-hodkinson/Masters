#include "ChannelInfoTool.h"

#include "GaudiKernel/ITHistSvc.h"
#include "TFile.h"
#include "TROOT.h"


ChannelInfoTool::ChannelInfoTool(const std::string& type, const std::string& name,
			 const IInterface* pParent) : CamTool(type, name, pParent), TChannelInfo("channelInfo")/*, m_channelInfo(0)*/
{

  declareInterface< IChannelInfoTool >(this);


   declareProperty("OutputFilename",m_outFile="");
   declareProperty("WriteToStreams",m_writeToStreams); //if blank, writes to ALL streams

   declareProperty("PrintCutflow",m_printCutflow=false);

}

// Destructor
ChannelInfoTool::~ChannelInfoTool()
{}

StatusCode ChannelInfoTool::initialize() { 

   //m_channelInfo = new TChannelInfo("channelInfo");
   return StatusCode::SUCCESS;

}
StatusCode ChannelInfoTool::finalize() { 

   if(m_printCutflow) {
      auto chanMap = GetMap();
      TString channels = "";
      for(auto& c : *chanMap) {
         if(channels=="") channels += TString::Format("#==%d",c.first);
         else channels += TString::Format(":#==%d",c.first);
      }
      if(channels=="") channels="1";
      TChannelInfo::Cutflow("",channels);
   }

   //also write me to every histsvc file that holds a tree 
   //put this into a TTree, for safe file merging
   ITHistSvc* m_histSvc = 0;
   service("THistSvc",m_histSvc).ignore(); //or check the statuscode
   std::vector<std::string> histsvcTrees = m_histSvc->getTrees(); 

   TString writeName = name(); if(writeName.BeginsWith("ToolSvc.")) writeName = writeName(8,writeName.Length()-8);
   writeName.ReplaceAll(".","_");

   std::set<TString> doneStreams;

   for(unsigned int i=0;i<histsvcTrees.size();i++) {

      //strip the tree name AND PATH (all after second /)
      TString ss(histsvcTrees[i].c_str());
      TString ssPath = ss(0,ss.Index("/",1));
      if(doneStreams.find(ssPath)!=doneStreams.end()) continue; //already written to this stream
      //if writeToStreams is specified, only write to this stream if we were told to 
      if(m_writeToStreams.value().size()>0) {
         bool found(false);
         for(unsigned int j=0;j<m_writeToStreams.value().size();++j) {
            if(ssPath.Contains(m_writeToStreams.value()[j].c_str())) { found=true; break; }
         }
         if(!found) continue;
      }
      doneStreams.insert(ssPath);
      ssPath += "/"; ssPath += writeName;
      if(m_doneDirs[ssPath]) continue;
      std::string sPath(ssPath.Data());
      ATH_MSG_INFO("Writing ChannelInfo to file: " << ssPath);
      //need to create a copy of the ChannelInfo object, so it doesn't get destroyed before write
      //turns out need one copy for each output 
      TChannelInfo* clone = dynamic_cast<TChannelInfo*>(this->Clone(writeName));
      m_histSvc->regGraph(sPath,reinterpret_cast<TGraph*>(clone)).ignore(); //muhaha this is so clever!
      m_doneDirs[ssPath]=true;
   }


   //delete m_channelInfo;

   if(m_outFile=="") return StatusCode::SUCCESS;

   //write me to my output file
   TFile f1(m_outFile.c_str(),"RECREATE");
   //m_channelInfo->Write();
   this->Write();
   f1.Close();

   return StatusCode::SUCCESS;

}
/*
TChannelInfo* ChannelInfoTool::getChannelInfo() {
   return m_channelInfo;
}*/

