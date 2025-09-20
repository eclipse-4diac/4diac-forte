#include "forte/funcbloc.h"
#include "forte/resource.h"
#include "../../resource_internal.h"

namespace forte {
  void CFunctionBlock::traceOutputEvent(TEventID paEOID, CEventChainExecutionThread *const paECET) {
    if (auto &tracer = getResource()->getInternal().getTracer(); tracer.isEnabled()) {

      // don't trace events that are not connected since they don't increase the event counter
      if (getEOConUnchecked(static_cast<TPortId>(paEOID))->getDestinationList().size() == 0) {
        return;
      }

      std::vector<std::string> outputs(getFBInterfaceSpec().getNumDOs());
      std::vector<const char *> outputs_c_str(outputs.size());

      for (TPortId i = 0; i < outputs.size(); ++i) {
        CIEC_ANY *value = getDO(i);
        std::string &valueString = outputs[i];
        value->toString(valueString);
        outputs_c_str[i] = valueString.c_str();
      }
      tracer.traceSendOutputEvent(getFBTypeName(), getFullQualifiedApplicationInstanceName('.').c_str(),
                                  static_cast<uint64_t>(paEOID), paECET->mEventCounter,
                                  static_cast<uint32_t>(outputs.size()), outputs_c_str.data());
    }
  }
} // namespace forte
