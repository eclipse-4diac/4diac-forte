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
#include <io/mapper.h>

namespace EmBrick {
namespace FunctionBlocks {

const char * const Slave::scmOK = "OK";
const char * const Slave::scmSlow = "Slow";
const char * const Slave::scmInterrupted = "Interrupted";
const char * const Slave::scmError = "Error";
const char * const Slave::scmUnknown = "Invalid status code";
const char * const Slave::scmNotFound = "Module not found";
const char * const Slave::scmIncorrectType = "Module type is incorrect";

Slave::Slave(CResource *pa_poSrcRes,
    const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    CFunctionBlock(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId,
        pa_acFBConnData, pa_acFBVarsData), type(UnknownSlave), slave(0), ready(
        false) {
  UpdateIntervalDefault = false;
}

Slave::~Slave() {

}

void Slave::executeEvent(int pa_nEIID) {
  if (BusAdapterIn().INIT() == pa_nEIID) {
    // Init slave
    slaveMutex.lock();

    if (!UpdateIntervalDefault && !UpdateInterval())
      UpdateIntervalDefault = true;

    if (UpdateIntervalDefault)
      UpdateInterval() = BusAdapterIn().UpdateInterval();

    QO() = ready = init(BusAdapterIn().INDEX());
    slaveMutex.unlock();

    if (QO())
      DEVLOG_INFO("emBrick[SlaveFunctionBlock]: Init slave %d success.\n",
          (int ) BusAdapterIn().INDEX());
    else
      DEVLOG_ERROR("emBrick[SlaveFunctionBlock]: Init slave %d failed.\n",
          (int ) BusAdapterIn().INDEX());

    if (BusAdapterOut().getPeer() != 0) {
      // Init next slave
      BusAdapterOut().INDEX() = BusAdapterIn().INDEX() + 1;
      BusAdapterOut().UpdateInterval() = BusAdapterIn().UpdateInterval();
      sendAdapterEvent(scm_nBusAdapterOutAdpNum, BusAdapter::scm_nEventINITID);
    } else {
      // Send confirmation of init
      BusAdapterIn().QO() = QO();
      sendAdapterEvent(scm_nBusAdapterInAdpNum, BusAdapter::scm_nEventINITOID);
    }
  } else if (BusAdapterOut().INITO() == pa_nEIID) {
    // Forward confirmation of init
    BusAdapterIn().QO() = BusAdapterOut().QO() && QO();
    sendAdapterEvent(scm_nBusAdapterInAdpNum, BusAdapter::scm_nEventINITOID);
  }

  switch (pa_nEIID) {
  case scm_nEventMAPID:
    slaveMutex.lock();

    if (!ready)
      break;

    // Drop all existing handles
    dropHandles();

    if (true == QI())
      initHandles();

    slaveMutex.unlock();

    QO() = QI();
    sendOutputEvent(scm_nEventMAPOID);
    break;
  }
}

bool Slave::init(int index) {
  slave = 0;

  BusHandler &bus = BusHandler::getInstance();

  slave = bus.getSlave(index);
  if (!slave) {
    STATUS() = scmNotFound;
    return false;
  }

  if (slave->type != type) {
    STATUS() = scmIncorrectType;
    return false;
  }

  slave->delegate = this;

  EmBrick::Slave::Config config;
  config.UpdateInterval = UpdateInterval();
  slave->setConfig(config);

  dropHandles();
  if (true == QI())
    initHandles();

  return true;
}

void Slave::dropHandles() {
  slave->dropHandles();
}

void Slave::addBitHandle(IOMapper::Direction direction, CIEC_WSTRING id,
    uint8_t offset, uint8_t pos) {
  if (id == "")
    return;

  SlaveHandle* handle = new BitSlaveHandle(direction, offset, pos, slave);

  if (IOMapper::getInstance().registerHandle(id, handle))
    slave->addHandle(handle);
  else
    delete handle;
}

void Slave::onSlaveStatus(SlaveStatus status, SlaveStatus) {
  switch (status) {
  case OK:
    STATUS() = scmOK;
    break;
  case Slow:
    STATUS() = scmSlow;
    break;
  case Interrupted:
    STATUS() = scmInterrupted;
    break;
  case Error:
    STATUS() = scmError;
    break;
  default:
    STATUS() = scmUnknown;
    break;
  }
  sendOutputEvent(scm_nEventINDID);
}

void Slave::onSlaveDestroy() {
  slaveMutex.lock();

  slave = 0;
  QO() = ready = false;
  STATUS() = scmError;
  // TODO Recognize device kill. Causes segfault otherwise
//  sendOutputEvent(scm_nEventINDID);

  slaveMutex.unlock();
}

} /* namespace FunctionsBlocks */
} /* namespace EmBrick */
