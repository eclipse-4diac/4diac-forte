/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
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

#include "Slave.h"
#include <io/mapper/io_mapper.h>

const char * const EmbrickSlave::scmSlow = "Slow";
const char * const EmbrickSlave::scmInterrupted = "Interrupted";
const char * const EmbrickSlave::scmError = "Error";
const char * const EmbrickSlave::scmUnknown = "Invalid status code";

EmbrickSlave::EmbrickSlave(const TForteUInt8* const paSlaveConfigurationIO, const TForteUInt8 paSlaveConfigurationIO_num, int paType, CResource *paSrcRes,
    const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
        forte::core::io::IOConfigFBMultiSlave(paSlaveConfigurationIO, paSlaveConfigurationIO_num, paType, paSrcRes, paInterfaceSpec, paInstanceNameId,
          paFBConnData, paFBVarsData), mSlave(0) {
}

EmbrickSlave::~EmbrickSlave() {
  deInit();
}

const char* EmbrickSlave::init() {
  CCriticalRegion criticalRegion(mSlaveMutex);

  EmbrickBusHandler &bus = *static_cast<EmbrickBusHandler*>(&getController());

  mSlave = bus.getSlave(mIndex);
  mSlave->mDelegate = this;

  EmbrickSlaveHandler::Config config;
  config.mUpdateInterval = UpdateInterval();
  mSlave->setConfig(config);

  return 0;
}

void EmbrickSlave::deInit() {
  CCriticalRegion criticalRegion(mSlaveMutex);

  if(mSlave != 0) {
    mSlave->mDelegate = 0;
    mSlave = 0;
  }

}

void EmbrickSlave::onSlaveStatus(EmbrickSlaveHandler::SlaveStatus paStatus, EmbrickSlaveHandler::SlaveStatus) {
  switch(paStatus){
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

}

