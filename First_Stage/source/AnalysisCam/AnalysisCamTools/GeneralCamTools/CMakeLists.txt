
atlas_subdir( GeneralCamTools )

find_package( ROOT COMPONENTS MathCore RIO Core Tree Hist )
find_package( Boost COMPONENTS filesystem thread system )


atlas_add_library( GeneralCamToolsLib
		   src/*.cxx
		   PUBLIC_HEADERS GeneralCamTools
		   PRIVATE_INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS}
           LINK_LIBRARIES AnalysisCamEventLib 
                          AnalysisCamROOTLib  )

atlas_add_component( GeneralCamTools src/components/*.cxx
                     NOCLIDDB
		     LINK_LIBRARIES GeneralCamToolsLib 
)

