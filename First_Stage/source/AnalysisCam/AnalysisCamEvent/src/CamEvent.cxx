
#include "AnalysisCamEvent/CamEvent.h"

#include "AnalysisCamEvent/ICamTool.h"
#include "AnalysisCamEvent/ICamService.h"
#include "GaudiKernel/ServiceHandle.h"
#include "GaudiKernel/ToolHandle.h"
#include "AthenaBaseComps/AthAlgorithm.h"
#include "AthenaBaseComps/AthAlgTool.h"

#include "xAODBase/IParticleContainer.h"

#include "xAODMissingET/MissingETContainer.h"

#include "AnalysisCamEvent/CamAudit.h"

bool CamEvent::AutoCreateObjects = true;

bool CamEvent::isSimulation(CamObject& c) {
   return c.get<uint>("eventTypeBitmask")&xAOD::EventInfo::IS_SIMULATION;
}

CamEvent::CamEvent() : CamObject(-1){
   setAuxElement(m_evtStore->tryConstRetrieve<xAOD::EventInfo>());
   m_d3pdIndex = -1; //because EventInfo has an index of 0, so we move back to -1
   if(m_xAODParticle) 
      this->set("isSimulation","(AnalysisCamEvent)CamEvent::isSimulation - bool for if simulation or not") = CamObject::Function(&CamEvent::isSimulation);

}

CamEvent::CamEvent(AthAlgorithm* caller) : CamObject(-1){
   m_evtStore = caller->evtStore();
   setAuxElement(m_evtStore->tryConstRetrieve<xAOD::EventInfo>());
   m_d3pdIndex = -1; //because EventInfo has an index of 0, so we move back to -1
   if(m_xAODParticle) 
      this->set("isSimulation","(AnalysisCamEvent)CamEvent::isSimulation - bool for if simulation or not") = CamObject::Function(&CamEvent::isSimulation);

}

CamEvent::CamEvent(AthAlgTool* caller) : CamObject(-1){
   m_evtStore = caller->evtStore();
   setAuxElement(m_evtStore->tryConstRetrieve<xAOD::EventInfo>());
   m_d3pdIndex = -1; //because EventInfo has an index of 0, so we move back to -1
   if(m_xAODParticle) 
      this->set("isSimulation","(AnalysisCamEvent)CamEvent::isSimulation - bool for if simulation or not") = CamObject::Function(&CamEvent::isSimulation);

}

CamEvent::~CamEvent() {this->clear();}

void CamEvent::clear() {
   std::map<std::string,CamObject*>::iterator itEnd = m_objects.end();

   //go through deques removing unowned (stops attempts to access already deleted objects)
   for(std::map<std::string,CamObject*>::iterator it = m_objects.begin();it!=itEnd;++it) {
      if(it->second->parent()!=this) continue;
      CamDeque *deque = dynamic_cast<CamDeque*>(it->second);
      if(deque) deque->removeUnowned();
   }
   //only delete my children
   for(std::map<std::string,CamObject*>::iterator it = m_objects.begin();it!=itEnd;++it) { 
      if(it->second->parent()==this) delete it->second; }
   m_objects.clear();

   m_propMap.clear();
   m_xAODParticle = 0;

}

std::string CamEvent::applyTool(const std::string& toolName,std::string label,const std::string& option) {
   //if(toolName.find("/")==std::string::npos) { toolName = toolName+"/"+toolName; }
   ToolHandle<ICamTool> th(toolName);
   if(th.retrieve().isFailure()) CAMERROR("CamEvent::apply - CamTool " << toolName << " either does not exist or failed in initialization");
   if(CamAudit::what&CamAudit::TOOLS) CamAudit::chronoSvc->chronoStart( "CamEvent::applyTool(" + toolName + ")" );
   if(th->apply(this,label,option).isFailure()) CAMERROR("CamEvent::apply - failed to apply CamTool " << toolName);
   if(CamAudit::what&CamAudit::TOOLS) CamAudit::chronoSvc->chronoStop( "CamEvent::applyTool(" + toolName + ")" );
   //tools should modify their label string in order to communicate back to apply method
   return label;
}

std::string CamEvent::applyService(const std::string& toolName,std::string label,const std::string& option) {
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
   
   ServiceHandle<ICamService> sh(toolName,"CamEvent");
   if(sh.retrieve().isFailure()) CAMERROR("CamEvent::apply - CamService " << toolName << " either does not exist or failed in initialization");
   if(CamAudit::what&CamAudit::SERVICES) CamAudit::chronoSvc->chronoStart( "CamEvent::applyService(" + toolName + ")" );
   if(sh->apply(this,label,option).isFailure()) CAMERROR("CamEvent::apply - failed to apply CamService " << toolName);
    if(CamAudit::what&CamAudit::SERVICES) CamAudit::chronoSvc->chronoStop( "CamEvent::applyService(" + toolName + ")" );
   //tools should modify their label string in order to communicate back to apply method
   return label;
}

CP::SystematicSet CamEvent::getSystematics(const std::string& toolName) {
  //check if toolName is a CamTool valid CamTool
  ToolHandle<ICamTool> th(toolName);
  if(th.retrieve().isFailure()) {
    std::cout << "CAMERROR: CamEvent::getSystematics(" << toolName << ") - CamTool either does not exist or failed in initialization .. returning empty systematic set" << std::endl;
    return CP::SystematicSet();
  }
  return th->getSystematics();
}

CamVector* CamEvent::getVector(const std::string& toolName, const CP::SystematicSet& syst) {
  ToolHandle<ICamTool> th(toolName);
  
  //first check if the variation we want exists
  std::string systName = syst.name();
  std::string vName = th.name();
  if(systName.length()) vName += "__" + syst.name();
  if( hasVector(vName) ) return getVector(vName);
  
  
  if(th.retrieve().isFailure()) CAMERROR("CamEvent::getVector(" << toolName << "," << syst.name() << ") - CamTool either does not exist or failed in initialization");
  
  //ask the CamTool for its systematics and use to produce a filtered list ...
  CP::SystematicSet reduced;
  const CP::SystematicSet& allSysts = th->getSystematics();for(auto& sv : syst) if(allSysts.find(sv)!=allSysts.end()) reduced.insert(sv);
  //CP::SystematicSet::filterForAffectingSystematics( syst, th->getSystematics(), reduced ).ignore(); FOUND THIS TO BE VERY SLOW!!!
  vName = th.name();
  for(auto& variation : reduced) vName += "__" + variation.name();
  
  if( hasVector(vName) ) return getVector(vName);
  
  //if got here, we've no choice but to apply the systematic variation and ask the tool for the vector
  if(CamAudit::what&CamAudit::TOOLS) CamAudit::chronoSvc->chronoStart( "CamEvent::getVector(" + toolName + ((CamAudit::what&CamAudit::PERSYST) ? (","+syst.name()) : "") + ")" );
  th->applySystematicSet( syst );
  if(th->apply(this,vName,"v").isFailure()) CAMERROR("Failed to use tool " << toolName << " to create CamVector");
  if(CamAudit::what&CamAudit::TOOLS) CamAudit::chronoSvc->chronoStop( "CamEvent::getVector(" + toolName + ((CamAudit::what&CamAudit::PERSYST) ? (","+syst.name()) : "") + ")" );

  auto it = m_objects.find(vName);
  if(it==m_objects.end()) CAMERROR("Could not find vector " << vName);
  return dynamic_cast<CamVector*>(it->second);

}


CamDeque* CamEvent::getDeque(const std::string& toolName, const CP::SystematicSet& syst) {
  ToolHandle<ICamTool> th(toolName);
  
  //first check if the variation we want exists
  std::string systName = syst.name();
  std::string vName = th.name();
  if(systName.length()) vName += "__" + systName;
  if( hasDeque(vName) ) return getDeque(vName);
  
  
  if(th.retrieve().isFailure()) CAMERROR("CamEvent::getDeque(" << toolName << "," << syst.name() << ") - CamTool either does not exist or failed in initialization");
  
  //ask the CamTool for its systematics and use to produce a filtered list ...

  CP::SystematicSet reduced;
  const CP::SystematicSet& allSysts = th->getSystematics();for(auto& sv : syst) if(allSysts.find(sv)!=allSysts.end()) reduced.insert(sv);
  //CP::SystematicSet::filterForAffectingSystematics( syst, th->getSystematics(), reduced ).ignore(); FOUND THIS TO BE VERY SLOW!!!
  
  vName = th.name();
  for(auto& variation : reduced) vName += "__" + variation.name();
  if( hasDeque(vName) ) return getDeque(vName);
  
  //if got here, we've no choice but to apply the systematic variation and ask the tool for the vector
  if(CamAudit::what&CamAudit::TOOLS) CamAudit::chronoSvc->chronoStart( "CamEvent::getDeque(" + toolName + ((CamAudit::what&CamAudit::PERSYST) ? (","+syst.name()) : "") + ")" );
  th->applySystematicSet( syst );
  if(th->apply(this,vName,"d").isFailure()) CAMERROR("Failed to use tool " << toolName << " to create CamDeque");
  if(CamAudit::what&CamAudit::TOOLS) CamAudit::chronoSvc->chronoStop( "CamEvent::getDeque(" + toolName + ((CamAudit::what&CamAudit::PERSYST) ? (","+syst.name()) : "") + ")" );

  auto it = m_objects.find(vName);
  if(it==m_objects.end()) CAMERROR("Could not find deque " << vName);
  return dynamic_cast<CamDeque*>(it->second);

}


/*
CamDeque* CamEvent::addDeque(std::string name,std::string prefix,std::string autoFill) {
   //check deque doesn't already exist
   if(m_deques.find(name)!=m_deques.end()) { std::cout << "CamEvent::addDeque(" << name << ") - deque with this name already exists" << std::endl; throw 100; }
   //CamDeque *out = new CamDeque(prefix);
//    out->setParent(this);
//    if(autoFill.length()>0) out->fill(autoFill);
//    m_deques[name]=out;
    CamDeque& newDeque = m_deques[name];
    newDeque.setPrefix(prefix);if(prefix.length()>0)newDeque.setUsePrefix(true);
    newDeque.setParent(this);
    if(autoFill.length()>0) newDeque.fill(autoFill);
   
   return &(newDeque);
}

//add this deque and return. Creates a copy of the incoming deque
CamDeque* CamEvent::addDeque(std::string name,CamDeque &in) {
   if(m_deques.find(name)!=m_deques.end()) { std::cout << "CamEvent::addDeque(" << name << ") - deque with this name already exists" << std::endl; throw 100; }
   m_deques[name]=in;
   m_deques[name].setParent(this);
   //the new deque should adopt it's children
   m_deques[name].adopt();
   return &(m_deques[name]);
}

CamDeque* CamEvent::getDeque(std::string name) {
   std::map<std::string,CamDeque>::iterator it = m_deques.find(name);
   if(it==m_deques.end()) {
      //can we make the deque from the d3pd?
      if(this->contains<int>(name+"_n") || this->contains<uint>(name+"_n")) {
         return addDeque(name,name+"_","n");
      }
      //try to get the deque from an object definition
      ToolHandle<ICamObjectDefinition> m_def(name+"/"+name);
      std::string label = "";
      if(m_def.retrieve().isFailure()) { std::cout << "CamEvent::getDeque(" << name << ") - deque not found and no object definition with this name exists" << std::endl; throw 101; }
      if(m_def->applyToEvent(this,label).isFailure()) { std::cout << "CamEvent::getDeque(" << name << ") - failed to apply object definition " << name << std::endl; throw 103;}
      return getDeque(label); //label becomes modified to say what should be returned
   }
   return &(it->second); 
}*/

bool CamEvent::hasDeque(const std::string& name) {
   std::map<std::string,CamObject*>::iterator it = m_objects.find(name);
   if(it==m_objects.end()) return false;
   if(dynamic_cast<CamDeque*>(it->second)==0) return false;
   return true;
}
bool CamEvent::hasObject(const std::string& name) {
   std::map<std::string,CamObject*>::iterator it = m_objects.find(name);
   if(it==m_objects.end()) return false;
   if(dynamic_cast<CamObject*>(it->second)==0) return false;
   return true;
}
bool CamEvent::hasVector(const std::string& name) {
   std::map<std::string,CamObject*>::iterator it = m_objects.find(name);
   if(it==m_objects.end()) return false;
   if(dynamic_cast<CamVector*>(it->second)==0) return false;
   return true;
}

CamVector* CamEvent::createVector(const std::string& name,const xAOD::IParticleContainer* cont) {
   CamVector* vector = new CamVector;
   CamVector* out = addVector(vector, name);
   if(cont) out->setIParticleContainer(*cont);
   return out;
}

CamDeque* CamEvent::createDeque(const std::string& name,const xAOD::IParticleContainer* cont) {
   CamDeque* deque = new CamDeque;
   CamDeque* out = addDeque(deque, name);
   if(cont) out->setIParticleContainer(*cont);
   return out;
}

CamObject* CamEvent::createObject(const std::string& name) {
   //make a new object and attach it to the event with the given name
   CamObject* obj = new CamObject(-1,this,name+"_");
   return addObject(obj,name);
}

CamVector* CamEvent::getVector(const std::string& name) {
   std::map<std::string,CamObject*>::iterator it = m_objects.find(name);
   if(it==m_objects.end()) {
      if(CamEvent::AutoCreateObjects) {
         //can we make the deque from the d3pd?
         if(this->contains<int>(name+"_n") || this->contains<uint>(name+"_n")) {
            //check object doesn't already exist
            if(m_objects.find(name)!=m_objects.end()) { CAMERROR("CamEvent::getVector(" << name << ") - object with this name already exists"); throw 100; }
            //return addDeque(name,name+"_","n");
            CamVector *newVector = new CamVector(name+"_");
            newVector->setParent(this);
            newVector->resize("n");
            return addVector(newVector,name);
         } else if(m_evtStore->contains<xAOD::IParticleContainer>(name)) { //make from xAOD Collection 
            //check object doesn't already exist
            if(m_objects.find(name)!=m_objects.end()) { CAMERROR("CamEvent::getVector(" << name << ") - object with this name already exists"); throw 100; }
            //return addDeque(name,name+"_","n");
            const xAOD::IParticleContainer* cp = 0;
            m_evtStore->retrieve(cp,name).ignore();
            if(!cp) { 
                CAMERROR("CamEvent::getVector(" << name << ") - could not retrieve xAOD::IParticleContainer collection"); throw 100;
            }
            CamVector *newVector = new CamVector(*cp);
            newVector->setParent(this);
            return addVector(newVector,name);
         } else if(m_evtStore->contains<std::vector<double>>(name+"_tlv_pt")) {
            //check object doesn't already exist
            if(m_objects.find(name)!=m_objects.end()) { CAMERROR("CamEvent::getVector(" << name << ") - object with this name already exists"); throw 100; }
            //return addDeque(name,name+"_","n");
            CamVector *newVector = new CamVector(name+"_");
            newVector->setParent(this);
            const std::vector<double> *n = 0;
            if(m_evtStore->retrieve(n,name+"_tlv_pt").isFailure()) throw 20;
            newVector->resize(n->size());
            return addVector(newVector,name);
         }
         //try applying a CamTool to get the deque 
         
         
         ToolHandle<ICamTool> th(name);
         if(th.isValid()) {
          std::string label = th.name();
          //v option signals we want a vector ideally
          if(CamAudit::what&CamAudit::TOOLS) CamAudit::chronoSvc->chronoStart( "CamEvent::getVector(" + name + ")" );
          if(th->apply(this,label,"v").isFailure()) CAMERROR("CamEvent::getVector - failed to apply CamTool " << name);
          if(CamAudit::what&CamAudit::TOOLS) CamAudit::chronoSvc->chronoStop( "CamEvent::getVector(" + name + ")" );
          it = m_objects.find(label); //now try and find the vector. If it's still not there, give up
         }

         if(it==m_objects.end()) CAMERROR("Could not find vector " << name << ". Unable to auto create too");
         CamVector* out = dynamic_cast<CamVector*>(it->second);
         //if(out==0) CAMERROR("Created object: " << name << " but it is not a vector!?"); 
         return out;
      } else {
         CAMERROR("Could not find vector " << name);
         return 0;
      }
   }
   CamVector* out = dynamic_cast<CamVector*>(it->second);
   //if(out==0) CAMERROR(name << " found but it is not a vector!?"); 
   return out; 
}


CamDeque* CamEvent::getDeque(const std::string& name) {
   std::map<std::string,CamObject*>::iterator it = m_objects.find(name);
   if(it==m_objects.end()) {
      if(CamEvent::AutoCreateObjects) {
         //can we make the deque from the d3pd?
         if(this->contains<int>(name+"_n") || this->contains<uint>(name+"_n")) {
            //check object doesn't already exist
            if(m_objects.find(name)!=m_objects.end()) { CAMERROR("CamEvent::getDeque(" << name << ") - object with this name already exists"); throw 100; }
            //return addDeque(name,name+"_","n");
            CamDeque *newDeque = new CamDeque(name+"_");
            newDeque->setParent(this);
            newDeque->resize("n");
            return addDeque(newDeque,name);
         } else if(m_evtStore->contains<xAOD::IParticleContainer>(name)) { //make from xAOD Collection 
            //check object doesn't already exist
            if(m_objects.find(name)!=m_objects.end()) { CAMERROR("CamEvent::getDeque(" << name << ") - object with this name already exists"); throw 100; }
            //return addDeque(name,name+"_","n");
            const xAOD::IParticleContainer* cp = 0;
            m_evtStore->retrieve(cp,name).ignore();
            if(!cp) { 
                CAMERROR("CamEvent::getDeque(" << name << ") - could not retrieve xAOD::IParticleContainer collection"); throw 100;
            }
            CamDeque *newDeque = new CamDeque(*cp);
            newDeque->setParent(this);
            return addDeque(newDeque,name);
         } else if(m_evtStore->contains<std::vector<double>>(name+"_tlv_pt")) {
            //check object doesn't already exist
            if(m_objects.find(name)!=m_objects.end()) { CAMERROR("CamEvent::getDeque(" << name << ") - object with this name already exists"); throw 100; }
            //return addDeque(name,name+"_","n");
            CamDeque *newDeque = new CamDeque(name+"_");
            newDeque->setParent(this);
            const std::vector<double> *n = 0;
            if(m_evtStore->retrieve(n,name+"_tlv_pt").isFailure()) throw 20;
            newDeque->resize(n->size());
            return addDeque(newDeque,name);
         }
         ToolHandle<ICamTool> th(name);
         if(th.isValid()) {
          std::string label = th.name();
          //d option signals we want a deque ideally
          if(CamAudit::what&CamAudit::TOOLS) CamAudit::chronoSvc->chronoStart( "CamEvent::getDeque(" + name + ")" );
          if(th->apply(this,label,"d").isFailure()) CAMERROR("CamEvent::getDeque - failed to apply CamTool " << name);
          if(CamAudit::what&CamAudit::TOOLS) CamAudit::chronoSvc->chronoStop( "CamEvent::getDeque(" + name + ")" );
          it = m_objects.find(label); //now try and find the deque. If it's still not there, give up
         }
         if(it==m_objects.end()) CAMERROR("Could not find deque " << name << ". Unable to auto create too");
         CamDeque* out = dynamic_cast<CamDeque*>(it->second);
         //if(out==0) CAMERROR("Created object: " << name << " but it is not a deque!?"); 
         return out;
      } else {
         CAMERROR("Could not find deque " << name);
         return 0;
      }
   }
   CamDeque* out = dynamic_cast<CamDeque*>(it->second);
   //if(out==0) CAMERROR(name << " but it is not a deque!?"); 
   return out; 
}

CamObject* CamEvent::getObject(const std::string& name) {
   std::map<std::string,CamObject*>::iterator it = m_objects.find(name);
   if(it==m_objects.end()) {
      if(CamEvent::AutoCreateObjects) {
         //try to make from tool
         std::string label = "";
         label = applyTool(name,label,"");//label becomes modified to say what object to get
         //now try and find the object. If it's still not there, give up
         it = m_objects.find(label);
         if(it==m_objects.end()) CAMERROR("Could not find object " << name << ". Unable to auto create too");
         CamObject* out = dynamic_cast<CamObject*>(it->second);
         if(out==0) CAMERROR("Created object: " << name << " but it is not an object!?"); 
         return out;
      } else {
         CAMERROR("Could not find object " << name);
         return 0;
      }
   }
   return it->second; 
}

CamObject* CamEvent::getMETObject(const std::string& containerName, const std::string& term) {
   const xAOD::MissingETContainer* metContainer = 0;
   if(m_evtStore->retrieve(metContainer,containerName).isFailure()) {
      CAMFATAL("CamEvent::getMETObject(" << containerName << "," << term << ") - could not find MissingETContainer with name " << containerName); 
      return 0;
   }

   auto it = metContainer->find(term);
   if(it==metContainer->end()) {
      CAMFATAL("CamEvent::getMETObject(" << containerName << "," << term << ") - could not find term " << term); 
   }

   CamObject* met = new CamObject(**it);
   met->SetPx(met->get("mpx"));met->SetPy(met->get("mpy"));
   return addObject(met,containerName+term);

}

CamVector* CamEvent::addVector(CamVector *vector,std::string name) {
   //if no name, take from vector's prefix, as long as prefix ends in underscore
   if(name.length()==0) { 
      std::string prefix=vector->getPrefix();
      if(prefix[prefix.length()-1]=='_') { 
         name=prefix.substr(0,prefix.length()-1); 
      } else { 
         CAMFATAL("CamEvent::addVector() - you must specify a name for the vector, unless the vector has prefix ending with underscore"); 
      }
   }
   //check object doesn't already exist
   if(m_objects.find(name)!=m_objects.end()) { CAMFATAL("CamEvent::addVector(" << name << ") - object with this name already exists"); }
   //check object not already added, if it is, print a warning and do nothing 
   for(std::map<std::string,CamObject*>::iterator it=m_objects.begin();it!=m_objects.end();++it) {
      if(it->second == vector) {
         std::cout << "CamEvent::addVector with name=" << name << ": vector already in event with name=" << it->first << std::endl;
         return vector;
      }
   }
   m_objects[name] = vector;
   vector->setParent(this);
   return vector;
}
CamDeque* CamEvent::addDeque(CamDeque *deque,std::string name) {
   //if no name, take from deque's prefix, as long as prefix ends in underscore
   if(name.length()==0) { 
      std::string prefix=deque->getPrefix();
      if(prefix[prefix.length()-1]=='_') { 
         name=prefix.substr(0,prefix.length()-1); 
      } else { 
         CAMFATAL("CamEvent::addDeque() - you must specify a name for the deque, unless the deque has prefix ending with underscore");
      }
   }
   //check object doesn't already exist
   if(m_objects.find(name)!=m_objects.end()) { CAMFATAL("CamEvent::addDeque(" << name << ") - object with this name already exists"); }
   //check object not already added, if it is, print a warning and do nothing 
   for(std::map<std::string,CamObject*>::iterator it=m_objects.begin();it!=m_objects.end();++it) {
      if(it->second == deque) {
         std::cout << "CamEvent::addDeque with name=" << name << ": deque already in event with name=" << it->first << std::endl;
         return deque;
      }
   }
   m_objects[name] = deque;
   deque->setParent(this);
   return deque;
}
CamObject* CamEvent::addObject(CamObject *object,std::string name) {
   //if no name, take from object's prefix, as long as prefix ends in underscore
   if(name.length()==0) { 
      std::string prefix=object->getPrefix();
      if(prefix[prefix.length()-1]=='_') { 
         name=prefix.substr(0,prefix.length()-1); 
      } else { 
         CAMFATAL("CamEvent::addObject() - you must specify a name for the object, unless the object has prefix ending with underscore"); 
      }
   }
   //check object doesn't already exist
   if(m_objects.find(name)!=m_objects.end()) { CAMERROR("CamEvent::addObject(" << name << ") - object with this name already exists"); throw 100; }
   //check object not already added, if it is, print a warning and do nothing 
   for(std::map<std::string,CamObject*>::iterator it=m_objects.begin();it!=m_objects.end();++it) {
      if(it->second == object) {
         std::cout << "CamEvent::addObject with name=" << name << ": object already in event with name=" << it->first << std::endl;
         return object;
      }
   }
   m_objects[name] = object;
   object->setParent(this);
   return object;
}

void CamEvent::changeName(CamObject* obj, const std::string& newName) {
   if(newName=="") return;
   //find this obj if we can
   for(std::map<std::string,CamObject*>::iterator it=m_objects.begin();it!=m_objects.end();++it) {
      if(it->second == obj) {
         //found object. do we need to do anything?
         if(newName==it->first) return;
         //ok. Remove this object
         m_objects.erase(it);
         break;
      }
   }
   //now add it back in with the new name
   if(m_objects.find(newName) != m_objects.end()) {
      CAMERROR("CamEvent::changeName() - object with name " << newName << " already exists"); throw 1;
   }
   m_objects[newName] = obj;
}

const std::string& CamEvent::getObjectName(CamObject* obj) {
   //find this obj if we can
   for(std::map<std::string,CamObject*>::iterator it=m_objects.begin();it!=m_objects.end();++it) {
      if(it->second == obj) {
         //found object. do we need to do anything?
         return it->first;
      }
   }
   return CamObject::emptyString;
}

void CamEvent::print(const std::string& propsToShow, bool callDynamic) {
  if(propsToShow.find("vectors")!=std::string::npos) {
    //list all the IParticleContainers
    ServiceHandle<IClassIDSvc> clidSvc("ClassIDSvc","CamEvent");
    CLID id;
    clidSvc->getIDOfTypeName("xAOD::IParticleContainer",id).ignore();
    std::vector<std::string> myKeys;
    m_evtStore->keys(id,myKeys);
    std::cout << "Vectors: " << std::endl;
    for(auto& s : myKeys) std::cout << "  " << s << std::endl;
    return;
  }
  CamObject::print(propsToShow,callDynamic);
}

void CamEvent::reinit() {
  clear();
  setAuxElement(m_evtStore->tryConstRetrieve<xAOD::EventInfo>());
  m_d3pdIndex = -1; //because EventInfo has an index of 0, so we move back to -1
  if(m_xAODParticle) 
     this->set("isSimulation","(AnalysisCamEvent)CamEvent::isSimulation - bool for if simulation or not") = CamObject::Function(&CamEvent::isSimulation);
}
