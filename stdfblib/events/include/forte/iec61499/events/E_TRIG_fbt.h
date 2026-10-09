/*******************************************************************************
 * Copyright (c) 2025 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Mario Kastner - initial API and implementation and/or initial documentation
 *   Alois Zoitl   - reworked to not overload the event queue
 *
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_TRIG
 *** Description: Trigger unconnected input events of the specified type inside a resource
 *** Version:
 ***     1.0: 2025-01-08/Mario Kastner -  -
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_string.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

#include <optional>

namespace forte::iec61499::events {
  class FORTE_E_TRIG final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_E_TRIG)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      CEventChainExecutionThread *mEcet;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_E_TRIG(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_STRING var_EVENTTYPE;

      CEventConnection conn_CNF;

      CDataConnection *conn_EVENTTYPE;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_STRING &paEVENTTYPE) {
        var_EVENTTYPE = paEVENTTYPE;
        executeEvent(scmEventREQID, nullptr);
      }

      void operator()(const CIEC_STRING &paEVENTTYPE) {
        evt_REQ(paEVENTTYPE);
      }

    private:
      void handleREQ(CEventChainExecutionThread *const paECET);
      void handleOneTriggerEvent(CEventChainExecutionThread *const paECET);

      class CTriggerEventCursor {
        public:
          CTriggerEventCursor() = default;
          CTriggerEventCursor(CFBContainer *paRoot, TEventTypeID paEventType);

          std::optional<TEventEntry> next();

          bool isActive() {
            return mEventType;
          }

        private:
          void enter(CFBContainer *paContainer);

          TEventTypeID mEventType;
          CFunctionBlock *mFb = nullptr;
          TEventID mEventId = 0;
          std::vector<std::span<CFBContainer *const>> mStack;
      };

      CTriggerEventCursor mCursor;
  };
} // namespace forte::iec61499::events
