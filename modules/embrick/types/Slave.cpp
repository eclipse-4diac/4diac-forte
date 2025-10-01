/*******************************************************************************
 * Copyright (c) 2016, 2025 Johannes Messmer (admin@jomess.com), fortiss GmbH
 *
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

#include "Slave.h"
#include "../handler/bus.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::embrick {

  const CIEC_WSTRING EmbrickSlave::scmSlow("Slow");
  const CIEC_WSTRING EmbrickSlave::scmInterrupted("Interrupted");
  const CIEC_WSTRING EmbrickSlave::scmError("Error");
  const CIEC_WSTRING EmbrickSlave::scmUnknown("Invalid status code");

  EmbrickSlave::EmbrickSlave(const TForteUInt8 *const paSlaveConfigurationIO,
                             const TForteUInt8 paSlaveConfigurationIO_num,
                             int paType,
                             CFBContainer &paContainer,
                             const SFBInterfaceSpec &paInterfaceSpec,
                             const forte::StringId paInstanceNameId) :
      IOConfigFBMultiSlave(
          paSlaveConfigurationIO, paSlaveConfigurationIO_num, paType, paContainer, paInterfaceSpec, paInstanceNameId),
      var_BusAdapterIn("BusAdapterIn"_STRID, *this, 0),
      var_BusAdapterOut("BusAdapterOut"_STRID, *this, 0),
      mSlave(nullptr) {
  }

  EmbrickSlave::~EmbrickSlave() {
    deInit();
  }

  forte::IPlugPin *EmbrickSlave::getPlugPinUnchecked(size_t paIndex) {
    return (paIndex == 0) ? &var_BusAdapterOut : nullptr;
  }

  forte::ISocketPin *EmbrickSlave::getSocketPinUnchecked(size_t paIndex) {
    return (paIndex == 0) ? &var_BusAdapterIn : nullptr;
  }

  const char *EmbrickSlave::init() {
    util::CCriticalRegion criticalRegion(mSlaveMutex);

    EmbrickBusHandler &bus = *static_cast<EmbrickBusHandler *>(&getController());

    mSlave = bus.getSlave(mIndex);
    mSlave->mDelegate = this;

    EmbrickSlaveHandler::Config config;
    config.mUpdateInterval = UpdateInterval().operator TForteUInt16();
    mSlave->setConfig(config);

    return nullptr;
  }

  void EmbrickSlave::deInit() {
    util::CCriticalRegion criticalRegion(mSlaveMutex);

    if (mSlave != nullptr) {
      mSlave->mDelegate = nullptr;
      mSlave = nullptr;
    }
  }

  void EmbrickSlave::onSlaveStatus(EmbrickSlaveHandler::SlaveStatus paStatus, EmbrickSlaveHandler::SlaveStatus) {
    switch (paStatus) {
      case EmbrickSlaveHandler::OK: STATUS() = scmOK; break;
      case EmbrickSlaveHandler::Slow: STATUS() = scmSlow; break;
      case EmbrickSlaveHandler::Interrupted: STATUS() = scmInterrupted; break;
      case EmbrickSlaveHandler::Error: STATUS() = scmError; break;
      default: STATUS() = scmUnknown; break;
    }

    sendOutputEvent(scmEventINDID, getEventChainExecutor());
  }

  void EmbrickSlave::onSlaveDestroy() {
    deInit();

    QO() = false_BOOL;
    STATUS() = scmError;
  }

} // namespace forte::eclipse4diac::io::embrick
