/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH, Sichuan Qunyuan Technology Co., Ltd.
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *   Zijun Tang - add createSlaveHandler() hook and generic FB constructor
 *******************************************************************************/

#ifndef SRC_CORE_IO_CONFIGFB_SLAVE_MULTI_H_
#define SRC_CORE_IO_CONFIGFB_SLAVE_MULTI_H_

#include "forte/io/configFB/io_base.h"
#include "forte/io/device/io_controller_multi.h"
#include "forte/io/configFB/io_adapter_multi.h"
#include "forte/io/configFB/io_master_multi.h"

namespace forte::io {

  class IOConfigFBMultiSlave : public IOConfigFBBase {
    public:
      IOConfigFBMultiSlave(const TForteUInt8 *const paSlaveConfigurationIO,
                           const TForteUInt8 paSlaveConfigurationIONum,
                           int paType,
                           CFBContainer &paContainer,
                           const SFBInterfaceSpec &paInterfaceSpec,
                           const StringId paInstanceNameId);

      IOConfigFBMultiSlave(CFBContainer &paContainer,
                           const SFBInterfaceSpec &paInterfaceSpec,
                           forte::StringId paInstanceNameId,
                           const TForteUInt8 *const paSlaveConfigurationIO,
                           TForteUInt8 paSlaveConfigurationIONum,
                           int paType);

      ~IOConfigFBMultiSlave() override;

    protected:
      CIEC_BOOL &QI() {
        return *static_cast<CIEC_BOOL *>(getDI(0));
      }

      CIEC_BOOL &QO() {
        return *static_cast<CIEC_BOOL *>(getDO(0));
      }

      CIEC_WSTRING &STATUS() {
        return *static_cast<CIEC_WSTRING *>(getDO(1));
      }

      static const TEventID scmEventMAPID = 0;

      static const TEventID scmEventMAPOID = 0;
      static const TEventID scmEventINDID = 1;

      IOConfigFBMultiAdapter &BusAdapterOut() {
        return (*static_cast<IOConfigFBMultiAdapter *>(getPlugPinUnchecked(0)->getAdapterBlock()));
      }

      IOConfigFBMultiAdapter &BusAdapterIn() {
        return (*static_cast<IOConfigFBMultiAdapter *>(getSocketPinUnchecked(0)->getAdapterBlock()));
      }

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      IODeviceMultiController &getController() {
        return (*static_cast<IODeviceMultiController *>(mMaster->getDeviceController()));
      }

      virtual const char *init() {
        return nullptr;
      }

      virtual void deInit() {
        // do nothing
      }

      virtual void initHandles() = 0;

      virtual bool createSlaveHandler() {
        return true;
      }

      void initHandle(IODeviceController::HandleDescriptor &paHandleDescriptor);

      static const CIEC_WSTRING scmOK;
      static const char *const scmMasterNotFound;

      size_t mIndex;

      const TForteUInt8 *mSlaveConfigurationIO;

    private:
      IOConfigFBMultiMaster *mMaster;

      int mType;

      bool mInitialized;

      TForteUInt8 mSlaveConfigurationIONum;
      bool *mSlaveConfigurationIOIsDefault;

      const char *handleInitEvent();

      static const CIEC_WSTRING scmStopped;
      static const char *const scmNotFound;
      static const char *const scmIncorrectType;
      static const char *const scmCreateSlaveHandlerFailed;
  };

} // namespace forte::io

#endif /* SRC_CORE_IO_CONFIGFB_SLAVE_MULTI_H_ */
