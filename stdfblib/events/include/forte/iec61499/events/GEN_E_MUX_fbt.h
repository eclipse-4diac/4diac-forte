/*******************************************************************************
 * Copyright (c) 2011, 2025 ACIN
 *                          Johannes Kepler University
 *                          Martin Erich Jobst
 *                          HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl
 *     - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB namespace forte::iec61499::events {
class for better handling generic FBs
 *    Martin Jobst - add generic readInputData and writeOutputData
 *....Franz Höpfinger - Update it to represent latest Generation Format from 4diac IDE. no Functional Changes.
 *******************************************************************************/

#pragma once

#include "forte/genfb.h"
#include "forte/datatypes/forte_uint.h"

namespace forte::iec61499::events {
  class GEN_E_MUX final : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_E_MUX)

    protected:
      size_t getGenEOOffset() override {
        return 1;
      }

      size_t getGenDOOffset() override {
        return 1;
      }

    private:
      static const TEventID scmEventEOID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      std::vector<StringId> mEventInputNames;

    public:
      GEN_E_MUX(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_UINT var_K;

      CEventConnection conn_EO;

      COutDataConnection<CIEC_UINT> conn_K;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };
} // namespace forte::iec61499::events
