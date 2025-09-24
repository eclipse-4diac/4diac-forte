/*******************************************************************************
 * Copyright (c) 2022 Martin Erich Jobst
 *               2023 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Jobst
 *      - initial implementation
 *******************************************************************************/
#ifndef BARECTF_PLATFORM_FORTE_H
#define BARECTF_PLATFORM_FORTE_H

#include <string>
#include <fstream>
#include <memory>
#include <filesystem>

#include "forte/stringid.h"

#include "barectf.h"

namespace forte::trace {
  /**
   * @brief BareCTF tracer
   *
   * It receives the data and it traces into the corresponding files
   */
  class BarectfPlatformFORTE final {
    public:
      BarectfPlatformFORTE(std::filesystem::path filename, size_t bufferSize);
      BarectfPlatformFORTE(StringId instanceName, size_t bufferSize);
      ~BarectfPlatformFORTE();

      BarectfPlatformFORTE(const BarectfPlatformFORTE &) = delete;
      BarectfPlatformFORTE &operator=(const BarectfPlatformFORTE &) = delete;

      BarectfPlatformFORTE(BarectfPlatformFORTE &&) = delete;
      BarectfPlatformFORTE &operator=(BarectfPlatformFORTE &&) = delete;

      void traceInstanceData(const char *const paTypeName,
                             const char *const paInstanceName,
                             const uint32_t paInputsLength,
                             const char *const *const paInputs,
                             const uint32_t paOutputsLength,
                             const char *const *const paOutputs,
                             const uint32_t paInternalLength,
                             const char *const *const paInternal,
                             const uint32_t paInternalFBsLength,
                             const char *const *const paInternalFBs) {
        barectf_default_trace_instanceData(&context, paTypeName, paInstanceName, paInputsLength, paInputs,
                                           paOutputsLength, paOutputs, paInternalLength, paInternal,
                                           paInternalFBsLength, paInternalFBs);
      }

      void
      traceReceiveInputEvent(const char *const paTypeName, const char *const paInstanceName, const uint64_t paEventId) {
        barectf_default_trace_receiveInputEvent(&context, paTypeName, paInstanceName, paEventId);
      }

      void traceSendOutputEvent(const char *const paTypeName,
                                const char *const paInstanceName,
                                const uint64_t paEventId
#ifdef FORTE_TRACE_CTF_REPLAY_DEBUGGING
                                ,
                                const uint64_t paEventCounter,
                                const uint32_t paOutputsLength,
                                const char *const *const paOutputs
#endif // FORTE_TRACE_CTF_REPLAY_DEBUGGING
      ) {
        barectf_default_trace_sendOutputEvent(&context, paTypeName, paInstanceName, paEventId
#ifdef FORTE_TRACE_CTF_REPLAY_DEBUGGING
                                              ,
                                              paEventCounter, paOutputsLength, paOutputs
#endif // FORTE_TRACE_CTF_REPLAY_DEBUGGING
        );
      }

      void traceInputData(const char *const paTypeName,
                          const char *const paInstanceName,
                          const uint64_t paDataId,
                          const char *const paValue) {
        barectf_default_trace_inputData(&context, paTypeName, paInstanceName, paDataId, paValue);
      }

      void traceOutputData(const char *const paTypeName,
                           const char *const paInstanceName,
                           const uint64_t paDataId,
                           const char *const paValue) {
        barectf_default_trace_outputData(&context, paTypeName, paInstanceName, paDataId, paValue);
      }

      bool isEnabled() {
        return barectf_is_tracing_enabled(&context);
      }

      static void setup(std::string_view paDirectory);

    private:
      std::ofstream output;
      std::unique_ptr<uint8_t[]> buffer;
      barectf_default_ctx context;

      static uint64_t getClock(void *data);
      static int isBackendFull(void *data);
      static void openPacket(void *data);
      static void closePacket(void *data);
      static constinit barectf_platform_callbacks barectfCallbacks;
  };
} // namespace forte::trace
#endif // BARECTF_PLATFORM_FORTE_H
