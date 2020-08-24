file(REMOVE_RECURSE
  "../x86_64-slc6-gcc62-opt/include/MyPackage"
  "../x86_64-slc6-gcc62-opt/python/MyPackage/__init__.py"
  "../x86_64-slc6-gcc62-opt/jobOptions/MyPackage/MyPackageAlgJobOptions.py"
  "CMakeFiles/MyPackageComponentsList"
  "../x86_64-slc6-gcc62-opt/lib/libMyPackage.components"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/MyPackageComponentsList.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
