/*******************************************************************************
 * Copyright (c) 2016, 2025 Johannes Messmer (admin@jomess.com), fortiss GmbH,
 *                          Johannes Kepler University Linz, Thomas Öllinger
 *                          HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral      - cleaning of namespaces
 *   Alois Zoitl      - further clean-ups and performance improvements on IOHandle
 *   Thomas Öllinger  - use PARAMS to reference to I/O configuration
 *   Alois ZOitl      - removed old process interface structure, added common
 *                      interface elements
 *******************************************************************************/

#include <processinterfacefb.h>
#include "criticalregion.h"

using namespace forte::core::io;
using namespace std::string_literals;

const CIEC_STRING CProcessInterfaceFB::scmOK("OK"_STRING);
const CIEC_STRING CProcessInterfaceFB::scmWaitingForHandle("Waiting for handle.."_STRING);
const CIEC_STRING CProcessInterfaceFB::scmFailedToRegister("Failed to register observer."_STRING);
const CIEC_STRING CProcessInterfaceFB::scmMappedWrongDirectionOutput("Mapped invalid direction. A Q block requires an output handle."_STRING);
const CIEC_STRING CProcessInterfaceFB::scmMappedWrongDirectionInput("Mapped invalid direction. An I block requires an input handle."_STRING);
const CIEC_STRING CProcessInterfaceFB::scmMappedWrongDataType("Mapped invalid data type."_STRING);


CProcessInterfaceFB::CProcessInterfaceFB(forte::core::CFBContainer &paContainer, const SFBInterfaceSpec& paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId) :
    CEventSourceFB(paContainer, paInterfaceSpec, paInstanceNameId),
    IOObserver(),
    var_QI(0_BOOL),
    var_PARAMS(""_STRING),
    var_QO(0_BOOL),
    var_STATUS(""_STRING),
    var_conn_QO(var_QO),
    var_conn_STATUS(var_STATUS),
    conn_INITO(this, 0),
    conn_CNF(this, 1),
    conn_QI(nullptr),
    conn_PARAMS(nullptr),
    conn_QO(this, 0, &var_conn_QO),
    conn_STATUS(this, 1, &var_conn_STATUS) {
  mIsListening = false;
  mIsReady = false;
}

void CProcessInterfaceFB::setInitialValues() {
  var_QI = 0_BOOL;
  var_PARAMS = ""_STRING;
  var_QO = 0_BOOL;
  var_STATUS = ""_STRING;
}

void CProcessInterfaceFB::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_PARAMS, conn_PARAMS);
      break;
    }
    case scmEventREQID: {
      readData(0, var_QI, conn_QI);
      break;
    }
    default:
      break;
  }
}

void CProcessInterfaceFB::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
      break;
    }
    case scmEventCNFID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
      break;
    }
    default:
      break;
  }
}

CProcessInterfaceFB::~CProcessInterfaceFB() {
  deinitialise();
}

bool CProcessInterfaceFB::initialise(bool paIsInput, CEventChainExecutionThread *const paECET) {
  mDirection = paIsInput ? IOMapper::In : IOMapper::Out;
  if(paIsInput && (getFBInterfaceSpec().mNumDOs < 3)) {
    mType = CIEC_ANY::e_Max; //we assume that any FB which has no "IN" Output must be a EVENT-Only FB.
  }
  else {
    //as it has a index 2 here, we safely can do this
    mType = (paIsInput ? getDO(2) : getDI(2))->getDataTypeID();
  }

  mIsReady = false;
  var_STATUS = scmWaitingForHandle;

  // Reset before initialization
  deinitialise();

  // Register interface
  if(!(mIsListening = IOMapper::getInstance().registerObserver(getId(), this))) {
    var_STATUS = scmFailedToRegister;
    return false;
  }

  if(mDirection == IOMapper::In) {
    setEventChainExecutor(paECET);
  }

  return mIsReady;
}

// If PARAMS is not empty use the string specified here "as is" for the IO mapping to the configuration.
// Otherwise use the full qualified instance name of our FB.
std::string CProcessInterfaceFB::getId()  const {
  if (!var_PARAMS.empty()) {
    return static_cast<std::string>(var_PARAMS);
  }
  else {
    return getFullQualifiedApplicationInstanceName('.');
  }
}

bool CProcessInterfaceFB::deinitialise() {
  // Deregister interface
  if(mIsListening) {
    IOMapper::getInstance().deregisterObserver(this);
    mIsListening = false; // we are deregistered ! 
  }

  return !mIsReady;
}

CIEC_BOOL CProcessInterfaceFB::read(CIEC_ANY &paData) {
  CCriticalRegion criticalRegion(mSyncMutex);
  if(!mIsReady) {
    return false_BOOL;
  }

  mHandle->get(paData);

  return true_BOOL;
}

CIEC_BOOL CProcessInterfaceFB::write(const CIEC_ANY &paData) {
  CCriticalRegion criticalRegion(mSyncMutex);
  if(!mIsReady) {
    return false_BOOL;
  }

  mHandle->set(paData);

  return true_BOOL;
}

bool CProcessInterfaceFB::onChange() {
  return read();
}

void CProcessInterfaceFB::onHandle(IOHandle* const  paHandle) {
  {
    CCriticalRegion criticalRegion(mSyncMutex);

    IOObserver::onHandle(paHandle);

    if(paHandle->getIOHandleDataType() != mType) {
      var_STATUS = scmMappedWrongDataType;
      return;
    }

    if(paHandle->getDirection() != mDirection) {
      var_STATUS = mDirection == IOMapper::In ? scmMappedWrongDirectionInput : scmMappedWrongDirectionOutput;
      return;
    }

    var_STATUS = scmOK;
    mIsReady = true;
  }

  // Read & write current state
  if(mDirection == IOMapper::In) {
    var_QO = read();
  } else {
    var_QO = write();
  }
}

EMGMResponse CProcessInterfaceFB::changeExecutionState(EMGMCommandType paCommand) {
  if(EMGMCommandType::Kill == paCommand) {
    deinitialise();
  }
  return CFunctionBlock::changeExecutionState(paCommand);
}

CIEC_ANY *CProcessInterfaceFB::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_PARAMS;
  }
  return nullptr;
}

CIEC_ANY *CProcessInterfaceFB::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *CProcessInterfaceFB::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **CProcessInterfaceFB::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_PARAMS;
  }
  return nullptr;
}

CDataConnection *CProcessInterfaceFB::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}

void CProcessInterfaceFB::dropHandle() {
  CCriticalRegion criticalRegion(mSyncMutex);

  IOObserver::dropHandle();

  var_QO = CIEC_BOOL(false);
  var_STATUS = scmWaitingForHandle;
  mIsReady = false;
}

