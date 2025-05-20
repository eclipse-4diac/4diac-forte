#include "core/funcbloc.h"
#include "core/resource.h"

void CFunctionBlock::traceOutputEvent(TEventID paEOID, CEventChainExecutionThread *const) {
  if (auto &tracer = getResource()->getTracer(); tracer.isEnabled()) {
    auto typeName = getFBTypeName();

    tracer.traceSendOutputEvent(typeName ? typeName : "null", getFullQualifiedApplicationInstanceName('.').c_str(),
                                static_cast<uint64_t>(paEOID));
  }
}
