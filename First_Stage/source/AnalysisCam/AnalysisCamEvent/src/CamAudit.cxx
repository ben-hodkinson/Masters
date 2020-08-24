
#include "AnalysisCamEvent/CamAudit.h"

ServiceHandle<IChronoStatSvc> CamAudit::chronoSvc = ServiceHandle<IChronoStatSvc>("ChronoStatSvc","CamAudit");
int CamAudit::what = 0;
