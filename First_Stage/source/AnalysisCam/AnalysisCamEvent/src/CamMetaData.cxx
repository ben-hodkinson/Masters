#include "AnalysisCamEvent/CamMetaData.h"

#include "TLeaf.h"
#include "TH1F.h"

#include "GaudiKernel/PathResolver.h"

CamMetaData::CamMetaData(const std::string& type, const std::string& name,
			 const IInterface* pParent) : CamTool(type, name, pParent), 
   m_metaTree(0),
   m_dump(false)

{

   declareProperty("ReadFiles",m_readFiles,"Files to read in and store as metadata");
   declareProperty("IndexBranches",m_indexBranches={"mcChannelNumber"},"Branches to use as indices when looking up metadata, up to 2 allowed - must be ints");

   declareProperty("Dump",m_dump=false,"Print all MetaData contents at start of each file");


}

// Destructor
CamMetaData::~CamMetaData() {
}


Int_t CamMetaData::ReadFile(const char* filename) {
   ATH_MSG_INFO("Reading File: " << filename);
   if(!m_metaTree) {m_metaTree = new TTree("MetaData","MetaData"); m_metaTree->SetDirectory(0);}
   TTree* tree = m_metaTree;
   tree->ReadFile(filename);

   return 0;
}


StatusCode CamMetaData::initialize() {


   //load predefined branches, if specified
   if(m_readFiles.size()>0) {
      for(unsigned int i=0;i<m_readFiles.size();i++) {
         std::string tmp = m_readFiles[i];
         std::string tmp2 = System::PathResolver::find_file(tmp,"DATAPATH");
         if(tmp2=="") { ATH_MSG_ERROR("Unable to find the file: " << tmp); return StatusCode::FAILURE; }
         ReadFile(tmp2.c_str());
      }
      //build index with specified branches
      if(m_indexBranches.size()>0 && m_indexBranches.size()<3) {
         if(m_metaTree) m_metaTree->BuildIndex(m_indexBranches[0].c_str(),(m_indexBranches.size()>1)? m_indexBranches[1].c_str() : "0");
         ATH_MSG_DEBUG("Built index with: " << m_indexBranches[0] );
      }

      //do the notify now, as the tool only gets initialized once inside the loop
      return beginInputFile();
   }

   return StatusCode::SUCCESS;
}

StatusCode CamMetaData::finalize() {
   if(m_metaTree) delete m_metaTree;
   return StatusCode::SUCCESS;
}

StatusCode CamMetaData::beginInputFile() {
   if(m_dump) {
      CamEvent evt;
      ATH_MSG_INFO("MetaData for current file (" << currentFile()->GetName() << "):");

      if(m_metaTree) {
         std::cout << "meta_tree_info:" << std::endl;
         if(m_indexBranches.size()==1) {
            if(!evt.contains<int>(m_indexBranches[0]) && !evt.contains<uint>(m_indexBranches[0])) return StatusCode::SUCCESS;
            m_metaTree->GetEntryWithIndex(evt.get<int>(m_indexBranches[0]));
            m_metaTree->Show();
         } else if(m_indexBranches.size()==2) {
            if((!evt.contains<int>(m_indexBranches[0]) && !evt.contains<uint>(m_indexBranches[0])) || (!evt.contains<int>(m_indexBranches[1]) && !evt.contains<uint>(m_indexBranches[1]))) return StatusCode::SUCCESS;
            m_metaTree->GetEntryWithIndex(evt.get<int>(m_indexBranches[0]),evt.get<int>(m_indexBranches[1]));
            m_metaTree->Show();
         }
      }
   }

   return StatusCode::SUCCESS;
}

std::string CamMetaData::getMetaString(CamObject& event, va_list args) {
   //there should only be one arg
   std::string key = va_arg(args,char*);
   //check for branch of type "Char_t" in the metaTree
   if(!m_metaTree) {
      ATH_MSG_ERROR("No meta tree. Cannot retrieve " << key); throw 10;
   }
   TLeaf* b = m_metaTree->FindLeaf(key.c_str());

   TString leafType=b->GetTypeName();
   if(leafType(0)!='C') {
      ATH_MSG_ERROR("Could not find " << key << " in meta tree"); throw 10;
   }


    //hunt for entry by index matching
   int index[2] = {0,0};
   for(uint i=0;i<2;i++) {
      if(m_indexBranches.size()<=i) break;
      if(event.contains<int>(m_indexBranches[i])) index[i] = event.get<int>(m_indexBranches[i]);
      else if(event.contains<unsigned int>(m_indexBranches[i])) index[i] = event.get<unsigned int>(m_indexBranches[i]);
      else {ATH_MSG_ERROR("Could not find index property: " << m_indexBranches[i]);throw 10;}
   }
   if(m_metaTree->GetEntryWithIndex(index[0],index[1])==-1) {ATH_MSG_ERROR("Could not find meta_string: " << key << " for event with " << m_indexBranches << " =(" << index[0] << "," << index[1] << ")");throw 10;}
   std::string myString = ((char*)(b->GetValuePointer()));
   return myString;


}

double CamMetaData::getMetaDouble(CamObject& event, va_list args) {
   //there should only be one arg
   std::string key = va_arg(args,char*);
   //check for branch of type "Double_t" in the metaTree
   if(!m_metaTree) {
      ATH_MSG_ERROR("No meta tree. Cannot retrieve " << key); throw 10;
   }
   TLeaf* b = m_metaTree->FindLeaf(key.c_str());

   TString leafType=b->GetTypeName();
   if(leafType(0)!='D') {
      ATH_MSG_ERROR("Could not find " << key << " in meta tree"); throw 10;
   }


    //hunt for entry by index matching
   int index[2] = {0,0};
   for(uint i=0;i<2;i++) {
      if(m_indexBranches.size()<=i) break;
      if(event.contains<int>(m_indexBranches[i])) index[i] = event.get<int>(m_indexBranches[i]);
      else if(event.contains<unsigned int>(m_indexBranches[i])) index[i] = event.get<unsigned int>(m_indexBranches[i]);
      else {ATH_MSG_ERROR("Could not find index property: " << m_indexBranches[i]);throw 10;}
   }
   if(m_metaTree->GetEntryWithIndex(index[0],index[1])==-1) {ATH_MSG_ERROR("Could not find meta_double: " << key << " for event with " << m_indexBranches << " =(" << index[0] << "," << index[1] << ")");throw 10;}
   return b->GetValue();

}

int CamMetaData::getMetaInt(CamObject& event, va_list args) {
   //there should only be one arg
   std::string key = va_arg(args,char*);
   //check for branch of type "Double_t" in the metaTree
   if(!m_metaTree) {
      ATH_MSG_ERROR("No meta tree. Cannot retrieve " << key); throw 10;
   }
   TLeaf* b = m_metaTree->FindLeaf(key.c_str());

   TString leafType=b->GetTypeName();
   if(leafType(0)!='I') {
      ATH_MSG_ERROR("Could not find " << key << " in meta tree"); throw 10;
   }


   //hunt for entry by index matching
   int index[2] = {0,0};
   for(uint i=0;i<2;i++) {
      if(m_indexBranches.size()<=i) break;
      if(event.contains<int>(m_indexBranches[i])) index[i] = event.get<int>(m_indexBranches[i]);
      else if(event.contains<unsigned int>(m_indexBranches[i])) index[i] = event.get<unsigned int>(m_indexBranches[i]);
      else {ATH_MSG_ERROR("Could not find index property: " << m_indexBranches[i]);throw 10;}
   }
   if(m_metaTree->GetEntryWithIndex(index[0],index[1])==-1) {ATH_MSG_ERROR("Could not find meta_int: " << key << " for event with " << m_indexBranches << " =(" << index[0] << "," << index[1] << ")");throw 10;}
   return b->GetValue();

}

StatusCode CamMetaData::apply(CamObject* event, std::string&, const std::string&) {
/* COMMENTED OUT UNTIL REPLACE WITH std::function !!
   event->set("meta_stream_jetetmiss",this,&CamMetaData::meta_stream_jetetmiss);
   event->set("meta_stream_muons",this,&CamMetaData::meta_stream_muons);
   event->set("meta_stream_egamma",this,&CamMetaData::meta_stream_egamma);

   event->set("meta_mc_beamspot_zWidth",this,&CamMetaData::meta_mc_beamspot_zWidth);
   event->set("meta_mc_year",this,&CamMetaData::meta_mc_year);

   event->set("meta_year",this,&CamMetaData::meta_year);
   event->set("meta_mc_prod",this,&CamMetaData::meta_mc_prod);
*/


   event->set("meta_string") = CamObject::Function(&CamMetaData::getMetaString,this);
   event->set("meta_double") = CamObject::Function(&CamMetaData::getMetaDouble,this);
   event->set("meta_int") = CamObject::Function(&CamMetaData::getMetaInt,this);



    return StatusCode::SUCCESS;
}
