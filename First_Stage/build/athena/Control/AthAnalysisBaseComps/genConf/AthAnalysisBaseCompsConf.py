#Fri Jul 26 12:30:56 2019"""Automatically generated. DO NOT EDIT please"""
from AthenaCommon.Configurable import *

class AthROOTErrorHandlerSvc( ConfigurableService ) :
  __slots__ = { 
    'OutputLevel' : 7, # int
    'AuditServices' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditReInitialize' : False, # bool
    'AuditReStart' : False, # bool
    'ThrowSources' : { 'TBranch::GetBasket' : 3000 , 'TFile::ReadBuffer' : 3000 }, # list
    'CatchLevel' : 6001, # int
  }
  _propertyDocDct = { 
    'CatchLevel' : """ Throw runtime error for all messages at this level or HIGHER """,
    'ThrowSources' : """ Map from source to error level. Any message at level or HIGHER will trigger runtime error """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(AthROOTErrorHandlerSvc, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'AthAnalysisBaseComps'
  def getType( self ):
      return 'AthROOTErrorHandlerSvc'
  pass # class AthROOTErrorHandlerSvc
