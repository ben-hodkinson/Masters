
#usage:
#import AnalysisCamEvent.ExecuteTimeEstimator

from AthenaCommon.AppMgr import theApp
from AthenaCommon.AppMgr import ServiceMgr as svcMgr
from AthenaCommon import CfgMgr

if not hasattr(svcMgr, 'ExecuteTimeEstimator'):
   svcMgr += CfgMgr.ExecuteTimeEstimator("ExecuteTimeEstimator")
   theApp.CreateSvc += ['ExecuteTimeEstimator/ExecuteTimeEstimator']
