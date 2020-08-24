
#include "GaudiKernel/DeclareFactoryEntries.h"

#include "../MyPackageAlg.h"

DECLARE_ALGORITHM_FACTORY( MyPackageAlg )

DECLARE_FACTORY_ENTRIES( MyPackage ) 
{
  DECLARE_ALGORITHM( MyPackageAlg );
}
