/*******************************************************************************
 * Copyright (c) 2011, 2023 ACIN
 *                          Johannes Kepler University
 *                          Martin Erich Jobst
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
 *******************************************************************************/
#pragma once

#include "forte/genfb.h"
#include "forte/datatypes/forte_uint.h"

#include <memory>

namespace forte::iec61499::events {
  class GEN_E_DEMUX : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_E_DEMUX)

    protected:
      CIEC_ANY *getDI(size_t) override;
      CDataConnection **getDIConUnchecked(const TPortId paIndex) override;

      size_t getGenDIOffset() override {
        return 0;
      }

    private:
      static const TEventID scmEventEIID = 0;

      std::vector<StringId> mEventOutputNames;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEI) override;
      void writeOutputData(TEventID paEO) override;

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      CIEC_UINT var_K;
      CDataConnection *conn_K;

    public:
      GEN_E_DEMUX(const StringId paInstanceNameId, CFBContainer &paContainer);
      ~GEN_E_DEMUX() override = default;
  };
} // namespace forte::iec61499::events
