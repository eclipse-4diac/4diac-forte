/*******************************************************************************
 * Copyright (c) 2017 - 2018 fortiss GmbH
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

#ifndef SRC_CORE_IO_CONFIGFB_SLAVE_MULTI_H_
#define SRC_CORE_IO_CONFIGFB_SLAVE_MULTI_H_

#include "io_base.h"
#include <io/device/io_controller_multi.h>
#include "io_adapter_multi.h"
#include "io_master_multi.h"

namespace forte {
  namespace core {
    namespace io {

#define FUNCTION_BLOCK_CTOR_FOR_IO_MULTI_SLAVE(fbclass, fbBaseClass, type) \
 fbclass(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) : \
 fbBaseClass( (const TForteUInt8* const) &scmSlaveConfigurationIO, scmSlaveConfigurationIONum, type, paSrcRes, &scmFBInterfaceSpec, paInstanceNameId)

      class IOConfigFBMultiSlave : public IOConfigFBBase {
        public:
          IOConfigFBMultiSlave(const TForteUInt8* const paSlaveConfigurationIO, const TForteUInt8 paSlaveConfigurationIONum, int paType, CResource *paSrcRes,
              const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId);
          ~IOConfigFBMultiSlave() override;

        protected:
          CIEC_BOOL &QI() {
            return *static_cast<CIEC_BOOL*>(getDI(0));
          }

          CIEC_BOOL &QO() {
            return *static_cast<CIEC_BOOL*>(getDO(0));
          }

          CIEC_WSTRING &STATUS() {
            return *static_cast<CIEC_WSTRING*>(getDO(1));
          }

          static const TEventID scmEventMAPID = 0;

          static const TEventID scmEventMAPOID = 0;
          static const TEventID scmEventINDID = 1;

          IOConfigFBMultiAdapter& BusAdapterOut() {
            return (*static_cast<IOConfigFBMultiAdapter*>(mAdapters[0]));
          }

          static const int scmBusAdapterOutAdpNum = 0;
          IOConfigFBMultiAdapter& BusAdapterIn() {
            return (*static_cast<IOConfigFBMultiAdapter*>(mAdapters[1]));
          }

          static const int scmBusAdapterInAdpNum = 1;

          void executeEvent(TEventID paEIID, CEventChainExecutionThread * const paECET) override;

          IODeviceMultiController& getController() {
            return (*static_cast<IODeviceMultiController*>(mMaster->getDeviceController()));
          }

          virtual const char* init() {
            return nullptr;
          }

          virtual void deInit() {
            //do nothing
          }

          virtual void initHandles() = 0;

          void initHandle(IODeviceController::HandleDescriptor *paHandleDescriptor);

          static const CIEC_WSTRING scmOK;
          static const char* const scmMasterNotFound;

          int mIndex;

          const TForteUInt8* mSlaveConfigurationIO;

        private:

          IOConfigFBMultiMaster* mMaster;

          int mType;

          bool mInitialized;

          TForteUInt8 mSlaveConfigurationIONum;
          bool* mSlaveConfigurationIOIsDefault;

          const char* handleInitEvent();

          static const CIEC_WSTRING scmStopped;
          static const char* const scmNotFound;
          static const char* const scmIncorrectType;

      };

    } //namespace IO
  } //namepsace core
} //namespace forte

#endif /* SRC_CORE_IO_CONFIGFB_SLAVE_MULTI_H_ */
