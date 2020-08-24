#ifndef ANALYSISCAM_CAMDEQUE_H
#define ANALYSISCAM_CAMDEQUE_H

#include "CamObject.h"
#include "xAODBase/IParticleContainer.h"
#include <deque>

class CamVector;

//not allowed vectors/deques of references
class CamDeque : public CamObject, public std::deque<CamObject*> {
  friend class CamVector;
   private:
      template<typename T> static bool compareAny(const CamObject* c1,const CamObject* c2,const std::string& key) { return (const_cast<CamObject*>(c1)->get<T>(key)>const_cast<CamObject*>(c2)->get<T>(key)); } 
      template<typename T> static bool compareAnyReverse(const CamObject* c1,const CamObject* c2,const std::string& key) { return (const_cast<CamObject*>(c1)->get<T>(key)<const_cast<CamObject*>(c2)->get<T>(key)); } 
      
      static bool comparePt(const CamObject* c1,const CamObject* c2) { return (c1->Pt()>c2->Pt()); }

   public:
      using CamObject::operator=;


      operator std::vector<const xAOD::IParticle*>() const {
         std::vector<const xAOD::IParticle*> out;
         for(auto i : *this) out.push_back(i->ae<xAOD::IParticle>());
         return out;
      } 

      virtual std::string applyTool(const std::string& toolName,std::string label="",const std::string& option="");
      virtual std::string applyService(const std::string& toolName,std::string label="",const std::string& option="");

      CamDeque(const CamDeque&) = default; //must be explicitly defined here 
      CamDeque(CamDeque&&) = default;

      CamDeque(const xAOD::IParticleContainer &);
       CamDeque(std::string prefix="",int d3pdIndex=-1);
       ~CamDeque();

      void sort_Pt() { std::sort(this->begin(),this->end(),CamDeque::comparePt); }
      template<typename T> void sort(const std::string& key, bool ascending=false) { 
         std::sort(this->begin(),this->end(),std::bind<bool>( (ascending) ? CamDeque::compareAnyReverse<T> : CamDeque::compareAny<T>, std::placeholders::_1, std::placeholders::_2, key ) ); } //sort by the property given by key.
      void sort(const std::string& key, bool ascending=false) { return sort<double>(key, ascending); }

      void resize(const std::string&& key);
      void resize(unsigned int nLoad);

      virtual bool any_is(const std::string& key, int max=-1);
      virtual bool any_is(std::function<bool(CamObject&)> r, int max=-1);
      unsigned int size(const std::string& key="");
      unsigned int size(std::function<bool(CamObject&)> r);
      
      unsigned int removeOverlapping(CamObject& c,double dR,std::string eta="",std::string phi="",std::string eta2="",std::string phi2=""); //eta1/phi1 is this deque's properties
      unsigned int removeOverlapping(CamDeque& c,double dR,std::string eta1="",std::string phi1="",std::string eta2="",std::string phi2="");
      unsigned int removeOverlapping(CamVector& c,double dR,std::string eta="",std::string phi="",std::string eta2="",std::string phi2="");


      unsigned int remove(std::string flag,bool deleteOnRemove=false);

      void adopt();

      CamDeque* subset(std::string key,bool adopt=false, bool addToEvent=true);//if adopt is true, subset becomes object parents
      CamDeque* independentSubset(std::string key, bool addToEvent=true);
      CamDeque* subset(std::function<bool(CamObject&)> func,bool adopt=false, bool addToEvent=true);//if adopt is true, subset becomes object parents
      CamDeque* independentSubset(std::function<bool(CamObject&)> func, bool addToEvent=true);

      template<typename W, typename V> CamDeque* subset(W* instance, V (W::*pointer)(CamObject&), bool adopt=false, bool addToEvent=true ) {
         CamDeque *out = new CamDeque();
         out->setParent(m_parent);//give subset the same parent as this set
         CamDeque::iterator it = CamDeque::begin();
         if(addToEvent) addSubsetToEvent(out);
         for(;it!=CamDeque::end();++it) {
            if((instance->*pointer)(**it)) {
               if(adopt) (*it)->setParent(out); //adopt the object if necessary
               out->push_back(*it);
            }
         }
         return out;
      }

      template<typename W, typename V> CamDeque* independentSubset(W* instance, V (W::*pointer)(CamObject&), bool addToEvent=true ) {
         CamDeque *out = new CamDeque();
         out->setParent(m_parent);//give subset the same parent as this set
         CamDeque::iterator it = CamDeque::begin();
         if(addToEvent) addSubsetToEvent(out);
         for(;it!=CamDeque::end();++it) {
            if((instance->*pointer)(**it)) {
               CamObject *newObj = new CamObject(**it); newObj->setParent(out); out->push_back(newObj);
            }
         }
         return out;
      }

      void all_call(std::string key);

      template<typename W> void all_cache(std::string searchTerm) {
         CamDeque::iterator end = CamDeque::end();
         for(CamDeque::iterator it = CamDeque::begin();it!=end;++it) {
            (*it)->cache<W>(searchTerm);
         }
      }

      void all_print(std::string propsToShow="") {
         CamDeque::iterator end = CamDeque::end();
         for(CamDeque::iterator it = CamDeque::begin();it!=end;++it) {
            (*it)->print(propsToShow);
         }
      }

      void all_cacheAny(std::string searchTerm);

      void removeUnowned(); //should be called on all deques before clearing. CamEvent will do this
      void clear();


      void push_back(CamObject* in);


      template<typename W> std::vector<W> all_Pt() {
         std::vector<W> out;
         std::deque<CamObject*>::iterator end = std::deque<CamObject*>::end();
         for(std::deque<CamObject*>::iterator it = std::deque<CamObject*>::begin();it!=end;++it) {
            out.push_back((*it)->Pt());
         }
         return out;
      }

      template<typename W> std::vector<W> all_Eta() {
         std::vector<W> out;
         std::deque<CamObject*>::iterator end = std::deque<CamObject*>::end();
         for(std::deque<CamObject*>::iterator it = std::deque<CamObject*>::begin();it!=end;++it) {
            out.push_back((*it)->Eta());
         }
         return out;
      }

      template<typename W> std::vector<W> all_Phi() {
         std::vector<W> out;
         std::deque<CamObject*>::iterator end = std::deque<CamObject*>::end();
         for(std::deque<CamObject*>::iterator it = std::deque<CamObject*>::begin();it!=end;++it) {
            out.push_back((*it)->Phi());
         }
         return out;
      }

      template<typename W> std::vector<W> all_E() {
         std::vector<W> out;
         std::deque<CamObject*>::iterator end = std::deque<CamObject*>::end();
         for(std::deque<CamObject*>::iterator it = std::deque<CamObject*>::begin();it!=end;++it) {
            out.push_back((*it)->E());
         }
         return out;
      }

      template<typename W> std::vector<W> all_get(const std::string& key) {
         std::vector<W> out;
         std::deque<CamObject*>::iterator end = std::deque<CamObject*>::end();
         for(std::deque<CamObject*>::iterator it = std::deque<CamObject*>::begin();it!=end;++it) {
            out.push_back((*it)->get<W>(key));
         }
         return out;
      }
      //need next function to resolve overload ambiguities introduced by having all_get method that accepts std::function (see below)
      template<typename W> std::vector<W> all_get(const char* key) {
         return all_get<W>(std::string(key));
      }

      template<typename W> std::vector<W> all_get(W (CamObject::*memberMethod)() const) {
         std::vector<W> out;
         std::deque<CamObject*>::iterator end = std::deque<CamObject*>::end();
         for(std::deque<CamObject*>::iterator it = std::deque<CamObject*>::begin();it!=end;++it) {
            out.push_back((*it)->get<W>(memberMethod));
         }
         return out;
      }
      template<typename W> std::vector<W> all_get(W (CamObject::*memberMethod)()) {
         std::vector<W> out;
         std::deque<CamObject*>::iterator end = std::deque<CamObject*>::end();
         for(std::deque<CamObject*>::iterator it = std::deque<CamObject*>::begin();it!=end;++it) {
            out.push_back((*it)->get<W>(memberMethod));
         }
         return out;
      }

      template<typename W> std::vector<W> all_get(std::function<W(CamObject&)> func) {
         std::vector<W> out;
         std::deque<CamObject*>::iterator end = std::deque<CamObject*>::end();
         for(std::deque<CamObject*>::iterator it = std::deque<CamObject*>::begin();it!=end;++it) {
            out.push_back(func((**it)));
         }
         return out;
      }
       void all_call(std::function<void(CamObject&)> func) {
         std::deque<CamObject*>::iterator end = std::deque<CamObject*>::end();
         for(std::deque<CamObject*>::iterator it = std::deque<CamObject*>::begin();it!=end;++it) {
           func((**it));
         }
      }

      template<typename W> std::vector<W> all_valid_get(const std::string& key, bool& isValid, bool checkMap=true, bool checkStoregate=true) {
         std::vector<W> out;
         std::deque<CamObject*>::iterator end = std::deque<CamObject*>::end();
         bool anyValid(false); //need at least 1 to be valid for return to be 'valid'
         for(std::deque<CamObject*>::iterator it = std::deque<CamObject*>::begin();it!=end;++it) {
            out.push_back((*it)->valid_get<W>(key,isValid,checkMap,checkStoregate));
            if(isValid) anyValid=true;
         }
         isValid=anyValid;
         return out;
      }

      std::vector<TLorentzVector> getTLVVector() {
         std::vector<TLorentzVector> out;
         std::deque<CamObject*>::iterator end = std::deque<CamObject*>::end();
         for(std::deque<CamObject*>::iterator it = std::deque<CamObject*>::begin();it!=end;++it) {
            out.push_back((**it));
         }
         return out;
      }


      virtual CamDeque& all_evaluate_set(std::string key) { evaluateDynamic=true;lastActionWasAdd=true;lastKey=key;return(*this);} //FIXME!
      virtual CamDeque& all_set(std::string key,const char* docu="") { 
         if(CamObject::Documentation) m_docuMap[key] = docu;
         evaluateDynamic=false;lastActionWasAdd=true;lastKey=key;return (*this); }
      virtual CamDeque& set(std::string key,const char* docu="") { 
         if(CamObject::Documentation) m_docuMap[key] = docu;
         lastActionWasAdd=false;lastKey = key; return (*this);}
      //virtual CamDeque& set(const std::string&& key) { lastActionWasAdd=false;lastKey = std::move(key); return (*this);}


      //appends elements of a vector to this deque ... it's very important the vectors aren't 'modified' after this operation
      virtual CamDeque& operator+=(CamVector& v);
      virtual CamDeque& operator+=(CamDeque& v);

      virtual CamObject* operator[](unsigned int a) { return this->at(a); }

      //wrap char* in std::string
      virtual void operator=(const char* a) { 
         if(lastActionWasAdd) { //sets to all children 
            if(CamObject::Documentation){ for(auto& obj : *this) obj->set(lastKey,m_docuMap[lastKey].c_str()) = a; m_docuMap.erase(lastKey); }
            else for(auto& obj : *this) obj->set(lastKey) = a; //the object does the wrapping for us
            lastActionWasAdd=false;
         } else {
            CamObject::operator=(a);
         }
         lastKey=""; 
      }

      template <typename W> void operator=(const W& a) { 
         if(lastActionWasAdd) { //sets to all children 
            if(CamObject::Documentation){ for(auto& obj : *this) obj->set(lastKey,m_docuMap[lastKey].c_str()) = a; m_docuMap.erase(lastKey);}
            else for(auto& obj : *this) obj->set(lastKey) = a;
            lastActionWasAdd=false;
         } else {
            CamObject::operator=(a);
         }
         lastKey=""; 
      }

      template <typename W> CamObject& operator=(const W&& a) { 
         if(lastActionWasAdd) { //sets to all children ... 
            if(CamObject::Documentation){ for(auto& obj : *this) obj->set(lastKey,m_docuMap[lastKey].c_str()) = a; m_docuMap.erase(lastKey);}
            else for(auto& obj : *this) obj->set(lastKey) = a;
            lastActionWasAdd=false;
         } else {
            CamObject::operator=(std::move(a));
         }
         lastKey=""; return *this;
      }

      //looks for the object with d3pdIndex given by i. If not found, then return 0
      CamObject* at_index(int i);

      //resizes to IParticleContainer size and sets auxelements of each child to each IParticle
      void setIParticleContainer(const xAOD::IParticleContainer&);

      const xAOD::IParticleContainer* container() const { return m_cont; }

   protected:

   private:

      void addSubsetToEvent(CamDeque* in);

      bool evaluateDynamic;
      bool lastActionWasAdd;
      unsigned int m_subsetCounter;
      
      const xAOD::IParticleContainer* m_cont = 0;

 public: 
      ClassDef(CamDeque,1)

};
CLASS_DEF( CamDeque , 226773339 , 1 )


#endif
