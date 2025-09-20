/*******************************************************************************
 * Copyright (c) 2013, 2023 Profactor GmbH, fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Michael Hofmann, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst - add readInputData and writeOutputData
 *******************************************************************************/

#pragma once

#include "forte/funcbloc.h"

namespace forte::eclipse4diac::utils {
  class FORTE_STEST_END final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_STEST_END)

    private:
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

    public:
      FORTE_STEST_END(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ() {
        executeEvent(scmEventREQID, nullptr);
      }

      void operator()() {
        evt_REQ();
      }

    protected:
      void setInitialValues() override {
        // no variables so nothing to do
      }
  };
} // namespace forte::eclipse4diac::utils
