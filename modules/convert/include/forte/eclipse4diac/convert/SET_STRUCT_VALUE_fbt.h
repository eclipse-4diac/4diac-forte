/*******************************************************************************
 * Copyright (c) 2018, 2023 fortiss GmbH, Primetals Technologies Austria GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *   Alois Zoitl - adapted this FB from the code in GET_STRUCT_VALUE
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_any_variant.h"
#include "forte/datatypes/forte_string.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::convert {
  class FORTE_SET_STRUCT_VALUE : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_SET_STRUCT_VALUE)

    private:
      static const TEventID scmEventREQID = 0;

      static const TEventID scmEventCNFID = 0;

      CIEC_ANY *lookForMember(CIEC_STRUCT &paWhereToLook, char *paMemberName);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

    public:
      FORTE_SET_STRUCT_VALUE(const StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_ANY_VARIANT var_in_struct;
      CIEC_STRING var_member;
      CIEC_ANY_VARIANT var_element_value;
      CIEC_ANY_VARIANT var_out_struct;

      CEventConnection conn_CNF;
      CDataConnection *conn_in_struct;
      CDataConnection *conn_member;
      CDataConnection *conn_element_value;
      COutDataConnection<CIEC_ANY_VARIANT> conn_out_struct;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_ANY &pa_in_struct,
                   const CIEC_STRING &pa_member,
                   const CIEC_ANY &pa_element_value,
                   COutputParameter<CIEC_ANY_VARIANT> pa_out_struct) {
        COutputGuard guard_pa_out_struct(pa_out_struct);
        var_in_struct = pa_in_struct;
        var_member = pa_member;
        var_element_value = pa_element_value;
        receiveInputEvent(scmEventREQID, nullptr);
        pa_out_struct->setValue(var_out_struct.unwrap());
      }

      void operator()(const CIEC_ANY &pa_in_struct,
                      const CIEC_STRING &pa_member,
                      const CIEC_ANY &pa_element_value,
                      COutputParameter<CIEC_ANY_VARIANT> pa_out_struct) {
        evt_REQ(pa_in_struct, pa_member, pa_element_value, pa_out_struct);
      }

    protected:
      void setInitialValues() override;
  };
} // namespace forte::eclipse4diac::convert
