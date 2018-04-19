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

#include "io_master.h"

namespace IO {
namespace ConfigurationFB {
namespace Multi {

const char * const Master::scmFailedToInitSlaves =
    "Failed to initialize slaves. Check if the configuration matches the hardware setup.";

TMasterList* Master::instances = new TMasterList();
TForteUInt16 Master::instancesIncrement = 0;

Master::Master(CResource *pa_poSrcRes,
    const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    Controller(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId,
        pa_acFBConnData, pa_acFBVarsData) {
  id = instancesIncrement++;
  instances->push_back(this);
}

Master* Master::getMasterById(TForteUInt16 id) {
  TMasterList::Iterator itEnd = instances->end();
  int i = 0;
  for (TMasterList::Iterator it = instances->begin(); it != itEnd; ++it, i++)
    if (id == i && *it != 0)
      return *it;
  return 0;
}

void Master::onStartup() {
  if (BusAdapterOut().getPeer() == 0) {
    return Controller::onStartup();
  }

  BusAdapterOut().MasterId() = id;
  BusAdapterOut().Index() = 0;
  BusAdapterOut().QI() = true;
  sendAdapterEvent(scm_nBusAdapterAdpNum, Adapter::scm_nEventINITID);
}

void Master::onStop() {
  if (BusAdapterOut().getPeer() == 0) {
    return Controller::onStop();
  }

  BusAdapterOut().QI() = false;
  sendAdapterEvent(scm_nBusAdapterAdpNum, Adapter::scm_nEventINITID);
}

void Master::executeEvent(int pa_nEIID) {
  Controller::executeEvent(pa_nEIID);

  if (BusAdapterOut().INITO() == pa_nEIID) {
    QO() = BusAdapterOut().QO();

    if (BusAdapterOut().QI() == true) {
      if (BusAdapterOut().QO() == true)
        Controller::onStartup();
      else
        started(scmFailedToInitSlaves);
    } else {
      Controller::onStop();
    }
  }

}

} /* namespace Multi */
} /* namespace ConfigurationFB */
} /* namespace IO */
