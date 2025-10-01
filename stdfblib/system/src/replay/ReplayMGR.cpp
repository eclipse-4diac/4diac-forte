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

#include "ReplayMGR.h"
#include "../../../core/src/ecetFake.h"
#include "../../../core/src/trace/reader/utils.h"
#include "ReplayDevice.h"

namespace forte::iec61499::system {
  ReplayMGR::ReplayMGR(ReplayDevice &paDevice, OPCUA_MGR &paOpcuaMgr) : mDevice(paDevice), mOpcuaMgr(paOpcuaMgr) {
  }

  bool ReplayMGR::initialize() {
    addReadTracesMethod();
    addReplayNextEventMethod();

    return true;
  }

  std::string &ReplayMGR::getArgumentString(std::string paString) {
    return mArgumentsInformation.emplace_back(std::move(paString));
  }

  void ReplayMGR::addReadTracesMethod() {
    OPCUA_MGR::MethodInformation newMethod;

    newMethod.mMethodName = "Read Traces";
    newMethod.mDisplayName = "Read Traces";
    newMethod.mDescription = "Read Traces and load them for later reproduction";
    newMethod.mCallback = &ReplayMGR::onReadTraces;
    newMethod.mNodeContext = this;

    newMethod.mInArguments.push_back(UA_Argument());

    OPCUA_MGR::initArgument(newMethod.mInArguments[0], UA_TYPES_STRING, getArgumentString("Path to traces").data(),
                            getArgumentString("Local path of the folder containing the traces").data());

    mOpcuaMgr.addExtraMgmMethod(newMethod);
  }

  void ReplayMGR::addReplayNextEventMethod() {
    OPCUA_MGR::MethodInformation newMethod;

    newMethod.mMethodName = "Replay Next Event";
    newMethod.mDisplayName = "Replay Next Event";
    newMethod.mDescription = "Replay the next event in the resource";
    newMethod.mCallback = &ReplayMGR::onReplayNextEvent;
    newMethod.mNodeContext = this;

    newMethod.mOutArguments.push_back(UA_Argument());

    OPCUA_MGR::initArgument(newMethod.mOutArguments[0], UA_TYPES_STRING,
                            getArgumentString("The event that was executed").data(),
                            getArgumentString("The event that was lastlty executed").data());

    mOpcuaMgr.addExtraResourceMethod(newMethod);
  }

  UA_StatusCode ReplayMGR::onReadTraces(UA_Server *,
                                        const UA_NodeId *,
                                        void *,
                                        const UA_NodeId *,
                                        void *methodContext,
                                        const UA_NodeId *,
                                        void *,
                                        size_t,
                                        const UA_Variant *input,
                                        size_t,
                                        UA_Variant *) {

    auto replayMgr = static_cast<ReplayMGR *>(methodContext);
    auto uaStringInput = static_cast<UA_String *>(input[0].data);
    auto path = std::string((const char *) uaStringInput->data, uaStringInput->length);

    auto events = trace::reader::utils::getEventMessages(path);
    if (!events.has_value()) {
      return UA_STATUSCODE_BADINVALIDARGUMENT;
    }

    auto replayAlgorithmEvents = trace::reader::utils::filterEventsForReplayDevice(events.value(), replayMgr->mDevice);

    replayMgr->mDeviceReplayer =
        std::make_unique<CDeviceReplayer>(replayMgr->mDevice, std::move(replayAlgorithmEvents));

    // this will allow the Start command in the ReplayDevice to go through
    replayMgr->mDevice.startControlling();

    {
      // at this point the device is running,
      // but the deployed application (resources and FBs are still idle)
      // we then stop and start again the device
      // the stop returns a invalid state result because
      // the application cannot be stopped in the iddle state
      // but we don't care about it, since the start command later
      // can still start everything, the device and the application
      SManagementCMD command;
      command.mCMD = EMGMCommandType::Stop;
      command.mDestination = {};
      replayMgr->mDevice.executeMGMCommand(command);

      command.mCMD = EMGMCommandType::Start;
      replayMgr->mDevice.executeMGMCommand(command);
    }

    replayMgr->mDevice.startDevice();

    return UA_STATUSCODE_GOOD;
  }

  UA_StatusCode ReplayMGR::onReplayNextEvent(UA_Server *,
                                             const UA_NodeId *,
                                             void *,
                                             const UA_NodeId *,
                                             void *methodContext,
                                             const UA_NodeId *,
                                             void *objectContext,
                                             size_t,
                                             const UA_Variant *,
                                             size_t,
                                             UA_Variant *output) {

    auto replayMgr = static_cast<ReplayMGR *>(methodContext);

    auto resourceName = static_cast<const char *>(objectContext);

    if (replayMgr->mDeviceReplayer == nullptr) {
      return UA_STATUSCODE_BADINVALIDSTATE;
    }

    auto nextEvent = replayMgr->mDeviceReplayer->reproduceNextEvent(resourceName);

    UA_String response;
    if (nextEvent.has_value()) {
      auto event = nextEvent.value();
      auto functionBlockName = event.getFB().getFullQualifiedApplicationInstanceName('.');
      const auto interface = event.getFB().getFBInterfaceSpec();
      auto portName = std::string(interface.mEINames[event.getPortId()]);
      response = UA_String_fromChars(std::string(functionBlockName + "." + portName).c_str());
    } else {
      response = UA_String_fromChars("");
    }

    auto status = UA_Variant_setScalarCopy(output, &response, &UA_TYPES[UA_TYPES_STRING]);
    UA_String_clear(&response);

    return status;
  }
} // namespace forte::iec61499::system
