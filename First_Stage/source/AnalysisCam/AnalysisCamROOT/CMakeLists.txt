
atlas_subdir( AnalysisCamROOT )

find_package( ROOT COMPONENTS MathCore RIO Core Tree Hist TreePlayer  )

#CINT dictionary
atlas_add_root_dictionary( AnalysisCamROOTLib
			   AnalysisCamROOTDictSource
			   ROOT_HEADERS AnalysisCamROOT/TChannelInfo.h AnalysisCamROOT/LinkDef.h  )

atlas_add_library( AnalysisCamROOTLib src/*.cxx ${AnalysisCamROOTDictSource}
		   PUBLIC_HEADERS AnalysisCamROOT
		   PRIVATE_INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
		   LINK_LIBRARIES ${ROOT_LIBRARIES}
)


