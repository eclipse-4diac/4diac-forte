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

#include "io_slave_multi.h"

using namespace forte::core::io;

const char * const IOConfigFBMultiSlave::scmOK = "OK";
const char * const IOConfigFBMultiSlave::scmStopped = "Stopped";
const char * const IOConfigFBMultiSlave::scmMasterNotFound = "Master not found";
const char * const IOConfigFBMultiSlave::scmNotFound = "Module not found";
const char * const IOConfigFBMultiSlave::scmIncorrectType = "Module type is incorrect";

IOConfigFBMultiSlave::IOConfigFBMultiSlave(const TForteUInt8* const paSlaveConfigurationIO, const TForteUInt8 paSlaveConfigurationIONum, int paType,
    CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData,
    TForteByte *paFBVarsData) :
    IOConfigFBBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData), mIndex(-1), mSlaveConfigurationIO(paSlaveConfigurationIO),
        mMaster(0), mType(paType), mInitialized(false), mSlaveConfigurationIONum(paSlaveConfigurationIONum),
        mSlaveConfigurationIOIsDefault(new bool[paSlaveConfigurationIONum]()) {
  for(int i = 0; i < mSlaveConfigurationIONum; i++) {
    mSlaveConfigurationIOIsDefault[i] = false;
  }
}

IOConfigFBMultiSlave::~IOConfigFBMultiSlave() {
  delete[] mSlaveConfigurationIOIsDefault;
}

void IOConfigFBMultiSlave::executeEvent(int paEIID) {
  if(BusAdapterIn().INIT() == paEIID) {
    if(BusAdapterIn().QI() == true) {
      // Handle initialization event
      const char* const error = handleInitEvent();
      QO() = mInitialized = error == 0;

      if(mInitialized) {
        STATUS() = scmOK;
      } else {
        STATUS() = error;
      }

      if(BusAdapterOut().getPeer() != 0) {
        // Initialize next slave
        BusAdapterOut().QI() = BusAdapterIn().QI();
        BusAdapterOut().Index() = (TForteUInt16) (BusAdapterIn().Index() + 1);
        BusAdapterOut().MasterId() = BusAdapterIn().MasterId();

        for(int i = 0; i < BusAdapterIn().mSlaveConfigurationIONum; i++) {
          TIEC_ANYPtr ptr = BusAdapterIn().getSlaveConfig(i);
          if(CIEC_ANY::e_UINT == ptr->getDataTypeID()) {
            *static_cast<CIEC_UINT*>(BusAdapterOut().getSlaveConfig(i)) = *static_cast<CIEC_UINT*>(ptr);
          } else {
            DEVLOG_WARNING("[IOConfigFBMultiSlave] Unable to handle data type %d. Skip adapter configuration\n", ptr->getDataTypeID());
          }
        }

        sendAdapterEvent(scm_nBusAdapterOutAdpNum, IOConfigFBMultiAdapter::scmEventINITID);
        sendOutputEvent(scm_nEventINDID);
      } else {
        // Send confirmation of init
        BusAdapterIn().QO() = QO();
        sendAdapterEvent(scm_nBusAdapterInAdpNum, IOConfigFBMultiAdapter::scmEventINITOID);
      }
    } else {
      deInit();

      QO() = false;
      STATUS() = scmStopped;

      if(BusAdapterOut().getPeer() != 0) {
        // DeInit next slave
        BusAdapterOut().QI() = BusAdapterIn().QI();

        sendAdapterEvent(scm_nBusAdapterOutAdpNum, IOConfigFBMultiAdapter::scmEventINITID);
        sendOutputEvent(scm_nEventINDID);
      } else {
        // Send confirmation of deInit
        BusAdapterIn().QO() = QO();
        sendAdapterEvent(scm_nBusAdapterInAdpNum, IOConfigFBMultiAdapter::scmEventINITOID);
      }
    }
  } else if(BusAdapterOut().INITO() == paEIID) {
    // Forward confirmation of initialization
    BusAdapterIn().QO() = BusAdapterOut().QO() && QO();
    sendAdapterEvent(scm_nBusAdapterInAdpNum, IOConfigFBMultiAdapter::scmEventINITOID);
  }

  if(scm_nEventMAPID == paEIID) {
    if(mInitialized) {
      // Drop all existing handles
      getController().dropSlaveHandles(mIndex);

      if(true == QI()) {
        initHandles();
      }

      QO() = true;
    } else {
      QO() = false;
    }

    sendOutputEvent(scm_nEventMAPOID);
  }
}

void IOConfigFBMultiSlave::initHandle(IODeviceController::HandleDescriptor *paHandleDescriptor) {
  mMaster->initHandle(paHandleDescriptor);
}

const char* IOConfigFBMultiSlave::handleInitEvent() {
  // Get master by id
  mMaster = IOConfigFBMultiMaster::getMasterById(BusAdapterIn().MasterId());
  if(0 == mMaster) {
    return scmMasterNotFound;
  }
  mIndex = BusAdapterIn().Index();

  // Default parameters
  for(int i = 0; i < mSlaveConfigurationIONum; i++) {
    bool isSet = true;

    TIEC_ANYPtr ptr = getDI(mSlaveConfigurationIO[i]);
    if(CIEC_ANY::e_UINT == ptr->getDataTypeID()) {
      isSet = !!*static_cast<CIEC_UINT*>(ptr);
    } else {
      DEVLOG_WARNING("[IOConfigFBMultiSlave] Unable to handle data type %d. Skip slave configuration\n", ptr->getDataTypeID());
      continue;
    }

    if(!mSlaveConfigurationIOIsDefault[i] && !isSet) {
      mSlaveConfigurationIOIsDefault[i] = true;
    }

    if(mSlaveConfigurationIOIsDefault[i] && CIEC_ANY::e_UINT == ptr->getDataTypeID()) {
      *static_cast<CIEC_UINT*>(ptr) = *static_cast<CIEC_UINT*>(BusAdapterIn().getSlaveConfig(i));
    }
  }

  IODeviceMultiController& controller = getController();

  // Check if slave exists
  if(!controller.isSlaveAvailable(mIndex)) {
    DEVLOG_DEBUG("[IOConfigFBMultiSlave] No slave found at position %d\n", mIndex);
    return scmNotFound;
  }

  // Check if slave type is correct
  if(!controller.checkSlaveType(mIndex, mType)) {
    DEVLOG_DEBUG("[IOConfigFBMultiSlave] Found slave with incorrect type at position %d\n", mIndex);
    return scmIncorrectType;
  }

  // Perform slave initialization
  const char* const error = init();
  if(error != 0) {
    return error;
  }
  controller.dropSlaveHandles(mIndex);
  if(true == QI()) {
    initHandles();
  }

  DEVLOG_DEBUG("[IOConfigFBMultiSlave] Initialized slave at position %d\n", mIndex);

  return 0;
}

