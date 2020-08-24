
#include "AnalysisCamEvent/CamObject.h"

#include "AnalysisCamEvent/ICamTool.h"
#include "GaudiKernel/ToolHandle.h"
#include "AnalysisCamEvent/ICamService.h"
#include "GaudiKernel/ServiceHandle.h"
#include <execinfo.h>

#include <dlfcn.h>
#include <boost/filesystem.hpp> //for checking if a file exists when printing stack traces
#include <boost/algorithm/string/predicate.hpp> //for starts_with and ends_with methods
#include "AnalysisCamEvent/CamEvent.h"

#include "AthenaKernel/IClassIDSvc.h"

#include "CxxUtils/SealDebug.h"

#include "AthContainers/normalizedTypeinfoName.h"

#include "AnalysisCamEvent/CamAudit.h"

bool CamObject::ForceCorrectType = false;
bool CamObject::AutoSetTLV = false;
int CamObject::MAX_STOREGATE_SUGGESTIONS = 20;
bool CamObject::Documentation = false;
std::unordered_map<std::type_index, void*> CamObject::empty_types;
const std::string CamObject::emptyString  = "";

std::string CamObject::applyTool(const std::string& toolName,std::string label,const std::string& option) {
   //if(toolName.find("/")==std::string::npos) { toolName = toolName+"/"+toolName;/*if(label.length() ==0) toolName = toolName+"/"+toolName; else toolName = toolName+"/"+label;*/ }
   ToolHandle<ICamTool> m_def(toolName);
   if(m_def.retrieve().isFailure()) CAMERROR("CamObject::apply - could not retrieve CamTool " << toolName << ". Either tool is not installed, the instance does not exist, or the instance exists but failed in the initialization (look at lines above to see...)");
   if(CamAudit::what&CamAudit::TOOLS) CamAudit::chronoSvc->chronoStart( "CamObject::applyTool(" + toolName + ")" );
   if(m_def->apply(this,label,option).isFailure()) CAMERROR("CamObject::apply - failed to apply CamTool " << toolName);
   if(CamAudit::what&CamAudit::TOOLS) CamAudit::chronoSvc->chronoStop( "CamObject::applyTool(" + toolName + ")" );
   return label;
}

std::string CamObject::applyService(const std::string& toolName,std::string label,const std::string& option) {
   //if(toolName.find("/")==std::string::npos) { toolName = toolName+"/"+toolName; }
   
   if(toolName.find("__")!=std::string::npos) {
    //services with __ in their name have troubles, so we will auto-sanitize this ...
    std::string sanitizedToolName(toolName);
    std::string::size_type n = 0;
    while( ( n=sanitizedToolName.find("__",n) ) != std::string::npos ) {
      sanitizedToolName.replace(n,2,"_");
      n += 1;
    }
    return applyService(sanitizedToolName,label,option);
   }
   
   ServiceHandle<ICamService> m_serv(toolName,"CamObject");
   if(m_serv.retrieve().isFailure()) CAMERROR("CamObject::apply - CamService " << toolName << " either does not exist or failed in initialization");
   if(CamAudit::what&CamAudit::SERVICES) CamAudit::chronoSvc->chronoStart( "CamObject::applyService(" + toolName + ")" );
   if(m_serv->apply(this,label,option).isFailure()) CAMERROR("CamObject::apply - failed to apply CamService " << toolName);
   if(CamAudit::what&CamAudit::SERVICES) CamAudit::chronoSvc->chronoStop( "CamObject::applyService(" + toolName + ")" );
   //tools should modify their label string in order to communicate back to apply method
   return label;
}


/*CamObject::CamObject() :  m_d3pdIndex(-1), m_evtStore("StoreGateSvc","this") {
   StatusCode sc = m_evtStore.retrieve();
   if(sc.isFailure()) sc;
}*/

const std::string& CamObject::getName() {
   if(CamEvent* ev = event()) {
      //is the event the parent
      if(m_parent==dynamic_cast<CamObject*>(ev)) return ev->getObjectName(this); 
      else if(m_parent) return m_parent->getName(); 
   }
   return emptyString;
}


CamObject::CamObject(int index, CamObject* parent, std::string prefix) : m_evtStore("StoreGateSvc","this"), 
   m_d3pdIndex(index),
   m_prefix(prefix), 
   m_parent(parent), m_xAODParticle(0) {

   //if has a parent specifed, take the storegate from the parent
   if(parent) {
      m_evtStore = parent->evtStore();
   } else {
      StatusCode sc = m_evtStore.retrieve();
      if(sc.isFailure()) CAMERROR("CamObject::CamObject() unable to retrieve event store");//FIXME: this sometimes happens when objects created in initialize!!!??
   }

   if(CamObject::AutoSetTLV) {
      //load with (pt eta phi m) or (px py pz e) if available
      //require at least 3 properties?
      int propcount = 0;
      bool tmp = false;
      if(this->contains<double>("tlv_px",false,true,false,false,false)) {this->SetPx(this->retrieve_from_storegate<double>("tlv_px",tmp));propcount++;}
      if(this->contains<double>("tlv_py",false,true,false,false,false)) {this->SetPy(this->retrieve_from_storegate<double>("tlv_py",tmp));propcount++;}
      if(this->contains<double>("tlv_pz",false,true,false,false,false)) {this->SetPz(this->retrieve_from_storegate<double>("tlv_pz",tmp));propcount++;}
      if(this->contains<double>("tlv_e",false,true,false,false,false)) {this->SetE(this->retrieve_from_storegate<double>("tlv_e",tmp));propcount++;}
      if(propcount<3) {
         propcount=0;
         double pt = 0.,eta=0.,phi=0.;
         if(this->contains<double>("tlv_pt",false,true,false,false,false)) {pt = this->retrieve_from_storegate<double>("tlv_pt",tmp);propcount++;}
         if(this->contains<double>("tlv_eta",false,true,false,false,false)) {eta = this->retrieve_from_storegate<double>("tlv_eta",tmp);propcount++;}
         if(this->contains<double>("tlv_phi",false,true,false,false,false)) {phi = this->retrieve_from_storegate<double>("tlv_phi",tmp);propcount++;}
         if(this->contains<double>("tlv_m",false,true,false,false,false)) this->SetPtEtaPhiM(pt,eta,phi,this->retrieve_from_storegate<double>("tlv_m",tmp));
         else if(this->contains<double>("tlv_e",false,true,false,false,false)) this->SetPtEtaPhiE(pt,eta,phi,this->retrieve_from_storegate<double>("tlv_e",tmp));
         else if(propcount>1) this->SetPtEtaPhiM(pt,eta,phi,0);
      }
   }
}

CamObject::~CamObject() {}


///Evalues bool,uint,int . If not found then we assume this object "isNot", so returns false
bool CamObject::is(const std::string& key) {
   if(key=="1") return true;
   if(this->contains<bool>(key)) return this->get<bool>(key); 
   else if(this->contains<char>(key)) return this->get<char>(key);
   else if(this->contains<unsigned int>(key)) return this->get<unsigned int>(key);
   else if(this->contains<int>(key)) return this->get<int>(key);
   return false;
}

bool CamObject::is(const char* key) {
   if(strcmp(key,"1")==0) return true;
   if(this->contains<bool>(key)) return this->get<bool>(key); 
   else if(this->contains<char>(key)) return this->get<char>(key);
   else if(this->contains<unsigned int>(key)) return this->get<unsigned int>(key);
   else if(this->contains<int>(key)) return this->get<int>(key);
   return false;
}

void CamObject::print(std::string propsToShow, bool callDynamic) {
  std::cout << getName();
  if(m_prefix.length() || m_d3pdIndex!=-1) {
    std::cout << "[";
    if(m_prefix.length()) std::cout << m_prefix << ",";
    if(m_d3pdIndex!=-1) std::cout << m_d3pdIndex;
    std::cout << "]";
  }
  std::cout << ": ";
  //   std::cout << "CamObject [name,prefix,Index] = [" << getName() << "," << m_prefix << "," << m_d3pdIndex << "]: ";
   if(propsToShow.length()>0) {
      //printing specific property
      
      //loop over properties, comma deliminated
      size_t found;
      do {
         found = propsToShow.find_first_of(',');
         std::string thisProp;
         if(found!=std::string::npos) {
            thisProp = propsToShow.substr(0,found);
            propsToShow = propsToShow.substr(found+1);
         } else { thisProp = propsToShow; }

         if(thisProp=="tlv") {
            std::cout << "TLV [pt, eta, phi, m] = [" << this->Pt() << ", ";
            if(this->Pt()==0) std::cout << "null"; else std::cout << this->Eta();
            std::cout << ", " << this->Phi() << ", " << this->M() << "] ";
         } else {
            if(this->contains<double>(thisProp)) std::cout << thisProp << "=" << this->get(thisProp) << " ";
            else if(this->contains<float>(thisProp)) std::cout << thisProp << "=" << this->get<float>(thisProp) << " ";
            else if(this->contains<int>(thisProp)) std::cout << thisProp << "=" << this->get<int>(thisProp) << " ";
            else if(this->contains<uint>(thisProp)) std::cout << thisProp << "=" << this->get<uint>(thisProp) << " ";
            else if(this->contains<char>(thisProp)) std::cout << thisProp << "=" << int(this->get<char>(thisProp)) << " "; //always print as an int, that's what chars usually mean!
	    else if(this->contains<unsigned short>(thisProp)) std::cout << thisProp << "=" << this->get<unsigned short>(thisProp) << " ";
	    else if(this->contains<short>(thisProp)) std::cout << thisProp << "=" << this->get<short>(thisProp) << " ";
            else if(this->contains<bool>(thisProp)) std::cout << thisProp << "=" << this->get<bool>(thisProp) << " ";
            else if(this->contains<std::vector<double>>(thisProp)){
               std::vector<double> val = this->get<std::vector<double>>(thisProp);
               std::cout << thisProp << "= {";
               bool first=true;
               for(auto v : val) {if(!first) std::cout << ", "; first=false; std::cout << v;}
               std::cout << "} ";
            } else if(this->contains<std::vector<float>>(thisProp)){
               std::vector<float> val = this->get<std::vector<float>>(thisProp);
               std::cout << thisProp << "= {";
               bool first=true;
               for(auto v : val) {if(!first) std::cout << ", "; first=false; std::cout << v;}
               std::cout << "} ";
            } else if(this->contains<std::vector<int>>(thisProp)){
               std::vector<int> val = this->get<std::vector<int>>(thisProp);
               std::cout << thisProp << "= {";
               bool first=true;
               for(auto v : val) {if(!first) std::cout << ", "; first=false; std::cout << v;}
               std::cout << "} ";
            } else if(this->contains<std::vector<bool>>(thisProp)){
               std::vector<bool> val = this->get<std::vector<bool>>(thisProp);
               std::cout << thisProp << "= {";
               bool first=true;
               for(auto v : val) {if(!first) std::cout << ", "; first=false; std::cout << v;}
               std::cout << "} ";
            } else if(this->contains<std::string>(thisProp)) std::cout << thisProp << "=" << this->get<std::string>(thisProp) << " ";
         }
      } while (found!=std::string::npos);
      std::cout << std::endl;return;
   }
   std::cout << "    TLV [pt, eta, phi, m] = [" << this->Pt() << ", ";
   if(this->Pt()==0) std::cout << "null"; else std::cout << this->Eta();
   std::cout << ", " << this->Phi() << ", " << this->M() << "]" << std::endl;

   for(std::unordered_map<std::string,boost::any>::iterator it = m_propMap.begin();it!=m_propMap.end();++it) {
      std::cout << "    " << it->first << " = ";
      //need to set the current key before these attempts at "getting"   ... WHY?! (nov2014)
      //m_currentKey = it->first;
      if(it->second.type() == typeid(bool)) std::cout << boost::any_cast<bool>(it->second) << " (bool)";
      else if(it->second.type() == typeid(uint)) std::cout << boost::any_cast<uint>(it->second) << " (unsigned int)";
      else if(it->second.type() == typeid(int)) std::cout << boost::any_cast<int>(it->second) << " (int)";
      else if(it->second.type() == typeid(float)) std::cout << boost::any_cast<float>(it->second) << " (float)";
      else if(it->second.type() == typeid(double)) std::cout << boost::any_cast<double>(it->second) << " (double)";
      else if(it->second.type() == typeid(unsigned char)) std::cout << int(boost::any_cast<unsigned char>(it->second)) << " (unsigned char)";  //always print as an int, that's what chars usually mean!
      else if(it->second.type() == typeid(char)) std::cout << int(boost::any_cast<char>(it->second)) << " (char)";  //always print as an int, that's what chars usually mean!
      else if(it->second.type() == typeid(short)) std::cout << boost::any_cast<short>(it->second) << " (short)";
      else if(it->second.type() == typeid(unsigned short)) std::cout << boost::any_cast<unsigned short>(it->second) << " (unsigned short)";
      else if(callDynamic && it->second.type()==typeid(dldouble) ) std::cout << (boost::any_cast<dldouble>(it->second))(*this) << " (dynamic double)";

      else if(it->second.type() == typeid(std::vector<double>)) {
	std::vector<double> val = boost::any_cast<std::vector<double>>(it->second);
        std::cout << "{";
        bool first=true;
        for(auto v : val) {if(!first) std::cout << ", "; first=false; std::cout << v;}
        std::cout << "} (" << SG::normalizedTypeinfoName(it->second.type()) << ")";
      } else if(it->second.type() == typeid(std::vector<float>)) {
	std::vector<float> val = boost::any_cast<std::vector<float>>(it->second);
        std::cout << "{";
        bool first=true;
        for(auto v : val) {if(!first) std::cout << ", "; first=false; std::cout << v;}
        std::cout << "} (" << SG::normalizedTypeinfoName(it->second.type()) << ")";
      } else if(it->second.type() == typeid(std::vector<int>)) {
	std::vector<int> val = boost::any_cast<std::vector<int>>(it->second);
        std::cout << "{";
        bool first=true;
        for(auto v : val) {if(!first) std::cout << ", "; first=false; std::cout << v;}
        std::cout << "} (" << SG::normalizedTypeinfoName(it->second.type()) << ")";
      } else if(it->second.type() == typeid(std::vector<unsigned int>)) {
	std::vector<unsigned int> val = boost::any_cast<std::vector<unsigned int>>(it->second);
        std::cout << "{";
        bool first=true;
        for(auto v : val) {if(!first) std::cout << ", "; first=false; std::cout << v;}
        std::cout << "} (" << SG::normalizedTypeinfoName(it->second.type()) << ")";
      } else if(it->second.type() == typeid(std::vector<std::string>)) {
	std::vector<std::string> val = boost::any_cast<std::vector<std::string>>(it->second);
        std::cout << "{";
        bool first=true;
        for(auto v : val) {if(!first) std::cout << ", "; first=false; std::cout << v;}
        std::cout << "} (" << SG::normalizedTypeinfoName(it->second.type()) << ")";

      } else {
	std::cout << "(" << SG::normalizedTypeinfoName(it->second.type()) << ")";
      }
      if(CamObject::Documentation && m_docuMap.find(it->first)!=m_docuMap.end()) {
         std::cout << "  [" << m_docuMap[it->first] << "]";
      }
      std::cout << std::endl;
   }

   //also print the aux properties! 
   if(m_xAODParticle) {
      std::cout << "  AuxElement properties:" << std::endl;
      const SG::auxid_set_t& auxids = m_xAODParticle->getAuxIDs();
      SG::AuxTypeRegistry& reg = SG::AuxTypeRegistry::instance();
      for( SG::auxid_t aux : auxids) {
         std::string name = reg.getName( aux );
         std::cout << "    " << name << " = ";
         const std::type_info& typeinfo = *reg.getType(aux);
         if(typeinfo==typeid(bool) && m_xAODParticle->isAvailable<bool>(name)) std::cout << get<bool>(name) << " (bool)" << std::endl;
         else if(typeinfo==typeid(uint) && m_xAODParticle->isAvailable<uint>(name)) std::cout << get<uint>(name) << " (unsigned int)" << std::endl;
          else if(typeinfo==typeid(float) && m_xAODParticle->isAvailable<float>(name)) std::cout << get<float>(name) << " (float)" << std::endl;
          else if(typeinfo==typeid(int) && m_xAODParticle->isAvailable<int>(name)) std::cout << get<int>(name) << " (int)" << std::endl;
          else if(typeinfo==typeid(double) && m_xAODParticle->isAvailable<double>(name)) std::cout << get<double>(name) << " (double)" << std::endl;
          else if(typeinfo==typeid(unsigned char) && m_xAODParticle->isAvailable<unsigned char>(name)) std::cout << int(get<unsigned char>(name)) << " (unsigned char)" << std::endl;  //always print as an int, that's what chars usually mean!
          else if(typeinfo==typeid(char) && m_xAODParticle->isAvailable<char>(name)) std::cout << int(get<char>(name)) << " (char)" << std::endl;  //always print as an int, that's what chars usually mean!
          else if(typeinfo==typeid(short) && m_xAODParticle->isAvailable<short>(name)) std::cout << get<short>(name) << " (short)" << std::endl;
          else if(typeinfo==typeid(unsigned short) && m_xAODParticle->isAvailable<unsigned short>(name)) std::cout << get<unsigned short>(name) << " (unsigned short)" << std::endl;

	  else if(typeinfo == typeid(std::vector<double>) && m_xAODParticle->isAvailable<std::vector<double>>(name)) {
	    std::vector<double> val = get<std::vector<double>>(name);
	    std::cout << "{";
	    bool first=true;
	    for(auto v : val) {if(!first) std::cout << ", "; first=false; std::cout << v;}
	    std::cout << "} (" << SG::normalizedTypeinfoName(typeinfo) << ")"<< std::endl;
	  } else if(typeinfo== typeid(std::vector<float>)) {
	      std::vector<float> val = get<std::vector<float>>(name);
	    std::cout << "{";
	    bool first=true;
	    for(auto v : val) {if(!first) std::cout << ", "; first=false; std::cout << v;}
	    std::cout << "} (" << SG::normalizedTypeinfoName(typeinfo) << ")"<< std::endl;
	  } else if(typeinfo == typeid(std::vector<int>)) {
	    std::vector<int> val = get<std::vector<int>>(name);
	    std::cout << "{";
	    bool first=true;
	    for(auto v : val) {if(!first) std::cout << ", "; first=false; std::cout << v;}
	    std::cout << "} (" << SG::normalizedTypeinfoName(typeinfo) << ")"<< std::endl;
	  } else if(typeinfo == typeid(std::vector<unsigned int>)) {
	    std::vector<unsigned int> val = get<std::vector<unsigned int>>(name);
	    std::cout << "{";
	    bool first=true;
	    for(auto v : val) {if(!first) std::cout << ", "; first=false; std::cout << v;}
	    std::cout << "} (" << SG::normalizedTypeinfoName(typeinfo) << ")"<< std::endl;
	  } else if(typeinfo == typeid(std::vector<std::string>)) {
	    std::vector<std::string> val = get<std::vector<std::string>>(name);
	    std::cout << "{";
	    bool first=true;
	    for(auto v : val) {if(!first) std::cout << ", "; first=false; std::cout << v;}
	    std::cout << "} (" << SG::normalizedTypeinfoName(typeinfo) << ")" << std::endl;

	  }

         else std::cout << " (" << SG::normalizedTypeinfoName( typeinfo ) << ")" << std::endl;
      }
   }

}

void CamObject::printTraceConst(std::ostream& message, bool throwError) {
   std::ostringstream& s = dynamic_cast<std::ostringstream&>(message);
   if(s.str().length()>0) printf("\n\nCAMERROR: %s\n\n",s.str().c_str());
   printf("\nCamObject Stack Trace: \n");

   FILE* fp = tmpfile();
   int fd = fileno (fp);
   Athena::DebugAids::stacktrace (fd);
   fseek (fp, 0, SEEK_SET);
   char buf[65536];
   bool startPrint(false);
   while (fgets (buf, sizeof (buf), fp)) {
     //std::ostringstream s;
     //fputs (buf, s);
      std::string ss = buf;
      if(!startPrint && ss.find("printTraceConst")!=std::string::npos) { startPrint=true; continue; }
      if(ss.find("AthSequencer")!=std::string::npos || ss.find("AthAlgorithm")!=std::string::npos || ss.find("CamAlgorithm")!=std::string::npos) break; //stop here 
      if(startPrint) std::cout << ss << std::endl;
   }

   if(throwError) {
      std::string ss = "CamError: " + s.str() + "\n See information above to help debug.";
      throw std::runtime_error(ss.c_str());
   }
}

void CamObject::printTrace(std::ostream& message,std::string showSuggestions, bool throwError) {

   

   std::ostringstream& s = dynamic_cast<std::ostringstream&>(message);
   printf("\n\nCAMERROR: %s\n\n",s.str().c_str());
   //printf("\n\n Output of CamObject print:\n");
   this->print("",false); //don't call dynamic properties... if the error was caused by a dynamic property, we'd get an infinite loop
   std::ostringstream blank;
   printTraceConst(blank,false);
   if(showSuggestions.length()>0/* && m_prefix.length()>0*/) {
      //show suggestions from the storegate
      std::cout << "Storegate suggestions: " << std::endl;
      //get the storegate dump and parse it line by line 
      std::string sDump = m_evtStore->dump();
      size_t nPos = sDump.find('\n');
      std::string currType = "";

      std::vector<std::string> candidates;

      while(nPos != std::string::npos) {
         std::string line = sDump.substr(0,nPos);
         sDump = sDump.substr(nPos+1);

         //what line is this?
         if(line.find(" for ClassID ") != std::string::npos) {
            currType = line.substr(line.find(" for ClassID ")+13);
            currType = currType.substr(0,currType.length()-3);
            currType = currType.substr(currType.find(' ')+2); //removes the CLID
            if(m_d3pdIndex>=0 && currType.find("std::vector<")==0) {
               currType = currType.substr(12);
               currType = currType.substr(0,currType.length()-1);
            }
         } else if(line.find(" --- key: ") != std::string::npos) {
            std::string key = line.substr(line.find(" --- key: ")+10);
            if(key.find(m_prefix) == 0) {
               //std::cout << "  " << key.substr(m_prefix.length()) << " (" << currType << ")"  << std::endl;
               std::string b = key.substr(m_prefix.length()) + " (" + currType + ")";
               candidates.push_back(b);
            }
         }


         nPos = sDump.find('\n');
      }


      //now go through candidates and see how many satisfy the n characters of showSuggestions
      std::vector<std::string> candidatesFailing;
      std::vector<std::string> candidatesFailing2;
      int nChar = 1;
      while(candidates.size()>0 && uint(nChar)<=showSuggestions.length()) {
         //kill the last set of failing candidates
         candidatesFailing2 = candidatesFailing;
         candidatesFailing.clear();
         std::string search = showSuggestions.substr(0,nChar);
         std::vector<std::string>::iterator it=candidates.begin();
         while(it!=candidates.end()) {
            if(it->find(search)!=0) {
               candidatesFailing.push_back(*it);
               candidates.erase(it);
            } else {
               ++it;
            }
         }
         nChar++;
      }
      std::cout << "got here" <<std::endl;

      //print up to MAX_STOREGATE_SUGGESTIONS suggestions
      int count = 0;

      for(std::vector<std::string>::iterator it=candidatesFailing.begin();it!=candidatesFailing.end();++it) {
            if(count<MAX_STOREGATE_SUGGESTIONS) std::cout << "  " << *it << std::endl;
            count++;
      }
      for(std::vector<std::string>::iterator it=candidatesFailing2.begin();it!=candidatesFailing2.end();++it) {
            if(count<MAX_STOREGATE_SUGGESTIONS) std::cout << "  " << *it << std::endl;
            count++;
      }
      if(count>MAX_STOREGATE_SUGGESTIONS) { std::cout << "...and " << (count-MAX_STOREGATE_SUGGESTIONS) << " more" << std::endl; }

   }

   if(throwError) throw std::runtime_error("CamError. See information above to help debug.");
}


template<> uint CamObject::next_retrieve_from_storegate<uint>(const std::string& key,bool& isValid) {return retrieve_from_storegate<bool>(key,isValid);}
template<> int CamObject::next_retrieve_from_storegate<int>(const std::string& key,bool& isValid) {return retrieve_from_storegate<uint>(key,isValid);}
template<> float CamObject::next_retrieve_from_storegate<float>(const std::string& key,bool& isValid) {return retrieve_from_storegate<int>(key,isValid);}
template<> double CamObject::next_retrieve_from_storegate<double>(const std::string& key,bool& isValid) {return retrieve_from_storegate<float>(key,isValid);}

template<> uint  CamObject::nextExtract<uint>(boost::any& in,bool& isValid,va_list args) {return extract<bool>(in,isValid,args);}
template<> int  CamObject::nextExtract<int>(boost::any& in,bool& isValid,va_list args) {return extract<uint>(in,isValid,args);}
template<> float  CamObject::nextExtract<float>(boost::any& in,bool& isValid,va_list args) {return extract<int>(in,isValid,args);}
template<> double CamObject::nextExtract<double>(boost::any& in,bool& isValid,va_list args) {return extract<float>(in,isValid,args);}

template<> void* CamObject::extract<void*>(boost::any& in, bool& isValid, va_list) {
   isValid=true; //always 'valid'
   return boost::unsafe_any_cast<void*>(&in);
}

template<> void*& CamObject::extract<void*&>(boost::any& in, bool& isValid, va_list) {
   isValid=true; //always 'valid'
   return *(boost::unsafe_any_cast<void*>(&in));
}

template<> int CamObject::contains<void>(const std::string& key, bool checkMap, bool, bool, bool, bool) {
   if(checkMap) {
      std::unordered_map<std::string,boost::any>::iterator it = m_propMap.find(key);
      if(it!=m_propMap.end()) {
         //check the type matches
         if(it->second.type()==typeid(std::function<void(CamObject&)>)) return 1;
         else if(it->second.type()==typeid(std::function<void(CamObject&,va_list)>)) return 1;
         return 0;
      }
   }
/*
   if(checkParent) {
      if(m_parent && m_parent->contains<void>(key,checkMap,checkStoregate,checkParent)) return 3;
   }
*/
   return 0;
}

void CamObject::cacheAny(std::string searchTerm) {
   cache<double>(searchTerm);
   cache<float>(searchTerm);
   cache<int>(searchTerm);
   cache<unsigned int>(searchTerm);
   cache<bool>(searchTerm);
}

double CamObject::get(const std::string& key, ...) {
   //can't forward to get<double> because no effective way to pass the va_list, hence we have to do it ourselves
      //m_currentKey = key;
      va_list argptr;
      va_start(argptr,key);
      double result = getInternal<double>(key, argptr);
      va_end(argptr);
      return result;
}

double CamObject::get(const char* key, ...) {
   //can't forward to get<double> because no effective way to pass the va_list, hence we have to do it ourselves
      //m_currentKey = key;
      va_list argptr;
      va_start(argptr,key);
      //static const std::string myKey(key);
      double result = getInternal<double>(key, argptr);
      va_end(argptr);
      return result;
}

void CamObject::call(const std::string& key, ...) {
   //m_currentKey = key;
   va_list args;va_start(args,key);
   std::unordered_map<std::string,boost::any>::iterator it = m_propMap.find(key);
   if(it==m_propMap.end()) {
      CAMERROR("CamObject::call(\"" << key  << "\") - method not available");
   }
   typedef void (*gg)(CamObject&, va_list);

   if(std::function<void(CamObject&)> *dval = boost::any_cast<std::function<void(CamObject&)> >(&(it->second))) (*dval)(*this);
   else if(std::function<void(CamObject&,va_list)> *dval = boost::any_cast<std::function<void(CamObject&,va_list)> >(&(it->second))) (*dval)(*this,args);
   else { CAMERROR("call: Method with key=" << key << " is not a void type"); }
   va_end(args);
}

int CamObject::wildcmp(const char *wild, const char *string) {
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

std::string CamObject::type(const std::string& key, bool checkMap, bool checkStoregate, bool parseFunctions) {

   if(checkMap) {
      auto prop = m_propMap.find(key);
      if(prop!=m_propMap.end()) {
         std::string out =SG::normalizedTypeinfoName(prop->second.type());
         //now check if this is a function ... because those count as returnables 
         if(parseFunctions && boost::starts_with(out, "std::function<") && boost::ends_with(out,"(CamObject&)>")) {
            //extract return type of function ... 
            out = out.substr(14,out.find_first_of('(')-15); //there's an extra space before the first ( 
         }  else if(parseFunctions && out.find("(TLorentzVector::*)")!=std::string::npos) { //assume it's a method pointer
            out = out.substr(0,out.find_first_of('('));
         } else if(parseFunctions && out.find("(CamObject::*)")!=std::string::npos) { //assume it's a method pointer
            out = out.substr(0,out.find_first_of('('));
         }
         return out;
      }
   }
   if(checkStoregate) {
      //locate proxy and use ClassID to tell its type 
      if(CLID myCLID = m_evtStore->clid(m_prefix+key)) {
         std::string typeName;
         ServiceHandle<IClassIDSvc> m_clidSvc("ClassIDSvc","this");
         m_clidSvc->getTypeNameOfID(myCLID,typeName).ignore() ;
         if(m_d3pdIndex>=0) {
            //need to strip off of vector ... if it's not there, then obviously we don't "HAVE" this type!
            if(!boost::starts_with(typeName,"std::vector<")) return "";
            typeName = typeName.substr(12,typeName.length()-13);
         }
         return typeName;
      }
   }
/*
   if(checkParent && m_parent) {
      return m_parent->type(key,checkMap,checkStoregate,checkParent);
   }
*/
   return "";
}
