

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

#ifndef FLEXIBLE_TRACER_H
#define FLEXIBLE_TRACER_H

#include <variant>
#include <string>

#include "forte/stringid.h"
#include "../barectf_platform_forte.h"
#include "internalTracer.h"

namespace forte::trace {
  /**
   * @brief A tracer that can be changed to use the existing tracers: BarectfPlatformFORTE and CInternalTracer
   *
   */
  class CFlexibleTracer final {
    public:
      enum class AvailableTracers { BareCtf, Internal };

      using TracerVariant = std::variant<std::monostate, BarectfPlatformFORTE, CInternalTracer>;

      CFlexibleTracer(StringId instanceName, size_t bufferSize);

      ~CFlexibleTracer() = default;

      CFlexibleTracer(const CFlexibleTracer &) = delete;
      CFlexibleTracer &operator=(const CFlexibleTracer &) = delete;

      CFlexibleTracer(CFlexibleTracer &&) = delete;
      CFlexibleTracer &operator=(CFlexibleTracer &&) = delete;

      void traceInstanceData(const char *const paTypeName,
                             const char *const paInstanceName,
                             const uint32_t paInputsLength,
                             const char *const *const paInputs,
                             const uint32_t paOutputsLength,
                             const char *const *const paOutputs,
                             const uint32_t paInternalLength,
                             const char *const *const paInternal,
                             const uint32_t paInternalFBsLength,
                             const char *const *const paInternalFBs);

      void
      traceReceiveInputEvent(const char *const paTypeName, const char *const paInstanceName, const uint64_t paEventId);

      void traceSendOutputEvent(const char *const paTypeName,
                                const char *const paInstanceName,
                                const uint64_t paEventId
#ifdef FORTE_TRACE_CTF_REPLAY_DEBUGGING
                                ,
                                const uint64_t paEventCounter,
                                const uint32_t paOutputsLength,
                                const char *const *const paOutputs
#endif // FORTE_TRACE_CTF_REPLAY_DEBUGGING
      );

      void traceInputData(const char *const paTypeName,
                          const char *const paInstanceName,
                          const uint64_t paDataId,
                          const char *const paValue);

      void traceOutputData(const char *const paTypeName,
                           const char *const paInstanceName,
                           const uint64_t paDataId,
                           const char *const paValue);

      bool isEnabled();

      const TracerVariant &getTracerVariant() const;

      /**
       * @brief Select the tracer to use
       * @param paTracerType the type of tracer to be used
       */
      static void setTracer(AvailableTracers paTracerType);

    private:
      static inline AvailableTracers mCurrentTracer{AvailableTracers::BareCtf};

      TracerVariant mTracer{};
  };
} // namespace forte::trace
#endif // FLEXIBLE_TRACER_H
