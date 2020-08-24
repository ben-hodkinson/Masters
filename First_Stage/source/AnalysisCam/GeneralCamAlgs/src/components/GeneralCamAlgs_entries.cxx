
#include "GaudiKernel/DeclareFactoryEntries.h"

#include "../SumOfWeightsCamAlg.h"
#include "../NtupleCopyAlg.h"
#include "../NtupleCutAlg.h"

DECLARE_ALGORITHM_FACTORY( SumOfWeightsCamAlg )
DECLARE_ALGORITHM_FACTORY( NtupleCopyAlg )
DECLARE_ALGORITHM_FACTORY( NtupleCutAlg )

DECLARE_FACTORY_ENTRIES( GeneralCamAlgs ) 
{
  DECLARE_ALGORITHM( SumOfWeightsCamAlg );
  DECLARE_ALGORITHM( NtupleCopyAlg );
  DECLARE_ALGORITHM( NtupleCutAlg );
}
