#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "WorkDir::AnalysisCamEventLib" for configuration "RelWithDebInfo"
set_property(TARGET WorkDir::AnalysisCamEventLib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(WorkDir::AnalysisCamEventLib PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELWITHDEBINFO "EventInfo"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libAnalysisCamEventLib.so"
  IMPORTED_SONAME_RELWITHDEBINFO "libAnalysisCamEventLib.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS WorkDir::AnalysisCamEventLib )
list(APPEND _IMPORT_CHECK_FILES_FOR_WorkDir::AnalysisCamEventLib "${_IMPORT_PREFIX}/lib/libAnalysisCamEventLib.so" )

# Import target "WorkDir::AnalysisCamEvent" for configuration "RelWithDebInfo"
set_property(TARGET WorkDir::AnalysisCamEvent APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(WorkDir::AnalysisCamEvent PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libAnalysisCamEvent.so"
  IMPORTED_NO_SONAME_RELWITHDEBINFO "TRUE"
  )

list(APPEND _IMPORT_CHECK_TARGETS WorkDir::AnalysisCamEvent )
list(APPEND _IMPORT_CHECK_FILES_FOR_WorkDir::AnalysisCamEvent "${_IMPORT_PREFIX}/lib/libAnalysisCamEvent.so" )

# Import target "WorkDir::MyPackageLib" for configuration "RelWithDebInfo"
set_property(TARGET WorkDir::MyPackageLib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(WorkDir::MyPackageLib PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libMyPackageLib.so"
  IMPORTED_SONAME_RELWITHDEBINFO "libMyPackageLib.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS WorkDir::MyPackageLib )
list(APPEND _IMPORT_CHECK_FILES_FOR_WorkDir::MyPackageLib "${_IMPORT_PREFIX}/lib/libMyPackageLib.so" )

# Import target "WorkDir::MyPackage" for configuration "RelWithDebInfo"
set_property(TARGET WorkDir::MyPackage APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(WorkDir::MyPackage PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libMyPackage.so"
  IMPORTED_NO_SONAME_RELWITHDEBINFO "TRUE"
  )

list(APPEND _IMPORT_CHECK_TARGETS WorkDir::MyPackage )
list(APPEND _IMPORT_CHECK_FILES_FOR_WorkDir::MyPackage "${_IMPORT_PREFIX}/lib/libMyPackage.so" )

# Import target "WorkDir::AthAnalysisBaseCompsLib" for configuration "RelWithDebInfo"
set_property(TARGET WorkDir::AthAnalysisBaseCompsLib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(WorkDir::AthAnalysisBaseCompsLib PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libAthAnalysisBaseCompsLib.so"
  IMPORTED_SONAME_RELWITHDEBINFO "libAthAnalysisBaseCompsLib.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS WorkDir::AthAnalysisBaseCompsLib )
list(APPEND _IMPORT_CHECK_FILES_FOR_WorkDir::AthAnalysisBaseCompsLib "${_IMPORT_PREFIX}/lib/libAthAnalysisBaseCompsLib.so" )

# Import target "WorkDir::AthAnalysisBaseComps" for configuration "RelWithDebInfo"
set_property(TARGET WorkDir::AthAnalysisBaseComps APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(WorkDir::AthAnalysisBaseComps PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libAthAnalysisBaseComps.so"
  IMPORTED_NO_SONAME_RELWITHDEBINFO "TRUE"
  )

list(APPEND _IMPORT_CHECK_TARGETS WorkDir::AthAnalysisBaseComps )
list(APPEND _IMPORT_CHECK_FILES_FOR_WorkDir::AthAnalysisBaseComps "${_IMPORT_PREFIX}/lib/libAthAnalysisBaseComps.so" )

# Import target "WorkDir::SUSYToolsLib" for configuration "RelWithDebInfo"
set_property(TARGET WorkDir::SUSYToolsLib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(WorkDir::SUSYToolsLib PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELWITHDEBINFO "AthContainers;FourMomUtils;xAODBTagging;xAODBase;xAODPrimitives;IsolationSelectionLib;PileupReweightingLib;EgammaAnalysisHelpersLib;IsolationCorrectionsLib;JetSelectorToolsLib;TauAnalysisToolsLib;JetCalibToolsLib;JetInterface;JetResolutionLib;JetUncertaintiesLib;JetMomentToolsLib;METUtilitiesLib;PathResolver;TriggerMatchingToolLib;TrigConfxAODLib;xAODTrigMissingET;xAODMetaData;GaudiKernel;WorkDir::AthAnalysisBaseCompsLib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libSUSYToolsLib.so"
  IMPORTED_SONAME_RELWITHDEBINFO "libSUSYToolsLib.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS WorkDir::SUSYToolsLib )
list(APPEND _IMPORT_CHECK_FILES_FOR_WorkDir::SUSYToolsLib "${_IMPORT_PREFIX}/lib/libSUSYToolsLib.so" )

# Import target "WorkDir::SUSYTools" for configuration "RelWithDebInfo"
set_property(TARGET WorkDir::SUSYTools APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(WorkDir::SUSYTools PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libSUSYTools.so"
  IMPORTED_NO_SONAME_RELWITHDEBINFO "TRUE"
  )

list(APPEND _IMPORT_CHECK_TARGETS WorkDir::SUSYTools )
list(APPEND _IMPORT_CHECK_FILES_FOR_WorkDir::SUSYTools "${_IMPORT_PREFIX}/lib/libSUSYTools.so" )

# Import target "WorkDir::SUSYToolsTester" for configuration "RelWithDebInfo"
set_property(TARGET WorkDir::SUSYToolsTester APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(WorkDir::SUSYToolsTester PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/SUSYToolsTester"
  )

list(APPEND _IMPORT_CHECK_TARGETS WorkDir::SUSYToolsTester )
list(APPEND _IMPORT_CHECK_FILES_FOR_WorkDir::SUSYToolsTester "${_IMPORT_PREFIX}/bin/SUSYToolsTester" )

# Import target "WorkDir::SUSYTruthTester" for configuration "RelWithDebInfo"
set_property(TARGET WorkDir::SUSYTruthTester APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(WorkDir::SUSYTruthTester PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/SUSYTruthTester"
  )

list(APPEND _IMPORT_CHECK_TARGETS WorkDir::SUSYTruthTester )
list(APPEND _IMPORT_CHECK_FILES_FOR_WorkDir::SUSYTruthTester "${_IMPORT_PREFIX}/bin/SUSYTruthTester" )

# Import target "WorkDir::SUSYTools_check_xsections" for configuration "RelWithDebInfo"
set_property(TARGET WorkDir::SUSYTools_check_xsections APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(WorkDir::SUSYTools_check_xsections PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/SUSYTools_check_xsections"
  )

list(APPEND _IMPORT_CHECK_TARGETS WorkDir::SUSYTools_check_xsections )
list(APPEND _IMPORT_CHECK_FILES_FOR_WorkDir::SUSYTools_check_xsections "${_IMPORT_PREFIX}/bin/SUSYTools_check_xsections" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
