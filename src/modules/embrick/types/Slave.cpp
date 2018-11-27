/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "Slave.h"
#include <io/mapper/io_mapper.h>

const char * const EmbrickSlave::scmSlow = "Slow";
const char * const EmbrickSlave::scmInterrupted = "Interrupted";
const char * const EmbrickSlave::scmError = "Error";
const char * const EmbrickSlave::scmUnknown = "Invalid status code";

EmbrickSlave::EmbrickSlave(const TForteUInt8* const paSlaveConfigurationIO,
    const TForteUInt8 paSlaveConfigurationIO_num, int type,
    CResource *pa_poSrcRes,
    const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    forte::core::IO::IOConfigFBMultiSlave(
        paSlaveConfigurationIO,
        paSlaveConfigurationIO_num, type, pa_poSrcRes, pa_pstInterfaceSpec,
        pa_nInstanceNameId, pa_acFBConnData, pa_acFBVarsData), slave(0) {
}

EmbrickSlave::~EmbrickSlave() {
  deInit();
}

const char* EmbrickSlave::init() {
  CCriticalRegion criticalRegion(slaveMutex);

  EmbrickBusHandler &bus = *static_cast<EmbrickBusHandler*>(&getController());

  slave = bus.getSlave(index);
  slave->delegate = this;

  EmbrickSlaveHandler::Config config;
  config.UpdateInterval = UpdateInterval();
  slave->setConfig(config);


  return 0;
}

void EmbrickSlave::deInit() {
  CCriticalRegion criticalRegion(slaveMutex);

  if (slave != 0) {
    slave->delegate = 0;
    slave = 0;
  }

}

void EmbrickSlave::onSlaveStatus(EmbrickSlaveHandler::SlaveStatus status, EmbrickSlaveHandler::SlaveStatus) {
  switch (status) {
  case EmbrickSlaveHandler::OK:
    STATUS() = scmOK;
    break;
  case EmbrickSlaveHandler::Slow:
    STATUS() = scmSlow;
    break;
  case EmbrickSlaveHandler::Interrupted:
    STATUS() = scmInterrupted;
    break;
  case EmbrickSlaveHandler::Error:
    STATUS() = scmError;
    break;
  default:
    STATUS() = scmUnknown;
    break;
  }

  sendOutputEvent(scm_nEventINDID);
}

void EmbrickSlave::onSlaveDestroy() {
  deInit();

  QO() = false;
  STATUS() = scmError;

//  sendOutputEvent(scm_nEventINDID);
}



