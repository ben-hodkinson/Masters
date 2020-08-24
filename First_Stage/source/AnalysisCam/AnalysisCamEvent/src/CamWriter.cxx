#include "AnalysisCamEvent/CamWriter.h"

#include "AnalysisCamEvent/CamDeque.h"
#include "AnalysisCamEvent/CamObject.h"
#include "AnalysisCamEvent/CamEvent.h"

#include "TTree.h"
#include "TLeaf.h"
#include "GaudiKernel/ITHistSvc.h"

#include <boost/algorithm/string/predicate.hpp> //for starts_with and ends_with methods

#include "TLorentzVector.h"

std::map<TString,CamWriter::branchMakerMethod> CamWriter::branchMakerMethods;

CamWriter::CamWriter(const std::string& name, ISvcLocator *svcLoc) : CamService(name, svcLoc)
, m_doNotFill(false),m_ownsTree(false),nEntries(0),m_predefinedBranches(false)
{

   declareProperty("TreeName",m_streamName="/CAMTUP/CamTree","Name of stream in THistSvc to write TTree to, along with the Tree name");
   declareProperty("Branches",m_treeConfig,"List of tree branches, in notation <name>/<type>[/<alias>] .. if any branches specified then we are in 'pull' mode");
   
   declareProperty("TLVUnits",m_tlvScaleFactor=1.,"can switch to GeV by setting to 0.001"); 
   declareProperty("TLVPrefix",m_tlvPrefix="tlv_","prefix on tlv branches");
   declareProperty("FillEveryEvent",m_fillEveryEvent=false,"Should I do a Fill on every event?");
   declareProperty("EventWritesObjects",m_eventWritesObjects=false,"Should CamEvents write their deques and objects?");
   declareProperty("InactiveBranches",m_blacklist,"List of branches that should not be written");
   declareProperty("ActiveBranches",m_whitelist,"List of branches you MUST write out");
   declareProperty("WriteToStoregate",m_writeToStoregate=false,"Should I also write branches to storegate?");

   declareProperty("TLVFormat",m_tlvFormat="PtEtaPhiM","Specify from Pt, Eta, Phi, M, Px, Py, Pz, E, Et, TLV .. relevent only in 'push' mode");

   m_eventHasStuff=false;


   registerType<bool>();
   registerType<float>();
   registerType<int>();
   registerType<unsigned int>();
   registerType<double>();
   registerType<short>();
   registerType<char>();
   registerType<long>();
   registerType<long long>();
   registerType<unsigned long>();
   registerType<unsigned long long>();
   registerType<TLorentzVector>();
   registerType<std::string>();
   registerType<TLorentzVector>();


}

// Destructor
CamWriter::~CamWriter() {
   for(auto& cb : m_camBranches) {
      cb.second.deleteObject();
   }
}



TBranch* CamWriter::addBranch(std::string branchName, std::string branchIn) {

   
   TString branchType(branchIn.c_str());
   //check for aliases (extra slash in branchIn)
   TString branchAlias = "";
   std::size_t slashPosition = branchIn.find('/');
   if(slashPosition!=std::string::npos) {
      branchAlias = branchIn.substr(slashPosition+1,branchIn.length()-slashPosition);
      branchType = branchIn.substr(0,slashPosition);
   }

   //remove any "std::" in them
   branchType.ReplaceAll("std::","");
   branchType.ReplaceAll(" ","");
   branchType.ReplaceAll("unsigned","unsigned "); //put back the space after unsigned keyword
   branchType.ReplaceAll("longlong","long long");


   if(!m_predefinedBranches) ATH_MSG_INFO("Adding branch : " << branchName << " with type " << branchIn);

   if(branchMakerMethods.find(branchType)!=branchMakerMethods.end()) {
      return (this->*branchMakerMethods[branchType])(branchName,branchType,branchAlias);
   } else {
      return setupCamBranch<void*>(branchName,branchType,branchAlias); //try using void* for branch instead... wont be 
   }
}

#include "GaudiKernel/Property.h"

StatusCode CamWriter::initialize() {

   ServiceHandle<ITHistSvc> histSvc("THistSvc","");
   CHECK( histSvc.retrieve() );

   if(m_streamName=="") {
      m_doNotFill=true;
      //create a dummy tree 
      m_tree = new TTree("dummy","dummy");
      m_tree->SetDirectory(0);
   }
   else if( histSvc->exists(m_streamName) ) { //see if the tree already exists ..
      //tree already exists, so just use that 
      ATH_MSG_INFO("Using existing tree at " <<  m_streamName);
      CHECK(histSvc->getTree(m_streamName,m_tree) );
      m_ownsTree=false;
   } else  {
      //strip off everything but the last part of the treename 
      TString s(m_streamName);
      TString topName = s(1,s.Length());
      topName = topName(0,topName.First('/'));
      TString basename = s(s.Last('/')+1,s.Length());
      m_tree = new TTree(basename,basename);
      ATH_MSG_INFO("Creating " << m_tree->GetName() << " at " << m_streamName);

      //first check the stream exists, if it doesn't we will create it ...
      std::vector<std::string> outputs;
      CHECK(Gaudi::Parsers::parse(outputs,dynamic_cast<IProperty*>(&*histSvc)->getProperty("Output").toString()));
      bool found(false);
      for(auto& o : outputs) {
	std::string stream = o.substr(0,o.find(" "));
	if(topName == stream) {found=true;break;}
      }
      if(!found) {
	outputs.push_back(TString::Format("%s DATAFILE='%s.root' OPT='RECREATE'",topName.Data(),topName.Data()).Data());
	CHECK(dynamic_cast<IProperty*>(&*histSvc)->setProperty("Output",Gaudi::Utils::toString( outputs )));
      }

      if( histSvc->regTree(m_streamName,m_tree).isFailure() ) {
         ATH_MSG_ERROR("Unable to register tree " << basename << " in stream " << m_streamName);
         return StatusCode::FAILURE;
      }
      //record that i created this tree, so i should fill it at the end of the event 
      m_ownsTree = true;
   }

   

   //load predefined branches, if specified
   if(m_treeConfig.size()>0) {
      m_predefinedBranches = true;
      for(unsigned int i=0;i<m_treeConfig.size();i++) {
         std::string tmp = m_treeConfig[i];
         std::size_t slashPosition = tmp.find('/');
         if(slashPosition==std::string::npos) {
            ATH_MSG_ERROR("error in tree config: " << tmp << " has no type def");
            return StatusCode::FAILURE;
         }
         std::string branchName = tmp.substr(0,slashPosition);
         std::string branchT = tmp.substr(slashPosition+1,tmp.length()-slashPosition);
         if(!addBranch(branchName,branchT)) {
            //failed to add this branch ... because predefined, we die here 
            ATH_MSG_ERROR("Failed to add branch:" << branchName << " with type:" << branchT);
            return StatusCode::FAILURE;
         }
      }
   }


   return StatusCode::SUCCESS;
}

//overwrite CamService because only want to listen for endevent 
StatusCode CamWriter::sysInitialize() {
   ServiceHandle<IIncidentSvc> m_incidentSvc("IncidentSvc",name());
   m_incidentSvc->addListener(this,IncidentType::EndEvent);
   return AthService::sysInitialize();
}


StatusCode CamWriter::finalize() {
   if(m_predefinedBranches) {
      //print out the branches that never got filled
      bool printOne(false);
      for(auto& it : m_camBranches) {
         if(!it.second.tBranch->TestBit(3333) && (m_tree->GetEntries()>0 || nEntries>0) ) {
            if(!printOne) {
               ATH_MSG_WARNING("The following branches were predefined but never filled: ");printOne=true;
               std::cout << "  " << it.first;
            } else {
               std::cout << ", " << it.first;
            }
         }
      }
      if(printOne) std::cout << std::endl;
      printOne=false;
      for(std::map<TString,bool>::iterator it = m_nonZeroDequeFilled.begin();it!=m_nonZeroDequeFilled.end();it++) {
         if(it->second==true) continue;
         if(!printOne) {
               ATH_MSG_WARNING("Some of the above branches may not have been filled because the following collections were always empty: ");printOne=true;
         }
         std::cout << "  " << it->first;
      }
      if(printOne) std::cout << std::endl;

   }
  ATH_MSG_INFO("Wrote " << nEntries << " entries");

   return StatusCode::SUCCESS;
}

void CamWriter::fillTree() {
      if(m_fillEveryEvent || m_eventHasStuff) {
         int bytesWritten=0;
         if(!m_doNotFill || !m_ownsTree) bytesWritten=m_tree->Fill();
         nEntries++;
         //re-enable all the voids
         bool hasActiveBranch(false);


         for(auto& it : m_camBranches) {
            if(!it.second.tBranch->TestBit(kDoNotProcess)) { 
               hasActiveBranch=true;
            } else {
               ATH_MSG_VERBOSE("Clearing " << it.second.tBranch->GetName());
               //clear it because it was filled 
               it.second.clear();
               it.second.tBranch->SetStatus(1); //reset it to active
            }
         }
         if(!m_doNotFill && m_ownsTree && hasActiveBranch && bytesWritten<=0) {
            //had an active branch, should have written something, but apparently it didn't. This is terrible, throw exception
            ATH_MSG_FATAL("Failed to Fill TTree... no (or negative) bytes written when expected to. Possible disk space limit hit??");
            throw 10;
         }
      }
      m_eventHasStuff=false;

}

void CamWriter::handle(const Incident& incident) {
   if(incident.type() == IncidentType::EndEvent) {
      fillTree();
   }
}

//check a branchname against the black list and white list. Acceptance takes priority
bool CamWriter::isGoodBranch(TString branchName) {
   //quick check badBranchesMap 
   if(m_badBranches.find(branchName.Data())!=m_badBranches.end()) return false;
   for(std::vector<std::string>::iterator it = m_whitelist.begin();it!=m_whitelist.end();++it) {
      if(wildcmp(it->c_str(),branchName.Data())) return true;
   }

   for(std::vector<std::string>::iterator it = m_blacklist.begin();it!=m_blacklist.end();++it) {
      if(wildcmp(it->c_str(),branchName.Data())){ m_badBranches[branchName.Data()]=true;return false;}
   }
   return true;
}

StatusCode CamWriter::apply(CamObject* object, std::string& label, const std::string& option) {
   m_eventHasStuff=true; //only need it here since the other apply methods call this apply method

   
   //if branches are predefined, we loop over the branches, otherwise we loop over the properties
      if(m_predefinedBranches) {
         bool checkSG = (option.find("checkSG")!=std::string::npos); //if checkSG given as option, can only write the map properties
         for(auto& it : m_camBranches) {
            if(it.second.tBranch->TestBit(kDoNotProcess)) continue; //branch already been filled for this event
            if(!it.first.BeginsWith(label.c_str())) continue; //checks the prefix
            //try to get the required property from the object
            std::string currProp(it.first(label.length(),it.first.Length()).Data());
            //deal with tlv branches
            if(boost::starts_with(currProp, m_tlvPrefix)) {
               double value = 0.;
               if(currProp==m_tlvPrefix+"pt") value = object->Pt()*m_tlvScaleFactor;
               else if(currProp==m_tlvPrefix+"eta") value = object->Eta();
               else if(currProp==m_tlvPrefix+"phi") value = object->Phi();
               else if(currProp==m_tlvPrefix+"m") value = object->M()*m_tlvScaleFactor;
               else if(currProp==m_tlvPrefix+"e") value = object->E()*m_tlvScaleFactor;
               else if(currProp==m_tlvPrefix+"et") value = object->Et()*m_tlvScaleFactor;
               else if(currProp==m_tlvPrefix+"px") value = object->Px()*m_tlvScaleFactor;
               else if(currProp==m_tlvPrefix+"py") value = object->Py()*m_tlvScaleFactor;
               else if(currProp==m_tlvPrefix+"pz") value = object->Pz()*m_tlvScaleFactor;
               if(value) { //assume zero values are nothing
                  AddDouble(&(it.second),"",value);
                  continue;
               }
            }
            //for everything else, try to get from the object ...
            it.second.fillFromCamObject(*object,currProp,true,checkSG);
         } //end of loop over branches
         if(option.find("fillAfterApply")!=std::string::npos) { fillTree(); };
         return StatusCode::SUCCESS;
      }


   //no predefined branches - so loop over the properties

   //decide if we should keep the TLV or not
      TLorentzVector *tlv = dynamic_cast<TLorentzVector*>(object);
      if((*tlv)!=TLorentzVector(0,0,0,0)) {
         std::vector<bool>& tlvOptionVec = getTLVFormatVec(label, option);
         std::string fLabel = label+m_tlvPrefix;
         if(tlvOptionVec[0]) AddDouble(0,fLabel+"pt", tlv->Pt()*m_tlvScaleFactor);
         if(tlvOptionVec[1]) AddDouble(0,fLabel+"eta",tlv->Eta());
         if(tlvOptionVec[2]) AddDouble(0,fLabel+"phi",tlv->Phi());
         if(tlvOptionVec[3]) AddDouble(0,fLabel+"m", tlv->M()*m_tlvScaleFactor);
         if(tlvOptionVec[4]) AddDouble(0,fLabel+"px", tlv->Px()*m_tlvScaleFactor);
         if(tlvOptionVec[5]) AddDouble(0,fLabel+"py", tlv->Py()*m_tlvScaleFactor);
         if(tlvOptionVec[6]) AddDouble(0,fLabel+"pz", tlv->Pz()*m_tlvScaleFactor);
         if(tlvOptionVec[7]) AddDouble(0,fLabel+"e", tlv->E()*m_tlvScaleFactor);
         if(tlvOptionVec[8]) AddDouble(0,fLabel+"et", tlv->Et()*m_tlvScaleFactor);
         if(tlvOptionVec[9]) AddTLV(fLabel,tlv);
      }



      //iterate over the object's properties and write those out
      std::unordered_map<std::string,boost::any>* propMap = object->getPropertyMap();
      for(std::unordered_map<std::string,boost::any>::iterator prop = propMap->begin();prop!=propMap->end();++prop) {
         std::string branchName = label+prop->first;
         if(!isGoodBranch(branchName)) continue;
         //do we already know about this property 
         auto s = m_camBranches.find(branchName);
         if(s==m_camBranches.end()) {
            //try to make the branch
            std::string propType = object->type(prop->first,true,false,true/*parses functions*/); 
            if(propType=="") { 
               ATH_MSG_WARNING("Unknown type for " << prop->first << ". Will prevent from writing!");
               m_badBranches[branchName]=true; continue;
            } else if(propType=="void") { //void function 
               ATH_MSG_INFO("Cannot write void property " << prop->first << ". Will prevent from writing!");
               m_badBranches[branchName]=true; continue;
            } else if(boost::starts_with(propType,"std::function")) {
                ATH_MSG_INFO("Cannot write function property " << prop->first << ". Will prevent from writing!");
               m_badBranches[branchName]=true; continue;
            }
            ATH_MSG_DEBUG("Detected type : " << propType);
            if(!addBranch(branchName,propType)) { m_badBranches[branchName]=true; continue; }
            s= m_camBranches.find(branchName); //should have been created
         }

            m_camBranches[branchName].fillFromCamObject(*object,prop->first,true,false/*don't check storegate*/);
            continue;
         
      }
      //object->setUsePrefix(tmp);
      if(option.find("fillAfterApply")!=std::string::npos) { fillTree(); };
      return StatusCode::SUCCESS;
}


StatusCode CamWriter::apply(CamDeque* deque, std::string& label, const std::string& option) {
   ATH_MSG_DEBUG("applying to deque/vector with label=" << label);


   if(m_predefinedBranches) {
      bool checkSG = (option.find("checkSG")!=std::string::npos); //if checkSG given as option, can only write the map properties
      for(auto& it : m_camBranches) {
         if(it.second.tBranch->TestBit(kDoNotProcess)) continue; //branch already been filled for this event
         if(!it.first.BeginsWith(label.c_str())) continue; //checks the prefix
         //try to get the required property from the object
         TString currProp = it.first(label.length(),it.first.Length());
         if(currProp=="n") {
            if(!m_doNotFill) m_nonZeroDequeFilled[label] = (m_nonZeroDequeFilled[label] || deque->size()>0);
            if(it.second.branchType=="int") branchFiller<int>(it.second,int(deque->size()));
            else if(it.second.branchType=="unsigned int") branchFiller<unsigned int>(it.second,uint(deque->size()));
            else {
               ATH_MSG_FATAL("Branch " << it.first << " must only be int or unsigned int, but has type: " << it.second.branchType);
               return StatusCode::FAILURE;
            }
         }

         //everything else, attempt to 'get from the deque'
         //implicitly it must be a vector type ...
         if(!boost::starts_with(it.second.branchType,"vector<")) continue;

         //deal with tlv branches
         if(currProp.BeginsWith(m_tlvPrefix)) {
            //branch type must be vector of double or float 
            currProp = currProp(m_tlvPrefix.length(),currProp.Length());
            if(currProp=="pt") FillDoubleVector(&it.second,"",&TLorentzVector::Pt,m_tlvScaleFactor,deque,false/*do not create if not existing*/);
            else if(currProp=="eta") FillDoubleVector(&it.second,"",&TLorentzVector::Eta,1.,deque,false/*do not create if not existing*/);
            else if(currProp=="phi") FillDoubleVector(&it.second,"",&TLorentzVector::Phi,1.,deque,false/*do not create if not existing*/);
            else if(currProp=="m") FillDoubleVector(&it.second,"",&TLorentzVector::M,m_tlvScaleFactor,deque,false);
            else if(currProp=="px") FillDoubleVector(&it.second,"",&TLorentzVector::Px,m_tlvScaleFactor,deque,false);
            else if(currProp=="py") FillDoubleVector(&it.second,"",&TLorentzVector::Py,m_tlvScaleFactor,deque,false);
            else if(currProp=="pz") FillDoubleVector(&it.second,"",&TLorentzVector::Pz,m_tlvScaleFactor,deque,false);
            else if(currProp=="e") FillDoubleVector(&it.second,"",&TLorentzVector::E,m_tlvScaleFactor,deque,false);
            else if(currProp=="et") FillDoubleVector(&it.second,"",&TLorentzVector::Et,m_tlvScaleFactor,deque,false);
            continue;
         } //end of tlv branches

         std::string scurrProp(currProp.Data());
         it.second.fillFromCamDeque(*deque,scurrProp,true,checkSG/*check storegate*/);
      } //end of loop over branches
      //write the deque/vector as an object
      if(apply(dynamic_cast<CamObject*>(deque),label,option).isFailure()) {
         ATH_MSG_ERROR("Failed to write properties of CamVector/CamDeque:" << label); return StatusCode::FAILURE;
      }
      return StatusCode::SUCCESS;
   } //end of predefined branches section 


   ///WE GET HERE IF NO PREDEFINED BRANCHES .. writing in 'push' mode

   //write a "_n" branch for the number of objects
   //do we already know about this property 
   std::string branchName = label+"n";
   if(isGoodBranch(branchName)) {
      auto ss = m_camBranches.find(branchName);
      if(ss==m_camBranches.end()) {
         if(!addBranch(branchName,"int")) m_badBranches[branchName]=true;
         else ss = m_camBranches.find(branchName);
      } 

      if(ss!=m_camBranches.end() && !ss->second.tBranch->TestBit(kDoNotProcess)) {
         //fill ...
         
         ATH_MSG_VERBOSE("Filling " << branchName << " with " << deque->size());
         if(ss->second.branchType=="int") branchFiller<int>(ss->second,int(deque->size()));
         else if(ss->second.branchType=="unsigned int") branchFiller<unsigned int>(ss->second,uint(deque->size()));
         else {
            ATH_MSG_FATAL("Branch " << branchName << " must only be int or unsigned int, but has type: " << ss->second.branchType);
            return StatusCode::FAILURE;
         }
      }
   }


   std::vector<bool>& tlvOptionVec = getTLVFormatVec(label, option);
   const std::string fLabel=label+m_tlvPrefix;

   std::map<std::string,bool> doneBranches;
   bool doneTLVBranches=false;

   //we must determine which properties we have in the children .. each time we encounter a new property, fill it from the deque
   for(CamObject* obj : *deque) {
      if(!doneTLVBranches) {
         TLorentzVector *tlv = dynamic_cast<TLorentzVector*>(obj);
         if((*tlv)!=TLorentzVector(0,0,0,0)) {
            if(tlvOptionVec[0]) FillDoubleVector(0,fLabel+"pt",&TLorentzVector::Pt,m_tlvScaleFactor,deque);
            if(tlvOptionVec[1]) FillDoubleVector(0,fLabel+"eta",&TLorentzVector::Eta,1.,deque);
            if(tlvOptionVec[2]) FillDoubleVector(0,fLabel+"phi",&TLorentzVector::Phi,1.,deque);
            if(tlvOptionVec[3]) FillDoubleVector(0,fLabel+"m",&TLorentzVector::M,m_tlvScaleFactor,deque);
            if(tlvOptionVec[4]) FillDoubleVector(0,fLabel+"px",&TLorentzVector::Px,m_tlvScaleFactor,deque);
            if(tlvOptionVec[5]) FillDoubleVector(0,fLabel+"py",&TLorentzVector::Py,m_tlvScaleFactor,deque);
            if(tlvOptionVec[6]) FillDoubleVector(0,fLabel+"pz",&TLorentzVector::Pz,m_tlvScaleFactor,deque);
            if(tlvOptionVec[7]) FillDoubleVector(0,fLabel+"e",&TLorentzVector::E,m_tlvScaleFactor,deque);
            if(tlvOptionVec[8]) FillDoubleVector(0,fLabel+"et",&TLorentzVector::Et,m_tlvScaleFactor,deque);
            doneTLVBranches=true; 
         }
      }
      std::unordered_map<std::string,boost::any>* propMap = obj->getPropertyMap();
      std::unordered_map<std::string,boost::any>::iterator prop = propMap->begin();
      std::unordered_map<std::string,boost::any>::iterator propEnd = propMap->end();

      for(;prop!=propEnd;++prop) {
         if(doneBranches[prop->first]) continue;
         doneBranches[prop->first]=true; //about to do the branch!
         std::string branchName = label+prop->first;
         if(!isGoodBranch(branchName)) continue;
         //do we already know about this property 
         auto s = m_camBranches.find(branchName);
         if(s==m_camBranches.end()) {
            //try to make the branch
            std::string propType = obj->type(prop->first,true,false,true/*parses functions*/); 
            if(propType=="") { 
               ATH_MSG_WARNING("Unknown type for " << prop->first << ". Will prevent from writing!");
               m_badBranches[branchName]=true; continue;
            } else if(propType=="void") { //void function 
               ATH_MSG_INFO("Cannot write void property " << prop->first << ". Will prevent from writing!");
               m_badBranches[branchName]=true; continue;
            } else if(boost::starts_with(propType,"std::function")) {
                ATH_MSG_INFO("Cannot write function property " << prop->first << ". Will prevent from writing!");
               m_badBranches[branchName]=true; continue;
            }
            TBranch* b = 0;
            ATH_MSG_DEBUG("Detected type : " << propType);
            //check for basic types:
            std::string propType2 = "std::vector<"; propType2 += propType; propType2 += ">";
            b = addBranch(branchName,propType2); 
            if(!b) { m_badBranches[branchName]=true; continue; }
            s= m_camBranches.find(branchName); //should have been created
         }

         m_camBranches[branchName].fillFromCamDeque(*deque,prop->first,true,false/*don't check storegate*/);

         continue;
      }
   }

   if(apply(dynamic_cast<CamObject*>(deque),label,option).isFailure()) {
      ATH_MSG_ERROR("Failed to write properties of CamDeque/CamVector:" << label); return StatusCode::FAILURE;
   }
   return StatusCode::SUCCESS;
}




void CamWriter::FillDoubleVector(CamBranch* b, const std::string& branchName,double (TLorentzVector::*funcPtr)() const,double scaleFactor, CamDeque* deque, bool createIfNotExisting) {
            if(b==0) { 
               auto s = m_camBranches.find(branchName);
               if(s == m_camBranches.end()) {
                  if(!createIfNotExisting || !isGoodBranch(branchName)) return;
                  if(!addBranch(branchName,"vector<double>")) {m_badBranches[branchName]=true;return;}
                  b = &m_camBranches.find(branchName)->second;
               } else { b = &s->second; }
            } 
            if(b->tBranch->TestBit(kDoNotProcess)) return; //already filled
            else if(!b->vectorDoubleOrFloat) {
               ATH_MSG_FATAL("TLV branch " << branchName << " *MUST* be vector<double> or vector<float>, but detected type " << b->branchType);
               throw 10;
            }
            bool hadAValue(false);
            if(b->vectorDoubleOrFloat==1) { //is double type
               std::vector<double>* vec = static_cast<std::vector<double>*>(*(void**)(b->tBranch->GetAddress())); //only safe if we have the right type!!!
               vec->resize(deque->size(),0.);
               unsigned int i=0;
               for(auto obj : *deque) {
                  if( ((*vec)[i] = ((obj->*funcPtr)() * scaleFactor)) ) hadAValue=true;
                  ++i;
               }
            } else if(b->vectorDoubleOrFloat==2) {  //is float type 
               std::vector<float>* vec = static_cast<std::vector<float>*>(*(void**)(b->tBranch->GetAddress())); //only safe if we have the right type!!!
               vec->resize(deque->size(),0.);
               unsigned int i=0;
               for(auto obj : *deque) {
                  if( ((*vec)[i] = ((obj->*funcPtr)() * scaleFactor)) ) hadAValue=true;
                  ++i;
               }
            }

            if(!m_doNotFill) {
               if(b->tBranch->Fill()<=0) { 
                  ATH_MSG_FATAL("Fill of " << b->tBranch->GetName() << " wrote no (or negative) bytes! Disk space issue?"); 
                  throw 10;
               }
            }
            b->tBranch->SetStatus(0);
            if(hadAValue) b->tBranch->SetBit(3333); //indicates this branch has been filled
            return;
}


void CamWriter::AddDouble(CamBranch* b, const std::string& branchName,double value) {
      if(!b) {
         auto s = m_camBranches.find(branchName);
         if(s==m_camBranches.end()) {
            if(!isGoodBranch(branchName)) return;
            ATH_MSG_DEBUG("Adding Double branch: " << branchName);
            TBranch* b = addBranch(branchName,"double");
            if(!b) {m_badBranches[branchName]=true;return;}
            s = m_camBranches.find(branchName);
         }
         b = &(s->second);
      }
      branchFiller<double>(*b,value);
      return;
}



void CamWriter::AddTLV(std::string branchName,TLorentzVector* value) {
      auto s = m_camBranches.find(branchName);
      if(s==m_camBranches.end()) {
         if(!isGoodBranch(branchName)) return;
         ATH_MSG_DEBUG("Adding TLorentzVector branch: " << branchName);
         TBranch* b = addBranch(branchName,"TLorentzVector");
         if(!b) {m_badBranches[branchName]=true;return;}
         s = m_camBranches.find(branchName);
      } else if(s->second.branchType!="TLorentzVector") {
         ATH_MSG_FATAL("Branch " << branchName << " was supposed to be TLorentzVector but instead is: " << s->second.branchType);
         throw 10;
      }
      branchFiller<TLorentzVector>(s->second,*value);
      return;
}



StatusCode CamWriter::apply(CamEvent* event, std::string& label, const std::string& option) {

   if(!m_predefinedBranches) {
      //check if the label or option contain the "autoTreeConfig" 
      if(option.find("autoTreeConfig")!=std::string::npos) {
         //infer the tree config from the storegate. look for basic types, vectors of basic types, and vector vectors
         addBranchFromStoregate<bool>("O");
         addBranchFromStoregate<unsigned int>("i");
         addBranchFromStoregate<int>("I");
         addBranchFromStoregate<float>("F");
         addBranchFromStoregate<double>("D");
         addBranchFromStoregate<std::vector<bool> >("vector<bool>");
         addBranchFromStoregate<std::vector<unsigned int> >("vector<unsigned int>");
         addBranchFromStoregate<std::vector<int> >("vector<int>");
         addBranchFromStoregate<std::vector<float> >("vector<float>");
         addBranchFromStoregate<std::vector<double> >("vector<double>");
         addBranchFromStoregate<std::vector<short> >("vector<short>");
         addBranchFromStoregate<std::vector<unsigned short> >("vector<unsigned short>");
         addBranchFromStoregate<std::vector<unsigned long> >("vector<unsigned long>");
         //addBranchFromStoregate<std::vector<std::vector<bool> > >("vector<vector<bool> >"); - leave out for now, no classdef
         addBranchFromStoregate<std::vector<std::vector<unsigned int> > >("vector<vector<unsigned int> >");
         addBranchFromStoregate<std::vector<std::vector<int> > >("vector<vector<int> >");
         addBranchFromStoregate<std::vector<std::vector<float> > >("vector<vector<float> >");
         addBranchFromStoregate<std::vector<std::vector<double> > >("vector<vector<double> >");
         m_predefinedBranches=true;
      }
   }

   if(apply(dynamic_cast<CamObject*>(event),label,option).isFailure()) {
      ATH_MSG_ERROR("Failed to write the CamEvent's properties"); return StatusCode::FAILURE;
   }

      if(m_eventWritesObjects) { 
         //iterate over the CamObjects the event holds, writing out those as well
         std::map<std::string,CamObject*>* objects = event->getObjectMap();
         std::map<std::string,CamObject*>::iterator obj = objects->begin();
         for(;obj!=objects->end();++obj) {
            CamDeque *deque = dynamic_cast<CamDeque*>(obj->second);
            if(deque) {
               std::string label = obj->first + "_";
               if(apply(deque,label,option).isFailure()) { ATH_MSG_ERROR("Failed to write event's deque: " << obj->first); return StatusCode::FAILURE; }
            } else {
               //must be a normal object, write it as such
               std::string label = obj->first + "_";
               if(apply(obj->second,label,option).isFailure()) {
                  ATH_MSG_ERROR("Failed to write the CamEvent's object:" << obj->first); return StatusCode::FAILURE;
               }
            }
         }
      }

    return StatusCode::SUCCESS;
}

std::vector<bool>& CamWriter::getTLVFormatVec(std::string& label, const std::string& option) {
   if(m_options_tlvFormat.find(label)==m_options_tlvFormat.end()) {
      //check options for tlv= property before using this tool's default option
      std::string tlvOption = m_tlvFormat;
      if(option.find("tlv=")!=std::string::npos) {
         int tlvOptionPos=option.find("tlv=");
         int optionEnd = (option.find(" ",tlvOptionPos)==std::string::npos) ? option.length() :option.find(" ",tlvOptionPos); 
         tlvOption = option.substr(tlvOptionPos,optionEnd-tlvOptionPos);
      } 
      std::vector<bool> tlvOptionVec(10,false);
      if(tlvOption.find("Pt") != std::string::npos) tlvOptionVec[0]=true;
      if(tlvOption.find("Eta") != std::string::npos) tlvOptionVec[1]=true;
      if(tlvOption.find("Phi") != std::string::npos) tlvOptionVec[2]=true;
      if(tlvOption.find("M") != std::string::npos) tlvOptionVec[3]=true;
      if(tlvOption.find("Px") != std::string::npos) tlvOptionVec[4]=true;
      if(tlvOption.find("Py") != std::string::npos) tlvOptionVec[5]=true;
      if(tlvOption.find("Pz") != std::string::npos) tlvOptionVec[6]=true;
      if(tlvOption.find("TLV") != std::string::npos) tlvOptionVec[9]=true;
            for(unsigned int i=0;i<tlvOption.length();i++) {
               char c = tlvOption[i];
               //is this the last E or is it an E followed by a capital
               if(c=='E' && ((i==tlvOption.length()-1)||(tlvOption[i+1]>='A'&&tlvOption[i+1]<='Z'))) {
                  tlvOptionVec[7]=true;
               }
               //is this the last Et or is it followed by a capital
               if(i==tlvOption.length()-2) {
                  if(c=='E' && tlvOption[i+1]=='t') tlvOptionVec[8]=true;
               }
               else if(i<tlvOption.length()-2) {
                  if(c=='E' && tlvOption[i+1]=='t' && (tlvOption[i+2]>='A'&&tlvOption[i+2]<='Z')) tlvOptionVec[8]=true;
               }
            }
            m_options_tlvFormat[label]=tlvOptionVec;
         }

         return m_options_tlvFormat[label];

}

int CamWriter::wildcmp(const char *wild, const char *string) {
  // Written by Jack Handy - jakkhandy@hotmail.com

  const char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if ((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }

  while (*string) {
    if (*wild == '*') {
      if (!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string+1;
    } else if ((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    } else {
      wild = mp;
      string = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  return !*wild;
}
