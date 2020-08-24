file(REMOVE_RECURSE
  "../../../x86_64-slc6-gcc62-opt/include/AthAnalysisBaseComps"
  "../../../x86_64-slc6-gcc62-opt/python/AthAnalysisBaseComps/__init__.py"
  "../../../x86_64-slc6-gcc62-opt/jobOptions/AthAnalysisBaseComps/ContainerRemapping.py"
  "../../../x86_64-slc6-gcc62-opt/jobOptions/AthAnalysisBaseComps/SuppressLogging.py"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/Package_AthAnalysisBaseComps_tests.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
