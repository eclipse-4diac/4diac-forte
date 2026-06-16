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

#include "forte/io/configFB/io_slave_multi_handler.h"
#include "forte/iec61131_functions/func_ADD.h"
#include "forte/iec61131_functions/func_AND.h"

namespace forte::io {
  const CIEC_WSTRING IOConfigHandlerFBMultiSlave::scmOK("OK");
  const CIEC_WSTRING IOConfigHandlerFBMultiSlave::scmStopped("Stopped");
  const char *const IOConfigHandlerFBMultiSlave::scmMasterNotFound("Master not found");
  const char *const IOConfigHandlerFBMultiSlave::scmNotFound("Module not found");
  const char *const IOConfigHandlerFBMultiSlave::scmIncorrectType("Module type is incorrect");

  IOConfigHandlerFBMultiSlave::IOConfigHandlerFBMultiSlave(const TForteUInt8 *const paSlaveConfigurationIO,
                                                           const TForteUInt8 paSlaveConfigurationIONum,
                                                           int paType,
                                                           CFBContainer &paContainer,
                                                           const SFBInterfaceSpec &paInterfaceSpec,
                                                           const StringId paInstanceNameId) :
                                                           IOConfigFBBase(paContainer, paInterfaceSpec, paInstanceNameId),
      mIndex(static_cast<size_t>(-1)),
      mSlaveConfigurationIO(paSlaveConfigurationIO),
      mMaster(nullptr),
      mType(paType),
      mInitialized(false),
      mSlaveConfigurationIONum(paSlaveConfigurationIONum),
      mSlaveConfigurationIOIsDefault(new bool[paSlaveConfigurationIONum]()) {
    for (int i = 0; i < mSlaveConfigurationIONum; i++) {
      mSlaveConfigurationIOIsDefault[i] = false;
    }
  }

  IOConfigHandlerFBMultiSlave::IOConfigHandlerFBMultiSlave(CFBContainer &paContainer,
                                                           const SFBInterfaceSpec &paInterfaceSpec,
                                                           forte::StringId paInstanceNameId,
                                                           const TForteUInt8 *const paSlaveConfigurationIO,
                                                           TForteUInt8 paSlaveConfigurationIONum,
                                                           int paType) : 
                                IOConfigHandlerFBMultiSlave(paSlaveConfigurationIO,
                                                            paSlaveConfigurationIONum,
                                                            paType,
                                                            paContainer,
                                                            paInterfaceSpec,
                                                            paInstanceNameId) {
  }

  IOConfigHandlerFBMultiSlave::~IOConfigHandlerFBMultiSlave() {
    delete[] mSlaveConfigurationIOIsDefault;
  }

  void IOConfigHandlerFBMultiSlave::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (BusAdapterIn().INIT() == paEIID) {    
      if (BusAdapterIn().var_QI == true) {
        // Handle initialization event
        const char *const error = handleInitEvent();
        QO() = CIEC_BOOL(mInitialized = error == nullptr);

        if (mInitialized) {
          STATUS() = scmOK;
        } else {
          STATUS() = CIEC_WSTRING(error);
        }

        if (BusAdapterOut().getPeer() != nullptr) {
          // Initialize next slave
          BusAdapterOut().var_QI = BusAdapterIn().var_QI;
          BusAdapterOut().var_Index = func_ADD(BusAdapterIn().var_Index, CIEC_UINT(1));
          BusAdapterOut().var_MasterId = BusAdapterIn().var_MasterId;

          for (auto it : BusAdapterIn().cmSlaveConfigurationIO) {
            CIEC_ANY *inConfigPin = BusAdapterIn().getDeviceConfigPin(it);
            if (inConfigPin != nullptr) {
              BusAdapterOut().getDeviceConfigPin(it)->setValue(*inConfigPin);
            } else {
              DEVLOG_WARNING("[IOConfigHandlerFBMultiSlave] Unable to get device config pin #%d. Skip adapter configuration\n",it);
            }
          }

          sendAdapterEvent(BusAdapterOut(), IOConfigFBMultiAdapter::scmEventINITID, paECET);
          sendOutputEvent(scmEventINDID, paECET);
        } else {
          // Send confirmation of init
          BusAdapterIn().var_QO = QO();
          sendAdapterEvent(BusAdapterIn(), IOConfigFBMultiAdapter::scmEventINITOID, paECET);
        }
      } else {
        deInit();

        QO() = false_BOOL;
        STATUS() = scmStopped;

        if (BusAdapterOut().getPeer() != nullptr) {
          // DeInit next slave
          BusAdapterOut().var_QI = BusAdapterIn().var_QI;

          sendAdapterEvent(BusAdapterOut(), IOConfigFBMultiAdapter::scmEventINITID, paECET);
          sendOutputEvent(scmEventINDID, paECET);
        } else {
          // Send confirmation of deInit
          BusAdapterIn().var_QO = QO();
          sendAdapterEvent(BusAdapterIn(), IOConfigFBMultiAdapter::scmEventINITOID, paECET);
        }
      }
    } else if (BusAdapterOut().INITO() == paEIID) {
      // Forward confirmation of initialization
      BusAdapterIn().var_QO = func_AND(BusAdapterOut().var_QO, QO());
      sendAdapterEvent(BusAdapterIn(), IOConfigFBMultiAdapter::scmEventINITOID, paECET);
    }

    if (scmEventMAPID == paEIID) {
      if (mInitialized) {
        // Drop all existing handles
        getController().dropSlaveHandles(mIndex);

        if (true == QI()) {
          initHandles();
        }

        QO() = true_BOOL;
      } else {
        QO() = false_BOOL;
      }

      sendOutputEvent(scmEventMAPOID, paECET);
    }
  }

  void IOConfigHandlerFBMultiSlave::initHandle(IODeviceController::HandleDescriptor &paHandleDescriptor) {
    mMaster->initHandle(paHandleDescriptor);
  }

  const char *IOConfigHandlerFBMultiSlave::handleInitEvent() {
    // Get master by id
    mMaster = IOConfigFBMultiMaster::getMasterById(static_cast<CIEC_UINT::TValueType>(BusAdapterIn().var_MasterId));
    if (nullptr == mMaster) {
      return scmMasterNotFound;
    }
    mIndex = static_cast<CIEC_UINT::TValueType>(BusAdapterIn().var_Index);

    // Default parameters
    for (size_t i = 0; i < mSlaveConfigurationIONum; i++) {
      bool isSet = true;

      CIEC_ANY *ptr = getDI(mSlaveConfigurationIO[i]);
      if (CIEC_ANY::e_UINT == ptr->getDataTypeID()) {
        isSet = static_cast<CIEC_UINT::TValueType>(*static_cast<CIEC_UINT *>(ptr)) > 0 ? true : false;
      } else {
        DEVLOG_WARNING("[IOConfigHandlerFBMultiSlave] Unable to handle data type %d. Skip slave configuration\n",
                       ptr->getDataTypeID());
        continue;
      }

      if (!mSlaveConfigurationIOIsDefault[i] && !isSet) {
        mSlaveConfigurationIOIsDefault[i] = true;
      }

      if (mSlaveConfigurationIOIsDefault[i]) {
        ptr->setValue(*BusAdapterIn().getDeviceConfigPin(BusAdapterIn().cmSlaveConfigurationIO[i]));
      }
    }

    IODeviceMultiController &controller = getController();

    if(!createSlaveHandler()) {
      DEVLOG_ERROR("[IOConfigHandlerFBMultiSlave] Create Slave handler failed\n");
      return "[IOConfigHandlerFBMultiSlave] Create Slave handler failed";
    }

    // Perform slave initialization
    const char *const error = init();
    if (error != nullptr) {
      return error;
    }
    controller.dropSlaveHandles(mIndex);
    if (true == QI()) {
      initHandles();
    }

    DEVLOG_DEBUG("[IOConfigHandlerFBMultiSlave] Initialized slave at position %d\n", mIndex);

    return nullptr;
  }

}