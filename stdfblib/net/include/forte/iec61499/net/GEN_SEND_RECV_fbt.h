/*******************************************************************************
 * Copyright (c) 2025 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/

#pragma once

#include "forte/com/fb.h"

namespace forte::iec61499::net {
  class GEN_SEND_RECV : public com::CommunicationFB {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_SEND_RECV)

    public:
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventCNFID = 1;
      static const TEventID scmEventINDID = 2;
      static const TEventID scmEventINITID = 0;
      static const TEventID scmEventREQID = 1;
      static const TEventID scmEventRSPID = 2;

      CEventConnection conn_CNF;
      CEventConnection conn_IND;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *paECET) override;

      GEN_SEND_RECV(StringId paInstanceNameId, CFBContainer &paContainer);

      ~GEN_SEND_RECV() override = default;

      template<typename... Args>
      void evt_INIT(Args &&...paArgs) {
        writeInputArguments(std::forward<Args>(paArgs)...);
        receiveInputEvent(scmEventINITID, nullptr);
        readOutputArguments(std::forward<Args>(paArgs)...);
      }

      template<typename... Args>
      void evt_REQ(Args &&...paArgs) {
        writeInputArguments(std::forward<Args>(paArgs)...);
        receiveInputEvent(scmEventREQID, nullptr);
        readOutputArguments(std::forward<Args>(paArgs)...);
      }

      template<typename... Args>
      void evt_RSP(Args &&...paArgs) {
        writeInputArguments(std::forward<Args>(paArgs)...);
        receiveInputEvent(scmEventRSPID, nullptr);
        readOutputArguments(std::forward<Args>(paArgs)...);
      }

      template<typename... Args>
      void operator()(Args &&...paArgs) {
        evt_INIT(std::forward<Args>(paArgs)...);
      }

    protected:
      size_t getGenEOOffset() override {
        return 3;
      }

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      CEventConnection *getEOConUnchecked(TPortId paIndex) override;
  };
} // namespace forte::iec61499::net
