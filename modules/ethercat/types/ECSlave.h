/*******************************************************************************
 * Copyright (c) 2026 Sichuan Qunyuan Technology Co., Ltd.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Sichuan Qunyuan Technology Co., Ltd. - initial API and implementation
 *******************************************************************************/

#pragma once

#include "ECBusAdapter.h"
#include "../structs/ECSlaveConfig.h"
#include "../slave/slave.h"
#include "forte/adapter.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/io/configFB/io_slave_multi_handler.h"
#include "forte/genfb.h"
#include <string>

namespace forte::eclipse4diac::io::ethercat {
  class EsiFileParser;

  class FORTE_ECSlave : public CGenFunctionBlock<forte::io::IOConfigHandlerFBMultiSlave>, public ECSlaveHandler::Delegate {
      DECLARE_GENERIC_FIRMWARE_FB(FORTE_ECSlave)

    public:
      FORTE_ECSlave(forte::StringId paInstanceNameId,
                    CFBContainer &paContainer,
                    ECSlaveHandler::SlaveType paSlaveType = ECSlaveHandler::SlaveType::ECSlave);
      ~FORTE_ECSlave() override;

      void onSlaveStatus(ECSlaveHandler::SlaveStatus paStatus, ECSlaveHandler::SlaveStatus paOldStatus) override;
      void onSlaveDestroy() override;

      CIEC_ECSlaveConfig &Config() {
        return var_Config;
      }

      size_t numInMappings() const {
        return mNumInPdus;
      }

      size_t numOutMappings() const {
        return mNumOutPdus;
      }

    protected:
      virtual bool createSlaveHandler() override;
      const char *init() override;
      void deInit() override;
      void initHandles() override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

      size_t getGenDIOffset() override {
        return 2;
      }

      size_t getGenDOOffset() override {
        return 2;
      }

      void createGenInputData() override;

      CIEC_ANY *getDI(TPortId paIndex) override;
      CIEC_ANY *getDO(TPortId paIndex) override;

      CEventConnection *getEOConUnchecked(TPortId paIndex) override;
      CDataConnection **getDIConUnchecked(TPortId paIndex) override;
      CDataConnection *getDOConUnchecked(TPortId paIndex) override;

      forte::IPlugPin *getPlugPinUnchecked(size_t paIndex) override;
      forte::ISocketPin *getSocketPinUnchecked(size_t paIndex) override;

      CIEC_BOOL var_QI;
      CIEC_ECSlaveConfig var_Config;
      CIEC_BOOL var_QO;
      CIEC_WSTRING var_STATUS;

      CEventConnection conn_MAPO;
      CEventConnection conn_IND;

      CDataConnection *conn_QI;
      CDataConnection *conn_Config;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_WSTRING> conn_STATUS;

      forte::CSocketPin<FORTE_ECBusAdapter_Socket> var_BusAdapterIn;
      forte::CPlugPin<FORTE_ECBusAdapter_Plug> var_BusAdapterOut;

      std::unique_ptr<CIEC_ANY *[]> mGenDIs;

    protected:
      static const TForteUInt8 scmSlaveConfigurationIO[];
      static const TForteUInt8 scmSlaveConfigurationIONum;

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;
      void configureDIDOs(const char *paDIConfigString, const char *paDOConfigString, SFBInterfaceSpec &paInterfaceSpec);

      std::vector<StringId> mDiNames;
      std::vector<StringId> mDoNames;

      size_t mNumInPdus{};
      size_t mNumOutPdus{};
      std::string mLastError;

    private:
      friend class EsiFileParser;
  };

} // namespace forte::eclipse4diac::io::ethercat
