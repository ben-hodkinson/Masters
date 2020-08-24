atlas_subdir( xAODCamTools )

find_package( ROOT COMPONENTS MathCore RIO Core Tree Hist )
find_package( Boost COMPONENTS filesystem thread system )


atlas_add_library( xAODCamToolsLib
		   src/*.cxx
		   PUBLIC_HEADERS xAODCamTools
		   PRIVATE_INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS}
           LINK_LIBRARIES GeneralCamToolsLib 
                          GoodRunsListsLib 
                          xAODBase 
                          xAODLuminosity 
                          xAODRootAccess 
                          GoodRunsListsLib 
                          xAODEgamma 
                          xAODMuon 
			  xAODTau
			  xAODMissingET
			  METInterface
                          IsolationSelectionLib 
                          IsolationCorrectionsLib
                          EgammaAnalysisInterfacesLib
                          #ElectronPhotonFourMomentumCorrection 
                          #MuonMomentumCorrections 
                          JetJvtEfficiencyLib 
                          xAODBTaggingEfficiencyLib
                          MuonAnalysisInterfacesLib
                          MuonSelectorTools
                          ElectronEfficiencyCorrection 
                          MuonAnalysisInterfacesLib
			                    TauAnalysisTools
			  PileupReweighting
                          TrigDecisionTool 
                          JetCalibToolsLib JetCPInterfaces JetInterface )

atlas_add_component( xAODCamTools src/components/*.cxx
                     NOCLIDDB
		     LINK_LIBRARIES xAODCamToolsLib 
)

atlas_install_joboptions( share/configs )
