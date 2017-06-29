/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "io_slave.h"

namespace IO {
namespace ConfigurationFB {
namespace Multi {

const char * const Slave::scmOK = "OK";
const char * const Slave::scmStopped = "Stopped";
const char * const Slave::scmMasterNotFound = "Master not found";
const char * const Slave::scmNotFound = "Module not found";
const char * const Slave::scmIncorrectType = "Module type is incorrect";

Slave::Slave(const TForteUInt8* const scm_slaveConfigurationIO,
    const TForteUInt8 scm_slaveConfigurationIO_num, int type,
    CResource *pa_poSrcRes,
    const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    Base(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId, pa_acFBConnData,
        pa_acFBVarsData), master(0), index(-1), type(type), initialized(false), scm_slaveConfigurationIO(
        scm_slaveConfigurationIO), scm_slaveConfigurationIO_num(
        scm_slaveConfigurationIO_num), scm_slaveConfigurationIO_isDefault(
        new bool[scm_slaveConfigurationIO_num]()) {
  for (int i = 0; i < scm_slaveConfigurationIO_num; i++)
    scm_slaveConfigurationIO_isDefault[i] = false;
}

Slave::~Slave() {
}

void Slave::executeEvent(int pa_nEIID) {
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
        BusAdapterOut().Index() = BusAdapterIn().Index() + 1;
        BusAdapterOut().MasterId() = BusAdapterIn().MasterId();

        for (int i = 0; i < BusAdapterIn().scm_slaveConfigurationIO_num; i++) {
          TIEC_ANYPtr ptr = BusAdapterIn().getSlaveConfig(i);
          switch (ptr->getDataTypeID()) {
          case CIEC_ANY::e_UINT:
            *static_cast<CIEC_UINT*>(BusAdapterOut().getSlaveConfig(i)) =
                *static_cast<CIEC_UINT*>(ptr);
            break;
          default:
            DEVLOG_WARNING(
                "[IO:ConfigFB:Multi:Slave] Unable to handle data type %d. Skip adapter configuration\n",
                ptr->getDataTypeID());
            continue;
          }
        }

        sendAdapterEvent(scm_nBusAdapterOutAdpNum, Adapter::scm_nEventINITID);
        sendOutputEvent(scm_nEventINDID);
      } else {
        // Send confirmation of init
        BusAdapterIn().QO() = QO();
        sendAdapterEvent(scm_nBusAdapterInAdpNum, Adapter::scm_nEventINITOID);
      }
    } else {
      deInit();

      QO() = false;
      STATUS() = scmStopped;

      if (BusAdapterOut().getPeer() != 0) {
        // DeInit next slave
        BusAdapterOut().QI() = BusAdapterIn().QI();

        sendAdapterEvent(scm_nBusAdapterOutAdpNum, Adapter::scm_nEventINITID);
        sendOutputEvent(scm_nEventINDID);
      } else {
        // Send confirmation of deInit
        BusAdapterIn().QO() = QO();
        sendAdapterEvent(scm_nBusAdapterInAdpNum, Adapter::scm_nEventINITOID);
      }
    }
  } else if (BusAdapterOut().INITO() == pa_nEIID) {
    // Forward confirmation of initialization
    BusAdapterIn().QO() = BusAdapterOut().QO() && QO();
    sendAdapterEvent(scm_nBusAdapterInAdpNum, Adapter::scm_nEventINITOID);
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

void Slave::addHandle(CIEC_WSTRING const &id, Handle* handle) {
  if (Mapper::getInstance().registerHandle(id, handle))
    getController().addSlaveHandle(index, handle);
  else
    delete handle;
}

const char* const Slave::handleInitEvent() {
  // Get master by id
  master = Master::getMasterById(BusAdapterIn().MasterId());
  if (master == 0) {
    return scmMasterNotFound;
  }
  index = BusAdapterIn().Index();

  // Default parameters
  for (int i = 0; i < scm_slaveConfigurationIO_num; i++) {
    bool isSet = true;

    TIEC_ANYPtr ptr = getDI(scm_slaveConfigurationIO[i]);
    switch (ptr->getDataTypeID()) {
    case CIEC_ANY::e_UINT:
      isSet = !!*static_cast<CIEC_UINT*>(ptr);
      break;
    default:
      DEVLOG_WARNING(
          "[IO:ConfigFB:Multi:Slave] Unable to handle data type %d. Skip slave configuration\n",
          ptr->getDataTypeID());
      continue;
    }

    if (!scm_slaveConfigurationIO_isDefault[i] && !isSet) {
      scm_slaveConfigurationIO_isDefault[i] = true;
    }

    if (scm_slaveConfigurationIO_isDefault[i]) {
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

  Device::MultiController& controller = getController();

  // Check if slave exists
  if (!controller.isSlaveAvailable(index)) {
    DEVLOG_DEBUG("[IO:ConfigFB:Slave] No slave found at position %d\n", index);
    return scmNotFound;
  }

  // Check if slave type is correct
  if (!controller.checkSlaveType(index, type)) {
    DEVLOG_DEBUG(
        "[IO:ConfigFB:Slave] Found slave with incorrect type at position %d\n",
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

  DEVLOG_DEBUG("[IO:ConfigFB:Slave] Initialized slave at position %d\n", index);

  return 0;
}

} /* namespace Multi */
} /* namespace ConfigurationFB */
} /* namespace IO */
