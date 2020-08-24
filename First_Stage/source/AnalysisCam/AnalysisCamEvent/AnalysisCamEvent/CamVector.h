#ifndef ANALYSISCAM_CAMVECTOR_H
#define ANALYSISCAM_CAMVECTOR_H

#include "CamObject.h"
#include "xAODBase/IParticleContainer.h"
#include <vector>

class CamDeque;

//not allowed vectors/deques of references
class CamVector : public CamObject, public std::vector<CamObject> {
   private:
      template<typename T> static bool compareAny(const CamObject& c1,const CamObject& c2,const std::string& key) { 
         return (const_cast<CamObject&>(c1).get<T>(key)>const_cast<CamObject&>(c2).get<T>(key)); } 
      template<typename T> static bool compareAnyReverse(const CamObject& c1,const CamObject& c2,const std::string& key) { 
         return (const_cast<CamObject&>(c1).get<T>(key)<const_cast<CamObject&>(c2).get<T>(key)); } 
      static bool comparePt(const CamObject& c1,const CamObject& c2) { return (c1.Pt()>c2.Pt()); }

   public:
      using CamObject::operator=;


      virtual std::string applyTool(const std::string& toolName,std::string label="",const std::string& option="");
      virtual std::string applyService(const std::string& toolName,std::string label="",const std::string& option="");

      CamVector(const CamVector&) = default; //must be explicitly defined here 
      CamVector(CamVector&&) = default;

      CamVector(const xAOD::IParticleContainer &);
      CamVector(std::string prefix="",int d3pdIndex=-1);
      ~CamVector();

      void sort_Pt() { std::sort(this->begin(),this->end(),CamVector::comparePt); }
      template<typename T> void sort(const std::string& key, bool ascending=false) { //sort by the property given by key.
         std::sort(this->begin(),this->end(),std::bind<bool>( (ascending) ? CamVector::compareAnyReverse<T> : CamVector::compareAny<T>, std::placeholders::_1, std::placeholders::_2, key ) );
      }
      void sort(const std::string& key, bool ascending=false) { return sort<double>(key,ascending); }
      
      //template<typename T> void fill(std::string key);
      void resize(const std::string& key);
      void resize(unsigned int nLoad);

      //void push_back(const CamObject& x);
      bool any_is(const std::string& key, int max=-1);
      unsigned int size(const std::string& key="");
      bool any_is(const char* key, int max=-1);
      bool any_is(std::function<bool(CamObject&)> func, int max=-1);
      unsigned int size(std::function<bool(CamObject&)> func);
      
      unsigned int removeOverlapping(CamObject& c,double dR,std::string eta="",std::string phi="",std::string eta2="",std::string phi2="");
      unsigned int removeOverlapping(CamVector& c,double dR,std::string eta="",std::string phi="",std::string eta2="",std::string phi2="");
      unsigned int removeOverlapping(CamDeque& c,double dR,std::string eta="",std::string phi="",std::string eta2="",std::string phi2="");

      unsigned int remove(const std::string& flag);

      void adopt();

      CamVector* subset(const char* key, bool addToEvent=true);
      CamVector* subset(std::function<bool(CamObject&)> func, bool addToEvent=true);
      CamDeque* subsetDeque(const char* key, bool adopt=false, bool addToEvent=true); //creates a deque instead of a vector.... deque SHOULD NEVER OWN THESE OBJECTS!! Adoption should be temporary!!
      CamDeque* subsetDeque(std::function<bool(CamObject&)> func, bool adopt=false, bool addToEvent=true);

/*
      template<typename W, typename V> CamVector* subset(W* instance, V (W::*pointer)(CamObject&), bool addToEvent=true ) {
         CamVector *out = new CamVector(m_prefix);
         //copy the property maps
         out->setPropertyMap(m_propMap);
         out->setParent(m_parent);//give subset the same parent as this set
         CamVector::iterator it = CamVector::begin();
         if(addToEvent) addSubsetToEvent(out);
         for(;it!=CamVector::end();++it) {
            if((instance->*pointer)(*it)) {
               out->push_back(*it); //this creates a copy because this is a vector
               out->back().setParent(out);
            }
         }
         return out;
      }
*/

      void all_call(std::string key);

      template<typename W, typename V> void all_cache(std::string searchTerm) {
         CamVector::iterator end = CamVector::end();
         for(CamVector::iterator it = CamVector::begin();it!=end;++it) {
            it->cache<W>(searchTerm);
         }
      }

      void all_print(std::string propsToShow="") {
         CamVector::iterator end = CamVector::end();
         for(CamVector::iterator it = CamVector::begin();it!=end;++it) {
            it->print(propsToShow);
         }
      }

      void all_cacheAny(std::string searchTerm);


      void push_back(CamObject& in);

      void append(CamVector& in); //add the elements of the in vector to this vector (copies them)

      template<typename W> std::vector<W> all_Pt() {
         std::vector<W> out;
         std::vector<CamObject>::iterator end = std::vector<CamObject>::end();
         for(std::vector<CamObject>::iterator it = std::vector<CamObject>::begin();it!=end;++it) {
            out.push_back((it)->Pt());
         }
         return out;
      }

      template<typename W> std::vector<W> all_Eta() {
         std::vector<W> out;
         std::vector<CamObject>::iterator end = std::vector<CamObject>::end();
         for(std::vector<CamObject>::iterator it = std::vector<CamObject>::begin();it!=end;++it) {
            out.push_back((it)->Eta());
         }
         return out;
      }

      template<typename W> std::vector<W> all_Phi() {
         std::vector<W> out;
         std::vector<CamObject>::iterator end = std::vector<CamObject>::end();
         for(std::vector<CamObject>::iterator it = std::vector<CamObject>::begin();it!=end;++it) {
            out.push_back((it)->Phi());
         }
         return out;
      }

      template<typename W> std::vector<W> all_E() {
         std::vector<W> out;
         std::vector<CamObject>::iterator end = std::vector<CamObject>::end();
         for(std::vector<CamObject>::iterator it = std::vector<CamObject>::begin();it!=end;++it) {
            out.push_back((it)->E());
         }
         return out;
      }

      template<typename W> std::vector<W> all_get(const std::string& key) {
         std::vector<W> out;
         std::vector<CamObject>::iterator end = std::vector<CamObject>::end();
         for(std::vector<CamObject>::iterator it = std::vector<CamObject>::begin();it!=end;++it) {
            out.push_back((it)->get<W>(key));
         }
         return out;
      }
      //need next function to resolve overload ambiguities introduced by having all_get method that accepts std::function (see below)
      template<typename W> std::vector<W> all_get(const char* key) {
         //static const std::string myKey(key);
         return all_get<W>(std::string(key));
      }

      template<typename W> std::vector<W> all_get(W (CamObject::*memberMethod)() const) {
         std::vector<W> out;
         std::vector<CamObject>::iterator end = std::vector<CamObject>::end();
         for(std::vector<CamObject>::iterator it = std::vector<CamObject>::begin();it!=end;++it) {
            out.push_back((it)->get<W>(memberMethod));
         }
         return out;
      }
      template<typename W> std::vector<W> all_get(W (CamObject::*memberMethod)()) {
         std::vector<W> out;
         std::vector<CamObject>::iterator end = std::vector<CamObject>::end();
         for(std::vector<CamObject>::iterator it = std::vector<CamObject>::begin();it!=end;++it) {
            out.push_back((it)->get<W>(memberMethod));
         }
         return out;
      }

      template<typename W> std::vector<W> all_get(std::function<W(CamObject&)> func) {
         std::vector<W> out;
         std::vector<CamObject>::iterator end = std::vector<CamObject>::end();
         for(std::vector<CamObject>::iterator it = std::vector<CamObject>::begin();it!=end;++it) {
            out.push_back(func((*it)));
         }
         return out;
      }
       void all_call(std::function<void(CamObject&)> func) {
         std::vector<CamObject>::iterator end = std::vector<CamObject>::end();
         for(std::vector<CamObject>::iterator it = std::vector<CamObject>::begin();it!=end;++it) {
           func((*it));
         }
      }

      std::vector<TLorentzVector> getTLVVector() {
         std::vector<TLorentzVector> out;
         std::vector<CamObject>::iterator end = std::vector<CamObject>::end();
         for(std::vector<CamObject>::iterator it = std::vector<CamObject>::begin();it!=end;++it) {
            out.push_back((*it));
         }
         return out;
      }


      virtual CamVector& all_evaluate_set(std::string key) { evaluateDynamic=true;lastActionWasAdd=true;lastKey=key;return(*this);} //FIXME
      virtual CamVector& all_set(std::string key, const char* docu="") { 
         if(CamObject::Documentation) m_docuMap[key] = docu;
         evaluateDynamic=false;lastActionWasAdd=true;lastKey=key;return (*this); 
      }
      virtual CamVector& set(std::string key, const char* docu="") { lastActionWasAdd=false;lastKey = key;
       if(CamObject::Documentation) m_docuMap[key] = docu;
       return (*this);}
      //virtual CamVector& set(const std::string&& key) { lastActionWasAdd=false;lastKey = std::move(key); return (*this);}


      //virtual CamObject& operator[](unsigned int a) { return this->at(a); }

      //Everything else

      //adding this so that assignment of a vector to a vector doesn't generate the warnings about move assignments (in CamObject's assignment operator)
      CamVector& operator=(const CamVector&& rhs) {
        CamObject::operator=((CamObject&&)std::move(rhs));
        std::vector<CamObject>::operator=(std::move(rhs));
        return *this;
      }

      //wrap char* in std::string
      virtual void operator=(const char* a) { 
         if(lastActionWasAdd) { //sets to all children 
            if(CamObject::Documentation){ for(auto& obj : *this) obj.set(lastKey,m_docuMap[lastKey].c_str()) = a; m_docuMap.erase(lastKey); }
            else for(auto& obj : *this) obj.set(lastKey) = a; //the object does the wrapping for us
            lastActionWasAdd=false;
         } else {
            CamObject::operator=(a);
         }
         lastKey=""; 
      }

      template <typename W> void operator=(const W& a) { 
         if(lastActionWasAdd) { //sets to all children 
            if(CamObject::Documentation){ for(auto& obj : *this) obj.set(lastKey,m_docuMap[lastKey].c_str()) = a; m_docuMap.erase(lastKey);}
            else for(auto& obj : *this) obj.set(lastKey) = a;
            lastActionWasAdd=false;
         } else {
            CamObject::operator=(a);
         }
         lastKey=""; 
      }

      template <typename W> CamObject& operator=(const W&& a) { 
         if(lastActionWasAdd) { //sets to all children ... 
            if(CamObject::Documentation){ for(auto& obj : *this) obj.set(lastKey,m_docuMap[lastKey].c_str()) = a; m_docuMap.erase(lastKey);}
            else for(auto& obj : *this) obj.set(lastKey) = a;
            lastActionWasAdd=false;
         } else {
            //m_propMap[lastKey]=std::move(a);
            CamObject::operator=(std::move(a));
         }
         lastKey="";  return *this;
      }

      CamObject& operator [] (unsigned int i) { return at(i); }

      //looks for the object with d3pdIndex given by i. If not found, then return 0
      CamObject* at_index(int i);
      //looks for the object with auxelement given by ae, returns 0 if not found
      CamObject* at_ae(const SG::AuxElement* ae);

      //resizes to IParticleContainer size and sets auxelements of each child to each IParticle
      void setIParticleContainer(const xAOD::IParticleContainer&);

      const xAOD::IParticleContainer* container() const { return m_cont; }

   protected:

     
   private:
      void addSubsetToEvent(CamObject* in);

      bool evaluateDynamic;
      bool lastActionWasAdd;
      
      unsigned int m_subsetCounter;
      
      const xAOD::IParticleContainer* m_cont = 0;

 public:
      ClassDef(CamVector,1)

};
CLASS_DEF( CamVector , 264666541 , 1 )


#endif
