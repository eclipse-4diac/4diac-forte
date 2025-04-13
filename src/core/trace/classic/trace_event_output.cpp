#include "core/funcbloc.h"
#include "core/resource.h"

void CFunctionBlock::traceOutputEvent(TEventID paEOID, CEventChainExecutionThread * const){
  if(auto& tracer = getResource()->getTracer(); tracer.isEnabled()){
 
    tracer.traceSendOutputEvent(getFBTypeName() ?: "null",
                                getFullQualifiedApplicationInstanceName('.').c_str() ?: "null",
                                static_cast<uint64_t>(paEOID));
  }
}