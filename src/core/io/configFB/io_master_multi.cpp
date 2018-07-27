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

#include "io_master_multi.h"

using namespace forte::core::IO;

const char * const IOConfigFBMultiMaster::scmFailedToInitSlaves =
    "Failed to initialize slaves. Check if the configuration matches the hardware setup.";

TMasterList* IOConfigFBMultiMaster::instances = new TMasterList();
TForteUInt16 IOConfigFBMultiMaster::instancesIncrement = 0;

IOConfigFBMultiMaster::IOConfigFBMultiMaster(CResource *pa_poSrcRes,
    const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    IOConfigFBController(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId,
        pa_acFBConnData, pa_acFBVarsData) {
  id = instancesIncrement++;
  instances->pushBack(this);
}

IOConfigFBMultiMaster* IOConfigFBMultiMaster::getMasterById(TForteUInt16 id) {
  TMasterList::Iterator itEnd = instances->end();
  int i = 0;
  for (TMasterList::Iterator it = instances->begin(); it != itEnd; ++it, i++)
    if (id == i && *it != 0)
      return *it;
  return 0;
}

void IOConfigFBMultiMaster::onStartup() {
  if (BusAdapterOut().getPeer() == 0) {
    return IOConfigFBController::onStartup();
  }

  BusAdapterOut().MasterId() = id;
  BusAdapterOut().Index() = 0;
  BusAdapterOut().QI() = true;
  sendAdapterEvent(scm_nBusAdapterAdpNum, IOConfigFBMultiAdapter::scm_nEventINITID);
}

void IOConfigFBMultiMaster::onStop() {
  if (BusAdapterOut().getPeer() == 0) {
    return IOConfigFBController::onStop();
  }

  BusAdapterOut().QI() = false;
  sendAdapterEvent(scm_nBusAdapterAdpNum, IOConfigFBMultiAdapter::scm_nEventINITID);
}

void IOConfigFBMultiMaster::executeEvent(int pa_nEIID) {
  IOConfigFBController::executeEvent(pa_nEIID);

  if (BusAdapterOut().INITO() == pa_nEIID) {
    QO() = BusAdapterOut().QO();

    if (BusAdapterOut().QI() == true) {
      if (BusAdapterOut().QO() == true)
        IOConfigFBController::onStartup();
      else
        started(scmFailedToInitSlaves);
    } else {
      IOConfigFBController::onStop();
    }
  }
}

