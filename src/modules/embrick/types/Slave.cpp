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

namespace EmBrick {
namespace FunctionBlocks {

Slave::Slave(CResource *pa_poSrcRes,
    const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    CFunctionBlock(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId,
        pa_acFBConnData, pa_acFBVarsData), type(None), slave(NULL) {

}

void Slave::executeEvent(int pa_nEIID) {
  if (BusAdapterIn().INIT() == pa_nEIID) {
    // Init slave
    if (init(BusAdapterIn().INDEX()))
      DEVLOG_INFO("emBrick[SlaveFunctionBlock]: Init slave %d success.\n",
          (int ) BusAdapterIn().INDEX());
    else
      DEVLOG_ERROR("emBrick[SlaveFunctionBlock]: Init slave %d failed.\n",
          (int ) BusAdapterIn().INDEX());

    // Init next slave
    BusAdapterOut().INDEX() = BusAdapterIn().INDEX() + 1;
    sendAdapterEvent(scm_nBusAdapterOutAdpNum, BusAdapter::scm_nEventINITID);
  }
}

bool Slave::init(int index) {
  BusHandler &bus = BusHandler::getInstance();

  slave = bus.getSlave(index);
  if (!slave)
    return false;

  if (slave->type != type)
    return false;

  return true;
}

} /* namespace FunctionsBlocks */
} /* namespace EmBrick */
