/*******************************************************************************
 * Copyright (c) 2017 - 2018 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "io_slave_multi.h"

using namespace forte::core::IO;

const char * const IOConfigFBMultiSlave::scmOK = "OK";
const char * const IOConfigFBMultiSlave::scmStopped = "Stopped";
const char * const IOConfigFBMultiSlave::scmMasterNotFound = "Master not found";
const char * const IOConfigFBMultiSlave::scmNotFound = "Module not found";
const char * const IOConfigFBMultiSlave::scmIncorrectType = "Module type is incorrect";

IOConfigFBMultiSlave::IOConfigFBMultiSlave(const TForteUInt8* const paSlaveConfigurationIO,
    const TForteUInt8 paSlaveConfigurationIO_num, int type,
    CResource *pa_poSrcRes,
    const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    IOConfigFBBase(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId, pa_acFBConnData,
        pa_acFBVarsData), master(0), index(-1), type(type), initialized(false), mSlaveConfigurationIO(
        paSlaveConfigurationIO), mSlaveConfigurationIO_num(
        paSlaveConfigurationIO_num), mSlaveConfigurationIO_isDefault(
        new bool[paSlaveConfigurationIO_num]()) {
  for (int i = 0; i < mSlaveConfigurationIO_num; i++)
    mSlaveConfigurationIO_isDefault[i] = false;
}

IOConfigFBMultiSlave::~IOConfigFBMultiSlave() {
  delete[] mSlaveConfigurationIO_isDefault;
}

void IOConfigFBMultiSlave::executeEvent(int pa_nEIID) {
  if (BusAdapterIn().INIT() == pa_nEIID) {
    if (BusAdapterIn().QI() == true) {
      // Handle initialization event
      const char* const error = handleInitEvent();
      QO() = initialized = error == 0;

      if (initialized) {
        STATUS() = scmOK;
      } else {
        STATUS() = error;
      }

      if (BusAdapterOut().getPeer() != 0) {
        // Initialize next slave
        BusAdapterOut().QI() = BusAdapterIn().QI();
        BusAdapterOut().Index() = (TForteUInt16) (BusAdapterIn().Index() + 1);
        BusAdapterOut().MasterId() = BusAdapterIn().MasterId();

        for (int i = 0; i < BusAdapterIn().mSlaveConfigurationIO_num; i++) {
          TIEC_ANYPtr ptr = BusAdapterIn().getSlaveConfig(i);
          switch (ptr->getDataTypeID()) {
          case CIEC_ANY::e_UINT:
            *static_cast<CIEC_UINT*>(BusAdapterOut().getSlaveConfig(i)) =
                *static_cast<CIEC_UINT*>(ptr);
            break;
          default:
            DEVLOG_WARNING(
                "[IOConfigFBMultiSlave] Unable to handle data type %d. Skip adapter configuration\n",
                ptr->getDataTypeID());
            continue;
          }
        }

        sendAdapterEvent(scm_nBusAdapterOutAdpNum, IOConfigFBMultiAdapter::scm_nEventINITID);
        sendOutputEvent(scm_nEventINDID);
      } else {
        // Send confirmation of init
        BusAdapterIn().QO() = QO();
        sendAdapterEvent(scm_nBusAdapterInAdpNum, IOConfigFBMultiAdapter::scm_nEventINITOID);
      }
    } else {
      deInit();

      QO() = false;
      STATUS() = scmStopped;

      if (BusAdapterOut().getPeer() != 0) {
        // DeInit next slave
        BusAdapterOut().QI() = BusAdapterIn().QI();

        sendAdapterEvent(scm_nBusAdapterOutAdpNum, IOConfigFBMultiAdapter::scm_nEventINITID);
        sendOutputEvent(scm_nEventINDID);
      } else {
        // Send confirmation of deInit
        BusAdapterIn().QO() = QO();
        sendAdapterEvent(scm_nBusAdapterInAdpNum, IOConfigFBMultiAdapter::scm_nEventINITOID);
      }
    }
  } else if (BusAdapterOut().INITO() == pa_nEIID) {
    // Forward confirmation of initialization
    BusAdapterIn().QO() = BusAdapterOut().QO() && QO();
    sendAdapterEvent(scm_nBusAdapterInAdpNum, IOConfigFBMultiAdapter::scm_nEventINITOID);
  }

  switch (pa_nEIID) {
  case scm_nEventMAPID:
    if (initialized) {
      // Drop all existing handles
      getController().dropSlaveHandles(index);

      if (true == QI())
        initHandles();

      QO() = true;
    } else {
      QO() = false;
    }

    sendOutputEvent(scm_nEventMAPOID);
    break;
  }
}

void IOConfigFBMultiSlave::initHandle(
    IODeviceMultiController::HandleDescriptor *handleDescriptor) {
  master->initHandle(handleDescriptor);
}

const char* IOConfigFBMultiSlave::handleInitEvent() {
  // Get master by id
  master = IOConfigFBMultiMaster::getMasterById(BusAdapterIn().MasterId());
  if (master == 0) {
    return scmMasterNotFound;
  }
  index = BusAdapterIn().Index();

  // Default parameters
  for (int i = 0; i < mSlaveConfigurationIO_num; i++) {
    bool isSet = true;

    TIEC_ANYPtr ptr = getDI(mSlaveConfigurationIO[i]);
    switch (ptr->getDataTypeID()) {
    case CIEC_ANY::e_UINT:
      isSet = !!*static_cast<CIEC_UINT*>(ptr);
      break;
    default:
      DEVLOG_WARNING(
          "[IOConfigFBMultiSlave] Unable to handle data type %d. Skip slave configuration\n",
          ptr->getDataTypeID());
      continue;
    }

    if (!mSlaveConfigurationIO_isDefault[i] && !isSet) {
      mSlaveConfigurationIO_isDefault[i] = true;
    }

    if (mSlaveConfigurationIO_isDefault[i]) {
      switch (ptr->getDataTypeID()) {
      case CIEC_ANY::e_UINT:
        *static_cast<CIEC_UINT*>(ptr) =
            *static_cast<CIEC_UINT*>(BusAdapterIn().getSlaveConfig(i));
        break;
      default:
        break;
      }
    }
  }

  IODeviceMultiController& controller = getController();

  // Check if slave exists
  if (!controller.isSlaveAvailable(index)) {
    DEVLOG_DEBUG("[IOConfigFBMultiSlave] No slave found at position %d\n", index);
    return scmNotFound;
  }

  // Check if slave type is correct
  if (!controller.checkSlaveType(index, type)) {
    DEVLOG_DEBUG(
        "[IOConfigFBMultiSlave] Found slave with incorrect type at position %d\n",
        index);
    return scmIncorrectType;
  }

  // Perform slave initialization
  const char* const error = init();
  if (error != 0)
    return error;

  controller.dropSlaveHandles(index);
  if (true == QI())
    initHandles();

  DEVLOG_DEBUG("[IOConfigFBMultiSlave] Initialized slave at position %d\n", index);

  return 0;
}

