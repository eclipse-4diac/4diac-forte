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

Slave::Slave(CResource *pa_poSrcRes,
    const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    CFunctionBlock(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId,
        pa_acFBConnData, pa_acFBVarsData), type(UnknownSlave), slave(0), ready(
        false) {

}

Slave::~Slave() {

}

void Slave::executeEvent(int pa_nEIID) {
  if (BusAdapterIn().INIT() == pa_nEIID) {
    // Init slave
    slaveMutex.lock();
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
    case scm_nEventREQID:
      // TODO add code for REQ event!
      /*
       do not forget to send output event, calling e.g.
       sendOutputEvent(scm_nEventCNFID);
       */
      break;
    case scm_nEventMAPID:
      slaveMutex.lock();

      if (!ready)
        break;

      // Drop all existing handles
      dropHandles();

      if (true == QI())
        initHandles();

      slaveMutex.unlock();
      break;
    }
}

bool Slave::init(int index) {
  slave = 0;

  BusHandler &bus = BusHandler::getInstance();

  slave = bus.getSlave(index);
  if (!slave)
    return false;

  if (slave->type != type)
    return false;

  slave->delegate = this;

  dropHandles();
  if (true == QI())
    initHandles();

  return true;
}

void Slave::dropHandles() {
  slave->dropHandles();
}

void Slave::addBitHandle(IOHandle::Direction direction, CIEC_WSTRING id,
    uint8_t offset, uint8_t pos) {
  if (id == "")
    return;

  SlaveHandle* handle = new BitSlaveHandle(direction, offset, pos, slave);

  if (IOMapper::getInstance().registerHandle(id, handle))
    slave->addHandle(handle);
  else
    delete handle;
}

void Slave::onSlaveDestroy() {
  slaveMutex.lock();

  slave = 0;
  QO() = ready = false;

  slaveMutex.unlock();
}

} /* namespace FunctionsBlocks */
} /* namespace EmBrick */
