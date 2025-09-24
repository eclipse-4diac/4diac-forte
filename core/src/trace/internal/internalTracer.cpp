/*******************************************************************************
 * Copyright (c) 2024 Jose Cabral
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral
 *      - initial implementation
 *******************************************************************************/

#include "internalTracer.h"

#include "forte/arch/forte_architecture_time.h"

namespace forte::trace {
  CInternalTracer::CInternalTracer(StringId, size_t) {
  }

  void CInternalTracer::traceInstanceData(const char *const paTypeName,
                                          const char *const paInstanceName,
                                          const uint32_t paInputsLength,
                                          const char *const *const paInputs,
                                          const uint32_t paOutputsLength,
                                          const char *const *const paOutputs,
                                          const uint32_t paInternalLength,
                                          const char *const *const paInternal,
                                          const uint32_t paInternalFBsLength,
                                          const char *const *const paInternalFBs) {

    std::vector<std::string> inputs(paInputsLength);
    std::vector<std::string> outputs(paOutputsLength);
    std::vector<std::string> internal(paInternalLength);
    std::vector<std::string> internalFBs(paInternalFBsLength);

    fillStringsVector(paInputs, paInputsLength, inputs);
    fillStringsVector(paOutputs, paOutputsLength, outputs);
    fillStringsVector(paInternal, paInternalLength, internal);
    fillStringsVector(paInternalFBs, paInternalFBsLength, internalFBs);

    mEvents.emplace_back(
        "instanceData",
        std::make_unique<FBInstanceDataPayload>(paTypeName, paInstanceName, inputs, outputs, internal, internalFBs),
        arch::getNanoSecondsMonotonic());
  }

  void CInternalTracer::traceReceiveInputEvent(const char *const paTypeName,
                                               const char *const paInstanceName,
                                               const uint64_t paEventId) {
    mEvents.emplace_back("receiveInputEvent",
                         std::make_unique<FBInputEventPayload>(paTypeName, paInstanceName, paEventId),
                         arch::getNanoSecondsMonotonic());
  }

  void CInternalTracer::traceSendOutputEvent(const char *const paTypeName,
                                             const char *const paInstanceName,
                                             const uint64_t paEventId
#ifdef FORTE_TRACE_CTF_REPLAY_DEBUGGING
                                             ,
                                             const uint64_t paEventCounter,
                                             const uint32_t paOutputsLength,
                                             const char *const *const paOutputs
#endif
  ) {
#ifdef FORTE_TRACE_CTF_REPLAY_DEBUGGING

    std::vector<std::string> outputs(paOutputsLength);
    fillStringsVector(paOutputs, paOutputsLength, outputs);

    mEvents.emplace_back(
        "sendOutputEvent",
        std::make_unique<FBOutputEventPayload>(paTypeName, paInstanceName, paEventId, paEventCounter, outputs),
        arch::getNanoSecondsMonotonic());
#else // FORTE_TRACE_CTF_REPLAY_DEBUGGING
    mEvents.emplace_back("sendOutputEvent",
                         std::make_unique<FBOutputEventPayload>(paTypeName, paInstanceName, paEventId),
                         arch::getNanoSecondsMonotonic());
#endif // FORTE_TRACE_CTF_REPLAY_DEBUGGING
  }

  void CInternalTracer::traceInputData(const char *const paTypeName,
                                       const char *const paInstanceName,
                                       const uint64_t paDataId,
                                       const char *const paValue) {

    mEvents.emplace_back("inputData", std::make_unique<FBDataPayload>(paTypeName, paInstanceName, paDataId, paValue),
                         arch::getNanoSecondsMonotonic());
  }

  void CInternalTracer::traceOutputData(const char *const paTypeName,
                                        const char *const paInstanceName,
                                        const uint64_t paDataId,
                                        const char *const paValue) {
    mEvents.emplace_back("outputData", std::make_unique<FBDataPayload>(paTypeName, paInstanceName, paDataId, paValue),
                         arch::getNanoSecondsMonotonic());
  }

  bool CInternalTracer::isEnabled() {
    return true;
  }

  void CInternalTracer::fillStringsVector(const char *const *const paIn,
                                          const uint32_t paLen,
                                          std::vector<std::string> &paOut) {
    for (uint32_t i = 0; i < paLen; i++) {
      paOut[i] = paIn[i];
    }
  }

  const std::vector<EventMessage> &CInternalTracer::getEvents() const {
    return mEvents;
  }
} // namespace forte::trace
