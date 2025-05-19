#include "core/funcbloc.h"
#include "core/resource.h"

void CFunctionBlock::traceOutputEvent(TEventID paEOID, CEventChainExecutionThread *const) {
  if (auto &tracer = getResource()->getTracer(); tracer.isEnabled()) {
    auto typeName = getFBTypeName();
    auto fullName = getFullQualifiedApplicationInstanceName('.').c_str();

    tracer.traceSendOutputEvent(typeName ? typeName : "null", fullName ? fullName : "null",
                                static_cast<uint64_t>(paEOID));
  }
}
