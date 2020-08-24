## automatically generated CMakeLists.txt file

# Declare the package
atlas_subdir( GeneralCamAlgs )

# Declare external dependencies ... default here is to include ROOT
find_package( ROOT COMPONENTS MathCore RIO Core Tree Hist )



# if you add components (tools, algorithms) to this package
# these lines are needed so you can configure them in joboptions
atlas_add_component( GeneralCamAlgs src/*.cxx src/components/*.cxx
                     PUBLIC_HEADERS GeneralCamAlgs
                     NOCLID
                   INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
                   PRIVATE_LINK_LIBRARIES ${ROOT_LIBRARIES}  GeneralCamToolsLib xAODCutFlow EventInfo
)

# if you add an application (exe) to this package
# declare it like this (note convention that apps go in the util dir)
# atlas_add_executable( MyApp util/myApp.cxx
#                       LINK_LIBRARIES GeneralCamAlgsLib
# )

# Install python modules, joboptions, and share content
atlas_install_python_modules( python/*.py )
atlas_install_joboptions( share/*.py )
atlas_install_data( data/* )
# You can access your data from code using path resolver, e.g.
# PathResolverFindCalibFile("GeneralCamAlgs/file.txt")



