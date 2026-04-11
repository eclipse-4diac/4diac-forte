/*************************************************************************
 *** Copyright (c) 2026 Andreas Demmler Fahrzeugbau
 ***
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.1.100.202604012115!
 ***
 *** Name: GEN_PROVIDE_ARR
 *** Description: FB to provide a array (generic FB)
 *** Version:
 ***     1.1: 2026-03-21/Moritz Ortmeier - Andreas Demmler Fahrzeugbau - changed naming and removed length output
 ***     1.0: 2026-01-22/Franz Höpfinger - Andreas Demmler Fahrzeugbau -
 *************************************************************************/

#pragma once

#include "forte/genfb.h"
#include "forte/datatypes/forte_array_dynamic.h"

namespace forte::eclipse4diac::convert::providers {
  class GEN_PROVIDE_ARR final : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_PROVIDE_ARR)

    private:
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventINITID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

    public:
      GEN_PROVIDE_ARR(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_ARRAY_DYNAMIC var_D1;

      CEventConnection conn_INITO;

      CInOutDataConnection *conn_inout_in_D1;

      COutInOutDataConnection<CIEC_ARRAY_DYNAMIC> conn_inout_out_D1;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CIEC_ANY *getDIO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
      CInOutDataConnection **getDIOInConUnchecked(TPortId) override;
      CInOutDataConnection *getDIOOutConUnchecked(TPortId) override;

  };
}

