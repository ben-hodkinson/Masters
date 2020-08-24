# Generated by CMake

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.5)
   message(FATAL_ERROR "CMake >= 2.6.0 required")
endif()
cmake_policy(PUSH)
cmake_policy(VERSION 2.6)
#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Protect against multiple inclusion, which would fail when already imported targets are added once more.
set(_targetsDefined)
set(_targetsNotDefined)
set(_expectedTargets)
foreach(_expectedTarget WorkDir::AnalysisCamEventPkg WorkDir::AnalysisCamEventPkgPrivate WorkDir::AnalysisCamEventLib WorkDir::AnalysisCamEvent WorkDir::MyPackagePkg WorkDir::MyPackagePkgPrivate WorkDir::MyPackageLib WorkDir::MyPackage WorkDir::AthAnalysisBaseCompsPkg WorkDir::AthAnalysisBaseCompsPkgPrivate WorkDir::AthAnalysisBaseCompsLib WorkDir::AthAnalysisBaseComps WorkDir::SUSYToolsPkg WorkDir::SUSYToolsPkgPrivate WorkDir::SUSYToolsLib WorkDir::SUSYTools WorkDir::SUSYToolsTester WorkDir::SUSYTruthTester WorkDir::SUSYTools_check_xsections)
  list(APPEND _expectedTargets ${_expectedTarget})
  if(NOT TARGET ${_expectedTarget})
    list(APPEND _targetsNotDefined ${_expectedTarget})
  endif()
  if(TARGET ${_expectedTarget})
    list(APPEND _targetsDefined ${_expectedTarget})
  endif()
endforeach()
if("${_targetsDefined}" STREQUAL "${_expectedTargets}")
  unset(_targetsDefined)
  unset(_targetsNotDefined)
  unset(_expectedTargets)
  set(CMAKE_IMPORT_FILE_VERSION)
  cmake_policy(POP)
  return()
endif()
if(NOT "${_targetsDefined}" STREQUAL "")
  message(FATAL_ERROR "Some (but not all) targets in this export set were already defined.\nTargets Defined: ${_targetsDefined}\nTargets not yet defined: ${_targetsNotDefined}\n")
endif()
unset(_targetsDefined)
unset(_targetsNotDefined)
unset(_expectedTargets)


# Compute the installation prefix relative to this file.
get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
if(_IMPORT_PREFIX STREQUAL "/")
  set(_IMPORT_PREFIX "")
endif()

# Create imported target WorkDir::AnalysisCamEventPkg
add_library(WorkDir::AnalysisCamEventPkg INTERFACE IMPORTED)

set_target_properties(WorkDir::AnalysisCamEventPkg PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/src/AnalysisCam/AnalysisCamEvent"
)

# Create imported target WorkDir::AnalysisCamEventPkgPrivate
add_library(WorkDir::AnalysisCamEventPkgPrivate INTERFACE IMPORTED)

# Create imported target WorkDir::AnalysisCamEventLib
add_library(WorkDir::AnalysisCamEventLib SHARED IMPORTED)

set_target_properties(WorkDir::AnalysisCamEventLib PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "\$<TARGET_PROPERTY:WorkDir::AnalysisCamEventPkg,INTERFACE_INCLUDE_DIRECTORIES>"
  INTERFACE_LINK_LIBRARIES "WorkDir::AthAnalysisBaseCompsLib;xAODMissingET;xAODBase;xAODEventInfo;SGTools;StoreGateLib;CxxUtils;PATInterfaces"
)

# Create imported target WorkDir::AnalysisCamEvent
add_library(WorkDir::AnalysisCamEvent MODULE IMPORTED)

set_target_properties(WorkDir::AnalysisCamEvent PROPERTIES
  INTERFACE_LINK_LIBRARIES "WorkDir::AnalysisCamEventLib;GaudiPluginService"
)

# Create imported target WorkDir::MyPackagePkg
add_library(WorkDir::MyPackagePkg INTERFACE IMPORTED)

set_target_properties(WorkDir::MyPackagePkg PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/src/MyPackage"
)

# Create imported target WorkDir::MyPackagePkgPrivate
add_library(WorkDir::MyPackagePkgPrivate INTERFACE IMPORTED)

# Create imported target WorkDir::MyPackageLib
add_library(WorkDir::MyPackageLib SHARED IMPORTED)

set_target_properties(WorkDir::MyPackageLib PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "\$<TARGET_PROPERTY:WorkDir::MyPackagePkg,INTERFACE_INCLUDE_DIRECTORIES>;${_IMPORT_PREFIX}/\${LCG_RELEASE_BASE}/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/include"
  INTERFACE_LINK_LIBRARIES "\${LCG_RELEASE_BASE}/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/lib/libMathCore.so;\${LCG_RELEASE_BASE}/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/lib/libRIO.so;\${LCG_RELEASE_BASE}/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/lib/libCore.so;\${LCG_RELEASE_BASE}/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/lib/libTree.so;\${LCG_RELEASE_BASE}/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/lib/libHist.so;WorkDir::AthAnalysisBaseCompsLib;WorkDir::SUSYToolsLib;WorkDir::AnalysisCamEventLib;xAODJet;xAODEventInfo;xAODEgamma;xAODMuon;xAODCore;xAODBase;xAODCutFlow"
  INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "\${LCG_RELEASE_BASE}/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/include"
)

# Create imported target WorkDir::MyPackage
add_library(WorkDir::MyPackage MODULE IMPORTED)

set_target_properties(WorkDir::MyPackage PROPERTIES
  INTERFACE_LINK_LIBRARIES "WorkDir::MyPackageLib;GaudiPluginService"
)

# Create imported target WorkDir::AthAnalysisBaseCompsPkg
add_library(WorkDir::AthAnalysisBaseCompsPkg INTERFACE IMPORTED)

set_target_properties(WorkDir::AthAnalysisBaseCompsPkg PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/src/athena/Control/AthAnalysisBaseComps"
  INTERFACE_LINK_LIBRARIES "AthenaBaseCompsPkg;StoreGatePkg;AthContainersPkg;IOVDbDataModelPkg;GaudiKernelPkg"
)

# Create imported target WorkDir::AthAnalysisBaseCompsPkgPrivate
add_library(WorkDir::AthAnalysisBaseCompsPkgPrivate INTERFACE IMPORTED)

# Create imported target WorkDir::AthAnalysisBaseCompsLib
add_library(WorkDir::AthAnalysisBaseCompsLib SHARED IMPORTED)

set_target_properties(WorkDir::AthAnalysisBaseCompsLib PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "\$<TARGET_PROPERTY:WorkDir::AthAnalysisBaseCompsPkg,INTERFACE_INCLUDE_DIRECTORIES>"
  INTERFACE_LINK_LIBRARIES "AthenaBaseComps;IOVDbDataModel;GaudiKernel;StoreGateLib;SGtests;AthContainers"
)

# Create imported target WorkDir::AthAnalysisBaseComps
add_library(WorkDir::AthAnalysisBaseComps MODULE IMPORTED)

set_target_properties(WorkDir::AthAnalysisBaseComps PROPERTIES
  INTERFACE_LINK_LIBRARIES "AthenaBaseComps;IOVDbDataModel;GaudiKernel;StoreGateLib;SGtests;AthContainers;WorkDir::AthAnalysisBaseCompsLib"
)

# Create imported target WorkDir::SUSYToolsPkg
add_library(WorkDir::SUSYToolsPkg INTERFACE IMPORTED)

set_target_properties(WorkDir::SUSYToolsPkg PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/src/athena/PhysicsAnalysis/SUSYPhys/SUSYTools"
  INTERFACE_LINK_LIBRARIES "AsgToolsPkg;xAODCorePkg;xAODEgammaPkg;xAODEventInfoPkg;xAODJetPkg;xAODMissingETPkg;xAODTrigMissingETPkg;xAODMuonPkg;xAODTauPkg;xAODTrackingPkg;xAODTruthPkg;xAODMetaDataPkg;AssociationUtilsPkg;PATInterfacesPkg;PMGAnalysisInterfacesPkg;MCTruthClassifierPkg;JetJvtEfficiencyPkg;JetSubStructureUtilsPkg;TrigDecisionToolPkg"
)

# Create imported target WorkDir::SUSYToolsPkgPrivate
add_library(WorkDir::SUSYToolsPkgPrivate INTERFACE IMPORTED)

set_target_properties(WorkDir::SUSYToolsPkgPrivate PROPERTIES
  INTERFACE_LINK_LIBRARIES "Asg_TestPkg;xAODRootAccessPkg;AthContainersPkg;GoodRunsListsPkg;EventPrimitivesPkg;FourMomUtilsPkg;xAODBTaggingPkg;xAODBasePkg;xAODCaloEventPkg;xAODCutFlowPkg;xAODPrimitivesPkg;IsolationSelectionPkg;PileupReweightingPkg;IsolationCorrectionsPkg;EgammaAnalysisHelpersPkg;EgammaAnalysisInterfacesPkg;AsgAnalysisInterfacesPkg;FTagAnalysisInterfacesPkg;MuonAnalysisInterfacesPkg;TriggerAnalysisInterfacesPkg;JetSelectorToolsPkg;TauAnalysisToolsPkg;JetCPInterfacesPkg;JetCalibToolsPkg;JetInterfacePkg;JetResolutionPkg;JetUncertaintiesPkg;JetMomentToolsPkg;METInterfacePkg;METUtilitiesPkg;tauRecToolsPkg;PathResolverPkg;TriggerMatchingToolPkg;TrigConfInterfacesPkg;TrigConfxAODPkg;GaudiKernelPkg;POOLRootAccessPkg;WorkDir::AthAnalysisBaseCompsPkg;AthenaBaseCompsPkg;AthenaKernelPkg"
)

# Create imported target WorkDir::SUSYToolsLib
add_library(WorkDir::SUSYToolsLib SHARED IMPORTED)

set_target_properties(WorkDir::SUSYToolsLib PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "\$<TARGET_PROPERTY:WorkDir::SUSYToolsPkg,INTERFACE_INCLUDE_DIRECTORIES>"
  INTERFACE_LINK_LIBRARIES "AsgTools;xAODCore;xAODEgamma;xAODEventInfo;xAODJet;xAODMissingET;xAODMuon;xAODTau;xAODTracking;xAODTruth;AssociationUtilsLib;PATInterfaces;TrigDecisionToolLib;MCTruthClassifierLib;JetJvtEfficiencyLib;JetSubStructureUtils"
)

# Create imported target WorkDir::SUSYTools
add_library(WorkDir::SUSYTools MODULE IMPORTED)

set_target_properties(WorkDir::SUSYTools PROPERTIES
  INTERFACE_LINK_LIBRARIES "\${LCG_RELEASE_BASE}/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/lib/libCore.so;\${LCG_RELEASE_BASE}/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/lib/libTree.so;\${LCG_RELEASE_BASE}/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/lib/libMathCore.so;\${LCG_RELEASE_BASE}/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/lib/libHist.so;\${LCG_RELEASE_BASE}/LCG_93/ROOT/6.12.06/x86_64-slc6-gcc62-opt/lib/libRIO.so;AthenaBaseComps;GaudiKernel;AthenaKernel;WorkDir::AthAnalysisBaseCompsLib;xAODEventInfo;xAODMuon;xAODPrimitives;xAODJet;xAODBTagging;xAODEgamma;xAODMissingET;xAODTracking;xAODTau;TauAnalysisToolsLib;xAODCore;AthContainers;AsgTools;xAODBase;xAODCutFlow;PATInterfaces;PathResolver;WorkDir::SUSYToolsLib;GaudiPluginService"
)

# Create imported target WorkDir::SUSYToolsTester
add_executable(WorkDir::SUSYToolsTester IMPORTED)

# Create imported target WorkDir::SUSYTruthTester
add_executable(WorkDir::SUSYTruthTester IMPORTED)

# Create imported target WorkDir::SUSYTools_check_xsections
add_executable(WorkDir::SUSYTools_check_xsections IMPORTED)

if(CMAKE_VERSION VERSION_LESS 3.0.0)
  message(FATAL_ERROR "This file relies on consumers using CMake 3.0.0 or greater.")
endif()

# Load information for each installed configuration.
get_filename_component(_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
file(GLOB CONFIG_FILES "${_DIR}/WorkDirConfig-targets-*.cmake")
foreach(f ${CONFIG_FILES})
  include(${f})
endforeach()

# Cleanup temporary variables.
set(_IMPORT_PREFIX)

# Loop over all imported files and verify that they actually exist
foreach(target ${_IMPORT_CHECK_TARGETS} )
  foreach(file ${_IMPORT_CHECK_FILES_FOR_${target}} )
    if(NOT EXISTS "${file}" )
      message(FATAL_ERROR "The imported target \"${target}\" references the file
   \"${file}\"
but this file does not exist.  Possible reasons include:
* The file was deleted, renamed, or moved to another location.
* An install or uninstall procedure did not complete successfully.
* The installation package was faulty and contained
   \"${CMAKE_CURRENT_LIST_FILE}\"
but not all the files it references.
")
    endif()
  endforeach()
  unset(_IMPORT_CHECK_FILES_FOR_${target})
endforeach()
unset(_IMPORT_CHECK_TARGETS)

# This file does not depend on other imported targets which have
# been exported from the same project but in a separate export set.

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
cmake_policy(POP)
