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

#include "DebugMGR.h"

#include "core/ecetFactory.h"
#include "core/ecetFake.h"

DebugMGR::DebugMGR(CDevice& paDevice, OPCUA_MGR& paOpcuaMgr) : 
  mDevice(paDevice), mOpcuaMgr(paOpcuaMgr) {
  // we need the fake ecet to debug control the device remotely
  EcetFactory::setEcetToCreate(EcetFactory::AvailableEcets::fake);
}

bool DebugMGR::initialize(){
  addRemoteControlMethod();
  addTriggerNextEventMethod();
  addAddBreakpointMethod();
  addRemoveBreakpointMethod();

  return true;
}

std::string& DebugMGR::getArgumentString(std::string paString) {
  return mArgumentsInformation.emplace_back(std::move(paString));
}

void DebugMGR::iterateResources(ResourceIteratorCallback paCallback){
  for(auto child : mDevice.getChildren()){
    if(auto resource = static_cast<CResource*>(child); // the first generation of children under the device are always resources
        !paCallback(resource, static_cast<CFakeEventExecutionThread*>(resource->getResourceEventExecution()))){
      break;
    }
  }
}

std::optional<TEventEntry> DebugMGR::getEventEntry(CResource* paResource, std::string paDestination){
  forte::core::TNameIdentifier fullName;

  // fill out the TNameIdentifier
  size_t index = paDestination.find_first_of(".");
  while (index != std::string::npos) {
    auto currentPart = paDestination.substr(0, index); 
    fullName.pushBack(CStringDictionary::getInstance().insert(currentPart.c_str()));
    paDestination = paDestination.substr(currentPart.length() + 1);
    index = paDestination.find_first_of(".");
  }
  fullName.pushBack(CStringDictionary::getInstance().insert(paDestination.substr(0, index).c_str()));

  if(fullName.size() < 2){ // at least the FB and the event port must be present
    return std::nullopt;
  }

  // separate the fb name from the event id
  auto eventId = fullName.back();
  fullName.popBack();

  forte::core::TNameIdentifier::CIterator it(fullName, 0);
  auto functionBlock = paResource->getFB(it);
  if(functionBlock == nullptr){
    // no function block found
    return std::nullopt;
  }

  auto portID = functionBlock->getEIID(eventId);
  if(portID == cgInvalidPortId){
    // no port ID found in the function block
    return std::nullopt;
  }

  return TEventEntry{functionBlock, portID};
}

void DebugMGR::addRemoteControlMethod(){
  OPCUA_MGR::MethodInformation newMethod;

  newMethod.mMethodName = "Remote Control";
  newMethod.mDisplayName = "Remote Control";
  newMethod.mDescription = "Set control of the device from a remote source or not";
  newMethod.mCallback = &DebugMGR::onSetRemoteControl;
  newMethod.mNodeContext = this;

  newMethod.mInArguments.push_back(UA_Argument());

  OPCUA_MGR::initArgument(
    newMethod.mInArguments[0], 
    UA_TYPES_BOOLEAN, 
    getArgumentString("Enable Remote Control").data(), 
    getArgumentString("True if the remote control should be enabled, false if it should be disabled").data());

  mOpcuaMgr.addExtraMgmMethod(newMethod);
}

void DebugMGR::addTriggerNextEventMethod(){
  OPCUA_MGR::MethodInformation newMethod;

  newMethod.mMethodName = "Trigger Next Event";
  newMethod.mDisplayName = "Trigger Next Event";
  newMethod.mDescription = "Trigger the next event in the resource";
  newMethod.mCallback = &DebugMGR::onTriggerNextEvent;
  newMethod.mNodeContext = this;
  
  mOpcuaMgr.addExtraResourceMethod(newMethod);
}

void DebugMGR::addAddBreakpointMethod() {
OPCUA_MGR::MethodInformation newMethod;

  newMethod.mMethodName = "Add Breakpoint";
  newMethod.mDisplayName = "Add Breakpoint";
  newMethod.mDescription = "Add a breakpoint";
  newMethod.mCallback = &DebugMGR::onAddBreakpoint;
  newMethod.mNodeContext = this;

  newMethod.mInArguments.push_back(UA_Argument());

  OPCUA_MGR::initArgument(
    newMethod.mInArguments[0], 
    UA_TYPES_STRING, 
    getArgumentString("Port Name").data(), 
    getArgumentString("Full name of the port inside the resource").data());
  
  mOpcuaMgr.addExtraResourceMethod(newMethod);
}

void DebugMGR::addRemoveBreakpointMethod() {
OPCUA_MGR::MethodInformation newMethod;

  newMethod.mMethodName = "Remove Breakpoint";
  newMethod.mDisplayName = "Remove Breakpoint";
  newMethod.mDescription = "Remove a breakpoint";
  newMethod.mCallback = &DebugMGR::onRemoveBreakpoint;
  newMethod.mNodeContext = this;

  newMethod.mInArguments.push_back(UA_Argument());

  OPCUA_MGR::initArgument(
    newMethod.mInArguments[0], 
    UA_TYPES_STRING, 
    getArgumentString("Port Name").data(), 
    getArgumentString("Full name of the port inside the resource").data());
  
  mOpcuaMgr.addExtraResourceMethod(newMethod);
}

UA_StatusCode DebugMGR::onSetRemoteControl(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void*,
  size_t, const UA_Variant* input,
  size_t, UA_Variant* ) {

  auto debugMgr = static_cast<DebugMGR*>(methodContext);
  const auto enableRemoteControl = *static_cast<UA_Boolean*>(input[0].data);

  debugMgr->iterateResources([enableRemoteControl](CResource*, CFakeEventExecutionThread* paEcet){
     enableRemoteControl ? paEcet->takeExternalControl() : paEcet->removeExternalControl();
     return true;
  });

  return UA_STATUSCODE_GOOD;
}

UA_StatusCode DebugMGR::onTriggerNextEvent(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void* objectContext,
  size_t, const UA_Variant*,
  size_t, UA_Variant*) {

  auto debugMgr = static_cast<DebugMGR*>(methodContext);

  auto resourceName = static_cast<const char*>(objectContext);
  bool wasTriggered = false;
  debugMgr->iterateResources([resourceName, &wasTriggered](CResource* paResource, CFakeEventExecutionThread* paEcet){
    if(strcmp(paResource->getInstanceName(), resourceName) != 0){
      // not the resource we are looking for
      return true;
    }

    paEcet->triggerNextEvent();
    wasTriggered = true;
    return false;
  });

  if(!wasTriggered){
    return UA_STATUSCODE_BADNOTSUPPORTED;
  }

  return UA_STATUSCODE_GOOD;
}

UA_StatusCode DebugMGR::onAddBreakpoint(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void* objectContext,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*){

  auto debugMgr = static_cast<DebugMGR*>(methodContext);
  auto resourceName = static_cast<const char*>(objectContext);
  UA_StatusCode result = UA_STATUSCODE_BADINVALIDARGUMENT;

  // look for the resource in the device, and inside of it find the PortId of the Function block, if they exist
  debugMgr->iterateResources([debugMgr, resourceName, input, &result](CResource* paResource, CFakeEventExecutionThread* paEcet){
    
    if(strcmp(paResource->getInstanceName(), resourceName) != 0){
      // not the resource we are looking for
      return true;
    }

    auto uaStringInput = static_cast<UA_String*>(input[0].data);
    if(auto desiredEventEntry = debugMgr->getEventEntry(paResource, std::string((const char*)uaStringInput->data, uaStringInput->length)); 
          desiredEventEntry.has_value()){
      paEcet->addBreakpoint(desiredEventEntry.value());
      result = UA_STATUSCODE_GOOD;
    }

    // setting the breakpoint callback needs to be done only once per ecet, but
    // since we don't get a callback everytime a resource is created,
    // we need to set the breakpoint hit callback eveytime a breakpoint is added
    // the callback takes external control of all ecets in the device
    paEcet->setBreakpointHitCallback([debugMgr](TEventEntry paEntry){
      (void) paEntry;   // we don't do anything with the current breakpoint. We could set something later with this info
                        
      // if a breakpoint is hit, halt all resources
      debugMgr->iterateResources([](CResource*, CFakeEventExecutionThread* paCurrentEcet){
        paCurrentEcet->takeExternalControl();
        return true;
      });
    });

    return false;
  });

  return result;
}

UA_StatusCode DebugMGR::onRemoveBreakpoint(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void* objectContext,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*){
    
  auto debugMgr = static_cast<DebugMGR*>(methodContext);
  auto resourceName = static_cast<const char*>(objectContext);
  UA_StatusCode result = UA_STATUSCODE_BADINVALIDARGUMENT;

  // look for the resource in the device, and inseide of it find the PortId of the Function block, if they exist
  debugMgr->iterateResources([debugMgr, resourceName, input, &result](CResource* paResource, CFakeEventExecutionThread* paEcet){
    if(strcmp(paResource->getInstanceName(), resourceName) != 0){
      // not the resource we are looking for
      return true;
    }

    auto uaStringInput = static_cast<UA_String*>(input[0].data);
    if(auto desiredEventEntry = debugMgr->getEventEntry(paResource, std::string((const char*)uaStringInput->data, uaStringInput->length)); 
        desiredEventEntry.has_value()){
      paEcet->removeBreakpoint(desiredEventEntry.value());
      result = UA_STATUSCODE_GOOD;
    }

    return false;
  });

  return result;
}
