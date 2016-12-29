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
const char * const ProcessInterface::scmWaitingForHandle =
    "Waiting for handle..";
const char * const ProcessInterface::scmFailedToRegister =
    "Failed to register observer.";
const char * const ProcessInterface::scmMappedWrongDirectionOutput =
    "Mapped invalid direction. A Q block requires an output handle.";
const char * const ProcessInterface::scmMappedWrongDirectionInput =
    "Mapped invalid direction. An I block requires an input handle.";
const char * const ProcessInterface::scmMappedWrongDataType =
    "Mapped invalid data type.";

ProcessInterface::ProcessInterface(CResource *paSrcRes,
    const SFBInterfaceSpec *paInterfaceSpec,
    const CStringDictionary::TStringId paInstanceNameId,
    TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId,
        paFBConnData, paFBVarsData), IOObserver() {
  isListening = false;
  isReady = false;
  isInput = false;
}

ProcessInterface::~ProcessInterface() {
  deinitialise();
}

bool ProcessInterface::initialise(bool paIsInput) {
  isInput = paIsInput;

  isReady = false;
  STATUS() = scmWaitingForHandle;

  // Reset before initialization
  deinitialise();

  // Register interface
  if (!(isListening = IOMapper::getInstance().registerObserver(
      getInstanceName(), this))) {
    STATUS() = scmFailedToRegister;
    return false;
  }

  return isReady;
}

bool ProcessInterface::deinitialise() {
  // Deregister interface
  if (isListening)
    IOMapper::getInstance().deregisterObserver(this);

  return !isReady;
}

bool ProcessInterface::readPin() {
  syncMutex.lock();
  if (!isReady) {
    syncMutex.unlock();
    return false;
  }

  handle->get(IN_X());

  syncMutex.unlock();
  return true;
}

bool ProcessInterface::writePin() {
  syncMutex.lock();
  if (!isReady) {
    syncMutex.unlock();
    return false;
  }

  handle->set(OUT_X());

  syncMutex.unlock();
  return true;
}

bool ProcessInterface::readWord() {
  syncMutex.lock();
  if (!isReady) {
    syncMutex.unlock();
    return false;
  }

  handle->get(IN_W());

  syncMutex.unlock();
  return true;
}

bool ProcessInterface::writeWord() {
  syncMutex.lock();
  if (!isReady) {
    syncMutex.unlock();
    return false;
  }

  handle->set(OUT_W());

  syncMutex.unlock();
  return true;
}

bool ProcessInterface::readDWord() {
  syncMutex.lock();
  if (!isReady) {
    syncMutex.unlock();
    return false;
  }

  handle->get(IN_D());

  syncMutex.unlock();
  return true;
}

bool ProcessInterface::writeDWord() {
  syncMutex.lock();
  if (!isReady) {
    syncMutex.unlock();
    return false;
  }

  handle->set(OUT_D());

  syncMutex.unlock();
  return true;
}

bool ProcessInterface::onChange() {
  syncMutex.lock();
  if (!isReady) {
    syncMutex.unlock();
    return false;
  }

  if (handle->is(CIEC_ANY::e_BOOL)) {
    handle->get(IN_X());
  } else if (handle->is(CIEC_ANY::e_WORD)) {
    handle->get(IN_D());
  } else if (handle->is(CIEC_ANY::e_DWORD)) {
    handle->get(IN_W());
  } else {
    syncMutex.unlock();
    return false;
  }

  QO() = true;

  syncMutex.unlock();
  return true;
}

void ProcessInterface::onHandle(IOHandle* handle) {
  syncMutex.lock();

  IOObserver::onHandle(handle);

  if (isInput) {
    if (!handle->is(getDO(2)->getDataTypeID())) {
      STATUS() = scmMappedWrongDataType;
      return syncMutex.unlock();
    }

    if (!handle->is(IOHandle::Input)) {
      STATUS() = scmMappedWrongDirectionInput;
      return syncMutex.unlock();
    }

    setEventChainExecutor(m_poInvokingExecEnv);
  } else {
    if (!handle->is(getDI(2)->getDataTypeID())) {
      STATUS() = scmMappedWrongDataType;
      return syncMutex.unlock();
    }

    if (!handle->is(IOHandle::Output)) {
      STATUS() = scmMappedWrongDirectionOutput;
      return syncMutex.unlock();
    }
  }

  STATUS() = scmOK;
  QO() = true;
  isReady = true;

  syncMutex.unlock();
}

void ProcessInterface::dropHandle() {
  syncMutex.lock();

  IOObserver::dropHandle();

  QO() = false;
  isReady = false;

  syncMutex.unlock();
}

}
