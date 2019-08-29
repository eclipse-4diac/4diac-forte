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

#include "processinterface.h"
#include "criticalregion.h"

using namespace forte::core::io;

const char * const ProcessInterface::scmOK = "OK";
const char * const ProcessInterface::scmWaitingForHandle = "Waiting for handle..";
const char * const ProcessInterface::scmFailedToRegister = "Failed to register observer.";
const char * const ProcessInterface::scmMappedWrongDirectionOutput = "Mapped invalid direction. A Q block requires an output handle.";
const char * const ProcessInterface::scmMappedWrongDirectionInput = "Mapped invalid direction. An I block requires an input handle.";
const char * const ProcessInterface::scmMappedWrongDataType = "Mapped invalid data type.";

ProcessInterface::ProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId,
    TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData), IOObserver() {
  mIsListening = false;
  mIsReady = false;
}

ProcessInterface::~ProcessInterface() {
  deinitialise();
}

bool ProcessInterface::initialise(bool paIsInput) {
  mDirection = paIsInput ? IOMapper::In : IOMapper::Out;
  mType = (paIsInput ? getDO(2) : getDI(2))->getDataTypeID();

  mIsReady = false;
  STATUS() = scmWaitingForHandle;

  // Reset before initialization
  deinitialise();

  // Register interface
  if(!(mIsListening = IOMapper::getInstance().registerObserver(getInstanceName(), this))) {
    STATUS() = scmFailedToRegister;
    return false;
  }

  return mIsReady;
}

bool ProcessInterface::deinitialise() {
  // Deregister interface
  if(mIsListening) {
    IOMapper::getInstance().deregisterObserver(this);
  }

  return !mIsReady;
}

bool ProcessInterface::read(CIEC_ANY &paData) {
  CCriticalRegion criticalRegion(mSyncMutex);
  if(!mIsReady) {
    return false;
  }

  mHandle->get(paData);

  return true;
}

bool ProcessInterface::write(CIEC_ANY &paData) {
  CCriticalRegion criticalRegion(mSyncMutex);
  if(!mIsReady) {
    return false;
  }

  mHandle->set(paData);

  return true;
}

bool ProcessInterface::read() {
  CCriticalRegion criticalRegion(mSyncMutex);
  if(!mIsReady) {
    return false;
  }

  if(mHandle->is(CIEC_ANY::e_BOOL)) {
    mHandle->get(IN_X());
  } else if(mHandle->is(CIEC_ANY::e_WORD)) {
    mHandle->get(IN_W());
  } else if(mHandle->is(CIEC_ANY::e_DWORD)) {
    mHandle->get(IN_D());
  } else {
    return false;
  }

  return true;
}

bool ProcessInterface::write() {
  CCriticalRegion criticalRegion(mSyncMutex);
  if(!mIsReady) {
    return false;
  }

  if(mHandle->is(CIEC_ANY::e_BOOL)) {
    mHandle->set(OUT_X());
  } else if(mHandle->is(CIEC_ANY::e_WORD)) {
    mHandle->set(OUT_W());
  } else if(mHandle->is(CIEC_ANY::e_DWORD)) {
    mHandle->set(OUT_D());
  } else {
    return false;
  }

  return true;
}

bool ProcessInterface::onChange() {
  return read();
}

void ProcessInterface::onHandle(IOHandle* paHandle) {
  {
    CCriticalRegion criticalRegion(mSyncMutex);

    IOObserver::onHandle(paHandle);

    if(!paHandle->is(mType)) {
      STATUS() = scmMappedWrongDataType;
      return;
    }

    if(!paHandle->is(mDirection)) {
      STATUS() = mDirection == IOMapper::In ? scmMappedWrongDirectionInput : scmMappedWrongDirectionOutput;
      return;
    }

    if(mDirection == IOMapper::In) {
      setEventChainExecutor(m_poInvokingExecEnv);
    }

    STATUS() = scmOK;
    mIsReady = true;
  }

  // Read & write current state
  if(mDirection == IOMapper::In) {
    QO() = read();
  } else {
    QO() = write();
  }
}

void ProcessInterface::dropHandle() {
  CCriticalRegion criticalRegion(mSyncMutex);

  IOObserver::dropHandle();

  QO() = false;
  STATUS() = scmWaitingForHandle;
  mIsReady = false;
}

