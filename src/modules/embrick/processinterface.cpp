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

#include "processinterface.h"
#include <io/mapper.h>

namespace EmBrick {

const char * const ProcessInterface::scmOK = "OK";
const char * const ProcessInterface::scmMappedWrongDataType =
    "Mapped invalid data type.";

ProcessInterface::ProcessInterface(CResource *paSrcRes,
    const SFBInterfaceSpec *paInterfaceSpec,
    const CStringDictionary::TStringId paInstanceNameId,
    TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId,
        paFBConnData, paFBVarsData), IOObserver() {
  isReady = false;
  isInput = false;
}

ProcessInterface::~ProcessInterface() {
  deinitialise();
}

bool ProcessInterface::initialise(bool paIsInput) {
  isInput = paIsInput;

  isReady = false;
  QO() = false;

  // Register interface
  IOMapper::getInstance().registerObserver(getInstanceName(), this);

  return isReady;
}

bool ProcessInterface::deinitialise() {
  // Deregister interface
  IOMapper::getInstance().deregisterObserver(this);

  return !isReady;
}

bool ProcessInterface::readPin() {
  if (!isReady)
    return false;

  handle->get(IN_X());

  return true;
}

bool ProcessInterface::writePin() {
  if (!isReady)
    return false;

  handle->set(OUT_X());

  return true;
}

bool ProcessInterface::readWord() {
  if (!isReady)
    return false;

  handle->get(IN_W());

  return true;
}

bool ProcessInterface::writeWord() {
  if (!isReady)
    return false;

  handle->set(OUT_W());

  return true;
}

bool ProcessInterface::readDWord() {
  if (!isReady)
    return false;

  handle->get(IN_D());

  return true;
}

bool ProcessInterface::writeDWord() {
  if (!isReady)
    return false;

  handle->set(OUT_D());

  return true;
}

bool ProcessInterface::onChange() {
  if (handle->is(CIEC_ANY::e_BOOL)) {
    QO() = readPin();
  } else if (handle->is(CIEC_ANY::e_WORD)) {
    QO() = readWord();
  } else if (handle->is(CIEC_ANY::e_DWORD)) {
    QO() = readDWord();
  }

  return true;
}

void ProcessInterface::onHandle(IOHandle* handle) {
  IOObserver::onHandle(handle);

  if (isInput) {
    setEventChainExecutor(m_poInvokingExecEnv);

    if (!handle->is(getDO(2)->getDataTypeID())) {
      STATUS() = scmMappedWrongDataType;
      return;
    }
  } else {
    if (!handle->is(getDI(2)->getDataTypeID())) {
      STATUS() = scmMappedWrongDataType;
      return;
    }
  }

  STATUS() = scmOK;
  QO() = true;
  isReady = true;
}

void ProcessInterface::dropHandle() {
  IOObserver::dropHandle();

  QO() = false;
  isReady = false;
}

}
