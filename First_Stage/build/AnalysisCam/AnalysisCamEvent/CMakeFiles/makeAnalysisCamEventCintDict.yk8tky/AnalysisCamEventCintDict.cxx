// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME AnalysisCamEventCintDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "AnalysisCamEvent/CamObject.h"
#include "AnalysisCamEvent/CamVector.h"
#include "AnalysisCamEvent/CamDeque.h"
#include "AnalysisCamEvent/CamEvent.h"
#include "AnalysisCamEvent/CamEvent.h"
#include "AnalysisCamEvent/CamDeque.h"
#include "AnalysisCamEvent/CamVector.h"
#include "AnalysisCamEvent/CamObject.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_CamObject(void *p = 0);
   static void *newArray_CamObject(Long_t size, void *p);
   static void delete_CamObject(void *p);
   static void deleteArray_CamObject(void *p);
   static void destruct_CamObject(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CamObject*)
   {
      ::CamObject *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CamObject >(0);
      static ::ROOT::TGenericClassInfo 
         instance("CamObject", ::CamObject::Class_Version(), "AnalysisCamEvent/CamObject.h", 82,
                  typeid(::CamObject), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CamObject::Dictionary, isa_proxy, 4,
                  sizeof(::CamObject) );
      instance.SetNew(&new_CamObject);
      instance.SetNewArray(&newArray_CamObject);
      instance.SetDelete(&delete_CamObject);
      instance.SetDeleteArray(&deleteArray_CamObject);
      instance.SetDestructor(&destruct_CamObject);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CamObject*)
   {
      return GenerateInitInstanceLocal((::CamObject*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CamObject*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CamDeque(void *p = 0);
   static void *newArray_CamDeque(Long_t size, void *p);
   static void delete_CamDeque(void *p);
   static void deleteArray_CamDeque(void *p);
   static void destruct_CamDeque(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CamDeque*)
   {
      ::CamDeque *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CamDeque >(0);
      static ::ROOT::TGenericClassInfo 
         instance("CamDeque", ::CamDeque::Class_Version(), "AnalysisCamEvent/CamDeque.h", 11,
                  typeid(::CamDeque), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CamDeque::Dictionary, isa_proxy, 4,
                  sizeof(::CamDeque) );
      instance.SetNew(&new_CamDeque);
      instance.SetNewArray(&newArray_CamDeque);
      instance.SetDelete(&delete_CamDeque);
      instance.SetDeleteArray(&deleteArray_CamDeque);
      instance.SetDestructor(&destruct_CamDeque);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CamDeque*)
   {
      return GenerateInitInstanceLocal((::CamDeque*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CamDeque*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CamVector(void *p = 0);
   static void *newArray_CamVector(Long_t size, void *p);
   static void delete_CamVector(void *p);
   static void deleteArray_CamVector(void *p);
   static void destruct_CamVector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CamVector*)
   {
      ::CamVector *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CamVector >(0);
      static ::ROOT::TGenericClassInfo 
         instance("CamVector", ::CamVector::Class_Version(), "AnalysisCamEvent/CamVector.h", 11,
                  typeid(::CamVector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CamVector::Dictionary, isa_proxy, 4,
                  sizeof(::CamVector) );
      instance.SetNew(&new_CamVector);
      instance.SetNewArray(&newArray_CamVector);
      instance.SetDelete(&delete_CamVector);
      instance.SetDeleteArray(&deleteArray_CamVector);
      instance.SetDestructor(&destruct_CamVector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CamVector*)
   {
      return GenerateInitInstanceLocal((::CamVector*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CamVector*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CamEvent(void *p = 0);
   static void *newArray_CamEvent(Long_t size, void *p);
   static void delete_CamEvent(void *p);
   static void deleteArray_CamEvent(void *p);
   static void destruct_CamEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CamEvent*)
   {
      ::CamEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CamEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("CamEvent", ::CamEvent::Class_Version(), "AnalysisCamEvent/CamEvent.h", 19,
                  typeid(::CamEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CamEvent::Dictionary, isa_proxy, 4,
                  sizeof(::CamEvent) );
      instance.SetNew(&new_CamEvent);
      instance.SetNewArray(&newArray_CamEvent);
      instance.SetDelete(&delete_CamEvent);
      instance.SetDeleteArray(&deleteArray_CamEvent);
      instance.SetDestructor(&destruct_CamEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CamEvent*)
   {
      return GenerateInitInstanceLocal((::CamEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CamEvent*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CamObject::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *CamObject::Class_Name()
{
   return "CamObject";
}

//______________________________________________________________________________
const char *CamObject::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CamObject*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int CamObject::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CamObject*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CamObject::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CamObject*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CamObject::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CamObject*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CamDeque::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *CamDeque::Class_Name()
{
   return "CamDeque";
}

//______________________________________________________________________________
const char *CamDeque::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CamDeque*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int CamDeque::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CamDeque*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CamDeque::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CamDeque*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CamDeque::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CamDeque*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CamVector::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *CamVector::Class_Name()
{
   return "CamVector";
}

//______________________________________________________________________________
const char *CamVector::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CamVector*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int CamVector::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CamVector*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CamVector::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CamVector*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CamVector::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CamVector*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CamEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *CamEvent::Class_Name()
{
   return "CamEvent";
}

//______________________________________________________________________________
const char *CamEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CamEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int CamEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CamEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CamEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CamEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CamEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CamEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CamObject::Streamer(TBuffer &R__b)
{
   // Stream an object of class CamObject.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CamObject::Class(),this);
   } else {
      R__b.WriteClassBuffer(CamObject::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CamObject(void *p) {
      return  p ? new(p) ::CamObject : new ::CamObject;
   }
   static void *newArray_CamObject(Long_t nElements, void *p) {
      return p ? new(p) ::CamObject[nElements] : new ::CamObject[nElements];
   }
   // Wrapper around operator delete
   static void delete_CamObject(void *p) {
      delete ((::CamObject*)p);
   }
   static void deleteArray_CamObject(void *p) {
      delete [] ((::CamObject*)p);
   }
   static void destruct_CamObject(void *p) {
      typedef ::CamObject current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CamObject

//______________________________________________________________________________
void CamDeque::Streamer(TBuffer &R__b)
{
   // Stream an object of class CamDeque.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CamDeque::Class(),this);
   } else {
      R__b.WriteClassBuffer(CamDeque::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CamDeque(void *p) {
      return  p ? new(p) ::CamDeque : new ::CamDeque;
   }
   static void *newArray_CamDeque(Long_t nElements, void *p) {
      return p ? new(p) ::CamDeque[nElements] : new ::CamDeque[nElements];
   }
   // Wrapper around operator delete
   static void delete_CamDeque(void *p) {
      delete ((::CamDeque*)p);
   }
   static void deleteArray_CamDeque(void *p) {
      delete [] ((::CamDeque*)p);
   }
   static void destruct_CamDeque(void *p) {
      typedef ::CamDeque current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CamDeque

//______________________________________________________________________________
void CamVector::Streamer(TBuffer &R__b)
{
   // Stream an object of class CamVector.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CamVector::Class(),this);
   } else {
      R__b.WriteClassBuffer(CamVector::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CamVector(void *p) {
      return  p ? new(p) ::CamVector : new ::CamVector;
   }
   static void *newArray_CamVector(Long_t nElements, void *p) {
      return p ? new(p) ::CamVector[nElements] : new ::CamVector[nElements];
   }
   // Wrapper around operator delete
   static void delete_CamVector(void *p) {
      delete ((::CamVector*)p);
   }
   static void deleteArray_CamVector(void *p) {
      delete [] ((::CamVector*)p);
   }
   static void destruct_CamVector(void *p) {
      typedef ::CamVector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CamVector

//______________________________________________________________________________
void CamEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class CamEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CamEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(CamEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CamEvent(void *p) {
      return  p ? new(p) ::CamEvent : new ::CamEvent;
   }
   static void *newArray_CamEvent(Long_t nElements, void *p) {
      return p ? new(p) ::CamEvent[nElements] : new ::CamEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_CamEvent(void *p) {
      delete ((::CamEvent*)p);
   }
   static void deleteArray_CamEvent(void *p) {
      delete [] ((::CamEvent*)p);
   }
   static void destruct_CamEvent(void *p) {
      typedef ::CamEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CamEvent

namespace ROOT {
   static TClass *vectorlECamObjectgR_Dictionary();
   static void vectorlECamObjectgR_TClassManip(TClass*);
   static void *new_vectorlECamObjectgR(void *p = 0);
   static void *newArray_vectorlECamObjectgR(Long_t size, void *p);
   static void delete_vectorlECamObjectgR(void *p);
   static void deleteArray_vectorlECamObjectgR(void *p);
   static void destruct_vectorlECamObjectgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CamObject>*)
   {
      vector<CamObject> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CamObject>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CamObject>", -2, "vector", 214,
                  typeid(vector<CamObject>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECamObjectgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CamObject>) );
      instance.SetNew(&new_vectorlECamObjectgR);
      instance.SetNewArray(&newArray_vectorlECamObjectgR);
      instance.SetDelete(&delete_vectorlECamObjectgR);
      instance.SetDeleteArray(&deleteArray_vectorlECamObjectgR);
      instance.SetDestructor(&destruct_vectorlECamObjectgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CamObject> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CamObject>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECamObjectgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CamObject>*)0x0)->GetClass();
      vectorlECamObjectgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECamObjectgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECamObjectgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CamObject> : new vector<CamObject>;
   }
   static void *newArray_vectorlECamObjectgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CamObject>[nElements] : new vector<CamObject>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECamObjectgR(void *p) {
      delete ((vector<CamObject>*)p);
   }
   static void deleteArray_vectorlECamObjectgR(void *p) {
      delete [] ((vector<CamObject>*)p);
   }
   static void destruct_vectorlECamObjectgR(void *p) {
      typedef vector<CamObject> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CamObject>

namespace ROOT {
   static TClass *vectorlECamObjectmUgR_Dictionary();
   static void vectorlECamObjectmUgR_TClassManip(TClass*);
   static void *new_vectorlECamObjectmUgR(void *p = 0);
   static void *newArray_vectorlECamObjectmUgR(Long_t size, void *p);
   static void delete_vectorlECamObjectmUgR(void *p);
   static void deleteArray_vectorlECamObjectmUgR(void *p);
   static void destruct_vectorlECamObjectmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CamObject*>*)
   {
      vector<CamObject*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CamObject*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CamObject*>", -2, "vector", 214,
                  typeid(vector<CamObject*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECamObjectmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CamObject*>) );
      instance.SetNew(&new_vectorlECamObjectmUgR);
      instance.SetNewArray(&newArray_vectorlECamObjectmUgR);
      instance.SetDelete(&delete_vectorlECamObjectmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECamObjectmUgR);
      instance.SetDestructor(&destruct_vectorlECamObjectmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CamObject*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CamObject*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECamObjectmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CamObject*>*)0x0)->GetClass();
      vectorlECamObjectmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECamObjectmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECamObjectmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CamObject*> : new vector<CamObject*>;
   }
   static void *newArray_vectorlECamObjectmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CamObject*>[nElements] : new vector<CamObject*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECamObjectmUgR(void *p) {
      delete ((vector<CamObject*>*)p);
   }
   static void deleteArray_vectorlECamObjectmUgR(void *p) {
      delete [] ((vector<CamObject*>*)p);
   }
   static void destruct_vectorlECamObjectmUgR(void *p) {
      typedef vector<CamObject*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CamObject*>

namespace ROOT {
   static TClass *unordered_maplEstringcOstringgR_Dictionary();
   static void unordered_maplEstringcOstringgR_TClassManip(TClass*);
   static void *new_unordered_maplEstringcOstringgR(void *p = 0);
   static void *newArray_unordered_maplEstringcOstringgR(Long_t size, void *p);
   static void delete_unordered_maplEstringcOstringgR(void *p);
   static void deleteArray_unordered_maplEstringcOstringgR(void *p);
   static void destruct_unordered_maplEstringcOstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const unordered_map<string,string>*)
   {
      unordered_map<string,string> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(unordered_map<string,string>));
      static ::ROOT::TGenericClassInfo 
         instance("unordered_map<string,string>", -2, "unordered_map", 98,
                  typeid(unordered_map<string,string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &unordered_maplEstringcOstringgR_Dictionary, isa_proxy, 0,
                  sizeof(unordered_map<string,string>) );
      instance.SetNew(&new_unordered_maplEstringcOstringgR);
      instance.SetNewArray(&newArray_unordered_maplEstringcOstringgR);
      instance.SetDelete(&delete_unordered_maplEstringcOstringgR);
      instance.SetDeleteArray(&deleteArray_unordered_maplEstringcOstringgR);
      instance.SetDestructor(&destruct_unordered_maplEstringcOstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< unordered_map<string,string> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const unordered_map<string,string>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *unordered_maplEstringcOstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const unordered_map<string,string>*)0x0)->GetClass();
      unordered_maplEstringcOstringgR_TClassManip(theClass);
   return theClass;
   }

   static void unordered_maplEstringcOstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_unordered_maplEstringcOstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) unordered_map<string,string> : new unordered_map<string,string>;
   }
   static void *newArray_unordered_maplEstringcOstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) unordered_map<string,string>[nElements] : new unordered_map<string,string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_unordered_maplEstringcOstringgR(void *p) {
      delete ((unordered_map<string,string>*)p);
   }
   static void deleteArray_unordered_maplEstringcOstringgR(void *p) {
      delete [] ((unordered_map<string,string>*)p);
   }
   static void destruct_unordered_maplEstringcOstringgR(void *p) {
      typedef unordered_map<string,string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class unordered_map<string,string>

namespace ROOT {
   static TClass *unordered_maplEstringcOboostcLcLanygR_Dictionary();
   static void unordered_maplEstringcOboostcLcLanygR_TClassManip(TClass*);
   static void *new_unordered_maplEstringcOboostcLcLanygR(void *p = 0);
   static void *newArray_unordered_maplEstringcOboostcLcLanygR(Long_t size, void *p);
   static void delete_unordered_maplEstringcOboostcLcLanygR(void *p);
   static void deleteArray_unordered_maplEstringcOboostcLcLanygR(void *p);
   static void destruct_unordered_maplEstringcOboostcLcLanygR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const unordered_map<string,boost::any>*)
   {
      unordered_map<string,boost::any> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(unordered_map<string,boost::any>));
      static ::ROOT::TGenericClassInfo 
         instance("unordered_map<string,boost::any>", -2, "unordered_map", 98,
                  typeid(unordered_map<string,boost::any>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &unordered_maplEstringcOboostcLcLanygR_Dictionary, isa_proxy, 0,
                  sizeof(unordered_map<string,boost::any>) );
      instance.SetNew(&new_unordered_maplEstringcOboostcLcLanygR);
      instance.SetNewArray(&newArray_unordered_maplEstringcOboostcLcLanygR);
      instance.SetDelete(&delete_unordered_maplEstringcOboostcLcLanygR);
      instance.SetDeleteArray(&deleteArray_unordered_maplEstringcOboostcLcLanygR);
      instance.SetDestructor(&destruct_unordered_maplEstringcOboostcLcLanygR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< unordered_map<string,boost::any> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const unordered_map<string,boost::any>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *unordered_maplEstringcOboostcLcLanygR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const unordered_map<string,boost::any>*)0x0)->GetClass();
      unordered_maplEstringcOboostcLcLanygR_TClassManip(theClass);
   return theClass;
   }

   static void unordered_maplEstringcOboostcLcLanygR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_unordered_maplEstringcOboostcLcLanygR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) unordered_map<string,boost::any> : new unordered_map<string,boost::any>;
   }
   static void *newArray_unordered_maplEstringcOboostcLcLanygR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) unordered_map<string,boost::any>[nElements] : new unordered_map<string,boost::any>[nElements];
   }
   // Wrapper around operator delete
   static void delete_unordered_maplEstringcOboostcLcLanygR(void *p) {
      delete ((unordered_map<string,boost::any>*)p);
   }
   static void deleteArray_unordered_maplEstringcOboostcLcLanygR(void *p) {
      delete [] ((unordered_map<string,boost::any>*)p);
   }
   static void destruct_unordered_maplEstringcOboostcLcLanygR(void *p) {
      typedef unordered_map<string,boost::any> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class unordered_map<string,boost::any>

namespace ROOT {
   static TClass *maplEstringcOCamObjectmUgR_Dictionary();
   static void maplEstringcOCamObjectmUgR_TClassManip(TClass*);
   static void *new_maplEstringcOCamObjectmUgR(void *p = 0);
   static void *newArray_maplEstringcOCamObjectmUgR(Long_t size, void *p);
   static void delete_maplEstringcOCamObjectmUgR(void *p);
   static void deleteArray_maplEstringcOCamObjectmUgR(void *p);
   static void destruct_maplEstringcOCamObjectmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,CamObject*>*)
   {
      map<string,CamObject*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,CamObject*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,CamObject*>", -2, "map", 96,
                  typeid(map<string,CamObject*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOCamObjectmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,CamObject*>) );
      instance.SetNew(&new_maplEstringcOCamObjectmUgR);
      instance.SetNewArray(&newArray_maplEstringcOCamObjectmUgR);
      instance.SetDelete(&delete_maplEstringcOCamObjectmUgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOCamObjectmUgR);
      instance.SetDestructor(&destruct_maplEstringcOCamObjectmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,CamObject*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,CamObject*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOCamObjectmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,CamObject*>*)0x0)->GetClass();
      maplEstringcOCamObjectmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOCamObjectmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOCamObjectmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,CamObject*> : new map<string,CamObject*>;
   }
   static void *newArray_maplEstringcOCamObjectmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,CamObject*>[nElements] : new map<string,CamObject*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOCamObjectmUgR(void *p) {
      delete ((map<string,CamObject*>*)p);
   }
   static void deleteArray_maplEstringcOCamObjectmUgR(void *p) {
      delete [] ((map<string,CamObject*>*)p);
   }
   static void destruct_maplEstringcOCamObjectmUgR(void *p) {
      typedef map<string,CamObject*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,CamObject*>

namespace ROOT {
   static TClass *dequelECamObjectmUgR_Dictionary();
   static void dequelECamObjectmUgR_TClassManip(TClass*);
   static void *new_dequelECamObjectmUgR(void *p = 0);
   static void *newArray_dequelECamObjectmUgR(Long_t size, void *p);
   static void delete_dequelECamObjectmUgR(void *p);
   static void deleteArray_dequelECamObjectmUgR(void *p);
   static void destruct_dequelECamObjectmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const deque<CamObject*>*)
   {
      deque<CamObject*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(deque<CamObject*>));
      static ::ROOT::TGenericClassInfo 
         instance("deque<CamObject*>", -2, "deque", 829,
                  typeid(deque<CamObject*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &dequelECamObjectmUgR_Dictionary, isa_proxy, 0,
                  sizeof(deque<CamObject*>) );
      instance.SetNew(&new_dequelECamObjectmUgR);
      instance.SetNewArray(&newArray_dequelECamObjectmUgR);
      instance.SetDelete(&delete_dequelECamObjectmUgR);
      instance.SetDeleteArray(&deleteArray_dequelECamObjectmUgR);
      instance.SetDestructor(&destruct_dequelECamObjectmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< deque<CamObject*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const deque<CamObject*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *dequelECamObjectmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const deque<CamObject*>*)0x0)->GetClass();
      dequelECamObjectmUgR_TClassManip(theClass);
   return theClass;
   }

   static void dequelECamObjectmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_dequelECamObjectmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) deque<CamObject*> : new deque<CamObject*>;
   }
   static void *newArray_dequelECamObjectmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) deque<CamObject*>[nElements] : new deque<CamObject*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_dequelECamObjectmUgR(void *p) {
      delete ((deque<CamObject*>*)p);
   }
   static void deleteArray_dequelECamObjectmUgR(void *p) {
      delete [] ((deque<CamObject*>*)p);
   }
   static void destruct_dequelECamObjectmUgR(void *p) {
      typedef deque<CamObject*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class deque<CamObject*>

namespace {
  void TriggerDictionaryInitialization_libAnalysisCamEvent_Impl() {
    static const char* headers[] = {
"AnalysisCamEvent/CamObject.h",
"AnalysisCamEvent/CamVector.h",
"AnalysisCamEvent/CamDeque.h",
"AnalysisCamEvent/CamEvent.h",
0
    };
    static const char* includePaths[] = {
"/home/mleigh/workdir/First_Stage/source/AnalysisCam/AnalysisCamEvent",
"/home/mleigh/workdir/First_Stage/source/AnalysisCam/AnalysisCamEvent",
"/home/mleigh/workdir/First_Stage/source/AnalysisCam/AnalysisCamEvent",
"/home/mleigh/workdir/First_Stage/source/AnalysisCam/AnalysisCamEvent",
"/home/mleigh/workdir/First_Stage/source/AnalysisCam/AnalysisCamEvent",
"/home/mleigh/workdir/First_Stage/source/athena/Control/AthAnalysisBaseComps",
"/home/mleigh/workdir/First_Stage/source/athena/Control/AthAnalysisBaseComps",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Control/AthenaBaseComps",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Control/AthenaKernel",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/GAUDI/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/usr/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Control/CxxUtils",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Control/DataModelRoot",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/GAUDI/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/tbb/2018_U1/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/GAUDI/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Control/SGTools",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Control/StoreGate",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Control/AthContainersInterfaces",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/tbb/2018_U1/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Control/AthAllocators",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Database/IOVDbDataModel",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Database/AthenaPOOL/AthenaPoolUtilities",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/CORAL/3_2_0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Control/DataModel",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Control/AthContainers",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Control/AthLinks",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Database/AthenaPOOL/DBDataModel",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Control/CLIDSvc",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Database/PersistentDataModel",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/CORAL/3_2_0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Python/2.7.13/x86_64-slc6-gcc62-opt/include/python2.7",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Control/RootUtils",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Python/2.7.13/x86_64-slc6-gcc62-opt/include/python2.7",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/AtlasTest/TestTools",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Control/AthenaCommon",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Event/xAOD/xAODMissingET",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Event/xAOD/xAODBase",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Event/xAOD/xAODCore",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Event/xAOD/xAODJet",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Event/xAOD/xAODBTagging",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Event/xAOD/xAODTracking",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/eigen/3.2.9/x86_64-slc6-gcc62-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/DetectorDescription/GeoPrimitives",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/../../../../AthAnalysisExternals/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/eigen/3.2.9/x86_64-slc6-gcc62-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Event/EventPrimitives",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/../../../../AthAnalysisExternals/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/eigen/3.2.9/x86_64-slc6-gcc62-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Event/xAOD/xAODPFlow",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Event/xAOD/xAODCaloEvent",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/eigen/3.2.9/x86_64-slc6-gcc62-opt/include/eigen3",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Calorimeter/CaloGeoHelpers",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Event/xAOD/xAODTrigger",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Event/xAOD/xAODEventInfo",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/PhysicsAnalysis/AnalysisCommon/PATInterfaces",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/LCG_93/Boost/1.66.0/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Control/AthToolSupport/AsgTools",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Control/xAODRootAccess",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Event/xAOD/xAODEventFormat",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/src/Control/xAODRootAccessInterfaces",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AthAnalysis/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/../../../../AthAnalysisExternals/21.2.31/InstallArea/x86_64-slc6-gcc62-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/sw/lcg/releases/ROOT/6.12.06-0f687/x86_64-slc6-gcc62-opt/include",
"/atlas/mleigh/First_Stage/build/AnalysisCam/AnalysisCamEvent/CMakeFiles/makeAnalysisCamEventCintDict.yk8tky/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libAnalysisCamEvent dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$AnalysisCamEvent/CamEvent.h")))  CamObject;
class __attribute__((annotate("$clingAutoload$AnalysisCamEvent/CamEvent.h")))  CamDeque;
class __attribute__((annotate("$clingAutoload$AnalysisCamEvent/CamEvent.h")))  CamVector;
class __attribute__((annotate("$clingAutoload$AnalysisCamEvent/CamEvent.h")))  CamEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libAnalysisCamEvent dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef HAVE_PRETTY_FUNCTION
  #define HAVE_PRETTY_FUNCTION 1
#endif
#ifndef HAVE_64_BITS
  #define HAVE_64_BITS 1
#endif
#ifndef __IDENTIFIER_64BIT__
  #define __IDENTIFIER_64BIT__ 1
#endif
#ifndef ATLAS
  #define ATLAS 1
#endif
#ifndef GAUDI_V20_COMPAT
  #define GAUDI_V20_COMPAT 1
#endif
#ifndef ATLAS_GAUDI_V21
  #define ATLAS_GAUDI_V21 1
#endif
#ifndef HAVE_GAUDI_PLUGINSVC
  #define HAVE_GAUDI_PLUGINSVC 1
#endif
#ifndef XAOD_ANALYSIS
  #define XAOD_ANALYSIS 1
#endif
#ifndef ROOTCORE_RELEASE_SERIES
  #define ROOTCORE_RELEASE_SERIES 25
#endif
#ifndef PACKAGE_VERSION
  #define PACKAGE_VERSION "AnalysisCamEvent-02-00-00-branch"
#endif
#ifndef PACKAGE_VERSION_UQ
  #define PACKAGE_VERSION_UQ AnalysisCamEvent-02-00-00-branch
#endif
#ifndef CLHEP_MAX_MIN_DEFINED
  #define CLHEP_MAX_MIN_DEFINED 1
#endif
#ifndef CLHEP_ABS_DEFINED
  #define CLHEP_ABS_DEFINED 1
#endif
#ifndef CLHEP_SQR_DEFINED
  #define CLHEP_SQR_DEFINED 1
#endif
#ifndef CLHEP_MAX_MIN_DEFINED
  #define CLHEP_MAX_MIN_DEFINED 1
#endif
#ifndef CLHEP_ABS_DEFINED
  #define CLHEP_ABS_DEFINED 1
#endif
#ifndef CLHEP_SQR_DEFINED
  #define CLHEP_SQR_DEFINED 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "AnalysisCamEvent/CamObject.h"
#include "AnalysisCamEvent/CamVector.h"
#include "AnalysisCamEvent/CamDeque.h"
#include "AnalysisCamEvent/CamEvent.h"
#ifndef ANALYSISCAMEVENT_ANALYSISCAMEVENTDICT_H
#define ANALYSISCAMEVENT_ANALYSISCAMEVENTDICT_H

//this define is needed in 2.4 series, for dictionary compilation 
#ifndef GAUDI_V20_COMPAT
#define GAUDI_V20_COMPAT
#endif

#include "AnalysisCamEvent/CamEvent.h"
#include "AnalysisCamEvent/CamDeque.h"
#include "AnalysisCamEvent/CamVector.h"
#include "AnalysisCamEvent/CamObject.h"

#pragma link C++ class CamObject+;
#pragma link C++ class CamVector+;
#pragma link C++ class CamDeque+;
#pragma link C++ class CamEvent+;

#endif
#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"CamDeque", payloadCode, "@",
"CamEvent", payloadCode, "@",
"CamObject", payloadCode, "@",
"CamVector", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libAnalysisCamEvent",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libAnalysisCamEvent_Impl, {{"namespace DataVector_detail { template <typename B> class DVLEltBase_init; }", 1},{"namespace DataVector_detail { template <typename B1, typename B2, typename B3> class VirtBases; }", 1},{"template <typename T> class DataVectorBase;", 1},{"template <typename T, typename BASE> class DataVector;", 1}}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libAnalysisCamEvent_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libAnalysisCamEvent() {
  TriggerDictionaryInitialization_libAnalysisCamEvent_Impl();
}
