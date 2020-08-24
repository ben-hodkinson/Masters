#Fri Jul 26 13:53:51 2019"""Automatically generated. DO NOT EDIT please"""
from GaudiKernel.GaudiHandles import *
from AthenaCommon.Configurable import *

class ST__ApplySUSYTools( ConfigurableAlgorithm ) :
  __slots__ = { 
    'OutputLevel' : 0, # int
    'Enable' : True, # bool
    'ErrorMax' : 1, # int
    'ErrorCounter' : 0, # int
    'ExtraInputs' : [], # list
    'ExtraOutputs' : [], # list
    'AuditAlgorithms' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditReinitialize' : False, # bool
    'AuditRestart' : False, # bool
    'AuditExecute' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditBeginRun' : False, # bool
    'AuditEndRun' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'Timeline' : True, # bool
    'MonitorService' : 'MonitorSvc', # str
    'RegisterForContextService' : False, # bool
    'IsClonable' : False, # bool
    'Cardinality' : 1, # int
    'NeededResources' : [  ], # list
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
    'MuonsName' : 'Muons', # str
    'MuonSpecTracksName' : 'MuonSpectrometerTrackParticles', # str
    'ElectronsName' : 'Electrons', # str
    'JetsName' : 'AntiKt4EMTopoJets', # str
    'FatJetsName' : 'AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets', # str
    'BTaggingName' : 'BTagging_AntiKt4EMTopo', # str
    'TruthJetsName' : 'AntiKt4TruthJets', # str
    'METsName' : 'MET_Reference_AntiKt4EMTopo', # str
    'JetsPtCut' : 20000.0, # float
    'InDetTracksName' : 'InDetTrackParticles', # str
    'InDetPtCut' : 1000.00, # float
    'GSFTracksName' : 'GSFTrackParticles', # str
    'GSFVerticesName' : 'GSFConversionVertices', # str
    'ElectronTriggers' : [  ], # list
    'MuonTriggers' : [  ], # list
    'DoTST' : True, # bool
    'TauJetsName' : 'TauJets', # str
    'PhotonsName' : 'Photons', # str
    'PhotonTriggers' : [  ], # list
    'IsData' : 0, # int
    'MaxCount' : 0, # int
    'ThinningSvc' : ServiceHandle('ThinningSvc/ThinningSvc'), # GaudiHandle
    'TauTruthMatchingTool' : PublicToolHandle(''), # GaudiHandle
    'SUSYTools' : PublicToolHandle('SUSYObjDef_xAOD/dummy'), # GaudiHandle
  }
  _propertyDocDct = { 
    'SUSYTools' : """ The SUSYTools instance """,
    'TauTruthMatchingTool' : """ The TTMT """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'Cardinality' : """ How many clones to create """,
    'RegisterForContextService' : """ The flag to enforce the registration for Algorithm Context Service """,
    'IsClonable' : """ Thread-safe enough for cloning? """,
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(ST__ApplySUSYTools, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'SUSYTools'
  def getType( self ):
      return 'ST::ApplySUSYTools'
  pass # class ST__ApplySUSYTools

class ST__SUSYObjDef_xAOD( ConfigurableAlgTool ) :
  __slots__ = { 
    'MonitorService' : 'MonitorSvc', # str
    'OutputLevel' : 7, # int
    'AuditTools' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'AuditFinalize' : False, # bool
    'ExtraInputs' : [], # list
    'ExtraOutputs' : [], # list
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
    'DataSource' : -1, # int
    'DebugMode' : False, # bool
    'ConfigFile' : 'SUSYTools/SUSYTools_Default.conf', # str
    'DoTauOR' : False, # bool
    'DoPhotonOR' : False, # bool
    'DoEleJetOR' : True, # bool
    'DoMuonJetOR' : True, # bool
    'DoBjetOR' : False, # bool
    'DoElBjetOR' : True, # bool
    'DoMuBjetOR' : True, # bool
    'DoTauBjetOR' : True, # bool
    'UseBtagging' : False, # bool
    'DoBoostedElectronOR' : False, # bool
    'BoostedElectronORC1' : -999.00000, # float
    'BoostedElectronORC2' : -999.00000, # float
    'BoostedElectronORMaxConeSize' : -999.00000, # float
    'DoBoostedMuonOR' : True, # bool
    'BoostedMuonORC1' : -999.00000, # float
    'BoostedMuonORC2' : -999.00000, # float
    'BoostedMuonORMaxConeSize' : -999.00000, # float
    'ORDoMuonJetGhostAssociation' : True, # bool
    'ORRemoveCaloMuons' : True, # bool
    'ORMuJetApplyRelPt' : False, # bool
    'ORMuJetPtRatio' : -999.00000, # float
    'ORMuJetInnerDR' : -999.00000, # float
    'ORJetTrkPtRatio' : -999.00000, # float
    'ORInputLabel' : '', # str
    'DoFatJetOR' : False, # bool
    'OREleFatJetDR' : -999.00000, # float
    'ORJetFatJetDR' : -999.00000, # float
    'METEleTerm' : '', # str
    'METGammaTerm' : '', # str
    'METTauTerm' : '', # str
    'METJetTerm' : '', # str
    'METMuonTerm' : '', # str
    'METOutputTerm' : '', # str
    'METDoTrkSyst' : False, # bool
    'METDoCaloSyst' : False, # bool
    'METDoTrkJetSyst' : False, # bool
    'METJetSelection' : '', # str
    'METSysConfigPrefix' : '', # str
    'METRemoveORCaloTaggedMuons' : True, # bool
    'METDoSetMuonJetEMScale' : True, # bool
    'METDoRemoveMuonJets' : True, # bool
    'METUseGhostMuons' : False, # bool
    'METDoMuonEloss' : False, # bool
    'METDoGreedyPhotons' : False, # bool
    'METDoVeryGreedyPhotons' : False, # bool
    'SoftTermParam' : 0, # int
    'TreatPUJets' : True, # bool
    'DoPhiReso' : True, # bool
    'FwdJetDoJVT' : False, # bool
    'FwdJetUseTightOP' : False, # bool
    'JetJMSCalib' : '', # str
    'BtagTagger' : '', # str
    'BtagWPOR' : '', # str
    'BtagWP' : '', # str
    'BtagCalibPath' : '', # str
    'EleBaselinePt' : -99.000000, # float
    'ElePt' : -99.000000, # float
    'EleBaselineEta' : -99.000000, # float
    'EleEta' : -99.000000, # float
    'EleBaselineId' : '', # str
    'EleBaselineCrackVeto' : False, # bool
    'EleId' : '', # str
    'EleIso' : '', # str
    'EleIsoHighPt' : '', # str
    'EleCFT' : '', # str
    'EleD0sig' : -99.000000, # float
    'EleZ0' : -99.000000, # float
    'EleBaselineD0sig' : -99.000000, # float
    'EleBaselineZ0' : -99.000000, # float
    'EleCrackVeto' : False, # bool
    'EleForceNoId' : False, # bool
    'EleEffMapFilePath' : '', # str
    'MuonBaselinePt' : -99.000000, # float
    'MuonPt' : -99.000000, # float
    'MuonBaselineEta' : -99.000000, # float
    'MuonEta' : -99.000000, # float
    'MuonBaselineId' : 3, # int
    'MuonId' : 3, # int
    'MuonIso' : '', # str
    'MuonD0sig' : -99.000000, # float
    'MuonZ0' : -99.000000, # float
    'MuonBaselineD0sig' : -99.000000, # float
    'MuonBaselineZ0' : -99.000000, # float
    'MuonRequireHighPtCuts' : False, # bool
    'MuonForceNoId' : False, # bool
    'MuonTTVASF' : True, # bool
    'PhotonBaselinePt' : -99.000000, # float
    'PhotonPt' : -99.000000, # float
    'PhotonBaselineEta' : -99.000000, # float
    'PhotonEta' : -99.000000, # float
    'PhotonBaselineId' : '', # str
    'PhotonId' : '', # str
    'PhotonIso' : '', # str
    'PhotonTriggerName' : '', # str
    'PhotonBaselineCrackVeto' : True, # bool
    'PhotonCrackVeto' : True, # bool
    'PhotonAllowLate' : False, # bool
    'PhotonEffMapFilePath' : '', # str
    'TauBaselineId' : '', # str
    'TauId' : '', # str
    'TauIdConfigPathBaseline' : '', # str
    'TauIdConfigPath' : '', # str
    'TauDoTruthMatching' : False, # bool
    'TauRecalcElOLR' : False, # bool
    'TauIDRedecorate' : False, # bool
    'SigLepRequireIso' : True, # bool
    'SigElRequireIso' : True, # bool
    'SigMuRequireIso' : True, # bool
    'SigPhRequireIso' : True, # bool
    'SigLepPhIsoCloseByOR' : True, # bool
    'AutoconfigurePRWTool' : True, # bool
    'mcCampaign' : '', # str
    'PRWConfigFiles' : [  ], # list
    'PRWLumiCalcFiles' : [  ], # list
    'PRWActualMuFile' : '', # str
    'PRWMuUncertainty' : -99.000000, # float
    'PRWDataScaleFactor' : -99.000000, # float
    'PRWDataScaleFactorUP' : -99.000000, # float
    'PRWDataScaleFactorDOWN' : -99.000000, # float
    'JetLargeRuncConfig' : '', # str
    'JetLargeRuncVars' : '', # str
    'ShowerType' : 0, # int
    'ElEffNPcorrModel' : '', # str
    'ElectronTriggerSFStringSingle' : '', # str
    'JetCalibTool' : PublicToolHandle(''), # GaudiHandle
    'FatJetCalibTool' : PublicToolHandle(''), # GaudiHandle
    'JERTool' : PublicToolHandle(''), # GaudiHandle
    'JERSmearingTool' : PublicToolHandle(''), # GaudiHandle
    'JetUncertaintiesTool' : PublicToolHandle(''), # GaudiHandle
    'FatJetUncertaintiesTool' : PublicToolHandle(''), # GaudiHandle
    'JetCleaningTool' : PublicToolHandle(''), # GaudiHandle
    'JetJvtUpdateTool' : PublicToolHandle(''), # GaudiHandle
    'JetJvtEfficiencyTool' : PublicToolHandle(''), # GaudiHandle
    'JetFwdJvtEfficiencyTool' : PublicToolHandle(''), # GaudiHandle
    'MuonSelectionTool' : PublicToolHandle(''), # GaudiHandle
    'MuonSelectionHighPtTool' : PublicToolHandle(''), # GaudiHandle
    'MuonSelectionToolBaseline' : PublicToolHandle(''), # GaudiHandle
    'MuonCalibrationAndSmearingTool' : PublicToolHandle(''), # GaudiHandle
    'MuonEfficiencyScaleFactorsTool' : PublicToolHandle(''), # GaudiHandle
    'MuonBadMuonHighPtScaleFactorsTool' : PublicToolHandle(''), # GaudiHandle
    'MuonTTVAEfficiencyScaleFactorsTool' : PublicToolHandle(''), # GaudiHandle
    'MuonIsolationScaleFactorsTool' : PublicToolHandle(''), # GaudiHandle
    'MuonTriggerScaleFactorsTool' : PublicToolHandle(''), # GaudiHandle
    'ElectronEfficiencyCorrectionTool_reco' : PublicToolHandle(''), # GaudiHandle
    'ElectronEfficiencyCorrectionTool_trig_singleLep' : PublicToolHandle(''), # GaudiHandle
    'ElectronEfficiencyCorrectionTool_id' : PublicToolHandle(''), # GaudiHandle
    'ElectronEfficiencyCorrectionTool_iso' : PublicToolHandle(''), # GaudiHandle
    'ElectronEfficiencyCorrectionTool_isoHigPt' : PublicToolHandle(''), # GaudiHandle
    'ElectronEfficiencyCorrectionTool_chf' : PublicToolHandle(''), # GaudiHandle
    'ElectronLikelihoodTool' : PublicToolHandle(''), # GaudiHandle
    'ElectronLikelihoodToolBaseline' : PublicToolHandle(''), # GaudiHandle
    'EgammaAmbiguityTool' : PublicToolHandle(''), # GaudiHandle
    'ElectronChargeIDSelectorTool' : PublicToolHandle(''), # GaudiHandle
    'ElectronChargeEffCorrectionTool' : PublicToolHandle(''), # GaudiHandle
    'PhotonIsEMSelector' : PublicToolHandle(''), # GaudiHandle
    'PhotonIsEMSelectorBaseline' : PublicToolHandle(''), # GaudiHandle
    'PhotonEfficiencyCorrectionTool' : PublicToolHandle(''), # GaudiHandle
    'PhotonIsolationCorrectionTool' : PublicToolHandle(''), # GaudiHandle
    'PhotonTriggerEfficiencyCorrectionTool' : PublicToolHandle(''), # GaudiHandle
    'PhotonShowerShapeFudgeTool' : PublicToolHandle(''), # GaudiHandle
    'EgammaCalibrationAndSmearingTool' : PublicToolHandle(''), # GaudiHandle
    'TauSelectionTool' : PublicToolHandle(''), # GaudiHandle
    'TauSelectionToolBaseline' : PublicToolHandle(''), # GaudiHandle
    'TauSmearingTool' : PublicToolHandle(''), # GaudiHandle
    'TauTruthMatch' : PublicToolHandle(''), # GaudiHandle
    'TauEfficiencyCorrectionsTool' : PublicToolHandle(''), # GaudiHandle
    'TauTrigEfficiencyCorrectionsTool0' : PublicToolHandle(''), # GaudiHandle
    'TauTrigEfficiencyCorrectionsTool1' : PublicToolHandle(''), # GaudiHandle
    'TauTrigEfficiencyCorrectionsTool2' : PublicToolHandle(''), # GaudiHandle
    'TauTrigEfficiencyCorrectionsTool3' : PublicToolHandle(''), # GaudiHandle
    'TauTrigEfficiencyCorrectionsTool4' : PublicToolHandle(''), # GaudiHandle
    'TauOverlappingElectronLLHDecorator' : PublicToolHandle(''), # GaudiHandle
    'BTaggingEfficiencyTool' : PublicToolHandle(''), # GaudiHandle
    'BTaggingSelectionTool' : PublicToolHandle(''), # GaudiHandle
    'BTaggingSelectionTool_OR' : PublicToolHandle(''), # GaudiHandle
    'METMaker' : PublicToolHandle(''), # GaudiHandle
    'METMakerLS' : PublicToolHandle(''), # GaudiHandle
    'METMakerJV' : PublicToolHandle(''), # GaudiHandle
    'METSystTool' : PublicToolHandle(''), # GaudiHandle
    'METSignificance' : PublicToolHandle(''), # GaudiHandle
    'TrigGlobalEfficiencyCorrection_diLep' : PublicToolHandle(''), # GaudiHandle
    'TrigGlobalEfficiencyCorrection_multiLep' : PublicToolHandle(''), # GaudiHandle
    'TrigConfigTool' : PublicToolHandle(''), # GaudiHandle
    'TrigDecisionTool' : PublicToolHandle(''), # GaudiHandle
    'TrigMatchTool' : PublicToolHandle(''), # GaudiHandle
    'IsolationCorrectionTool' : PublicToolHandle(''), # GaudiHandle
    'IsolationSelectionTool' : PublicToolHandle(''), # GaudiHandle
    'IsolationSelectionTool_HighPt' : PublicToolHandle(''), # GaudiHandle
    'IsolationCloseByCorrectionTool' : PublicToolHandle(''), # GaudiHandle
    'PileupReweightingTool' : PublicToolHandle(''), # GaudiHandle
    'PMGSHVjetReweightTool' : PublicToolHandle(''), # GaudiHandle
    'PMGSHVjetReweightWZTool' : PublicToolHandle(''), # GaudiHandle
    'TauJetOverlapTool' : PublicToolHandle(''), # GaudiHandle
  }
  _propertyDocDct = { 
    'TauJetOverlapTool' : """ The TauJetOverlapTool """,
    'PMGSHVjetReweightWZTool' : """ The PMGSHVjetReweightTool (AntiKt4TruthWZJets) """,
    'PMGSHVjetReweightTool' : """ The PMGSHVjetReweightTool (AntiKt4TruthJets) """,
    'PileupReweightingTool' : """ The PRW tool """,
    'IsolationCloseByCorrectionTool' : """ The IsolationCloseByCorrectionTool """,
    'IsolationSelectionTool_HighPt' : """ The IsolationSelectionTool for High Pt """,
    'TrigMatchTool' : """ The TrigMatchingTool """,
    'TrigDecisionTool' : """ The TrigDecisionTool """,
    'TrigGlobalEfficiencyCorrection_diLep' : """ The TrigGlobalEfficiencyCorrection tool for dilepton """,
    'METSignificance' : """ The METSignifiance instance """,
    'METMakerJV' : """ The METMakerJV instance """,
    'BTaggingSelectionTool_OR' : """ The BTaggingSelectionTool used to select b-jets for overlap removal """,
    'BTaggingSelectionTool' : """ The main BTaggingSelectionTool """,
    'TauOverlappingElectronLLHDecorator' : """ The TauOverlappingElectronLLHDecorator tool """,
    'TauTrigEfficiencyCorrectionsTool3' : """ The TauEfficiencyCorrectionsTool for trigger 3 """,
    'TauTrigEfficiencyCorrectionsTool2' : """ The TauEfficiencyCorrectionsTool for trigger 2 """,
    'TauTrigEfficiencyCorrectionsTool1' : """ The TauEfficiencyCorrectionsTool for trigger 1 """,
    'METSystTool' : """ The METSystematicsTool """,
    'MuonTriggerScaleFactorsTool' : """ The MuonTriggerSFTool """,
    'MuonBadMuonHighPtScaleFactorsTool' : """ The MuonBadMuonHighPtSFTool """,
    'ElectronEfficiencyCorrectionTool_chf' : """ The ElectronEfficiencyCorrectionTool for charge-flip SFs """,
    'MuonIsolationScaleFactorsTool' : """ The MuonIsolationSFTool """,
    'MuonCalibrationAndSmearingTool' : """ The MuonCalibrationAndSmearingTool """,
    'PhotonIsEMSelectorBaseline' : """ The PhotonIsEMSelectorTool for baseline photons """,
    'MuonSelectionTool' : """ The MuonSelectionTool for signal muons """,
    'ElectronEfficiencyCorrectionTool_reco' : """ The ElectronEfficiencyCorrectionTool for reconstruction SFs """,
    'TauEfficiencyCorrectionsTool' : """ The TauEfficiencyCorrectionsTool """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'MuonEfficiencyScaleFactorsTool' : """ The MuonEfficiencySFTool """,
    'MuonTTVAEfficiencyScaleFactorsTool' : """ The MuonTTVAEfficiencySFTool """,
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'JetJvtEfficiencyTool' : """ The JetJvtEfficiencyTool """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
    'JetJvtUpdateTool' : """ The JetJvtUpdateTool """,
    'JetCalibTool' : """ The JetCalibTool """,
    'METMakerLS' : """ The METMakerLS instance """,
    'JERSmearingTool' : """ The JERSmearingTool """,
    'ElectronChargeEffCorrectionTool' : """ The ElectronChargeEffCorrectionTool """,
    'JERTool' : """ The JERTool """,
    'TauTrigEfficiencyCorrectionsTool4' : """ The TauEfficiencyCorrectionsTool for trigger 4 """,
    'FatJetUncertaintiesTool' : """ The JetUncertaintiesTool for large-R jets """,
    'MuonSelectionHighPtTool' : """ The MuonSelectionTool for signal muons (HighPt WP) """,
    'JetFwdJvtEfficiencyTool' : """ The JetFwdJvtTool """,
    'FatJetCalibTool' : """ The JetCalibTool for large-R jets """,
    'EgammaAmbiguityTool' : """ The EgammaAmbiguityTool """,
    'ElectronEfficiencyCorrectionTool_trig_singleLep' : """ The ElectronEfficiencyCorrectionTool for single-e triggers """,
    'ElectronEfficiencyCorrectionTool_id' : """ The ElectronEfficiencyCorrectionTool for ID SFs """,
    'JetUncertaintiesTool' : """ The JetUncertaintiesTool """,
    'ElectronLikelihoodTool' : """ The ElectronSelLikelihoodTool for signal electrons """,
    'TrigConfigTool' : """ The TrigConfigTool """,
    'TauSmearingTool' : """ The TauSmearingTool """,
    'ElectronEfficiencyCorrectionTool_iso' : """ The ElectronEfficiencyCorrectionTool for iso SFs """,
    'MuonSelectionToolBaseline' : """ The MuonSelectionTool for baseline muons """,
    'ElectronEfficiencyCorrectionTool_isoHigPt' : """ The ElectronEfficiencyCorrectionTool for iso high-pt SFs """,
    'BTaggingEfficiencyTool' : """ The BTaggingEfficiencyTool """,
    'PhotonIsolationCorrectionTool' : """ The PhotonEfficiencyCorrectionTool for iso SFs """,
    'PhotonEfficiencyCorrectionTool' : """ The PhotonEfficiencyCorrectionTool for reco SFs """,
    'JetCleaningTool' : """ The JetCleaningTool """,
    'ElectronLikelihoodToolBaseline' : """ The ElectronSelLikelihoodTool for baseline electrons """,
    'TrigGlobalEfficiencyCorrection_multiLep' : """ The TrigGlobalEfficiencyCorrection tool for trilepton """,
    'ElectronChargeIDSelectorTool' : """ The ElectronChargeIDSelectorTool """,
    'IsolationSelectionTool' : """ The IsolationSelectionTool """,
    'PhotonIsEMSelector' : """ The PhotonIsEMSelectorTool for signal photons """,
    'PhotonTriggerEfficiencyCorrectionTool' : """ The PhotonEfficiencyCorrectionTool for trigger SFs """,
    'PhotonShowerShapeFudgeTool' : """ The ElectronPhotonShowerShapeFudgeTool """,
    'METMaker' : """ The METMaker instance """,
    'EgammaCalibrationAndSmearingTool' : """ The EgammaCalibrationAndSmearingTool """,
    'TauSelectionTool' : """ The TauSelectionTool for signal taus """,
    'TauSelectionToolBaseline' : """ The TauSelectionTool for baseline taus """,
    'IsolationCorrectionTool' : """ The IsolationCorrectionTool """,
    'TauTruthMatch' : """ The TTMT """,
    'TauTrigEfficiencyCorrectionsTool0' : """ The TauEfficiencyCorrectionsTool for trigger 0 """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(ST__SUSYObjDef_xAOD, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'SUSYTools'
  def getType( self ):
      return 'ST::SUSYObjDef_xAOD'
  pass # class ST__SUSYObjDef_xAOD

class SUSYToolsAlg( ConfigurableAlgorithm ) :
  __slots__ = { 
    'OutputLevel' : 0, # int
    'Enable' : True, # bool
    'ErrorMax' : 1, # int
    'ErrorCounter' : 0, # int
    'ExtraInputs' : [], # list
    'ExtraOutputs' : [], # list
    'AuditAlgorithms' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditReinitialize' : False, # bool
    'AuditRestart' : False, # bool
    'AuditExecute' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditBeginRun' : False, # bool
    'AuditEndRun' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'Timeline' : True, # bool
    'MonitorService' : 'MonitorSvc', # str
    'RegisterForContextService' : False, # bool
    'IsClonable' : False, # bool
    'Cardinality' : 1, # int
    'NeededResources' : [  ], # list
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
    'THistSvc' : ServiceHandle('THistSvc/THistSvc'), # GaudiHandle
    'RootStreamName' : '/', # str
    'RootDirName' : '', # str
    'HistNamePrefix' : '', # str
    'HistNamePostfix' : '', # str
    'HistTitlePrefix' : '', # str
    'HistTitlePostfix' : '', # str
    'DataSource' : 1, # int
    'DoSyst' : False, # bool
    'RateMonitoringPath' : '.', # str
    'STConfigFile' : 'SUSYTools/SUSYTools_Default.conf', # str
    'CheckTruthJets' : True, # bool
    'TauTruthMatchingTool' : PublicToolHandle(''), # GaudiHandle
    'SUSYTools' : PublicToolHandle(''), # GaudiHandle
  }
  _propertyDocDct = { 
    'SUSYTools' : """ The SUSYTools instance """,
    'TauTruthMatchingTool' : """ The TTMT """,
    'RateMonitoringPath' : """ will try to add rate to file found in given path """,
    'HistTitlePostfix' : """ The postfix for the histogram THx title """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
    'IsClonable' : """ Thread-safe enough for cloning? """,
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'RegisterForContextService' : """ The flag to enforce the registration for Algorithm Context Service """,
    'THistSvc' : """ Handle to a THistSvc instance: it will be used to write ROOT objects to ROOT files """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'RootStreamName' : """ Name of the output ROOT stream (file) that the THistSvc uses """,
    'HistTitlePrefix' : """ The prefix for the histogram THx title """,
    'HistNamePostfix' : """ The postfix for the histogram THx name """,
    'RootDirName' : """ Name of the ROOT directory inside the ROOT file where the histograms will go """,
    'Cardinality' : """ How many clones to create """,
    'HistNamePrefix' : """ The prefix for the histogram THx name """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(SUSYToolsAlg, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'SUSYTools'
  def getType( self ):
      return 'SUSYToolsAlg'
  pass # class SUSYToolsAlg
