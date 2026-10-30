/*******************************************************************************
 * Copyright (c) 2016, 2025 Johannes Messmer (admin@jomess.com), fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#pragma once

#include "Slave.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/datatypes/forte_uint.h"

namespace forte::eclipse4diac::io::embrick {

  class FORTE_EBSlave2301 final : public EmbrickSlave {
      DECLARE_FIRMWARE_FB(FORTE_EBSlave2301)

    private:
      static const TEventID scmEventMAPID = 0;
      static const TEventID scmEventMAPOID = 0;
      static const TEventID scmEventINDID = 1;
      static const int scmBusAdapterInAdpNum = 0;
      static const int scmBusAdapterOutAdpNum = 1;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

      static const TForteUInt8 scmSlaveConfigurationIO[];
      static const TForteUInt8 scmSlaveConfigurationIONum;

      void initHandles() override;

    public:
      FORTE_EBSlave2301(forte::StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_QI;
      CIEC_WSTRING var_Relay_1;
      CIEC_WSTRING var_Relay_2;
      CIEC_WSTRING var_Relay_3;
      CIEC_WSTRING var_Relay_4;
      CIEC_WSTRING var_Relay_5;
      CIEC_WSTRING var_Relay_6;
      CIEC_UINT var_UpdateInterval;

      CIEC_BOOL var_QO;
      CIEC_WSTRING var_STATUS;

      CEventConnection conn_MAPO;
      CEventConnection conn_IND;

      CDataConnection *conn_QI;
      CDataConnection *conn_Relay_1;
      CDataConnection *conn_Relay_2;
      CDataConnection *conn_Relay_3;
      CDataConnection *conn_Relay_4;
      CDataConnection *conn_Relay_5;
      CDataConnection *conn_Relay_6;
      CDataConnection *conn_UpdateInterval;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_WSTRING> conn_STATUS;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;

      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };

} // namespace forte::eclipse4diac::io::embrick
