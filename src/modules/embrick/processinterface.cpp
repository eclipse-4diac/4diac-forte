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

  if (handle->is(X)) {
    IN_X() = ((IOHandleWrapper<CIEC_BOOL>*) handle)->get();
  } else {
    DEVLOG_ERROR("emBrick[PI]: Mapped wrong data type. Expected X handle.\n");
  }

  return true;
}

bool ProcessInterface::writePin() {
  if (!isReady)
    return false;

  if (handle->is(X)) {
    ((IOHandleWrapper<CIEC_BOOL>*) handle)->set(OUT_X());
  } else {
    DEVLOG_ERROR("emBrick[PI]: Mapped wrong data type. Expected X handle.\n");
  }

  return true;
}

bool ProcessInterface::readWord() {
  if (!isReady)
    return false;

  if (handle->is(W)) {
    IN_W() = ((IOHandleWrapper<CIEC_WORD>*) handle)->get();
  } else {
    DEVLOG_ERROR("emBrick[PI]: Mapped wrong data type. Expected W handle.\n");
  }

  return true;
}

bool ProcessInterface::writeWord() {
  if (!isReady)
    return false;

  if (handle->is(W)) {
    ((IOHandleWrapper<CIEC_WORD>*) handle)->set(OUT_W());
  } else {
    DEVLOG_ERROR("emBrick[PI]: Mapped wrong data type. Expected W handle.\n");
  }

  return true;
}

bool ProcessInterface::readDWord() {
  if (!isReady)
    return false;

  if (handle->is(D)) {
    IN_D() = ((IOHandleWrapper<CIEC_DWORD>*) handle)->get();
  } else {
    DEVLOG_ERROR("emBrick[PI]: Mapped wrong data type. Expected D handle.\n");
  }

  return true;
}

bool ProcessInterface::writeDWord() {
  if (!isReady)
    return false;

  if (handle->is(D)) {
    ((IOHandleWrapper<CIEC_DWORD>*) handle)->set(OUT_D());
  } else {
    DEVLOG_ERROR("emBrick[PI]: Mapped wrong data type. Expected D handle.\n");
  }

  return true;
}

bool ProcessInterface::onChange() {
  if (handle->is(X)) {
    QO() = readPin();
  } else if (handle->is(W)) {
    QO() = readWord();
  } else if (handle->is(D)) {
    QO() = readDWord();
  }

  return true;
}

void ProcessInterface::onHandle(IOHandle* handle) {
  IOObserver::onHandle(handle);

  if (isInput) {
    setEventChainExecutor(m_poInvokingExecEnv);
  }

  QO() = true;
  isReady = true;
}

void ProcessInterface::dropHandle() {
  IOObserver::dropHandle();

  QO() = false;
  isReady = false;
}

}
