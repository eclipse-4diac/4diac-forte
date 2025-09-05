#include "core/funcbloc.h"
#include "core/resource.h"
#include "core/resource_internal.h"

void CFunctionBlock::traceOutputEvent(TEventID paEOID, CEventChainExecutionThread *const) {
  if (auto &tracer = getResource()->getInternal().getTracer(); tracer.isEnabled()) {
    tracer.traceSendOutputEvent(getFBTypeName(), getFullQualifiedApplicationInstanceName('.').c_str(),
                                static_cast<uint64_t>(paEOID));
  }
}
