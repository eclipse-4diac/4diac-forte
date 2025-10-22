/*******************************************************************************
 * Copyright (c) 2014, 2023 Profactor GmbH, fortiss GbmH
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
 *   Matthias Plasch, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB namespace forte::eclipse4diac::utils {
class for better handling generic FBs
 *    Martin Jobst - add generic readInputData and writeOutputData
 *******************************************************************************/
#pragma once

#include "forte/genfb.h"

#include <memory>
#include "forte/dataconn.h"
#include "forte/datatypes/forte_any_variant.h"
#include "forte/datatypes/forte_array_dynamic.h"

namespace forte::eclipse4diac::convert {
  class GEN_ARRAY2VALUES final : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_ARRAY2VALUES)

    protected:
      void createGenOutputData() override;

      size_t getGenEOOffset() override {
        return 1;
      }

      size_t getGenDIOffset() override {
        return 1;
      }

      CEventConnection *getEOConUnchecked(TPortId paEONum) override;
      CIEC_ANY *getDI(size_t paIndex) override;
      CIEC_ANY *getDO(size_t paIndex) override;
      CDataConnection **getDIConUnchecked(const TPortId paIndex) override;

    private:
      std::vector<StringId> mDataOutputNames;

      static const TEventID scmEventREQID = 0;

      static const TEventID scmEventCNFID = 0;

      // self-defined members

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEI) override;
      void writeOutputData(TEventID paEO) override;

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      CEventConnection conn_CNF;

      CIEC_ARRAY_DYNAMIC var_IN;
      CDataConnection *conn_IN;

      std::unique_ptr<CIEC_ANY_VARIANT[]> mGenDOs;

    public:
      GEN_ARRAY2VALUES(const StringId paInstanceNameId, CFBContainer &paContainer);
      ~GEN_ARRAY2VALUES() override = default;
  };
} // namespace forte::eclipse4diac::convert
