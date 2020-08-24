// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME SUSYToolsDictReflexDict

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
#include "/home/mleigh/workdir/First_Stage/source/athena/PhysicsAnalysis/SUSYPhys/SUSYTools/SUSYTools/SUSYToolsDict.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *STcLcLISUSYObjDef_xAODTool_Dictionary();
   static void STcLcLISUSYObjDef_xAODTool_TClassManip(TClass*);
   static void delete_STcLcLISUSYObjDef_xAODTool(void *p);
   static void deleteArray_STcLcLISUSYObjDef_xAODTool(void *p);
   static void destruct_STcLcLISUSYObjDef_xAODTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ST::ISUSYObjDef_xAODTool*)
   {
      ::ST::ISUSYObjDef_xAODTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ST::ISUSYObjDef_xAODTool));
      static ::ROOT::TGenericClassInfo 
         instance("ST::ISUSYObjDef_xAODTool", "ISUSYObjDef_xAODTool.h", 195,
                  typeid(::ST::ISUSYObjDef_xAODTool), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &STcLcLISUSYObjDef_xAODTool_Dictionary, isa_proxy, 4,
                  sizeof(::ST::ISUSYObjDef_xAODTool) );
      instance.SetDelete(&delete_STcLcLISUSYObjDef_xAODTool);
      instance.SetDeleteArray(&deleteArray_STcLcLISUSYObjDef_xAODTool);
      instance.SetDestructor(&destruct_STcLcLISUSYObjDef_xAODTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ST::ISUSYObjDef_xAODTool*)
   {
      return GenerateInitInstanceLocal((::ST::ISUSYObjDef_xAODTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ST::ISUSYObjDef_xAODTool*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *STcLcLISUSYObjDef_xAODTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ST::ISUSYObjDef_xAODTool*)0x0)->GetClass();
      STcLcLISUSYObjDef_xAODTool_TClassManip(theClass);
   return theClass;
   }

   static void STcLcLISUSYObjDef_xAODTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *STcLcLSUSYObjDef_xAOD_Dictionary();
   static void STcLcLSUSYObjDef_xAOD_TClassManip(TClass*);
   static void delete_STcLcLSUSYObjDef_xAOD(void *p);
   static void deleteArray_STcLcLSUSYObjDef_xAOD(void *p);
   static void destruct_STcLcLSUSYObjDef_xAOD(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ST::SUSYObjDef_xAOD*)
   {
      ::ST::SUSYObjDef_xAOD *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ST::SUSYObjDef_xAOD));
      static ::ROOT::TGenericClassInfo 
         instance("ST::SUSYObjDef_xAOD", "SUSYObjDef_xAOD.h", 117,
                  typeid(::ST::SUSYObjDef_xAOD), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &STcLcLSUSYObjDef_xAOD_Dictionary, isa_proxy, 4,
                  sizeof(::ST::SUSYObjDef_xAOD) );
      instance.SetDelete(&delete_STcLcLSUSYObjDef_xAOD);
      instance.SetDeleteArray(&deleteArray_STcLcLSUSYObjDef_xAOD);
      instance.SetDestructor(&destruct_STcLcLSUSYObjDef_xAOD);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ST::SUSYObjDef_xAOD*)
   {
      return GenerateInitInstanceLocal((::ST::SUSYObjDef_xAOD*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ST::SUSYObjDef_xAOD*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *STcLcLSUSYObjDef_xAOD_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ST::SUSYObjDef_xAOD*)0x0)->GetClass();
      STcLcLSUSYObjDef_xAOD_TClassManip(theClass);
   return theClass;
   }

   static void STcLcLSUSYObjDef_xAOD_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *SUSYcLcLCrossSectionDB_Dictionary();
   static void SUSYcLcLCrossSectionDB_TClassManip(TClass*);
   static void *new_SUSYcLcLCrossSectionDB(void *p = 0);
   static void *newArray_SUSYcLcLCrossSectionDB(Long_t size, void *p);
   static void delete_SUSYcLcLCrossSectionDB(void *p);
   static void deleteArray_SUSYcLcLCrossSectionDB(void *p);
   static void destruct_SUSYcLcLCrossSectionDB(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SUSY::CrossSectionDB*)
   {
      ::SUSY::CrossSectionDB *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::SUSY::CrossSectionDB));
      static ::ROOT::TGenericClassInfo 
         instance("SUSY::CrossSectionDB", "SUSYCrossSection.h", 27,
                  typeid(::SUSY::CrossSectionDB), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &SUSYcLcLCrossSectionDB_Dictionary, isa_proxy, 4,
                  sizeof(::SUSY::CrossSectionDB) );
      instance.SetNew(&new_SUSYcLcLCrossSectionDB);
      instance.SetNewArray(&newArray_SUSYcLcLCrossSectionDB);
      instance.SetDelete(&delete_SUSYcLcLCrossSectionDB);
      instance.SetDeleteArray(&deleteArray_SUSYcLcLCrossSectionDB);
      instance.SetDestructor(&destruct_SUSYcLcLCrossSectionDB);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SUSY::CrossSectionDB*)
   {
      return GenerateInitInstanceLocal((::SUSY::CrossSectionDB*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::SUSY::CrossSectionDB*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *SUSYcLcLCrossSectionDB_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::SUSY::CrossSectionDB*)0x0)->GetClass();
      SUSYcLcLCrossSectionDB_TClassManip(theClass);
   return theClass;
   }

   static void SUSYcLcLCrossSectionDB_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrapper around operator delete
   static void delete_STcLcLISUSYObjDef_xAODTool(void *p) {
      delete ((::ST::ISUSYObjDef_xAODTool*)p);
   }
   static void deleteArray_STcLcLISUSYObjDef_xAODTool(void *p) {
      delete [] ((::ST::ISUSYObjDef_xAODTool*)p);
   }
   static void destruct_STcLcLISUSYObjDef_xAODTool(void *p) {
      typedef ::ST::ISUSYObjDef_xAODTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ST::ISUSYObjDef_xAODTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_STcLcLSUSYObjDef_xAOD(void *p) {
      delete ((::ST::SUSYObjDef_xAOD*)p);
   }
   static void deleteArray_STcLcLSUSYObjDef_xAOD(void *p) {
      delete [] ((::ST::SUSYObjDef_xAOD*)p);
   }
   static void destruct_STcLcLSUSYObjDef_xAOD(void *p) {
      typedef ::ST::SUSYObjDef_xAOD current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ST::SUSYObjDef_xAOD

namespace ROOT {
   // Wrappers around operator new
   static void *new_SUSYcLcLCrossSectionDB(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::SUSY::CrossSectionDB : new ::SUSY::CrossSectionDB;
   }
   static void *newArray_SUSYcLcLCrossSectionDB(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::SUSY::CrossSectionDB[nElements] : new ::SUSY::CrossSectionDB[nElements];
   }
   // Wrapper around operator delete
   static void delete_SUSYcLcLCrossSectionDB(void *p) {
      delete ((::SUSY::CrossSectionDB*)p);
   }
   static void deleteArray_SUSYcLcLCrossSectionDB(void *p) {
      delete [] ((::SUSY::CrossSectionDB*)p);
   }
   static void destruct_SUSYcLcLCrossSectionDB(void *p) {
      typedef ::SUSY::CrossSectionDB current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SUSY::CrossSectionDB

namespace {
  void TriggerDictionaryInitialization_libSUSYToolsDict_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libSUSYToolsDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace ST{class __attribute__((annotate("$clingAutoload$SUSYTools/ISUSYObjDef_xAODTool.h")))  __attribute__((annotate("$clingAutoload$SUSYTools/SUSYObjDef_xAOD.h")))  ISUSYObjDef_xAODTool;}
namespace ST{class __attribute__((annotate("$clingAutoload$SUSYTools/SUSYObjDef_xAOD.h")))  SUSYObjDef_xAOD;}
namespace SUSY{class __attribute__((annotate("$clingAutoload$SUSYTools/SUSYCrossSection.h")))  CrossSectionDB;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libSUSYToolsDict dictionary payload"

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
  #define PACKAGE_VERSION "SUSYTools-00-00-00"
#endif
#ifndef PACKAGE_VERSION_UQ
  #define PACKAGE_VERSION_UQ SUSYTools-00-00-00
#endif
#ifndef EIGEN_DONT_VECTORIZE
  #define EIGEN_DONT_VECTORIZE 1
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
#ifndef NO_SHOWERDECONSTRUCTION
  #define NO_SHOWERDECONSTRUCTION 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
/*
  Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
*/

#ifndef SUSYTOOLS_SUSYTOOLSDICT_H
#define SUSYTOOLS_SUSYTOOLSDICT_H

#include "SUSYTools/SUSYObjDef_xAOD.h"
#include "SUSYTools/SUSYCrossSection.h"
#endif // not SUSYTOOLS_SUSYTOOLSDICT_H

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ST::ISUSYObjDef_xAODTool", payloadCode, "@",
"ST::SUSYObjDef_xAOD", payloadCode, "@",
"SUSY::CrossSectionDB", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libSUSYToolsDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libSUSYToolsDict_Impl, {{"namespace DataVector_detail { template <typename B1, typename B2, typename B3> class VirtBases; }", 1},{"template <typename T> class DataVectorBase;", 1},{"template <typename T, typename BASE> class DataVector;", 1},{"namespace DataVector_detail { template <typename B> class DVLEltBase_init; }", 1},{"namespace DataVector_detail { template <typename T> class RegisterDVLEltBaseInit; }", 1},{"namespace DataVector_detail { template <typename T> class DVLEltBaseInit; }", 1}}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libSUSYToolsDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libSUSYToolsDict() {
  TriggerDictionaryInitialization_libSUSYToolsDict_Impl();
}
