#include "forte/funcbloc.h"
#include "forte/resource.h"
#include "../../resource_internal.h"

namespace forte {
  void CFunctionBlock::traceOutputEvent(TEventID paEOID, CEventChainExecutionThread *const) {
    if (auto &tracer = getResource()->getInternal().getTracer(); tracer.isEnabled()) {
      tracer.traceSendOutputEvent(getFBTypeName(), getFullQualifiedApplicationInstanceName('.').c_str(),
                                  static_cast<uint64_t>(paEOID));
    }
  }
} // namespace forte
