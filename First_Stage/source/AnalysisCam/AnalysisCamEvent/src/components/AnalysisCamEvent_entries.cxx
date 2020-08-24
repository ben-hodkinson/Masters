
#include "GaudiKernel/DeclareFactoryEntries.h"

#include "AnalysisCamEvent/CamWriter.h"
#include "AnalysisCamEvent/CamMetaData.h"
#include "AnalysisCamEvent/CamAlgorithm.h"
#include "AnalysisCamEvent/ExecuteTimeEstimator.h"


//DECLARE_TOOL_FACTORY(CamWriter)
DECLARE_SERVICE_FACTORY(CamWriter)
DECLARE_SERVICE_FACTORY(ExecuteTimeEstimator)


DECLARE_TOOL_FACTORY(CamMetaData)
DECLARE_ALGORITHM_FACTORY(CamAlgorithm )


#include "../CamExampleAlg.h"
DECLARE_ALGORITHM_FACTORY( CamExampleAlg )

#include "../CamxAODExampleAlg.h"
DECLARE_ALGORITHM_FACTORY( CamxAODExampleAlg )

DECLARE_FACTORY_ENTRIES(AnalysisCamEvent) {
  DECLARE_ALGORITHM( CamExampleAlg );
  DECLARE_ALGORITHM( CamxAODExampleAlg );
   //DECLARE_TOOL(CamWriter)
   DECLARE_SERVICE(CamWriter)
   DECLARE_SERVICE(ExecuteTimeEstimator)
   DECLARE_TOOL(CamMetaData)
   DECLARE_ALGORITHM(CamAlgorithm)
}