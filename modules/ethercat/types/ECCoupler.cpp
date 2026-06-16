/*******************************************************************************
 * Copyright (c) 2026 Sichuan Qunyuan Technology Co., Ltd.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Sichuan Qunyuan Technology Co., Ltd. - initial API and implementation
 *******************************************************************************/

#include "ECCoupler.h"

#include "../handler/bus.h"
#include "../slave/ec_device.h"
#include "forte/iec61131_functions/func_AND.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::ethercat {
  namespace {
    const auto cCouplerPlugNames = std::array{"BusAdapterOut"_STRID, "ModuleAdapterOut"_STRID};
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_ECCoupler, "eclipse4diac::io::ethercat::ECCoupler"_STRID)

  FORTE_ECCoupler::FORTE_ECCoupler(forte::StringId paInstanceNameId, CFBContainer &paContainer) :
      FORTE_ECSlave(paInstanceNameId, paContainer, ECSlaveHandler::SlaveType::ECCoupler),
      var_ModuleAdapterOut("ModuleAdapterOut"_STRID, *this, 1) {
  }

  bool FORTE_ECCoupler::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    if (!FORTE_ECSlave::createInterfaceSpec(paConfigString, paInterfaceSpec)) {
      return false;
    }
    paInterfaceSpec.mPlugNames = cCouplerPlugNames;
    return true;
  }

  void FORTE_ECCoupler::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (BusAdapterIn().INIT() == paEIID) {
      if(BusAdapterIn().var_QI == true) {
        if(var_ModuleAdapterOut.getAdapterBlock()->getPeer() != nullptr) {
          var_ModuleAdapterOut->var_QI = true_BOOL;
          var_ModuleAdapterOut->var_Index = CIEC_UINT((static_cast<TForteUInt16>(var_BusAdapterIn->var_Index) + 1) * 100);
          var_ModuleAdapterOut->var_MasterId = var_BusAdapterIn->var_MasterId;
          sendAdapterEvent(*var_ModuleAdapterOut, FORTE_ECBusAdapter::scmEventINITID, paECET);
        }    
      }
    }
  
    FORTE_ECSlave::executeEvent(paEIID, paECET);
  }

  void FORTE_ECCoupler::forwardInitConfirmation(CEventChainExecutionThread *const paECET) {
    CIEC_BOOL combinedQO = QO();
    if(var_BusAdapterOut.getAdapterBlock()->getPeer() != nullptr) {
      combinedQO = func_AND(combinedQO, var_BusAdapterOut->var_QO);
    }
    if(var_ModuleAdapterOut.getAdapterBlock()->getPeer() != nullptr) {
      combinedQO = func_AND(combinedQO, var_ModuleAdapterOut->var_QO);
    }
    var_BusAdapterIn->var_QO = combinedQO;
    sendAdapterEvent(*var_BusAdapterIn, forte::io::IOConfigFBMultiAdapter::scmEventINITOID, paECET);
  }

  forte::IPlugPin *FORTE_ECCoupler::getPlugPinUnchecked(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_BusAdapterOut;
      case 1: return &var_ModuleAdapterOut;
      default: break;
    }
    return nullptr;
  }

  bool FORTE_ECCoupler::createSlaveHandler() {  
    auto &bus = *static_cast<ECBusHandler *>(&getController());
    auto *handler = new ECDeviceHandler(&bus, ECSlaveHandler::SlaveType::ECCoupler, mIndex);
    ECDeviceHandler::Config cfg{};
    cfg.mAlias = static_cast<TForteUInt16>(Config().Alias);
    cfg.mPosition = static_cast<TForteUInt16>(Config().Position);
    cfg.mVendorId = static_cast<TForteUInt32>(Config().VendorId);
    cfg.mProductCode = static_cast<TForteUInt32>(Config().ProductCode);
    handler->setConfig(&cfg);
    handler->mDelegate = this;
    bus.addSlave(handler);
    return true;
  }

} // namespace forte::eclipse4diac::io::ethercat

