/*******************************************************************************
 * Copyright (c) 2025 Jose Cabral
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial implementation
 *******************************************************************************/

#include "MultiMGR.h"
#include "forte/devicefactory.h"
#include "MultiDevice.h"
#include "../OPCUA_MGR.h"

using namespace forte::literals;

namespace forte::iec61499::system {
  MultiMGR::MultiMGR(MultiDevice &paDevice, OPCUA_MGR &paOpcuaMgr) : mDevice(paDevice), mOpcuaMgr(paOpcuaMgr) {
  }

  bool MultiMGR::initialize() {
    addRestartDeviceMethod();
    addSetDefaultDeviceMethod();

    return true;
  }

  std::string &MultiMGR::getArgumentString(std::string paString) {
    return mArgumentsInformation.emplace_back(std::move(paString));
  }

  void MultiMGR::addRestartDeviceMethod() {
    OPCUA_MGR::MethodInformation newMethod;

    newMethod.mMethodName = "Restart Device";
    newMethod.mDisplayName = "Restart Device";
    newMethod.mDescription = "Reset the device";
    newMethod.mCallback = &MultiMGR::onRestartDevice;
    newMethod.mNodeContext = this;

    mOpcuaMgr.addExtraMgmMethod(newMethod);
  }

  void MultiMGR::addSetDefaultDeviceMethod() {
    OPCUA_MGR::MethodInformation newMethod;

    newMethod.mMethodName = "Set Default Device";
    newMethod.mDisplayName = "Set Default Device";
    newMethod.mDescription = "Set the default device to use";
    newMethod.mCallback = &MultiMGR::onSetDefaultDevice;
    newMethod.mNodeContext = this;

    newMethod.mInArguments.push_back(UA_Argument());

    std::string listOfDevices;
    for (const auto name : DeviceFactory::getNames()) {
      if (name == "Multi"_STRID) {
        continue;
      }
      if (!listOfDevices.empty()) {
        listOfDevices += ", ";
      }
      listOfDevices += name;
    }

    OPCUA_MGR::initArgument(newMethod.mInArguments[0], UA_TYPES_STRING, getArgumentString("Device Name").data(),
                            getArgumentString("Device name to use. Posible values: " + listOfDevices).data());

    mOpcuaMgr.addExtraMgmMethod(newMethod);
  }

  UA_StatusCode MultiMGR::onRestartDevice(UA_Server *,
                                          const UA_NodeId *,
                                          void *,
                                          const UA_NodeId *,
                                          void *methodContext,
                                          const UA_NodeId *,
                                          void *,
                                          size_t,
                                          const UA_Variant *,
                                          size_t,
                                          UA_Variant *) {

    auto multiMgr = static_cast<MultiMGR *>(methodContext);
    // restart the controlled device asynchronously
    multiMgr->mDevice.requestResetControlledDevice();

    return UA_STATUSCODE_GOOD;
  }

  UA_StatusCode MultiMGR::onSetDefaultDevice(UA_Server *,
                                             const UA_NodeId *,
                                             void *,
                                             const UA_NodeId *,
                                             void *,
                                             const UA_NodeId *,
                                             void *,
                                             size_t,
                                             const UA_Variant *input,
                                             size_t,
                                             UA_Variant *) {

    auto uaStringInput = static_cast<UA_String *>(input[0].data);
    const auto newDefaultDevice =
        StringId::lookup({reinterpret_cast<const char *>(uaStringInput->data), uaStringInput->length});
    if (newDefaultDevice == "Multi"_STRID) {
      return UA_STATUSCODE_BADNOTSUPPORTED;
    }
    if (!DeviceFactory::setDefaultImpl(newDefaultDevice)) {
      return UA_STATUSCODE_BADINVALIDARGUMENT;
    }
    return UA_STATUSCODE_GOOD;
  }
} // namespace forte::iec61499::system
