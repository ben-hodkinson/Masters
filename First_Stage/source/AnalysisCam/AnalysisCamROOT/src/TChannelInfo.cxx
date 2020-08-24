#include "AnalysisCamROOT/TChannelInfo.h"

#include "TH1D.h"
#include "TPRegexp.h"
#include <iostream>
#include <sstream>
#include "TLeaf.h"
#include "TBranch.h"
#include "TMath.h"
#include <math.h>

#include "TSystem.h"

#include "TFile.h"

//NOTE: Should add a 'Diff' or 'Compare' method so can quickly compare to TChannelInfo objects


//___________________________________
/* BEGIN_HTML
<p>This is a general meta-data and cutflow manager.</p>
<p>The information it holds is indexed by a channel number (int). The information has 5 possible types, which are 
(with the corresponding branch types)<br>
Fixed double (type=-2): double <br>
Fixed int (type=-1): int <br>
String (type=0): char <br>
Addable int (type=1): int <br>
Addable double (type=2): double <br>

<p><b>Adding Info</b><br>
1. Fill<br>
Filling defines addable ints and addable double properties. Think of them like bins of histograms:<br>
info->Fill(123,"myDouble",3); //fills the 'myDouble' bin of the '123' channel with a value of 3<br>

END_HTML */
//____________________________________

ClassImp(TChannelInfo)


TChannelInfo::TChannelInfo() : TNamed("TChannelInfo",""),m_tree(0),m_friendChain(0) {}

TChannelInfo::TChannelInfo(const char* name) : TNamed(name,""),m_tree(0),m_friendChain(0) {
   //Create instance with given name
}

TChannelInfo::~TChannelInfo() {
   if(m_tree) delete m_tree; 
   m_tree=0;
   //do not delete the friend chain - allow to persist beyond my death, in case it is being used
}


TString TChannelInfo::SetStringContent(Int_t channel, const char* name, TString value) {
   //Set a string property (the 'content') of a given channel
   //channel.name = value
   m_strings[channel][name]=value;
   return value;
}


TString TChannelInfo::GetStringContent(Int_t channel, const char* name) {
   //Get string property of channel, with property name given by 'name'
   return m_strings[channel][name];
}


Double_t TChannelInfo::Fill(Int_t channel, const char* name, Int_t value) {
   //Fill a channel's info property (double or int) with value given
   //
   //If the property has never been filled before (for any channel) then the 
   //property is defined as an 'addable int' type.

   if(m_types.find(name)==m_types.end()) {
      //add as an "addable" int
      //0=string, -1=fixed int, -2=fixed double, 1=addable int, 2=addable double
      m_types[name] = 1;
      //add to cutflow order too
      m_cutflow.push_back(name);
   } 

   if(m_types[name]<0) {
      Error("Fill","%s is a constant field. Cannot Fill, only SetInfoContent",name);
      return -1;
   }


   switch(m_types[name]) {
   case 0: //string
      Error("Fill","%s is a string field.",name); return -1; break;
   default: 
      m_doubles[channel][name] += value;
      m_doublesEntries[channel][name] += 1;
      m_doublesW2[channel][name] += value*value;
   }
   return m_doubles[channel][name];
}

Double_t TChannelInfo::Fill(Int_t channel, const char* name, Double_t value) {
   //Fill a channel's info property (double or int) with value given
   //
   //If the property has never been filled before (for any channel) then the 
   //property is defined as an 'addable double' type.

   if(m_types.find(name)==m_types.end()) {
      //add as an "addable" double
      //0=string, -1=fixed int, -2=fixed double, 1=addable int, 2=addable double
      m_types[name] = 2;
      m_cutflow.push_back(name);
   } 

   if(m_types[name]<0) {
      Error("Fill","%s is a constant field. Cannot Fill, only SetInfoContent",name);
      return -1;
   }

   switch(m_types[name]) {
   case 0: //string
      Error("Fill","%s is a string field.",name); return -1; break;
   default: 
      m_types[name] = 2; //change to double if it isn't already;
      m_doubles[channel][name] += value;
      m_doublesEntries[channel][name] += 1;
      m_doublesW2[channel][name] += value*value;
   }
   return m_doubles[channel][name];
}

Double_t TChannelInfo::Fill(Int_t channel, const char* name, Double_t value, Int_t entries, Double_t w2) {
   //Fill a channel's info property (double or int) with value given
   //
   //If the property has never been filled before (for any channel) then the 
   //property is defined as an 'addable double' type.

   if(m_types.find(name)==m_types.end()) {
      //add as an "addable" double
      //0=string, -1=fixed int, -2=fixed double, 1=addable int, 2=addable double
      m_types[name] = 2;
      m_cutflow.push_back(name);
   } 

   if(m_types[name]<0) {
      Error("Fill","%s is a constant field. Cannot Fill, only SetInfoContent",name);
      return -1;
   }

   switch(m_types[name]) {
   case 0: //string
      Error("Fill","%s is a string field.",name); return -1; break;
   default: 
      m_types[name] = 2; //change to double if it isn't already;
      m_doubles[channel][name] += value;
      m_doublesEntries[channel][name] += entries;
      m_doublesW2[channel][name] += w2;
   }
   return m_doubles[channel][name];
}

Int_t TChannelInfo::SetInfoContent(Int_t channel, const char* name, Int_t value) {
   //Set a channel's info property (double or int) with value given. Overrides any previous values set
   //
   //If the property has never been filled before (for any channel) then the 
   //property is defined as an 'fixed int' type.

   if(m_types.find(name)==m_types.end()) {
      //add as a fixed int
      //0=string, -1=fixed int, -2=fixed double, 1=addable int, 2=addable double
      m_types[name] = -1;
   }
   if(m_types[name]==0) {
      Error("Fill","%s is a string field.",name); return -1;
   } 
   m_doubles[channel][name] = value;
   return 0;
}

Double_t TChannelInfo::SetInfoContent(Int_t channel, const char* name, Double_t value) {
   //Set a channel's info property (double or int) with value given. Overrides any previous values set
   //
   //If the property has never been filled before (for any channel) then the 
   //property is defined as an 'fixed double' type.
   if(m_types.find(name)==m_types.end()) {
      //add as a fixed double
      //0=string, -1=fixed int, -2=fixed double, 1=addable int, 2=addable double
      m_types[name] = -2;
   }
   if(m_types[name]==0) {
      Error("Fill","%s is a string field.",name); return -1;
   } 
   m_doubles[channel][name] = value;
   return 0;
}

Long64_t TChannelInfo::GetInfoEntries(Int_t channel, const char* name) {
   //Get the number of times the channel.name property has been filled
   if(m_types.find(name)==m_types.end()) {
      Error("GetInfoEntries","%s unknown field",name);return 0;
   }

   if(m_types[name]==0) {
      Error("GetInfoEntries", "%s is a string field.",name); return 0;
   }

   return m_doublesEntries[channel][name];

}

Double_t TChannelInfo::GetInfoContent(Int_t channel, const char* name) {
   //Get the contents of the channel.name property (int or double, but always returns as double)
   if(m_types.find(name)==m_types.end()) {
      Error("GetInfoContent","%s unknown field",name);return 0;
   }

   if(m_types[name]==0) {
      Error("GetInfoContent", "%s is a string field.",name); return 0;
   }

   return m_doubles[channel][name];
}

Bool_t TChannelInfo::FriendTo(TTree* chain, const char* channelBranchName, const char* fileLocation) {
   //Attach this TChannelInfo's objects as an indexed friend chain to the provided chain/tree
   //the channelBranchName should match the name of the branch in the main chain that contains the channel id
   //fileLocation optionally specifies where to create the temporary file that the friend chain is constructed from

   //get the tree and write it out to our temp file
   TTree* t = GetTree(channelBranchName,false);
   TString sLocation(fileLocation); 
   if(sLocation=="") { //friend as a TTree.. Not Proof compatible!! Also user must delete the clone!
      TTree* cp = t->CloneTree(); cp->SetDirectory(0);
      chain->AddFriend(cp);
      return true;
   }
   sLocation += TString::Format("/%s_channelInfoTree.root",GetName());

   TFile f1(sLocation,"RECREATE");
   t->Write("channelInfoTree");
   f1.Close();

   if(m_friendChain==0) { //shouldn't share the chain because when tchain deleted, it deletes the friends
      TChain* friendChain = new TChain("channelInfoTree");
      Int_t n = friendChain->Add(sLocation);
      if(n==0) {
         Error("FriendTo","Could not add file %s to Friend Chain",sLocation.Data());
         delete friendChain; m_friendChain=0;
         return false;
      }
      friendChain->BuildIndex(channelBranchName);
      chain->AddFriend(friendChain);
   }

   
   return true;
}


TTree* TChannelInfo::GetTree(const char* channelBranchName,bool showWarnings) { 
   //builds a TTree from the contained information. Indexed by channel number
   //Note that each call to this method destroys the previously created tree
   //e.g.:
   //TTree* t = channelInfo->GetTree();
   //TTree* t2 = channelInfo->GetTree(); //t is now 'bad' and should not be used
   //You can always create a clone of a tree before it is destroyed, using TTree::CloneTree()

   if(m_tree) delete m_tree;
   m_tree = new TTree(TString(this->GetName())+"Tree",TString(this->GetName())+"Tree");
   m_tree->SetDirectory(0);
   Int_t channel=0;
   m_tree->Branch((strlen(channelBranchName)==0) ? "mc_channel_number" : channelBranchName,&channel);
   std::map<TString,Double_t> doubles;
   std::map<TString,Int_t> ints;
   std::map<TString,Long64_t> longs;
   std::map<TString,std::vector<Char_t> > strings;

   //create a branch for each field
   for(std::map<TString,Int_t>::iterator type=m_types.begin();type!=m_types.end();++type) {
      switch(type->second) {
         case -1: case 1: ints[type->first]=0;m_tree->Branch(type->first,&(ints[type->first]));
                  if(type->second>0) {
                     m_tree->Branch(type->first + "_entries",&(longs[type->first+"_entries"]));
                     m_tree->Branch(type->first + "_sumw2",&(doubles[type->first + "_sumw2"]));
                  }
                  break;
         case -2: case 2: doubles[type->first]=0;m_tree->Branch(type->first,&(doubles[type->first]));
                  if(type->second>0) {
                     m_tree->Branch(type->first + "_entries",&(longs[type->first+"_entries"]));
                     m_tree->Branch(type->first + "_sumw2",&(doubles[type->first + "_sumw2"]));
                  }
                  break;
         case 0:  
                  TString tmp = type->first + "/C";
                  strings[type->first].reserve(1000);strings[type->first][0]='\0';
                  m_tree->Branch(type->first,&(strings[type->first][0]),tmp.Data());
                  break;
      }
   } 
   //now loop over the channels and fill the TTree 
   for(std::map<Int_t,std::map<TString,Double_t> >::iterator it=m_doubles.begin();it!=m_doubles.end();++it) {
      channel=it->first;
      for(std::map<TString,Int_t>::iterator type=m_types.begin();type!=m_types.end();++type) {
         switch(type->second) {
            case 0: //strings
               if(m_strings[channel].find(type->first)==m_strings[channel].end()) {
                  strcpy(&(strings[type->first][0]),"");
                  if(showWarnings) Warning("GetTree","Channel %d does not have Info %s",channel,(type->first).Data());
               } else {
                  strcpy(&(strings[type->first][0]),m_strings[channel][type->first].Data());
               }
               break;
            case 1: case -1: //ints, all stored in m_doubles 
               if(m_doubles[channel].find(type->first)==m_doubles[channel].end()) {
                  ints[type->first]=0;
                  longs[type->first + "_entries"]=0;
                  doubles[type->first + "_sumw2"]=0;
                  if(showWarnings) Warning("GetTree","Channel %d does not have Info %s",channel,(type->first).Data());
               } else {
                  ints[type->first]=m_doubles[channel][type->first];
                  longs[type->first + "_entries"]=m_doublesEntries[channel][type->first];
                  doubles[type->first + "_sumw2"]=m_doublesW2[channel][type->first];
               }
               break;
            case 2: case -2: //doubles
               if(m_doubles[channel].find(type->first)==m_doubles[channel].end()) {
                  doubles[type->first]=0;
                  longs[type->first + "_entries"]=0;
                  doubles[type->first + "_sumw2"]=0;
                  if(showWarnings) Warning("GetTree","Channel %d does not have Info %s",channel,(type->first).Data());
               } else {
                  doubles[type->first]=m_doubles[channel][type->first];
                  longs[type->first + "_entries"]=m_doublesEntries[channel][type->first];
                  doubles[type->first + "_sumw2"]=m_doublesW2[channel][type->first];
               }
               break;

         }
      }
      m_tree->Fill();
   }
   m_tree->BuildIndex((strlen(channelBranchName)==0) ? "mc_channel_number" : channelBranchName);
   m_tree->ResetBranchAddresses();
   return m_tree;

}

Int_t TChannelInfo::ReadFile(const char* filename,const char* channelBranchName) {
   //Read metadata from an text file into the TChannelInfo
   //Can read Int, Double and Char branches

   //read the file as a TTree, then loop over the branches and load the information we can
   //files always loaded as fixed data
   TTree* tree = new TTree("ChannelInfo","ChannelInfo");
   tree->ReadFile(filename);

   Int_t chanNum = 0;
   std::map<TString,Double_t> doubles;
   std::map<TString,Int_t> ints;
   std::map<TString,std::vector<Char_t> > strings;

   if(tree==0) {Error("ReadTree","Invalid Tree"); return -1;}
   TObjArray *leaves = tree->GetListOfLeaves();
   if(leaves==0) {Error("ReadTree","No leaves"); return -1; }
   for(Int_t i=0;i<leaves->GetEntries();++i) {
      TLeaf *leaf = (TLeaf *)leaves->At(i);
      if(leaf) {
         TBranch *branch = leaf->GetBranch();
         if(strcmp(branch->GetName(),channelBranchName)==0) {
            //this is the channel branch
            if(strcmp(leaf->GetTypeName(),"Int_t")!=0) {
               Error("ReadTree","Channel Branch must be type Int_t"); return -1;
            }
            branch->SetAddress(&chanNum);
         } else if(strcmp(leaf->GetTypeName(),"Double_t")==0) {
               branch->SetAddress(&(doubles[branch->GetName()]));
         } else if(strcmp(leaf->GetTypeName(),"Int_t")==0) {
               branch->SetAddress(&(ints[branch->GetName()]));
         } else if(strcmp(leaf->GetTypeName(),"Char_t")==0) {
               strings[branch->GetName()].reserve(1000);
               strings[branch->GetName()][0] = '\0';
               branch->SetAddress(&(strings[branch->GetName()][0]));
         } else {
               Warning("ReadTree","Can only read Int_t, Double_t or Char_t. Cannot read: %s",branch->GetName());
         }
      }
   }

   //loop over tree entries and read in the values according to property type 
   for(Int_t i=0;i<tree->GetEntries();++i) {
      tree->GetEntry(i);
      //loop over doubles 
      for(std::map<TString, Double_t>::iterator it = doubles.begin();it!=doubles.end();++it) {
         if(m_doubles[chanNum].find(it->first)!=m_doubles[chanNum].end() && m_doubles[chanNum][it->first]!=it->second) {
            Warning("ReadFile", "Overriding metadata [%d,%s]. %f becomes %f",chanNum,(it->first).Data(),m_doubles[chanNum][it->first],it->second);
         }
         m_doubles[chanNum][it->first] = it->second;
         m_types[it->first]=-2;
      }
      //loop over ints
      for(std::map<TString, Int_t>::iterator it = ints.begin();it!=ints.end();++it) {
         if(m_doubles[chanNum].find(it->first)!=m_doubles[chanNum].end() && m_doubles[chanNum][it->first]!=it->second) {
            Warning("ReadFile", "Overriding metadata [%d,%s]. %f becomes %d",chanNum,(it->first).Data(),m_doubles[chanNum][it->first],it->second);
         }
         m_doubles[chanNum][it->first] = it->second;
         m_types[it->first]=-1;
      }
      //loop over strings
      for(std::map<TString, std::vector<Char_t> >::iterator it = strings.begin();it!=strings.end();++it) {
         TString tmp = &(it->second[0]);
         if(m_strings[chanNum].find(it->first)!=m_strings[chanNum].end() && m_strings[chanNum][it->first]!=tmp) {
            Warning("ReadFile", "Overriding metadata [%d,%s]. %s becomes %s",chanNum,(it->first).Data(),m_strings[chanNum][it->first].Data(),tmp.Data());
         }
         m_strings[chanNum][it->first] = tmp;
         m_types[it->first]=0;
      }
   }
   tree->ResetBranchAddresses();
   delete tree;
   if(m_tree) {delete m_tree; m_tree=0;}
   return 0;
}

Int_t TChannelInfo::Cutflow(const char* wildcmp, const char* groups){
   //Print the cutflow
   //equivalent to:
   //Scan("$cutflow(wildcmp),groups,"%f (%d)","$value:$entries")
   //see TChannelInfo::Scan for further details

   return Scan(TString::Format("$cutflow(%s)",wildcmp),groups,"%g (%ld)","$value:$entries");
}





Int_t TChannelInfo::Scan(const char* names, const char* groups, const char* format, const char* formulae, const char* separator) {
   //Scan is a way to print information from the TChannelInfo object.
   //names: the names of the properties you wish to print (each appears on it's own row).
   //       the list should be colon deliminated, e.g. "prop1:prop2"
   //       Special characters for the property names are:
   //          $blank: prints a blank row
   //          $cutflow: inserts the cutflow order
   //          $cutflow(filter): inserts the cutflow items with name matching '*filter*' wildcard
   //          $notable: prints without a table
   //
   //groups:   The groupings of the channels for each item in the scan
   //          Special characters for this is:
   //             #: which is a shortcut for 'mc_channel_number'
   //             1: All channels
   //             channels: Does separate column for each channel
   //
   //format: A c-style format for what to display. must match the number of formulae provided in next parameter
   //
   //formulae: Formulae constructed from the values of each property (which have been summed across the group)
   //          Each is separated by a ":" or the value of the separator parameter (optional)
   //          Special chars:
   //             
   //             $value: whatever the value of this property (given by the current "name") is 
   //             $entries: number of entries for this property
   //             $error: sqrt(sumw2) for this property
   //             $errup: upper limit of a 68% confidence interval, centered on the value (assumes poisson) 
   //             $errdown: lower limit of a 68% confidence interval, centered on the value (assumes poisson) 
   //             $first: whatever the value of the first property in the name list is
   //             $last: whatever the value of the last property in the name list is 
   //             $previous: whatever the value of the previous property in the name list is (in case of first, just uses itself)
   //             $next: whatever the value of the next property in the name list is (in case of last, just uses itself)
   //
   //separator: The formulae separator string, defaults to ":"

   //std::cout << format << std::endl;

   //use format to determine how many args we have
   TString sFormat(format);

   bool useDefaultFormat=false;
   if(sFormat.Length()==0) {
       //default format = $value ($entries)
       sFormat = "%g (%d)";
      useDefaultFormat=true;
   }

   int nArgs = sFormat.CountChar('%');
   //don't count the times where %% appears
   int pos(0);
   while(sFormat.Index("%%",pos)!=-1) {
      pos = sFormat.Index("%%",pos)+1;
      nArgs--;
   }
   //std::cout << "gr=" <<groups << std::endl;

   std::vector<TString> sFormulae;

   //special chars in names: 
   //$blank: just leaves a blank line
   //$cutflow: inserts cutflow
   //$notable: doesn't print a nice table, instead will print a simple comma deliminated list of group values

   //special chars used in groups:
   //#: the mc_channel_number, effectively

   //special chars used in formulae:
   

   if(useDefaultFormat) {
      sFormulae.push_back("$value");
      sFormulae.push_back("$entries");
   } else {
      //split formulae by separator character, exception is if default ":" char, don't split "::";
      bool isDefault = (strcmp(separator,":")==0);
      TString strFormulae(formulae);
      int fStart=0;int nextSep=strFormulae.Index(separator);
      while(nextSep>=0) {
         if(isDefault && nextSep!=(strFormulae.Length()-1) && strFormulae(nextSep+1)==':') {
            //skip this. is the special case "::";
            nextSep = strFormulae.Index(separator,nextSep+2);continue;
         }
         sFormulae.push_back(strFormulae(fStart,nextSep-fStart));
         fStart = nextSep+1;
         nextSep = strFormulae.Index(separator,fStart);
      }
      //don't miss the last one 
      sFormulae.push_back(strFormulae(fStart,strFormulae.Length()-fStart));

      /*va_list arguments;
      va_start (arguments, va_(format));
      for(int i=0;i<nArgs;i++) {
         const char* f = va_arg(arguments,const char*);
         TString fs(f);
         sFormulae.push_back(fs);
      }
      va_end(arguments);*/
   }

   //split names into a nameVector
   TStringToken st(names,":");
   std::vector<TString> nameVector;while(st.NextToken()) {
      TString ss = (TString)st;
      if(ss.BeginsWith("$cutflow")) {
         //extract the wildcmp if any $cutflow(wildcmp)
         TString wildcmp=""; 
         if(ss.Length()>8) wildcmp=ss(9,ss.Length()-10);
         for(unsigned int i=0;i<m_cutflow.size();i++) {
            if(!m_cutflow[i].BeginsWith(wildcmp)) continue;
            nameVector.push_back(m_cutflow[i]);
         }
      } else {nameVector.push_back((TString)st); }
   }

   std::vector<std::vector<TString> > output; //stored in rows and columns
   output.resize(nameVector.size()+1); //one extra for headings
   output[0].push_back(""); //top left corner cell

   TTree *tree = GetTree();

   std::vector<int> columnWidths;
   columnWidths.push_back(10); //for names

   TString sGroups(groups);
   if(sGroups=="channels") {
         //add a formula for each channel 
         auto chanMap = GetMap();
         sGroups = "";
         for(auto& c : *chanMap) {
            if(sGroups=="") sGroups += TString::Format("#==%d",c.first);
            else sGroups += TString::Format(":#==%d",c.first);
         }
         if(sGroups=="") sGroups="1";
   }

   std::vector<TTreeFormula*> groupFormulae;
   TStringToken groupsToken(sGroups,":");


   int j=0; //to label groupformulae
   while(groupsToken.NextToken()) {
      TString group = (TString)groupsToken;

      

      //replace "1" with "All"
      if(group=="1") {output[0].push_back("All");columnWidths.push_back(3);}
      else {
         output[0].push_back(group);
         columnWidths.push_back(group.Length());
      }
      group.ReplaceAll("#","mc_channel_number"); //alias for the channel number
      TTreeFormula *f = new TTreeFormula(Form("gf%d",j),group,tree);
      groupFormulae.push_back(f);
      j++;
   }
   std::cout << std::endl;


   TString firstProperty = ""; TString lastProperty = "";


   bool noTable(false);
   for(unsigned int i=0;i<nameVector.size();i++) {
      TString thisProperty = nameVector[i];
      if(thisProperty=="$notable") { noTable=true; continue; }
      if(thisProperty=="$blank") { output[i+1].push_back("");continue;}
      if(thisProperty.Length()>columnWidths[0]) columnWidths[0]=thisProperty.Length(); //resizes first column 
      output[i+1].push_back(thisProperty);
      //make the TTreeFormulae for this particular property, using the sFormulae vector of strings 
      std::vector<TTreeFormula*> partFormulae; //the parts get combined according to the format string to make the cell entry
      std::vector<std::vector<Double_t> > results; //results of formulae (summed) for each group 
      results.resize(groupFormulae.size());
      std::vector<int> combineRule; //0=add, 1=add in quadrature
      for(unsigned int j=0;j<sFormulae.size();j++) {
         TString formula(sFormulae[j]); //make a copy
         formula.ReplaceAll("$value",thisProperty);formula.ReplaceAll("$entries",thisProperty+"_entries");
         formula.ReplaceAll("$first",nameVector[0]);formula.ReplaceAll("$first_entries",nameVector[0]+"_entries");
         formula.ReplaceAll("$last",nameVector[nameVector.size()-1]);formula.ReplaceAll("$last_entries",nameVector[nameVector.size()-1]+"_entries");
         formula.ReplaceAll("$next",nameVector[((i+1 < nameVector.size()) ? i+1 : i)]);formula.ReplaceAll("$next_entries",nameVector[((i+1 < nameVector.size()) ? i+1 : i)]+"_entries");
         formula.ReplaceAll("$previous",nameVector[( (i>0) ? i-1 : i )]);formula.ReplaceAll("$previous_entries",nameVector[( (i>0) ? i-1 : i )]+"_entries");

         int c = (formula.Contains("$error") || formula.Contains("$errup") || formula.Contains("$errdown")) ? 1 : 0;
         formula.ReplaceAll("$error",Form("sqrt(%s_sumw2)",thisProperty.Data()));
         formula.ReplaceAll("$errup",Form("(0.5*TMath::ChisquareQuantile(1-0.1586555,2.*(%s+1)) - %s)",thisProperty.Data(),thisProperty.Data()));
         formula.ReplaceAll("$errdown",Form("(%s - 0.5*TMath::ChisquareQuantile(0.1586555,2.*(%s)))",thisProperty.Data(),thisProperty.Data()));
         TTreeFormula *f = new TTreeFormula(Form("f%d",j),formula,tree);
         partFormulae.push_back(f);
         combineRule.push_back(c);
         for(unsigned int j=0;j<groupFormulae.size();j++) results[j].push_back(0);
      }

      //iterate over rows of TTree... decide which group the row belongs to (if any), evaluate the formula, and add to the group result
      for(long ie=0;ie<tree->GetEntries();ie++) {
         tree->GetEntry(ie);
         for(unsigned int j=0;j<groupFormulae.size();++j) {
            if(groupFormulae[j]->EvalInstance()) { //does this row belong in this group
               for(unsigned int k=0;k<partFormulae.size();k++) {
                  double tmp = partFormulae[k]->EvalInstance();
                  if(!partFormulae[k]->GetNdim()) continue; //bad formula
                  //must decide how to combine this result with existing results 
                  switch(combineRule[k]) {
                     case 0: results[j][k] += tmp; break;
                     case 1: results[j][k] = sqrt(results[j][k]*results[j][k] + tmp*tmp); break;
                  }
               }
            }
         }
      }
      //clean up the partFormulae 
      for(unsigned int k=0;k<partFormulae.size();k++) delete partFormulae[k];
      partFormulae.clear();

      //prepare the cell contents for each group 
      for(unsigned int j=0;j<groupFormulae.size();++j) {
         TString cellValue = sFormat; //make copy, now replace %f and %d with subsequent results
         int k=0; //the parts of the cell
         int idx = cellValue.Index("%",0);
         while(idx!=-1) {
            if(idx==cellValue.Length()-1) break; //reached end 
            if(cellValue[idx+1]=='%') {
               //just a %%, so skip 
               idx = cellValue.Index("%",idx+2);
            } else if(cellValue[idx+1]=='f') {
               //replace this %f with a floating point result 
               cellValue = Form("%s%f%s",TString(cellValue(0,idx)).Data(),float(results[j][k]),TString(cellValue(idx+2,cellValue.Length())).Data());
               idx = cellValue.Index("%",idx); k++;
            } else if(cellValue[idx+1]=='g') {
              cellValue = Form("%s%g%s",TString(cellValue(0,idx)).Data(),float(results[j][k]),TString(cellValue(idx+2,cellValue.Length())).Data());
              idx = cellValue.Index("%",idx); k++;
            } else if(cellValue[idx+1]=='d') {
               cellValue = Form("%s%d%s",TString(cellValue(0,idx)).Data(),int(results[j][k]),TString(cellValue(idx+2,cellValue.Length())).Data());
               idx = cellValue.Index("%",idx); k++;
            } else if(cellValue[idx+1]=='l' && cellValue[idx+2]=='d') {
               cellValue = Form("%s%ld%s",TString(cellValue(0,idx)).Data(),long(results[j][k]),TString(cellValue(idx+3,cellValue.Length())).Data());
               idx = cellValue.Index("%",idx); k++;
            } 
         }
         output[i+1].push_back(cellValue);
         if(cellValue.Length()>columnWidths[j+1]) columnWidths[j+1]=cellValue.Length();
      }
   }

   //finally, print all the strings
   for(unsigned int i=0;i<output.size();i++) {
      for(unsigned int j=0;j<output[i].size();j++) {
         output[i][j].Resize(columnWidths[j]);
         if(noTable) { if(j>0) std::cout << output[i][j]; } else std::cout << "|" << output[i][j];
         if(noTable && j!=output[i].size()-1 && j!=0) std::cout << ",";
      }
      if(!noTable) std::cout << "|" << std::endl; else std::cout << std::endl;
      if(i==0||i==(output.size()-1)) {
         for(unsigned int j=0;j<output[i].size();j++) {
            std::cout << "-"; //for border marker
            for(int k=0;k<columnWidths[j];k++) {
               std::cout << "-";
            }
         }
         std::cout << "-" << std::endl;
      }
   }

   std::cout << std::endl;

   return 0;
}

void TChannelInfo::Diff(TChannelInfo* rhs, const char* what) {
   //loop over values and entries, comparing the thing given for each channel. report any differences 
   std::map<Int_t,std::map<TString,Double_t> >* rhsMap = rhs->GetMap();
   std::map<Int_t,std::map<TString,Long64_t> >* rhsEntriesMap = rhs->GetEntriesMap();
   
   for(std::map<Int_t,std::map<TString,Long64_t> >::iterator it = m_doublesEntries.begin();it!=m_doublesEntries.end();++it) {
      int chanNum = it->first;
      if(rhsEntriesMap->find(chanNum)==rhsEntriesMap->end()) { Info("Diff", "Me has %d, RHS does not",chanNum); continue; }

      std::map<TString,Long64_t>& tmpMap = rhsEntriesMap->at(chanNum);
      for(std::map<TString,Long64_t>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
         if((it2->first+"_entries")==what) {
            if(tmpMap.find(it2->first)==tmpMap.end()) { Info("Diff", "Me has [%d,%s=%lld], RHS has [%d,-]",chanNum,what,it2->second,chanNum); break; }
            else if(tmpMap[it2->first]!=it2->second) { Info("Diff", "Me has [%d,%s=%lld], RHS has [%d,%s=%lld]",chanNum,what,it2->second,chanNum,what,tmpMap[it2->first]); break; }
         }
      }
   }

   for(std::map<Int_t,std::map<TString,Double_t> >::iterator it = m_doubles.begin();it!=m_doubles.end();++it) {
      int chanNum = it->first;
      if(rhsMap->find(chanNum)==rhsMap->end()) { /*Info("Diff", "Me has %d, RHS does not",chanNum);*/ continue; }

      std::map<TString,Double_t>& tmpMap = rhsMap->at(chanNum);
      for(std::map<TString,Double_t>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
         if((it2->first)==what) {
            if(tmpMap.find(it2->first)==tmpMap.end()) { Info("Diff", "Me has [%d,%s=%f], RHS has [%d,-]",chanNum,what,it2->second,chanNum); break; }
            else if(tmpMap[it2->first]!=it2->second) { Info("Diff", "Me has [%d,%s=%f], RHS has [%d,%s=%f]",chanNum,what,it2->second,chanNum,what,tmpMap[it2->first]); break; }
         }
      }
   }

}

//void Print(const char* format, ... );
void TChannelInfo::Print(Option_t*) const {
   //Lists the names of the properties available 
   std::cout << "TChannelInfo[" << GetName() << "] : ";
   for(std::map<TString,Int_t>::const_iterator it=m_types.begin();it!=m_types.end();++it) {
      std::cout << it->first << " (type=" << it->second << ") ,";
   }
   std::cout << std::endl;
}

void TChannelInfo::CompareToAMI(const char* datasetField, const char* whatME,const char* whatAMI) {

   TString sWhatME(whatME);
   if(sWhatME.EndsWith("_entries")) {
      for(std::map<Int_t,std::map<TString,Long64_t> >::iterator it = m_doublesEntries.begin();it!=m_doublesEntries.end();++it) {
         int chanNum = it->first;
         if(m_strings.find(chanNum)==m_strings.end() || m_strings[chanNum].find(datasetField)==m_strings[chanNum].end()) {
            Error("CompareToAMI","Could not find %s for channel %d",datasetField,chanNum); continue;
         }
         TString datasetName = m_strings[chanNum][datasetField]; 
         if(datasetName=="") continue;
         for(std::map<TString,Long64_t>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            if((it2->first+"_entries")==whatME) {
               TString result = gSystem->GetFromPipe(TString::Format("ami dataset info %s | awk '/%s/{ print $2}'",datasetName.Data(),whatAMI));
               if(it2->second != result.Atoi()) {
                  Warning("CompareToAMI", "Channel %d (%s): Me has %lld, AMI has %d",chanNum,datasetName.Data(),it2->second,result.Atoi());
               } else { Info("CompareToAMI", "Channel %d (%s) match!",chanNum,datasetName.Data()); }
               break;
            }
         }
      }
   }
}

Int_t TChannelInfo::Merge(TCollection *coll) {
   if(!coll) return 0;
   if(coll->IsEmpty()) return 0;
   if(m_tree){delete m_tree;m_tree=0;}//resets TTree
   // Iterate over the elements of the collection:
   TIter next( coll );
   TObject* obj = 0;
   while( ( obj = next() ) ) {

      // Check that the element is of the right type:
      TChannelInfo* vobj = dynamic_cast< TChannelInfo* >( obj );
      if( ! vobj ) {
         Error( "Merge", "Unknown object type encountered: %s",obj->ClassName() );
         return 0;
      }

      std::map<Int_t,std::map<TString,Double_t> >* rhsMap = vobj->GetMap();
      std::map<Int_t,std::map<TString,Long64_t> >* rhsEntriesMap = vobj->GetEntriesMap();
      std::map<Int_t,std::map<TString,Double_t> >* rhsSumw2Map = vobj->GetSumw2Map();
      std::map<Int_t,std::map<TString,TString> >* rhsStringMap = vobj->GetStringMap();

      std::map<TString,Int_t>* rhsTypes = vobj->GetTypeMap();

      for(std::map<TString,Int_t>::iterator rhsType = rhsTypes->begin();rhsType!=rhsTypes->end();++rhsType) {
         switch(rhsType->second) {
            case 0: //string
               for(std::map<Int_t,std::map<TString,TString> >::iterator rhsStrings = rhsStringMap->begin();rhsStrings!=rhsStringMap->end();++rhsStrings) {
                  if(rhsStrings->second.find(rhsType->first)!=rhsStrings->second.end()) {
                     //this channel has this property. Attempt to override
                     if(m_strings[rhsStrings->first].find(rhsType->first)!=m_strings[rhsStrings->first].end() && m_strings[rhsStrings->first][rhsType->first]!=rhsStrings->second[rhsType->first]) {
                        Warning("Merge", "Overriding string field [%d,%s]. %s becomes %s",rhsStrings->first,rhsType->first.Data(),m_strings[rhsStrings->first][rhsType->first].Data(),rhsStrings->second[rhsType->first].Data());
                     }
                     m_strings[rhsStrings->first][rhsType->first]=rhsStrings->second[rhsType->first];
                     m_types[rhsType->first] = rhsType->second;
                  }
               }
               break;
            default: //something else 
               for(std::map<Int_t,std::map<TString,Double_t> >::iterator rhs = rhsMap->begin();rhs!=rhsMap->end();++rhs) {
                  if(rhs->second.find(rhsType->first)!=rhs->second.end()) {
                     //this channel has this property.
                     if(rhsType->second < 0) {
                        //attempt to override;
                        if(m_doubles[rhs->first].find(rhsType->first)!=m_doubles[rhs->first].end() && m_doubles[rhs->first][rhsType->first]!=rhs->second[rhsType->first]) {
                           Warning("Merge", "Overriding string field [%d,%s]. %f becomes %f",rhs->first,rhsType->first.Data(),m_doubles[rhs->first][rhsType->first],rhs->second[rhsType->first]);
                        }
                        m_doubles[rhs->first][rhsType->first]=rhs->second[rhsType->first];
                     } else {
                        //simple add
                        m_doubles[rhs->first][rhsType->first]+=rhs->second[rhsType->first];
                        m_doublesEntries[rhs->first][rhsType->first]+=(*rhsEntriesMap)[rhs->first][rhsType->first];
                        m_doublesW2[rhs->first][rhsType->first] += (*rhsSumw2Map)[rhs->first][rhsType->first];
                     }
                     m_types[rhsType->first] = rhsType->second;
                  }
                  
               }
               break;
         }

      }

      //also merge in the cutflow order.
      std::vector<TString>* rhsCutflow = vobj->GetCutflowOrder();

      int currentPosition = 0; //where we will insert the next missing cutflow
      for(unsigned int i=0;i<rhsCutflow->size();i++) {
         bool found=false;
         //try to find this entry by searching from current position to end
         for(int j=0;j<int(m_cutflow.size());j++) {
            if(rhsCutflow->at(i)==m_cutflow[j]) { currentPosition=j+1;found=true;break; }
         }
         if(!found) {
            //insert the rhs's cutflow into this object's cutflow
            //put it just below whatever we last matched (held in currentPosition)
            m_cutflow.push_back(TString(""));
            if(m_cutflow.size()==1) m_cutflow[0] = rhsCutflow->at(i);
            else {
               for(int j=int(m_cutflow.size())-2;j>=currentPosition;j--) {
                  m_cutflow[j+1] = m_cutflow[j];
               }
            }
            m_cutflow[currentPosition] = rhsCutflow->at(i);
            currentPosition++;
         }
      }

   }

   return 1;
}

#if defined(__MAKECINT__)
#pragma link C++ class TChannelInfo;
#endif


