// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME AthAnalysisBaseCompsDictReflexDict

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
#include "/home/mleigh/workdir/First_Stage/source/athena/Control/AthAnalysisBaseComps/AthAnalysisBaseComps/AthAnalysisBaseCompsDict.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *AthAnalysisHelper_Dictionary();
   static void AthAnalysisHelper_TClassManip(TClass*);
   static void *new_AthAnalysisHelper(void *p = 0);
   static void *newArray_AthAnalysisHelper(Long_t size, void *p);
   static void delete_AthAnalysisHelper(void *p);
   static void deleteArray_AthAnalysisHelper(void *p);
   static void destruct_AthAnalysisHelper(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AthAnalysisHelper*)
   {
      ::AthAnalysisHelper *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::AthAnalysisHelper));
      static ::ROOT::TGenericClassInfo 
         instance("AthAnalysisHelper", "AthAnalysisHelper.h", 34,
                  typeid(::AthAnalysisHelper), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AthAnalysisHelper_Dictionary, isa_proxy, 4,
                  sizeof(::AthAnalysisHelper) );
      instance.SetNew(&new_AthAnalysisHelper);
      instance.SetNewArray(&newArray_AthAnalysisHelper);
      instance.SetDelete(&delete_AthAnalysisHelper);
      instance.SetDeleteArray(&deleteArray_AthAnalysisHelper);
      instance.SetDestructor(&destruct_AthAnalysisHelper);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AthAnalysisHelper*)
   {
      return GenerateInitInstanceLocal((::AthAnalysisHelper*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AthAnalysisHelper*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AthAnalysisHelper_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AthAnalysisHelper*)0x0)->GetClass();
      AthAnalysisHelper_TClassManip(theClass);
   return theClass;
   }

   static void AthAnalysisHelper_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *AAH_Dictionary();
   static void AAH_TClassManip(TClass*);
   static void *new_AAH(void *p = 0);
   static void *newArray_AAH(Long_t size, void *p);
   static void delete_AAH(void *p);
   static void deleteArray_AAH(void *p);
   static void destruct_AAH(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AAH*)
   {
      ::AAH *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::AAH));
      static ::ROOT::TGenericClassInfo 
         instance("AAH", "AthAnalysisHelper.h", 370,
                  typeid(::AAH), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &AAH_Dictionary, isa_proxy, 4,
                  sizeof(::AAH) );
      instance.SetNew(&new_AAH);
      instance.SetNewArray(&newArray_AAH);
      instance.SetDelete(&delete_AAH);
      instance.SetDeleteArray(&deleteArray_AAH);
      instance.SetDestructor(&destruct_AAH);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AAH*)
   {
      return GenerateInitInstanceLocal((::AAH*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AAH*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AAH_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AAH*)0x0)->GetClass();
      AAH_TClassManip(theClass);
   return theClass;
   }

   static void AAH_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_AthAnalysisHelper(void *p) {
      return  p ? new(p) ::AthAnalysisHelper : new ::AthAnalysisHelper;
   }
   static void *newArray_AthAnalysisHelper(Long_t nElements, void *p) {
      return p ? new(p) ::AthAnalysisHelper[nElements] : new ::AthAnalysisHelper[nElements];
   }
   // Wrapper around operator delete
   static void delete_AthAnalysisHelper(void *p) {
      delete ((::AthAnalysisHelper*)p);
   }
   static void deleteArray_AthAnalysisHelper(void *p) {
      delete [] ((::AthAnalysisHelper*)p);
   }
   static void destruct_AthAnalysisHelper(void *p) {
      typedef ::AthAnalysisHelper current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AthAnalysisHelper

namespace ROOT {
   // Wrappers around operator new
   static void *new_AAH(void *p) {
      return  p ? new(p) ::AAH : new ::AAH;
   }
   static void *newArray_AAH(Long_t nElements, void *p) {
      return p ? new(p) ::AAH[nElements] : new ::AAH[nElements];
   }
   // Wrapper around operator delete
   static void delete_AAH(void *p) {
      delete ((::AAH*)p);
   }
   static void deleteArray_AAH(void *p) {
      delete [] ((::AAH*)p);
   }
   static void destruct_AAH(void *p) {
      typedef ::AAH current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AAH

namespace {
  void TriggerDictionaryInitialization_libAthAnalysisBaseCompsDict_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libAthAnalysisBaseCompsDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$AthAnalysisBaseComps/AthAnalysisHelper.h")))  AthAnalysisHelper;
class __attribute__((annotate("$clingAutoload$AthAnalysisBaseComps/AthAnalysisHelper.h")))  AAH;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libAthAnalysisBaseCompsDict dictionary payload"

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
  #define PACKAGE_VERSION "AthAnalysisBaseComps-00-00-00"
#endif
#ifndef PACKAGE_VERSION_UQ
  #define PACKAGE_VERSION_UQ AthAnalysisBaseComps-00-00-00
#endif
#ifndef EIGEN_DONT_VECTORIZE
  #define EIGEN_DONT_VECTORIZE 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef ATHANALYSISBASECOMPSDICT_H
#define ATHANALYSISBASECOMPSDICT_H 

#include "AthAnalysisBaseComps/AthAnalysisHelper.h"


#endif 
#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"AAH", payloadCode, "@",
"AthAnalysisHelper", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libAthAnalysisBaseCompsDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libAthAnalysisBaseCompsDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libAthAnalysisBaseCompsDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libAthAnalysisBaseCompsDict() {
  TriggerDictionaryInitialization_libAthAnalysisBaseCompsDict_Impl();
}
