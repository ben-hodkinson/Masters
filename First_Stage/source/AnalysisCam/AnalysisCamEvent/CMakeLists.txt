

atlas_subdir( AnalysisCamEvent )

find_package( ROOT COMPONENTS MathCore RIO Core Tree Hist )
find_package( Boost COMPONENTS filesystem thread system )

atlas_add_root_dictionary( AnalysisCamEvent AnalysisCamEventDictSource 
			   ROOT_HEADERS AnalysisCamEvent/CamObject.h AnalysisCamEvent/CamVector.h AnalysisCamEvent/CamDeque.h AnalysisCamEvent/CamEvent.h src/LinkDef.h )

atlas_add_library( AnalysisCamEventLib
		   AnalysisCamEvent/*.h src/*.cxx ${AnalysisCamEventDictSource}
		   PUBLIC_HEADERS AnalysisCamEvent
		   PRIVATE_INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS}
		   LINK_LIBRARIES AthAnalysisBaseCompsLib xAODMissingET 
		                  xAODBase xAODEventInfo SGTools StoreGateLib
				  CxxUtils PATInterfaces
	           PRIVATE_LINK_LIBRARIES EventInfo  )

#atlas_add_dictionary( AnalysisCamEventDict 
#		      AnalysisCamEvent/AnalysisCamEventDict.h 
#		      AnalysisCamEvent/selection.xml 
#		      LINK_LIBRARIES AnalysisCamEventLib )



atlas_add_component( AnalysisCamEvent src/components/*.cxx
		     LINK_LIBRARIES AnalysisCamEventLib )

atlas_install_python_modules ( python/*.py )
