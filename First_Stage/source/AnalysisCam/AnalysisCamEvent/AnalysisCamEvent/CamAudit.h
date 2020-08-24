
#include "GaudiKernel/ServiceHandle.h"
#include "GaudiKernel/IChronoStatSvc.h"


class CamAudit {
 public:
  static ServiceHandle<IChronoStatSvc> chronoSvc;
  static int what; //what to audit

  enum AuditOptions {
    TOOLS = 1,
    SERVICES = 2,
    PERSYST = 4
  };

};
