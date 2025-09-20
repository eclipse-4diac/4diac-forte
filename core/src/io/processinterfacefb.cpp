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

#include "forte/io/processinterfacefb.h"

using namespace std::string_literals;

namespace forte::io {
  const CIEC_STRING CProcessInterfaceFB::scmOK("OK"_STRING);
  const CIEC_STRING CProcessInterfaceFB::scmWaitingForHandle("Waiting for handle.."_STRING);
  const CIEC_STRING CProcessInterfaceFB::scmFailedToRegister("Failed to register observer."_STRING);
  const CIEC_STRING CProcessInterfaceFB::scmMappedWrongDirectionOutput(
      "Mapped invalid direction. A Q block requires an output handle."_STRING);
  const CIEC_STRING CProcessInterfaceFB::scmMappedWrongDirectionInput(
      "Mapped invalid direction. An I block requires an input handle."_STRING);
  const CIEC_STRING CProcessInterfaceFB::scmMappedWrongDataType("Mapped invalid data type."_STRING);

  CProcessInterfaceFB::CProcessInterfaceFB(CFBContainer &paContainer,
                                           const SFBInterfaceSpec &paInterfaceSpec,
                                           const StringId paInstanceNameId) :
      CEventSourceFB(paContainer, paInterfaceSpec, paInstanceNameId),
      IOObserver(),
      var_QI(0_BOOL),
      var_PARAMS(""_STRING),
      var_QO(0_BOOL),
      var_STATUS(""_STRING),
      conn_INITO(*this, 0),
      conn_CNF(*this, 1),
      conn_QI(nullptr),
      conn_PARAMS(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS) {
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
    switch (paEIID) {
      case scmEventINITID: {
        readData(0, var_QI, conn_QI);
        readData(1, var_PARAMS, conn_PARAMS);
        break;
      }
      case scmEventREQID: {
        readData(0, var_QI, conn_QI);
        break;
      }
      default: break;
    }
  }

  void CProcessInterfaceFB::writeOutputData(const TEventID paEIID) {
    size_t numDIs = getFBInterfaceSpec().getNumDIs();
    switch (paEIID) {
      case scmEventINITOID: {
        writeData(numDIs + 0, var_QO, conn_QO);
        writeData(numDIs + 1, var_STATUS, conn_STATUS);
        break;
      }
      case scmEventCNFID: {
        writeData(numDIs + 0, var_QO, conn_QO);
        writeData(numDIs + 1, var_STATUS, conn_STATUS);
        break;
      }
      default: break;
    }
  }

  CProcessInterfaceFB::~CProcessInterfaceFB() {
    deinitialise();
  }

  void CProcessInterfaceFB::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (paEIID == scmEventINITID) {
      if (var_QI) {
        var_QO = initialise(paECET);
      } else {
        var_QO = deinitialise();
      }
      sendOutputEvent(scmEventINITOID, paECET);
    }
  }

  CIEC_BOOL CProcessInterfaceFB::initialise(CEventChainExecutionThread *const paECET) {
    mIsReady = false;
    var_STATUS = scmWaitingForHandle;

    // Reset before initialization
    deinitialise();

    // Register interface
    if (!(mIsListening = IOMapper::getInstance().registerObserver(getId(), this))) {
      var_STATUS = scmFailedToRegister;
      return false_BOOL;
    }

    if (getDirection() == IOMapper::In) {
      setEventChainExecutor(paECET);
    }

    return mIsReady ? true_BOOL : false_BOOL;
  }

  // If PARAMS is not empty use the string specified here "as is" for the IO mapping to the configuration.
  // Otherwise use the full qualified instance name of our FB.
  std::string CProcessInterfaceFB::getId() const {
    if (!var_PARAMS.empty()) {
      return static_cast<std::string>(var_PARAMS);
    } else {
      return getFullQualifiedApplicationInstanceName('.');
    }
  }

  CIEC_BOOL CProcessInterfaceFB::deinitialise() {
    // Deregister interface
    if (mIsListening) {
      IOMapper::getInstance().deregisterObserver(this);
      mIsListening = false; // we are deregistered !
    }

    return mIsReady ? true_BOOL : false_BOOL;
  }

  bool CProcessInterfaceFB::onChange() {
    // per default we do not need to do anything
    return true;
  }

  void CProcessInterfaceFB::onHandle(IOHandle *const paHandle) {
    IOObserver::onHandle(paHandle);

    if (paHandle->getIOHandleDataType() != getIOObserverDataType()) {
      var_STATUS = scmMappedWrongDataType;
      return;
    }

    if (paHandle->getDirection() != getDirection()) {
      var_STATUS = getDirection() == IOMapper::In ? scmMappedWrongDirectionInput : scmMappedWrongDirectionOutput;
      return;
    }

    var_STATUS = scmOK;
    mIsReady = true;
  }

  EMGMResponse CProcessInterfaceFB::changeExecutionState(EMGMCommandType paCommand) {
    EMGMResponse response = CFunctionBlock::changeExecutionState(paCommand);
    if (response == EMGMResponse::Ready &&
        (paCommand == EMGMCommandType::Kill || paCommand == EMGMCommandType::Reset)) {
      deinitialise();
    }
    return response;
  }

  CIEC_ANY *CProcessInterfaceFB::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_PARAMS;
    }
    return nullptr;
  }

  CIEC_ANY *CProcessInterfaceFB::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *CProcessInterfaceFB::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **CProcessInterfaceFB::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_PARAMS;
    }
    return nullptr;
  }

  CDataConnection *CProcessInterfaceFB::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

  void CProcessInterfaceFB::dropHandle() {
    IOObserver::dropHandle();
    var_QO = false_BOOL;
    var_STATUS = scmWaitingForHandle;
    mIsReady = false;
  }
} // namespace forte::io
