#include "forte/funcbloc.h"
#include "forte/ecet.h"
#include "../../ecet_internal.h"

namespace forte {
  void CFunctionBlock::traceOutputEvent(TEventID paEOID, CEventChainExecutionThread *const paECET) {
    if (!paECET) {
      return;
    }

    if (auto &tracer = paECET->getInternal().getTracer(); tracer.isEnabled()) {
      tracer.traceSendOutputEvent(getFBTypeName(), getFullQualifiedApplicationInstanceName('.').c_str(),
                                  static_cast<uint64_t>(paEOID));
    }
  }
} // namespace forte
