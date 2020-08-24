#ifndef ANALYSISCAM_CAMOBJECT_H
#define ANALYSISCAM_CAMOBJECT_H

//this class represents the internal event object that is used in the analysis
//It is not output directly (but it can be, thanks to reflex dictionaries - not guarenteed all members are ok though)

#include <vector>
#include "TLorentzVector.h"
#include "SGTools/CLASS_DEF.h"

#include "GaudiKernel/ServiceHandle.h"
#include "StoreGate/StoreGateSvc.h"
#include "SGTools/BuiltinsClids.h" 
#include "SGTools/StlVectorClids.h"
#include "SGTools/ClassID_traits.h"
#include "SGTools/safe_clid.h"
#include <map>
#include <string>
#include <set>
#include <iostream>
#include <sstream>
#include <boost/any.hpp>

//#include <boost/unordered_map.hpp>
#include <unordered_map>
#include <typeindex>

#include <cxxabi.h> //needed for demangling types

#include "xAODBase/IParticle.h"
#include "AthContainers/AuxTypeRegistry.h"

#include "AnalysisCamEvent/CamProperty.h"

#include "AthContainers/DataVector.h"

class CamObject;
class CamEvent;

template<typename W> class TLVGetter {
   public:
      static W get2(const std::string&, bool& isValid, CamObject*);
};

template<typename W> class ChildGetter {
   public:
      static bool contains(const std::string&, CamObject*) { return false; }
      static const W get2(const std::string&, bool& isValid, CamObject*);
};


/// This dummy object is needed for the templated get method
template<typename W, bool B> class DummyObj {
   public: 
      typedef typename std::remove_reference<W>::type WnoRef;
      static bool contains(const std::string&, ServiceHandle<StoreGateSvc>&) { return false; }
      static const WnoRef* get2(const std::string&, bool& isValid, ServiceHandle<StoreGateSvc>&) { isValid=false; return 0; }
};

template<typename W> class DummyObj<W,true> {
   public:
      static bool contains(const std::string& key, ServiceHandle<StoreGateSvc>& sg) { return sg->contains<W>(key); }
      static const W* get2(const std::string& key, bool& isValid, ServiceHandle<StoreGateSvc>& sg) { 
         if(!sg->contains<W>(key)) { 
            isValid=false;
            return 0;
         }
         const W* val;
         sg->retrieve(val,key).ignore();
         isValid=true;
         return val;
      }

};

#define CAMFATAL(x) (printTrace(std::ostringstream().flush() << x,"",true))
#define CAMERROR(x) (printTrace(std::ostringstream().flush() << x))
#define CAMERRORSUGGEST(x,y) (printTrace(std::ostringstream().flush() << x,y))



class CamObject : public TLorentzVector {
   friend class CamVector; //made friend so can modify private member properties freely
   friend class CamDeque;
   public:
    static const std::string emptyString;


      template<typename V> static auto Function(V myLambda) -> std::function<decltype(myLambda())()>  {
         return myLambda;
      }
#ifndef ANALYSISCAMEVENT_ANALYSISCAMEVENTDICT_H
#ifndef __CLING__
      template<typename V> static auto Function(V myLambda) -> std::function<decltype(myLambda(*this))(CamObject&)>  {
         return myLambda;
      }
      template<typename V> static auto Function(V myLambda) -> std::function<decltype(myLambda(*this,0))(CamObject&,va_list)>  {
         return myLambda;
      }
#endif
#endif

      template<typename V> static std::function<V()> Function(V (*pointer)()) { 
         return std::function<V()>(pointer); 
      }
      template<typename V> static std::function<V(CamObject&)> Function(V (*pointer)(CamObject&)) { 
         return std::function<V(CamObject&)>(pointer); 
      }
      template<typename V> static std::function<V(CamObject&,va_list)> Function(V (*pointer)(CamObject&, va_list)) { 
         return std::function<V(CamObject&,va_list)>(pointer); 
      }
      template<typename W, typename V> static std::function<V()> Function(V (W::*pointer)(),W* instance ) {
         return std::function<V(CamObject&)>(std::bind( pointer, instance ));
      }
      template<typename W, typename V> static std::function<V(CamObject&)> Function(V (W::*pointer)(CamObject&),W* instance ) {
         return std::function<V(CamObject&)>(std::bind( pointer, instance, std::placeholders::_1 ));
      }
      //in the method below, W is the class type, V is the return type
      template<typename W, typename V> static std::function<V(CamObject&,va_list)> Function(V (W::*pointer)(CamObject&, va_list),W* instance ) {
         return std::function<V(CamObject&,va_list)>(std::bind( pointer, instance, std::placeholders::_1,std::placeholders::_2 ));
      }

      

      using TLorentzVector::SetPtEtaPhiE;
      using TLorentzVector::Eta;

      Double_t Eta() const {
         if((*(dynamic_cast<const TLorentzVector*>(this)))==TLorentzVector(0,0,0,0)) {
            printTraceConst(std::ostringstream().flush() << "-----------WARNING: Something tried to get TLV Eta when not set... happened here...",false);
         }
         return TLorentzVector::Eta();
      }


      void SetPtEtaPhiE(Double_t pt, Double_t eta, Double_t phi, Double_t e) {
         if(e==0) {
            printTrace(std::ostringstream().flush() << "-----------WARNING: Something tried to SetPtEtaPhiE with E=0... happened here...","",false);
            CAMERROR("-----------WARNING: Something tried to SetPtEtaPhiE with E=0!!!!--------------");
         }
         TLorentzVector::SetPtEtaPhiE(pt,eta,phi,e);
      }

      static bool ForceCorrectType;
      static bool AutoSetTLV;
      static int MAX_STOREGATE_SUGGESTIONS;
      static bool Documentation;

      //const static std::string emptyString="";

       CamObject(const CamObject&) = default; //must be explicitly defined here so that not deleted by definition of move assignment operator below
       CamObject(const SG::AuxElement& a) : lastKey(""),m_evtStore("StoreGateSvc","this"), m_d3pdIndex(a.index()),m_prefix(""),m_parent(0),m_xAODParticle(&a) {}
       CamObject(const xAOD::IParticle& p) : TLorentzVector(p.p4()), lastKey(""),m_evtStore("StoreGateSvc","this"), m_d3pdIndex(p.index()),m_prefix(""),m_parent(0),m_xAODParticle(&p) {}
       CamObject(int index=-1, CamObject* parent=0, std::string prefix="");
       CamObject(const TLorentzVector& v) : TLorentzVector(v), lastKey(""), m_evtStore("StoreGateSvc","this"), m_d3pdIndex(-1),m_prefix(""),m_parent(0),m_xAODParticle(0) {  }
       ~CamObject();

      
      virtual std::string applyTool(const std::string& toolName,std::string label="",const std::string& option="");
      virtual std::string applyService(const std::string& toolName,std::string label="",const std::string& option="");
      
      int getIndex() const { return m_d3pdIndex;}
      //the prefix is only used for retrieving from storegate.
      //the properties are stored without the prefix
      void setPrefix(const std::string& in) { m_prefix=in;}
      const std::string& getPrefix() const { return m_prefix;}
      const std::string& getName();
      ServiceHandle<StoreGateSvc>& evtStore() { return m_evtStore; }
      virtual CamObject* parent() { if(m_parent) return m_parent; return 0; }//i.e .just return parent
      void setParent(CamObject *parent) { 
            m_parent = parent;
            //also reassign the storegate
            if(parent) m_evtStore = parent->evtStore();
      }
      virtual CamEvent* event() { if(m_parent) return m_parent->event(); return 0; }//if has no parent, could create CamObject and then remember to destroy it in destructor


      bool is(const char* key);
      bool is(const std::string& key);
      bool is(dbool r) { return r(*this); }
      bool is(std::function<bool(CamObject&)> func){ return func(*this); }

      static void printTraceConst(std::ostream& message, bool throwError=true);
      void printTrace(std::ostream& message,std::string showSuggestions="",bool throwError=false);
      void print(std::string propsToShow="", bool callDynamic=true); //dumps all the properties to cout is propsToShow=""

      template<typename W> W& auxdecor(const std::string& key) { 
         assert (m_xAODParticle);
         return m_xAODParticle->auxdecor<W>(key);
      }

      template<typename W> const W& auxdata(const std::string& key) { 
         assert (m_xAODParticle);
         return m_xAODParticle->auxdata<W>(key);
      }
      
      template<typename W> const W* getLink(const std::string& key) {
        typedef ElementLink<DataVector<W>> truthLink;
        return *get<truthLink>(key);
      }

   double get(const std::string& key, ...); //typeless 'get' just assumes double
   double get(const char* key, ...);
   void call(const std::string& key, ...); //void type version of 'get'
   template<typename W> W get(const char* key, ...) {  
         //m_currentKey = key;
         va_list argptr;
         va_start(argptr,key);
         W result = getInternal<W>(key/*std::move(key)*/, argptr);
         va_end(argptr);
         return result;
   }

   template<typename W> W get(const std::string& key) {
     va_list argptr;
     return getInternal<W>(key,argptr);
   }

   template<typename W> W get(const std::string&& key) {
     va_list argptr;
     return getInternal<W>(key,argptr);
   }


   //memeber method getter 
   template<typename W> W get(W (CamObject::*memberMethod)() const) {
      return (this->*memberMethod)();
   }
   template<typename W> W get(W (CamObject::*memberMethod)()) {
      return (this->*memberMethod)();
   }
   
      template<typename W> W getInternal(const std::string& key, va_list args) {
          bool isValid=false;
          W out = TLVGetter<W>::get2(key,isValid,this); if(isValid) return out;

         std::unordered_map<std::string,boost::any>::iterator it = m_propMap.find(key);
         if(it!=m_propMap.end()) {
            bool isValid=true; //extract method will update this to false if the extraction fails
            W val = extract<W>(it->second,isValid,args);
            if(isValid) return val;
         }
        

         //try from xAOD auxdataConst 
         //rule is types must match exactly from auxdata (can't always be lazy ;-) )
         if(m_xAODParticle) {
            typedef typename std::remove_reference<W>::type WnoRef;
            //need to find the aux_id for this key, then find that aux_id in the getAuxIDs() held in the object's auxstore 
            //question: what's faster? get auxid from registry and then 'find' in the auxids, or iterate over auxids and 
            const SG::auxid_set_t& myauxid = m_xAODParticle->getAuxIDs();
            SG::AuxTypeRegistry& reg = SG::AuxTypeRegistry::instance();
            SG::auxid_t auxID = reg.findAuxID(key); //might have to add the clsname as second argument!
            if(auxID != SG::null_auxid && myauxid.find(auxID) != myauxid.end() && (*reg.getType(auxID) == typeid(WnoRef))) {
               //find matching key+type 
               assert (m_xAODParticle->container() != 0);
               return const_cast<WnoRef&>(m_xAODParticle->container()->template getData<WnoRef> (auxID, m_xAODParticle->index()));
            }

//             if(m_xAODParticle->isAvailable<WnoRef>(key)) {
//                return const_cast<WnoRef&>(m_xAODParticle->auxdata<WnoRef>(key)); //NOTE: how does this work? surely we should be forced to use auxdataConst on a const IParticle*
//                //copy it in to property map as type WnoRef and then return it 
//                /*boost::any& in = m_propMap[key];
//                in = const_cast<WnoRef&>(m_xAODParticle->auxdataConst<WnoRef>(key));
//                return *boost::any_cast<WnoRef>((&in));*/
//                
//             }//return m_xAODParticle->auxdecor<WnoRef>(key);
         }

         //try getting from storegate
         W val = retrieve_from_storegate<W>(key,isValid); //automatically adds to property map if successful
         if(isValid) {return val;}
         //try getting from parents NO MORE!
         //if(m_parent) return m_parent->getInternal<W>(key,args);

         //try getting from children 
         if(m_children.size()>0) {

            //if the key begins with one of the child aliases, then try to get from the specific child 
            //syntax is the alias would be followed by an underscore, so strip that...
            std::size_t upos = key.find('_');
            if(upos != std::string::npos) {
               std::string aliasName = key.substr(0,upos);
               auto it = m_childrenAliases.find(aliasName);
               if(it != m_childrenAliases.end()) {
                  W val = it->second->valid_get<W>(key.substr(upos+1),isValid);
                  if(isValid) return val;
               }
            }

            W sVal = ChildGetter<W>::get2(key,isValid,this);
            if(isValid) return sVal;
         }

         //die
         int status; 
         std::string ss = key; ss += " ("; ss += abi::__cxa_demangle(typeid(W).name(),0,0,&status); ss += ")";
         CAMERRORSUGGEST("Unable to get property '" << key << "' with type '" << abi::__cxa_demangle(typeid(W).name(),0,0,&status) << "'",ss);
         throw 10;
      }

      template<typename W> W retrieve_from_storegate(const std::string& key, bool& isValid) {
         typedef typename std::remove_reference<W>::type WnoRef;
         if(m_d3pdIndex<0) {
            //DummyObj<WnoRef,(ClassID_traits<WnoRef>::s_version)> dObj(this);
            const WnoRef* sVal = DummyObj<WnoRef,(ClassID_traits<WnoRef>::s_version)>::get2(m_prefix+key,isValid,m_evtStore);
            if(!isValid) return next_retrieve_from_storegate<W>(key,isValid);
            //retrieve now pushes things into property map (as the type it got them as) 23/11/2012
            boost::any& obj = m_propMap[key];
            obj=(*sVal);
            return *(boost::unsafe_any_cast<WnoRef>(&obj));
            //return *sVal;
         }else{
            //DummyObj<std::vector<WnoRef>,(ClassID_traits<std::vector<WnoRef> >::s_version)> dObj(this);
            const std::vector<WnoRef>* sVal = DummyObj<std::vector<WnoRef>,(ClassID_traits<std::vector<WnoRef> >::s_version)>::get2(m_prefix+key,isValid,m_evtStore);
            if(!isValid) return next_retrieve_from_storegate<W>(key,isValid);
            if(int(sVal->size())<=m_d3pdIndex) { isValid=false; return next_retrieve_from_storegate<W>(key,isValid); } //vector wasn't long enough
            //retrieve now pushes things into the property map (as the type it got them as) 23/11/2012
            const WnoRef& val = sVal->at(m_d3pdIndex);
            boost::any& obj = m_propMap[key];
            obj=W(val); //must make a copy so that it's mutable
            return *(boost::unsafe_any_cast<WnoRef>(&obj)); 
         }
      }


      //works like get, but updates a valid flag to say if its ok
      template<typename W> W valid_get(const std::string& key, bool& isValid, bool checkMap=true, bool checkStoregate=true) {
         W out = TLVGetter<W>::get2(key,isValid,this); if(isValid) return out;

         va_list args;
         if(checkMap) {
            std::unordered_map<std::string,boost::any>::iterator it = m_propMap.find(key);
            if(it!=m_propMap.end()) {
               W val = extract<W>(it->second,isValid,args);
               if(isValid) return val;
            }
         }
         isValid=false;
         //try from AuxElement if available 
         if(m_xAODParticle) {
            typedef typename std::remove_reference<W>::type WnoRef;
            SG::AuxTypeRegistry& reg = SG::AuxTypeRegistry::instance();
            SG::auxid_t auxID = reg.findAuxID(key); //might have to add the clsname as second argument!
            const SG::auxid_set_t& myauxid = m_xAODParticle->getAuxIDs();
            if(auxID != SG::null_auxid && myauxid.find(auxID) != myauxid.end() && (*reg.getType(auxID) == typeid(WnoRef))) {
               isValid=true;
               assert (m_xAODParticle->container() != 0);
               return const_cast<WnoRef&>(m_xAODParticle->container()->template getData<WnoRef> (auxID, m_xAODParticle->index()));
            }
         }
         //try getting from storegate
         if(checkStoregate) {
            W val = retrieve_from_storegate<W>(key,isValid);
            if(isValid) {return val;}
         }
         //try getting from children 
         if(m_children.size()>0) {
            //if the key begins with one of the child aliases, then try to get from the specific child 
            //syntax is the alias would be followed by an underscore, so strip that...
            std::size_t upos = key.find('_');
            if(upos != std::string::npos) {
               std::string aliasName = key.substr(0,upos);
               auto it = m_childrenAliases.find(aliasName);
               if(it != m_childrenAliases.end()) {
                  W val = it->second->valid_get<W>(key.substr(upos+1),isValid,checkMap,checkStoregate);
                  if(isValid) return val;
               }
            }

            W sVal = ChildGetter<W>::get2(key,isValid,this);
            if(isValid) return sVal;
         }
         return emptyObject<W>();
      }



      template<typename W> W extract(boost::any& in, bool& isValid, va_list args) {
         typedef W (*gg)(CamObject&, va_list);
         typedef std::function<W(CamObject&,va_list)> ggf;
         typedef W (*hh)(CamObject&);
         typedef std::function<W(CamObject&)> hhf;

         typedef W (*ii)(); //straight function, no params 
         typedef std::function<W()> iif; //straight function, no params, c++11 style 

         typedef W (TLorentzVector::*tlvMethodConst)() const;
         typedef W (CamObject::*camMethod)();
         typedef W (CamObject::*camMethodConst)() const;
         typedef W (TLorentzVector::*tlvMethod)();

         typedef typename std::remove_reference<W>::type WnoRef;
         isValid=true; //gets updated to false in nextExtract after failure
/*
         static const std::type_index target_type(in.type());
         static const std::type_index WnoRef_type(typeid(WnoRef));if(WnoRef_type == target_type) return *(boost::any_cast<WnoRef>(&(in)));
         static const std::type_index hhf_type(typeid(hhf));if(hhf_type == target_type) return (*(boost::any_cast<hhf>(&(in))))(*this);
         static const std::type_index ggf_type(typeid(ggf));if(ggf_type == target_type) return (*(boost::any_cast<ggf>(&(in))))(*this,args);
         static const std::type_index iif_type(typeid(iif));if(iif_type == target_type) return (*(boost::any_cast<iif>(&(in))))();
         static const std::type_index tlvMethodConst_type(typeid(tlvMethodConst));if(tlvMethodConst_type == target_type) return (this->*(*(boost::any_cast<tlvMethodConst>(&(in)))))();
         static const std::type_index tlvMethod_type(typeid(tlvMethod));if(tlvMethod_type == target_type) return (this->*(*(boost::any_cast<tlvMethod>(&(in)))))();
         static const std::type_index camMethodConst_type(typeid(camMethodConst));if(camMethodConst_type == target_type) return (this->*(*(boost::any_cast<camMethodConst>(&(in)))))();
         static const std::type_index camMethod_type(typeid(camMethod));if(camMethod_type == target_type) return (this->*(*(boost::any_cast<camMethod>(&(in)))))();
         static const std::type_index gg_type(typeid(gg));if(gg_type == target_type) return (*(boost::any_cast<gg>(&(in))))(*this,args);
         static const std::type_index hh_type(typeid(hh));if(hh_type == target_type) return (*(boost::any_cast<hh>(&(in))))(*this);
         static const std::type_index ii_type(typeid(ii));if(ii_type == target_type) return (*(boost::any_cast<ii>(&(in))))();
*/

         if(WnoRef *tval = boost::any_cast<WnoRef>(&(in))) return *tval;
         else if(hhf *dval = boost::any_cast<hhf>(&(in))) return (*dval)(*this);
         else if(ggf* ddval = boost::any_cast<ggf>(&(in))) return (*ddval)(*this,args);
         else if(iif *dval = boost::any_cast<iif>(&(in))) return (*dval)();
         else if(tlvMethodConst* eval = boost::any_cast<tlvMethodConst>(&(in))) return (dynamic_cast<TLorentzVector*>(this)->*(*eval))();
         else if(camMethod* eval = boost::any_cast<camMethod>(&(in))) return (this->*(*eval))();
         else if(tlvMethod* eval = boost::any_cast<tlvMethod>(&(in))) return (dynamic_cast<TLorentzVector*>(this)->*(*eval))();
         else if(camMethodConst* eval = boost::any_cast<camMethodConst>(&(in))) return (this->*(*eval))();
         else if(hh *dval = boost::any_cast<hh>(&(in))) return (*dval)(*this); //old style fpointer ... deprecated
         else if(gg *dval = boost::any_cast<gg>(&(in))) return (*dval)(*this, args); //old style fpointer .. deprecated
         else if(ii *dval = boost::any_cast<ii>(&(in))) return (*dval)(); //old style fpointer ... deprecated 

         return nextExtract<W>(in,isValid,args);
      }


      

      //returns a string of the type of property, or '' if property is not found
      //parseFunctions=true means will try to detect std::function and give back the return type of that!
      std::string type(const std::string& key, bool checkMap=true, bool checkStoregate=true, bool parseFunctions=false);

      enum PropertyLocation { UNAVAILABLE=0, MAP=1, STOREGATE=2, PARENT=3/*deprecated*/,AUXELEMENT=4,CHILD=8,TLV=16 };

      //specify checkWhere as logical OR of PropertyLocations 
      template<typename W> int contains(const std::string& key, int checkWhere) {
        return contains<W>(key,checkWhere&MAP,checkWhere&STOREGATE,checkWhere&AUXELEMENT,checkWhere&CHILD,checkWhere&TLV);
      }

      template<typename W> int contains(const std::string& key, bool checkMap=true, bool checkStoregate=true, bool checkAuxElement=true, bool checkChild=true, bool checkTLV=true) {
         bool isValid(false);
         if(checkTLV) TLVGetter<W>::get2(key,isValid,this);
          if(isValid) return TLV;

         typedef typename std::remove_reference<W>::type WnoRef;
         if(checkMap) {
            //RRRstd::map<std::string,boost::any>::iterator it = m_propMap.find(key);
            std::unordered_map<std::string,boost::any>::iterator it = m_propMap.find(key);
            if(it!=m_propMap.end()) {
               //check the type matches
               typedef std::function<W(CamObject&,va_list)> ggf;
               typedef std::function<W(CamObject&)> hhf;
               //typedef W (*dT)(CamObject&); //static methods that accept the CamObject
               //typedef W (*dTv)(CamObject&, va_list); //static methods accepting a CamObject and a var list
               if(it->second.type()==typeid(WnoRef)) return MAP;
               else if(it->second.type()==typeid(ggf)) return MAP;
               else if(it->second.type()==typeid(hhf)) return MAP;
               return UNAVAILABLE; //stops here, but maybe we want to let it look for a different type ...?
            }
         }
         if(checkAuxElement && m_xAODParticle) {
            //see if auxid found in my aux ids 
            const SG::auxid_set_t& myauxid = m_xAODParticle->getAuxIDs();
            SG::AuxTypeRegistry& reg = SG::AuxTypeRegistry::instance();
            SG::auxid_t auxID = reg.findAuxID(key); //might have to add the clsname as second argument!
            if(auxID != SG::null_auxid && myauxid.find(auxID) != myauxid.end() && typeid(WnoRef)==*reg.getType(auxID)) return AUXELEMENT;
         }
         if(checkStoregate) {
            //have to test d3pdIndex to see if we are trying to get a vector of T or just a T
               if(m_d3pdIndex<0) {
                  //DummyObj<WnoRef,(ClassID_traits<WnoRef>::s_version)> dObj(this);
                  if(DummyObj<WnoRef,(ClassID_traits<WnoRef>::s_version)>::contains(m_prefix+key,m_evtStore)) return STOREGATE;
               }else{
                  //actually need to do a retrieve here to check the vector length is good enough ...
                  retrieve_from_storegate<W>(key,isValid);
                  if(isValid) return STOREGATE;
                  
                  //DummyObj<std::vector<WnoRef>,(ClassID_traits<std::vector<WnoRef> >::s_version)>::get(m_prefix+key,m_evtStore))
               }
         }
         //check children if there are any 
         if(checkChild && m_children.size()>0) if(ChildGetter<W>::contains(key,this)) return CHILD;
         return UNAVAILABLE;
      }

      int contains(const std::string& key,bool checkMap=true, bool checkStoregate=true) {
         return contains<double>(key,checkMap,checkStoregate);
      }

      //this method will search the storegate for extra properties matching the given searchterm
      //this is slow!!!
      template<typename W> void cache(std::string searchTerm) {
         //only load keys if been given a wild character
         if(searchTerm.find("*")==std::string::npos) {
            get<W>(searchTerm);return;
         } 
         std::vector<std::string> keys;
         if(m_d3pdIndex<0) m_evtStore->keys<W>(keys);
         else m_evtStore->keys<std::vector<W> >(keys);
         //pattern match with the searchTerm
         std::vector<std::string>::iterator end = keys.end();
         const char* s = searchTerm.c_str();
         for(std::vector<std::string>::iterator it=keys.begin();it!=end;++it) {
            //skip stuff not starting with our prefix
            if(it->find(m_prefix)!=0) continue;
            it->erase(0,m_prefix.length());
            //skip things we already loaded
            if(m_propMap.find((*it))!=m_propMap.end()) continue;
            if(wildcmp(s,it->c_str())) {
               get<W>((*it)); //loads it into the map
            }
         }
      }

      //call cache with the 5 basic types
      void cacheAny(std::string searchTerm);

      //return itself so that the assignment operator overloads can "set" the right map
      virtual CamObject& set(std::string key, const char* docu="") { /* pass by value and rely on move semantics */
         if(CamObject::Documentation) { m_docuMap[key] = std::string(docu); }
         lastKey = key; return (*this);} 
      //virtual CamObject& set(const std::string&& key) { lastKey = std::move(key); return (*this);}



      CamObject& operator=(const CamObject&& rhs) {
         if(lastKey.empty()) {
            m_propMap = rhs.m_propMap;
            if(CamObject::Documentation) m_docuMap = rhs.m_docuMap;
            lastKey = rhs.lastKey;
            //m_currentKey = rhs.m_currentKey;
            m_evtStore = rhs.m_evtStore;
            m_d3pdIndex = rhs.m_d3pdIndex;
            m_prefix = rhs.m_prefix;
            m_parent = rhs.m_parent; //should I std::swap this instead?
            m_children = rhs.m_children;m_childrenAliases = rhs.m_childrenAliases;
            m_xAODParticle = rhs.m_xAODParticle;
            TLorentzVector::operator=(rhs);
            return *this;
         } else {
            //someones creating a CamObject as an actual property ...
            m_propMap[lastKey]=rhs;lastKey="";
            return *this;
         }
      }
      //forces us to define the copy constructor too! .. see above



      void operator=(const TLorentzVector& a) { 
         if(lastKey=="") {
            TLorentzVector::operator=(a);
         } else {
            m_propMap[lastKey]=a;lastKey="";
         }
      }

      CamObject& operator=( const CamObject& rhs ) {
         if(lastKey.empty()) {
            m_propMap = rhs.m_propMap;
            if(CamObject::Documentation) m_docuMap = rhs.m_docuMap;
            lastKey = rhs.lastKey;
            //m_currentKey = rhs.m_currentKey;
            m_evtStore = rhs.m_evtStore;
            m_d3pdIndex = rhs.m_d3pdIndex;
            m_prefix = rhs.m_prefix;
            m_parent = rhs.m_parent;
            m_children = rhs.m_children;m_childrenAliases = rhs.m_childrenAliases;
            m_xAODParticle = rhs.m_xAODParticle;
            TLorentzVector::operator=(rhs);
            return *this;
         } else {
            //someones creating a CamObject as an actual property ...
            m_propMap[lastKey]=rhs;lastKey="";
            return *this;
         }
      }
/*
      void operator=(const CamObject& a) { 
         if(lastKey=="") {
            TLorentzVector::operator=(a);
            //m_propMap = *(a.getPropertyMap()); - to do a copy need const casting
         } else {
            m_propMap[lastKey]=a;lastKey="";
         }
      }
*/
      CamObject operator+(const CamObject& a) {
         //add the TLorentzVector bits... could also have it merge the property map in future maybe...
         TLorentzVector* me = dynamic_cast<TLorentzVector*>(this);
         CamObject out(*me); out.m_children.clear();out.m_children.push_back(this);
          out += a; //this adds to the children as well
         return out;
      }

      void operator++(int) {
         if(lastKey=="") {
            CAMERROR("++ called with no key??");
         } else {
            //can increment uints, int, float, double
            if(this->contains<int>(lastKey)) m_propMap[lastKey] = this->get<int>(lastKey)+1;
            else if(this->contains<uint>(lastKey)) m_propMap[lastKey] = this->get<uint>(lastKey)+1;
            else if(this->contains<float>(lastKey)) m_propMap[lastKey] = this->get<float>(lastKey)+1;
            else if(this->contains<double>(lastKey)) m_propMap[lastKey] = this->get<double>(lastKey)+1;
            else {
               CAMERROR("++ called on unknown property or not of type int/uint/float/double: " << lastKey);
            }
            lastKey="";
         }
      }

      void operator+=(int i) {(*this) += (double)i;}
      void operator+=(double i) {
         if(lastKey=="") {
            CAMERROR("+= called with no key??");
         } else {
            //can increment uints, int, float, double
            if(this->contains<int>(lastKey)) m_propMap[lastKey] = (int)(this->get<int>(lastKey)+i);
            else if(this->contains<uint>(lastKey)) m_propMap[lastKey] = (uint)(this->get<uint>(lastKey)+i);
            else if(this->contains<float>(lastKey)) m_propMap[lastKey] = (float)(this->get<float>(lastKey)+i);
            else if(this->contains<double>(lastKey)) m_propMap[lastKey] = this->get<double>(lastKey)+i;
            else {
               CAMERROR("++ called on unknown property or not of type int/uint/float/double: " << lastKey);
            }
            lastKey="";
         }
      }

      void operator+=(const CamObject& i) {
         if(lastKey=="") {
            TLorentzVector::operator+=(i);
            m_children.push_back(&const_cast<CamObject&>(i));
         } else {
            lastKey="";
         }
      }

      void operator+=(const TLorentzVector& i) {
         if(lastKey=="") {
            TLorentzVector::operator+=(i);
         } else {
            lastKey="";
         }
      }
      

      void operator*=(int i) {(*this) *= (double)i;}
      void operator*=(double i) {
         if(lastKey=="") {
            //apply to the TLorentzVector 
            TLorentzVector::operator*=(i);
         } else {
            //can increment uints, int, float, double
            if(this->contains<int>(lastKey)) m_propMap[lastKey] = (int)(this->get<int>(lastKey)*i);
            else if(this->contains<uint>(lastKey)) m_propMap[lastKey] = (uint)(this->get<uint>(lastKey)*i);
            else if(this->contains<float>(lastKey)) m_propMap[lastKey] = (float)(this->get<float>(lastKey)*i);
            else if(this->contains<double>(lastKey)) m_propMap[lastKey] = this->get<double>(lastKey)*i;
            else {
               CAMERROR("++ called on unknown property or not of type int/uint/float/double: " << lastKey);
            }
            lastKey="";
         }
      }

      //wrap char* in std::string
      void operator=(const char* a) { m_propMap[lastKey]=std::string(a);lastKey=""; }

      ///kept these in for convenience. Deprecated really in f
      virtual void operator=(dbool&& a) {m_propMap[lastKey]=std::function<bool(CamObject&)>(a);lastKey="";}
      virtual void operator=(dint&& a) {m_propMap[lastKey]=std::function<int(CamObject&)>(a);lastKey="";}
      virtual void operator=(dfloat&& a) {m_propMap[lastKey]=std::function<float(CamObject&)>(a);lastKey="";}
      virtual void operator=(ddouble&& a) {m_propMap[lastKey]=std::function<double(CamObject&)>(a);lastKey="";}
      virtual void operator=(duint&& a) {m_propMap[lastKey]=std::function<unsigned int(CamObject&)>(a);lastKey="";}
      virtual void operator=(dvoid&& a) {m_propMap[lastKey]=std::function<void(CamObject&)>(a);lastKey="";}


      //this gets called when setting from a named object 
      template <typename W> void operator=(const W& a) {
         if(lastKey.empty()) std::cout << "ASSIGNMENT CALLED WITH empty lastKey ... this should never happen!!" << std::endl;
         m_propMap[lastKey]=a;
         lastKey="";
      }


      //this gets called when setting from a temporary. Does a move assignment, which is more efficient than a copy
      //NOTE: This is returning a camobject, because I was trying to use it as move assignment operator before explicitly doing this above - maybe put it back to void return type?
      template <typename W> CamObject& operator=(const W&& a) {
         if(lastKey.empty()) std::cout << "MOVE ASSIGNMENT CALLED WITH empty lastKey ... this should never happen!!" << std::endl;
         m_propMap[lastKey]=std::move(a);
         lastKey="";
         return *this;
      }

      template <typename W> void operator=(W (CamObject::*myMethod)()) {
         m_propMap[lastKey]=myMethod;
         lastKey="";
      }
      template <typename W> void operator=(W (CamObject::*myMethod)() const) {
         m_propMap[lastKey]=myMethod;
         lastKey="";
      }
      template <typename W> void operator=(W (TLorentzVector::*myMethod)()) {
         m_propMap[lastKey]=myMethod;
         lastKey="";
      }
      template <typename W> void operator=(W (TLorentzVector::*myMethod)() const) {
         m_propMap[lastKey]=myMethod;
         lastKey="";
      }

      void addChildAlias(unsigned int index, const char* alias) {
         if(index>=m_children.size()) return;
         m_childrenAliases[alias]=m_children[index];
      }

      std::unordered_map<std::string,boost::any>* getPropertyMap() { return &m_propMap; }

      void setPropertyMap(std::unordered_map<std::string,boost::any>&& in) { std::swap(m_propMap,in); }
      void setPropertyMap(const std::unordered_map<std::string,boost::any>& in) { m_propMap=in; }

      std::unordered_map<std::string,std::string>* getDocuMap() { return &m_docuMap; }
      void setDocuMap(std::unordered_map<std::string,std::string>&& in) { std::swap(m_docuMap,in); }
      void setDocuMap(const std::unordered_map<std::string,std::string>& in) { m_docuMap=in; }

      void setAuxElement(const SG::AuxElement* a) { m_xAODParticle=a; m_d3pdIndex= (a) ? a->index() : -1;  }

      const SG::AuxElement* getAuxElement() { return m_xAODParticle; }

      operator const xAOD::IParticle&() const {
         return *ae<xAOD::IParticle>();
      }

      template<typename T> const T* ae() const { //ae = auxelement
         return static_cast<const T*>(m_xAODParticle);  //can't dynamic_cast because SG::AuxElement has no virtual method (needed for runtime cast)
      }

      //alternative method that retrieves something from the storegate and hopes its an AuxElement!
      template<typename T> StatusCode setAuxElement(const std::string& key) {
         const T* val = 0;
         CHECK( evtStore()->retrieve(val,key) );
         setAuxElement(val);
         return StatusCode::SUCCESS;
      }

      CamObject* child(uint i) { if(m_children.size()>i) return m_children[i]; return 0; }
      unsigned int nChildren() const { return m_children.size(); }

      template<typename W> static W& emptyObject() {
         typedef typename std::remove_reference<W>::type WnoRef;
         void* empty = empty_types[std::type_index(typeid(WnoRef))];
         if(!empty) {
            empty = (empty_types[std::type_index(typeid(WnoRef))] = new WnoRef());
         }
         return *(static_cast<WnoRef*>(empty));
      }

      //std::string currentKey() { return m_currentKey; }
   protected:
     


      std::unordered_map<std::string,boost::any> m_propMap;
      std::unordered_map<std::string,std::string> m_docuMap; //filled with documentation if CamObject::Documentation=true

      std::string lastKey;//std::string m_currentKey;
      ServiceHandle<StoreGateSvc> m_evtStore;
      int m_d3pdIndex;
      std::string m_prefix;
      CamObject* m_parent;
      std::vector<CamObject*> m_children; //pointers to children when adding CamObjects
      std::map<std::string, CamObject*> m_childrenAliases; // aliases for children used when retrieving
      //const xAOD::IParticle* m_xAODParticle;
      const SG::AuxElement* m_xAODParticle;
   private:
      //map to 'empty' objects, mapped by type. have to keep these for the get by reference
      static std::unordered_map<std::type_index, void*> empty_types;

      //returns a reference to the empty object type ... assumes W (or it's no_reference version) has a default constructor
      //could specialize, perhaps, for types that don't ..
      


      //this is specialized below to cascade through double->float->int->uint->bool
      template<typename W> W nextExtract(boost::any&,bool& isValid,va_list) {
         isValid=false;
         return emptyObject<W>();
      }
      //this is specialized below to cascade through double->float->int->uint->bool
      template<typename W> W next_retrieve_from_storegate(const std::string&,bool& isValid) {
         isValid=false;
         return emptyObject<W>();
      }

      int wildcmp(const char *wild, const char *string);
   public:
    ClassDef(CamObject,1)

};
CLASS_DEF( CamObject , 171724877 , 1 )
CLASS_DEF( TLorentzVector, 52280883, 1)
CLASS_DEF( std::vector<TLorentzVector>, 40468824, 1)


template<> uint  CamObject::nextExtract<uint>(boost::any&,bool&,va_list);
template<> int  CamObject::nextExtract<int>(boost::any&,bool&,va_list);
template<> float  CamObject::nextExtract<float>(boost::any&,bool&,va_list);
template<> double CamObject::nextExtract<double>(boost::any&,bool&,va_list);

template<> uint CamObject::next_retrieve_from_storegate<uint>(const std::string&,bool&);
template<> int CamObject::next_retrieve_from_storegate<int>(const std::string&,bool&);
template<> float CamObject::next_retrieve_from_storegate<float>(const std::string&,bool&);
template<> double CamObject::next_retrieve_from_storegate<double>(const std::string&,bool&);

template<> int CamObject::contains<void>(const std::string& key, bool checkMap, bool checkStoregate, bool checkAuxElement, bool checkChild, bool checkTLV);

//does an unsafe_cast of the property
template<> void* CamObject::extract<void*>(boost::any& in, bool& isValid, va_list args);
template<> void*& CamObject::extract<void*&>(boost::any& in,bool& isValid,va_list args);

template<typename W> const W ChildGetter<W>::get2(const std::string&, bool& isValid, CamObject*) { isValid=false; return CamObject::emptyObject<W>(); }

template<typename W> W TLVGetter<W>::get2(const std::string&, bool& isValid, CamObject*) { isValid=false; return CamObject::emptyObject<W>(); }
template<> inline double TLVGetter<double>::get2(const std::string& key, bool& isValid, CamObject* obj) { 
   if(key=="tlv_pt") { isValid=true; return obj->Pt(); }
   else if(key=="tlv_eta") { isValid=true; return obj->Eta(); }
   else if(key=="tlv_phi") { isValid=true; return obj->Phi(); }
   else if(key=="tlv_m") { isValid=true; return obj->M(); }
   isValid=false; return CamObject::emptyObject<double>(); 
}


template<typename W> class ChildGetter<std::vector<W, std::allocator<W> > > {
   public:
      static bool contains(const std::string& key, CamObject* c) { 
         //return true if any child contains what we want 
         for(unsigned int i=0;i<c->nChildren();i++) if( c->child(i)->contains<W>(key) ) return true;
         return false;
      }
      static const std::vector<W> get2(const std::string& key, bool& isValid, CamObject* c) { 
         std::vector<W> out;
         bool anyValid(false); //need at least 1 to be valid for return to be 'valid'
         for(unsigned int i=0;i<c->nChildren();i++) {
            out.push_back(c->child(i)->valid_get<W>(key,isValid,true,true));
            if(isValid) anyValid=true;
         }
         isValid=anyValid;
         return out;
      }
};

template<> inline bool ChildGetter<std::vector<double, std::allocator<double> > >::contains(const std::string& key, CamObject* c) { 
   if(c->nChildren()>0 && (key=="child_pt" || key=="child_eta" || key=="child_phi" || key=="child_m") ) return true;
   for(unsigned int i=0;i<c->nChildren();i++) if( c->child(i)->contains<double>(key) ) return true;
   return false;
}

//specialize the ChildGetter for double type, to allow retrieval of TLV properties
template<> inline const std::vector<double> ChildGetter<std::vector<double, std::allocator<double> > >::get2(const std::string& key, bool& isValid, CamObject* c) { 
   std::vector<double> out;
   bool anyValid(false); //need at least 1 to be valid for return to be 'valid'
   for(unsigned int i=0;i<c->nChildren();i++) {
      if(key=="child_pt") {out.push_back(c->child(i)->Pt());isValid=true;}
      else if(key=="child_eta") {out.push_back(c->child(i)->Eta());isValid=true;}
      else if(key=="child_phi") {out.push_back(c->child(i)->Phi());isValid=true;}
      else if(key=="child_m") {out.push_back(c->child(i)->M());isValid=true;}
      else out.push_back(c->child(i)->valid_get<double>(key,isValid,true,true));
      if(isValid) anyValid=true;
   }
   isValid=anyValid;
   return out;
}

#endif
