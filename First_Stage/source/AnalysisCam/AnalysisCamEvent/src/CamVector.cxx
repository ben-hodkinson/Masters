
#include "AnalysisCamEvent/CamVector.h"
#include "AnalysisCamEvent/CamDeque.h"
#include "AnalysisCamEvent/CamEvent.h"
#include "AnalysisCamEvent/ICamTool.h"
#include "GaudiKernel/ToolHandle.h"
#include "AnalysisCamEvent/ICamService.h"
#include "GaudiKernel/ServiceHandle.h"

#include "AnalysisCamEvent/CamAudit.h"

CamVector::CamVector(std::string prefix, int d3pdIndex) : CamObject(d3pdIndex) {m_prefix=prefix;m_subsetCounter=0;}

CamVector::CamVector(const xAOD::IParticleContainer& pc) : CamObject() {
   resize(pc.size());
   int i=0;
   for(auto& obj : *this) {
      obj.m_xAODParticle = pc[i];
      obj = pc[i]->p4(); //sets the TLorentzVector
      i++;
   }
   m_cont = &pc;
}

CamVector::~CamVector() {this->clear();}

std::string CamVector::applyTool(const std::string& toolName,std::string label,const std::string& option) {
   //if(toolName.find("/")==std::string::npos) { toolName = toolName+"/"+toolName;/*if(label.length() ==0) toolName = toolName+"/"+toolName; else toolName = toolName+"/"+label;*/ }
   ToolHandle<ICamTool> m_def(toolName);
   if(m_def.retrieve().isFailure()) { std::cout << "CamVector::apply - could not retrieve CamTool " << toolName << std::endl; throw 102; }
   if(CamAudit::what&CamAudit::TOOLS) CamAudit::chronoSvc->chronoStart( "CamVector::applyTool(" + toolName + ")" );
   if(m_def->apply(this,label,option).isFailure()) { std::cout << "CamVector::apply - failed to apply CamTool " << toolName << std::endl; throw 103;}
   if(CamAudit::what&CamAudit::TOOLS) CamAudit::chronoSvc->chronoStop( "CamVector::applyTool(" + toolName + ")" );
   return label;
}

std::string CamVector::applyService(const std::string& toolName,std::string label,const std::string& option) {
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
   
   ServiceHandle<ICamService> m_serv(toolName,"CamVector");
   if(m_serv.retrieve().isFailure()) CAMERROR("CamVector::apply - CamService " << toolName << " either does not exist or failed in initialization");
   if(CamAudit::what&CamAudit::SERVICES) CamAudit::chronoSvc->chronoStart( "CamVector::applyService(" + toolName + ")" );
   if(m_serv->apply(this,label,option).isFailure()) CAMERROR("CamVector::apply - failed to apply CamService " << toolName);
   if(CamAudit::what&CamAudit::SERVICES) CamAudit::chronoSvc->chronoStop( "CamVector::applyService(" + toolName + ")" );
   //tools should modify their label string in order to communicate back to apply method
   return label;
}


bool CamVector::any_is(const std::string& key,int max) {
   CamVector::iterator it = CamVector::begin();

   for(int i=0;i!=max&&it!=CamVector::end();++i,++it) {
      if(it->is(key)) return true;
   }
   return false;
}

bool CamVector::any_is(std::function<bool(CamObject&)> r,int max) {
   CamVector::iterator it = CamVector::begin();

   for(int i=0;i!=max&&it!=CamVector::end();++i,++it) {
      if(it->is(r)) return true;
   }
   return false;
}

unsigned int CamVector::remove(const std::string& flag) {
   unsigned int removeCount = 0;
   CamVector::iterator it = CamVector::begin();
   while(it!=CamVector::end()) {
      if(it->is(flag)) {
         removeCount++;
         it = CamVector::erase(it); 
      } else ++it;
   }
   return removeCount;
}

unsigned int CamVector::removeOverlapping(CamObject& c,double dR,std::string eta1,std::string phi1,std::string eta2,std::string phi2) {
   //single object overlap removal
   unsigned int removeCount = 0;
   CamVector::iterator it = CamVector::begin();
   TLorentzVector tmp = c;
   bool override2 = (eta2.length()>0 || phi2.length()>0);
   bool override1 = (eta1.length()>0 || phi1.length()>0);
   if(override2) {
      tmp.SetPtEtaPhiM(1,c.get(eta2),c.get(phi2),0);
   }
   while(it!=CamVector::end()) {
      TLorentzVector tmp2 = (*it);
      if(override1) {
         tmp2.SetPtEtaPhiM(1,it->get(eta1),it->get(phi1),0);
      }
      if(tmp2.DrEtaPhi(tmp)<dR) {
         removeCount++;
         it = CamVector::erase(it); 
      } else ++it;
   }
   return removeCount;
}

unsigned int CamVector::removeOverlapping(CamVector& c,double dR,std::string eta1,std::string phi1,std::string eta2,std::string phi2) {
   CamVector::iterator it = c.begin();
   unsigned int removeCount = 0;
   for(;it!=c.end();++it) {
      removeCount += removeOverlapping(*it,dR,eta1,phi1,eta2,phi2);
   }
   return removeCount;
}

unsigned int CamVector::removeOverlapping(CamDeque& c,double dR,std::string eta1,std::string phi1,std::string eta2,std::string phi2) {
   CamDeque::iterator it = c.begin();
   unsigned int removeCount = 0;
   for(;it!=c.end();++it) {
      removeCount += removeOverlapping(**it,dR,eta1,phi1,eta2,phi2);
   }
   return removeCount;
}

unsigned int CamVector::size(const std::string& key) {
   if(key.length()==0) return std::vector<CamObject>::size();
   unsigned int count=0;
   CamVector::iterator end = CamVector::end();
   for(CamVector::iterator it = CamVector::begin();it!=end;++it) {
      if(it->is(key)) count++;
   }
   return count;
}

unsigned int CamVector::size(std::function<bool(CamObject&)> r) {
   unsigned int count=0;
   CamVector::iterator end = CamVector::end();
   for(CamVector::iterator it = CamVector::begin();it!=end;++it) {
      if(it->is(r)) count++;
   }
   return count;
}

//tell this deque to take ownership of it's children
void CamVector::adopt() {
   CamVector::iterator it = CamVector::begin();
   for(;it!=CamVector::end();++it) {
      it->setParent(this);
   }
}

//this returns a vector of object copies
CamVector* CamVector::subset(const char* key,bool addToEvent) {
   CamVector *out = new CamVector(m_prefix);
   //copy the property maps
   out->setPropertyMap(m_propMap);
   out->setParent(m_parent);//give subset the same parent as this set
   if(addToEvent) addSubsetToEvent(out);
   //out->setOwnsObjects(false); //subsets do not own their objects
   CamVector::iterator it = CamVector::begin();
   for(;it!=CamVector::end();++it) {
      if(it->is(key)) {
         out->push_back(*it); //this creates a copy because this is a vector
         out->back().setParent(out);
      }
   }
   return out;
}

CamVector* CamVector::subset(std::function<bool(CamObject&)> func, bool addToEvent) {
   CamVector *out = new CamVector(m_prefix);
   //copy the property maps
   out->setPropertyMap(m_propMap);
   out->setParent(m_parent);//give subset the same parent as this set
   CamVector::iterator it = CamVector::begin();
   if(addToEvent) addSubsetToEvent(out);
   for(;it!=CamVector::end();++it) {
      if(func(*it)) {
         out->push_back(*it); //this creates a copy because this is a vector
         out->back().setParent(out);
      }
   }
   return out;
}

CamDeque* CamVector::subsetDeque(const char* key, bool adopt, bool addToEvent) {
   CamDeque *out = new CamDeque(m_prefix); //set same prefix
   //copy the property maps
   out->setPropertyMap(m_propMap);
   out->setParent(m_parent);//give subset the same parent as this set
   if(addToEvent) addSubsetToEvent(out);
   for(CamVector::iterator it = CamVector::begin();it!=CamVector::end();++it) {
      if(it->is(key)) {
         out->push_back(&(*it));
         if(adopt) it->setParent(out);
      }
   }
   out->m_cont = m_cont;
   return out;
}

CamDeque* CamVector::subsetDeque(std::function<bool(CamObject&)> func, bool adopt, bool addToEvent) {
   CamDeque *out = new CamDeque(m_prefix); //set same prefix
   //copy the property maps
   out->setPropertyMap(m_propMap);
   out->setParent(m_parent);//give subset the same parent as this set
   if(addToEvent) addSubsetToEvent(out);
   for(CamVector::iterator it = CamVector::begin();it!=CamVector::end();++it) {
      if(it->is(func)) {
         out->push_back(&(*it));
         if(adopt) it->setParent(out);
      }
   }
   out->m_cont = m_cont;
   return out;
}


void CamVector::all_call(std::string key) {
   CamVector::iterator end = CamVector::end();
   for(CamVector::iterator it = CamVector::begin();it!=end;++it) {
      it->call(key);
   }
}

void CamVector::all_cacheAny(std::string searchTerm) {
   CamVector::iterator end = CamVector::end();
   for(CamVector::iterator it = CamVector::begin();it!=end;++it) {
      it->cacheAny(searchTerm);
   }
}




void CamVector::push_back(CamObject& in) {
   //take ownership of the object 
   //in->setParent(this); //no. the subset method uses push_back so we don't want to take ownership unless necessary
   std::vector<CamObject>::push_back(in); //how to return this!??
}


void CamVector::append(CamVector& in) {
   for(CamVector::iterator it=in.begin();it!=in.end();++it) {
      this->push_back(*it);
   }
}

void CamVector::resize(unsigned int nLoad) {
   if(std::vector<CamObject>::size()>nLoad) { std::vector<CamObject>::resize(nLoad); return; } 
   unsigned int currSize = std::vector<CamObject>::size();
   for(unsigned int i=0;i<(nLoad-currSize);++i) {
      this->emplace_back(CamObject(currSize+i,this,m_prefix));
   }
}

void CamVector::resize(const std::string& key) {
   std::string fullKey = m_prefix+key;
   //use the given key to determine how many objects to load
   //try const int first
   unsigned int nLoad = 0;
   if(m_evtStore->contains<int>(fullKey)) {
      const int* n = 0; 
      if(m_evtStore->retrieve(n,fullKey).isFailure()) throw 20;
      nLoad = *n;
   } else if(m_evtStore->contains<unsigned int>(fullKey)) {
      const unsigned int* n = 0; 
      if(m_evtStore->retrieve(n,fullKey).isFailure()) throw 20;
      nLoad = *n;
   } else if(m_evtStore->contains<std::vector<int> >(fullKey)) {
      const std::vector<int> *n = 0;
      if(m_evtStore->retrieve(n,fullKey).isFailure()) throw 20;
      nLoad = n->size();
   } else {CAMERROR("CamVector::fill(" << key << ") Unable to find key with type int,unsigned int,vector<int>");}
   resize(nLoad);
}

void CamVector::addSubsetToEvent(CamObject* in) {
    if(event()) {
      m_subsetCounter++;
      //add this to the event so the event can manage the subset deletion too
      char buff[100]; //FIXME: what if this isn't enough!!
      std::string tmp; sprintf(buff,"%s_subset%d",this->getName().c_str(),m_subsetCounter); tmp = buff;
      event()->addObject(in,tmp);
   }
}

void CamVector::setIParticleContainer(const xAOD::IParticleContainer& cont) {
   this->resize(cont.size());
   int i=0;
   for(auto& obj : *this) {
      obj = cont[i]->p4();
      obj.setAuxElement( cont[i] );
      ++i;
   }
   m_cont = &cont;
}

CamObject* CamVector::at_index(int i) {
   for(auto& obj : *this) {
      if(obj.getIndex()==i) return &obj;
   }
   return 0;
}
CamObject* CamVector::at_ae(const SG::AuxElement* ae) {
   for(auto& obj : *this) {
      if(obj.getAuxElement()==ae) return &obj;
   }
   return 0;
}
