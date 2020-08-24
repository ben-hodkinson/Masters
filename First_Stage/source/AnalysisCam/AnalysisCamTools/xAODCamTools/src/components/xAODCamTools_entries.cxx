#include "GaudiKernel/DeclareFactoryEntries.h"

#include "../xAODLumiSvc.h"
#include "../CamJetProvider.h"
#include "../CamElProvider.h"
#include "../CamMuonProvider.h"
#include "../CamJetProvider.h"
#include "../CamEventPreselectionTool.h"
#include "../CamElSfProvider.h"
#include "../CamMuonSfProvider.h"
#include "../CamJetSfProvider.h"
#include "../CamEleSelector.h"
#include "../CamMuonSelector.h"
#include "../CamJetSelector.h"
#include "../CamJetSfProvider.h"
#include "../CamTauProvider.h"
#include "../CamTauSelector.h"
#include "../CamMETProvider.h"

DECLARE_SERVICE_FACTORY(xAODLumiSvc)

DECLARE_TOOL_FACTORY(CamJetProvider)

DECLARE_TOOL_FACTORY(CamElProvider)

DECLARE_TOOL_FACTORY(CamMuonProvider)

DECLARE_TOOL_FACTORY(CamMETProvider)

DECLARE_TOOL_FACTORY(CamEventPreselectionTool)

DECLARE_TOOL_FACTORY(CamElSfProvider)

DECLARE_TOOL_FACTORY(CamMuonSfProvider)

DECLARE_TOOL_FACTORY(CamJetSfProvider)

DECLARE_TOOL_FACTORY(CamEleSelector)
DECLARE_TOOL_FACTORY(CamMuonSelector)
DECLARE_TOOL_FACTORY(CamJetSelector)

DECLARE_TOOL_FACTORY(CamTauProvider)
DECLARE_TOOL_FACTORY(CamTauSelector)

DECLARE_FACTORY_ENTRIES(xAODCamTools) {

   DECLARE_SERVICE(xAODLumiSvc)
   DECLARE_TOOL(CamJetProvider)
   DECLARE_TOOL(CamElProvider)
   DECLARE_TOOL(CamMuonProvider)
   DECLARE_TOOL(CamElSfProvider)
   DECLARE_TOOL(CamMuonSfProvider)
   DECLARE_TOOL(CamJetSfProvider)
   DECLARE_TOOL(CamEleSelector)
   DECLARE_TOOL(CamMuonSelector)
   DECLARE_TOOL(CamJetSelector)
   DECLARE_TOOL(CamTauProvider)
   DECLARE_TOOL(CamTauSelector)
   DECLARE_TOOL(CamMETProvider)

}
