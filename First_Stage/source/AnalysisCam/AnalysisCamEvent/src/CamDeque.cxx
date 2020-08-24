
#include "AnalysisCamEvent/CamDeque.h"
#include "AnalysisCamEvent/CamVector.h"
#include "AnalysisCamEvent/ICamTool.h"
#include "AnalysisCamEvent/ICamService.h"
#include "GaudiKernel/ServiceHandle.h"
#include "GaudiKernel/ToolHandle.h"

#include "AnalysisCamEvent/CamEvent.h"

#include "AnalysisCamEvent/CamAudit.h"

CamDeque::CamDeque(std::string prefix, int d3pdIndex) : CamObject(d3pdIndex) {m_prefix=prefix; m_subsetCounter=0;}

CamDeque::CamDeque(const xAOD::IParticleContainer& pc) : CamObject() {
   resize(pc.size());
   int i=0;
   for(auto& obj : *this) {
      obj->m_xAODParticle = pc[i];
      *obj = pc[i]->p4(); //sets the TLorentzVector
      i++;
   }
   m_cont = &pc;
}

CamDeque::~CamDeque() {this->clear();}

CamDeque& CamDeque::operator+=(CamVector& v) { for(auto& c : v) this->push_back(&c); return *this; }
CamDeque& CamDeque::operator+=(CamDeque& v) { for(auto& c : v) this->push_back(c); return *this; }

std::string CamDeque::applyTool(const std::string& toolName,std::string label,const std::string& option) {
   //if(toolName.find("/")==std::string::npos) { toolName = toolName+"/"+toolName;/*if(label.length() ==0) toolName = toolName+"/"+toolName; else toolName = toolName+"/"+label;*/ }
   ToolHandle<ICamTool> m_def(toolName);
   if(m_def.retrieve().isFailure()) { std::cout << "CamDeque::apply - could not retrieve CamTool " << toolName << std::endl; throw 102; }
   if(CamAudit::what&CamAudit::TOOLS) CamAudit::chronoSvc->chronoStart( "CamDeque::applyTool(" + toolName + ")" );
   if(m_def->apply(this,label,option).isFailure()) { std::cout << "CamDeque::apply - failed to apply CamTool " << toolName << std::endl; throw 103;}
   if(CamAudit::what&CamAudit::TOOLS) CamAudit::chronoSvc->chronoStop( "CamDeque::applyTool(" + toolName + ")" );
   return label;
}

std::string CamDeque::applyService(const std::string& toolName,std::string label,const std::string& option) {
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
   
   ServiceHandle<ICamService> m_serv(toolName,"CamDeque");
   if(m_serv.retrieve().isFailure()) CAMERROR("CamDeque::apply - CamService " << toolName << " either does not exist or failed in initialization");
   if(CamAudit::what&CamAudit::SERVICES) CamAudit::chronoSvc->chronoStart( "CamDeque::applyService(" + toolName + ")" );
   if(m_serv->apply(this,label,option).isFailure()) CAMERROR("CamDeque::apply - failed to apply CamService " << toolName);
   if(CamAudit::what&CamAudit::SERVICES) CamAudit::chronoSvc->chronoStop( "CamDeque::applyService(" + toolName + ")" );
   //tools should modify their label string in order to communicate back to apply method
   return label;
}

bool CamDeque::any_is(const std::string& key,int max) {
   CamDeque::const_iterator it = CamDeque::begin();

   for(int i=0;i!=max&&it!=CamDeque::end();++i,++it) {
      if((*it)->is(key)) return true;
   }
   return false;
}

bool CamDeque::any_is(std::function<bool(CamObject&)> r,int max) {
   CamDeque::const_iterator it = CamDeque::begin();
   for(int i=0;i!=max&&it!=CamDeque::end();++i,++it) {
      if(r(**it)) return true;
   }
   return false;
}

unsigned int CamDeque::remove(std::string flag,bool deleteOnRemove) {
   unsigned int removeCount = 0;
   CamDeque::iterator it = CamDeque::begin();
   while(it!=CamDeque::end()) {
      if((*it)->is(flag)) {
         removeCount++;
         if(deleteOnRemove && (*it)->parent()==this) delete (*it); //can only delete owned children
         it = CamDeque::erase(it); 
      } else ++it;
   }
   return removeCount;
}

unsigned int CamDeque::removeOverlapping(CamObject& c,double dR,std::string eta1,std::string phi1,std::string eta2,std::string phi2) {
   //single object overlap removal
   unsigned int removeCount = 0;
   CamDeque::iterator it = CamDeque::begin();
   TLorentzVector tmp = c;
   bool override2 = (eta2.length()>0 || phi2.length()>0);
   bool override1 = (eta1.length()>0 || phi1.length()>0);
   if(override2) {
      tmp.SetPtEtaPhiM(1,c.get(eta2),c.get(phi2),0);
   }
   while(it!=CamDeque::end()) {
      TLorentzVector tmp2 = (**it);
      if(override1) {
         tmp2.SetPtEtaPhiM(1,(*it)->get(eta1),(*it)->get(phi1),0);
      }
      if(tmp2.DrEtaPhi(tmp)<dR) {
         removeCount++;
         //if we are the parent, we delete. Up to user to not screw things up with pointers in other deques 
         CamObject* theObj = (*it);
         it = CamDeque::erase(it); //FIXME: Potential memory leak if remove CamObject for last time. But if I delete it, other CamDeques that point to it wont work :-(
         if(theObj->parent()==this) delete theObj; //this is a fix - but means end user must be careful to not remove objects still referenced elsewhere
      } else ++it;
   }
   return removeCount;
}

unsigned int CamDeque::removeOverlapping(CamDeque& c,double dR,std::string eta1,std::string phi1,std::string eta2,std::string phi2) {
   //if overlap removing on myself, then follow rule that keep the 'first' element 
   if(this == &c) {
      unsigned int removeCount = 0;
      bool override2 = (eta2.length()>0 || phi2.length()>0);
      bool override1 = (eta1.length()>0 || phi1.length()>0);
      for(auto itr1 = CamDeque::begin(); itr1 != CamDeque::end(); ++itr1) {
         TLorentzVector tmp = **itr1;
         if(override1) tmp.SetPtEtaPhiM(1,(*itr1)->get(eta1),(*itr1)->get(phi1),0);
         CamDeque::iterator itr2 = itr1+1;
         while(itr2 != CamDeque::end()) {
            TLorentzVector tmp2 = **itr2;
            if(override2) tmp2.SetPtEtaPhiM(1,(*itr2)->get(eta2),(*itr2)->get(phi2),0);
            if(tmp2.DrEtaPhi(tmp)<dR) {
               removeCount++;
               CamObject* theObj = (*itr2);
               itr2 = CamDeque::erase(itr2); //FIXME: Potential memory leak if remove CamObject for last time. But if I delete it, other CamDeques that point to it wont work :-(
               if(theObj->parent()==this) delete theObj;
            } else ++itr2;
         }
      }
      return removeCount;
   }

   CamDeque::const_iterator it = c.begin();
   unsigned int removeCount = 0;
   for(;it!=c.end();++it) {
      removeCount += removeOverlapping(**it,dR,eta1,phi1,eta2,phi2);
   }
   return removeCount;
}

unsigned int CamDeque::removeOverlapping(CamVector& c,double dR,std::string eta1,std::string phi1,std::string eta2,std::string phi2) {
   CamVector::iterator it = c.begin();
   unsigned int removeCount = 0;
   for(;it!=c.end();++it) {
      removeCount += removeOverlapping(*it,dR,eta1,phi1,eta2,phi2);
   }
   return removeCount;
}

void CamDeque::removeUnowned() {
   //this is so when clear is called, we don't accidentally hit an already-deleted object
   CamDeque::iterator it = CamDeque::begin();
   //only delete not my children
   while(it!=CamDeque::end()) { 
      if((*it)->parent()!=this) {
         it = CamDeque::erase(it);
      } else ++it;
   }
}

void CamDeque::clear() {

   CamDeque::iterator it = CamDeque::begin();
   //only delete my children
   for(;it!=CamDeque::end();++it) { if((*it)->parent()==this) delete (*it); }

   std::deque<CamObject*>::clear();
}

unsigned int CamDeque::size(const std::string& key) {
   if(key.length()==0) return std::deque<CamObject*>::size();
   unsigned int count=0;
   CamDeque::iterator end = CamDeque::end();
   for(CamDeque::iterator it = CamDeque::begin();it!=end;++it) {
      if((*it)->is(key)) count++;
   }
   return count;
}

unsigned int CamDeque::size(std::function<bool(CamObject&)> r) {
   unsigned int count=0;
   CamDeque::iterator end = CamDeque::end();
   for(CamDeque::iterator it = CamDeque::begin();it!=end;++it) {
      if(r(**it)) count++;
   }
   return count;
}

//tell this deque to take ownership of it's children
void CamDeque::adopt() {
   CamDeque::iterator it = CamDeque::begin();
   for(;it!=CamDeque::end();++it) {
      (*it)->setParent(this);
   }
}

//this returns a deque of pointers to the original objects
//possible memory leak if not managed....don't delete the objects pointed to
CamDeque* CamDeque::subset(std::string key, bool adopt, bool addToEvent) {
   m_subsetCounter++;
   CamDeque *out = new CamDeque();
   out->setParent(m_parent);//give subset the same parent as this set
   if(addToEvent) addSubsetToEvent(out);
   //out->setOwnsObjects(false); //subsets do not own their objects
   CamDeque::iterator it = CamDeque::begin();
   for(;it!=CamDeque::end();++it) {
      if((*it)->is(key)) {
         if(adopt) (*it)->setParent(out); //adopt the object if necessary
         out->push_back(*it);
      }
   }
   out->m_cont = m_cont;
   return out;
}

CamDeque* CamDeque::subset(std::function<bool(CamObject&)> func, bool adopt, bool addToEvent) {
   CamDeque *out = new CamDeque();
   out->setParent(m_parent);//give subset the same parent as this set
   //out->setOwnsObjects(false); //subsets do not own their objects
   CamDeque::iterator it = CamDeque::begin();
   if(addToEvent) addSubsetToEvent(out);
   for(;it!=CamDeque::end();++it) {
      if(func(**it)) {
         if(adopt) (*it)->setParent(out); //adopt the object if necessary
         out->push_back(*it);
      }
   }
   out->m_cont = m_cont;
   return out;
}

//same as subset, except it creates a copy of the CamObject before inserting it into output deque
//the output deque takes ownership
CamDeque* CamDeque::independentSubset(std::string key,bool addToEvent) {
   CamDeque *out = new CamDeque();
   out->setParent(m_parent);//give subset the same parent as this set
   if(addToEvent) addSubsetToEvent(out);
   //out->setOwnsObjects(true); //independentSubsets do own their objects
   CamDeque::iterator it = CamDeque::begin();
   for(;it!=CamDeque::end();++it) {if((*it)->is(key)) { CamObject *newObj = new CamObject(**it); newObj->setParent(out); out->push_back(newObj);}}
   return out;
}

CamDeque* CamDeque::independentSubset(std::function<bool(CamObject&)> func,bool addToEvent) {
   CamDeque *out = new CamDeque();
   out->setParent(m_parent);//give subset the same parent as this set
   if(addToEvent) addSubsetToEvent(out);
   //out->setOwnsObjects(true); //independentSubsets do own their objects
   CamDeque::iterator it = CamDeque::begin();
   for(;it!=CamDeque::end();++it) {if(func(**it)) { CamObject *newObj = new CamObject(**it); newObj->setParent(out); out->push_back(newObj);}}
   return out;
}

void CamDeque::all_call(std::string key) {
   CamDeque::iterator end = CamDeque::end();
   for(CamDeque::iterator it = CamDeque::begin();it!=end;++it) {
      (*it)->call(key);
   }
}

void CamDeque::all_cacheAny(std::string searchTerm) {
   CamDeque::iterator end = CamDeque::end();
   for(CamDeque::iterator it = CamDeque::begin();it!=end;++it) {
      (*it)->cacheAny(searchTerm);
   }
}



void CamDeque::push_back(CamObject* in) {
   //take ownership of the object 
   //in->setParent(this); //no. the subset method uses push_back so we don't want to take ownership unless necessary
   std::deque<CamObject*>::push_back(in); //how to return this!??
}


void CamDeque::resize(unsigned int nLoad) {
   if(std::deque<CamObject*>::size()>nLoad) { std::deque<CamObject*>::resize(nLoad); return; } 
   unsigned int currSize = std::deque<CamObject*>::size();
   for(unsigned int i=0;i<(nLoad-currSize);++i) {
      this->push_back(new CamObject(currSize+i,this,m_prefix));
   }
}

void CamDeque::resize(const std::string&& key) {
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
   } else {CAMERROR("CamDeque::fill(" << key << ") Unable to find key with type int,unsigned int,vector<int>");}
   
   for(unsigned int i=0;i<nLoad;++i) {this->push_back(new CamObject(i,this,m_prefix));}

}


void CamDeque::addSubsetToEvent(CamDeque* in) {
    if(event()) {
      m_subsetCounter++;
      //add this to the event so the event can manage the subset deletion too
      char buff[100]; //FIXME: what if this isn't enough!!
      std::string tmp; sprintf(buff,"%s_subset%d",this->getName().c_str(),m_subsetCounter); tmp = buff;
      event()->addDeque(in,tmp);
   }
}

void CamDeque::setIParticleContainer(const xAOD::IParticleContainer& cont) {
   this->resize(cont.size());
   int i=0;
   for(auto& obj : *this) {
      (*obj) = cont[i]->p4();
      obj->setAuxElement( cont[i] );
      ++i;
   }
   m_cont = &cont;
}

CamObject* CamDeque::at_index(int i) {
   for(auto& obj : *this) {
      if(obj->getIndex()==i) return obj;
   }
   return 0;
}