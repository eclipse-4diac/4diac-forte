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
using namespace std::string_literals;

const std::string ProcessInterface::scmOK = "OK"s;
const std::string ProcessInterface::scmWaitingForHandle = "Waiting for handle.."s;
const std::string ProcessInterface::scmFailedToRegister = "Failed to register observer."s;
const std::string ProcessInterface::scmMappedWrongDirectionOutput = "Mapped invalid direction. A Q block requires an output handle."s;
const std::string ProcessInterface::scmMappedWrongDirectionInput = "Mapped invalid direction. An I block requires an input handle."s;
const std::string ProcessInterface::scmMappedWrongDataType = "Mapped invalid data type."s;

ProcessInterface::ProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId), IOObserver() {
  mIsListening = false;
  mIsReady = false;
}

ProcessInterface::~ProcessInterface() {
  deinitialise();
}

bool ProcessInterface::initialise(bool paIsInput) {
  mDirection = paIsInput ? IOMapper::In : IOMapper::Out;
  if(paIsInput && (getFBInterfaceSpec()->mNumDOs < 3))
  {
    mType = CIEC_ANY::e_Max; //we assume that any FB which has no "IN" Output must be a EVENT-Only FB.
  }
  else
  {
    //as it has a index 2 here, we safely can do this
  mType = (paIsInput ? getDO(2) : getDI(2))->getDataTypeID();
  }

  mIsReady = false;
  STATUS() = CIEC_STRING(scmWaitingForHandle);

  // Reset before initialization
  deinitialise();

  // Register interface
  if(!(mIsListening = IOMapper::getInstance().registerObserver(CIEC_WSTRING(getFullQualifiedInstanceName().c_str()), this))) {
    STATUS() = CIEC_STRING(scmFailedToRegister);
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
  } else if(mHandle->is(CIEC_ANY::e_BYTE)) {
      mHandle->get(IN_B());
  } else if(mHandle->is(CIEC_ANY::e_WORD)) {
    mHandle->get(IN_W());
  } else if(mHandle->is(CIEC_ANY::e_DWORD)) {
    mHandle->get(IN_D());
  } else if(mHandle->is(CIEC_ANY::e_LWORD)) {
    mHandle->get(IN_L());
  } else if(mHandle->is(CIEC_ANY::e_Max)) {
    return true; //it has no "IN" ...
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
  } else if(mHandle->is(CIEC_ANY::e_BYTE)) {
    mHandle->set(OUT_B());
  } else if(mHandle->is(CIEC_ANY::e_WORD)) {
    mHandle->set(OUT_W());
  } else if(mHandle->is(CIEC_ANY::e_DWORD)) {
    mHandle->set(OUT_D());
  } else if(mHandle->is(CIEC_ANY::e_LWORD)) {
    mHandle->set(OUT_L());
  } else if(mHandle->is(CIEC_ANY::e_Max)) { //not yet possible.
    return true; //it has no "OUT" ...
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
      STATUS() = CIEC_STRING(scmMappedWrongDataType);
      return;
    }

    if(!paHandle->is(mDirection)) {
      STATUS() = CIEC_STRING(mDirection == IOMapper::In ? scmMappedWrongDirectionInput : scmMappedWrongDirectionOutput);
      return;
    }

    if(mDirection == IOMapper::In) {
      setEventChainExecutor(mInvokingExecEnv);
    }

    STATUS() = CIEC_STRING(scmOK);
    mIsReady = true;
  }

  // Read & write current state
  if(mDirection == IOMapper::In) {
    QO() = CIEC_BOOL(read());
  } else {
    QO() = CIEC_BOOL(write());
  }
}

void ProcessInterface::dropHandle() {
  CCriticalRegion criticalRegion(mSyncMutex);

  IOObserver::dropHandle();

  QO() = CIEC_BOOL(false);
  STATUS() = CIEC_STRING(scmWaitingForHandle);
  mIsReady = CIEC_BOOL(false);
}

