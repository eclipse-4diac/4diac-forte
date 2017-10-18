/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "Slave.h"
#include <io/mapper/io_mapper.h>

namespace EmBrick {
namespace FunctionBlocks {

const char * const Slave::scmSlow = "Slow";
const char * const Slave::scmInterrupted = "Interrupted";
const char * const Slave::scmError = "Error";
const char * const Slave::scmUnknown = "Invalid status code";

Slave::Slave(const TForteUInt8* const scm_slaveConfigurationIO,
    const TForteUInt8 scm_slaveConfigurationIO_num, int type,
    CResource *pa_poSrcRes,
    const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    IO::ConfigurationFB::Multi::Slave(
        scm_slaveConfigurationIO,
        scm_slaveConfigurationIO_num, type, pa_poSrcRes, pa_pstInterfaceSpec,
        pa_nInstanceNameId, pa_acFBConnData, pa_acFBVarsData), slave(0) {
}

Slave::~Slave() {
  deInit();
}

const char* Slave::init() {
  slaveMutex.lock();

  Handlers::Bus &bus = *static_cast<Handlers::Bus*>(&getController());

  slave = bus.getSlave(index);
  slave->delegate = this;

  Handlers::Slave::Config config;
  config.UpdateInterval = UpdateInterval();
  slave->setConfig(config);

  slaveMutex.unlock();

  return 0;
}

void Slave::deInit() {
  slaveMutex.lock();

  if (slave != 0) {
    slave->delegate = 0;
    slave = 0;
  }

  slaveMutex.unlock();
}

void Slave::onSlaveStatus(Handlers::SlaveStatus status, Handlers::SlaveStatus) {
  switch (status) {
  case Handlers::OK:
    STATUS() = scmOK;
    break;
  case Handlers::Slow:
    STATUS() = scmSlow;
    break;
  case Handlers::Interrupted:
    STATUS() = scmInterrupted;
    break;
  case Handlers::Error:
    STATUS() = scmError;
    break;
  default:
    STATUS() = scmUnknown;
    break;
  }

  sendOutputEvent(scm_nEventINDID);
}

void Slave::onSlaveDestroy() {
  deInit();

  QO() = false;
  STATUS() = scmError;

//  sendOutputEvent(scm_nEventINDID);
}

} /* namespace FunctionsBlocks */
} /* namespace EmBrick */
