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

#pragma once

#include "OPCUA_MGR.h"

#include <functional>
#include <string>
#include <vector>
#include <optional>

class CDevice;
class CResource;
class CFakeEventExecutionThread;

/**
 * @brief Gets a OPCUA_MGR object and adds device and resource methods on top of that
 * Device-level Methods:
 * - Remote Control (bool): void -> Allows enabling/disabling remote control of the device. Pause/Continue buttons in regular debuggers
 * 
 * Resource-level methods:
 * - Trigger Next Event: void -> Execute one event in a resource. Step button in regular debuggers
 * - Add Breakpoint (string): void -> Adds a breakpoint at certain input port of a function block.
 * - Remove Breakpoint (string): void -> Removes a breakpoint at certain input port of a function block.
 */
class DebugMGR {

public:

  DebugMGR(CDevice& paDevice, OPCUA_MGR& paOpcuaMgr);
  ~DebugMGR() = default;
  
  /**
   * @brief Add debugging methods to the OPCUA_MGR object
   * 
   * @return true if no problem occurred while initializing, false otherwise.  
   */
  bool initialize();

private:

  // device on which the methods will be executed
  CDevice& mDevice;
  
  // OpcUa Mgr on top of which the extra debugging methods will be added
  OPCUA_MGR& mOpcuaMgr;


  // the definition of the methods (and therefore its arguments) is done in different functions
  // therefore we need to store the strings for the the arguments so they live until
  // the methods are created in the opc ua server
  std::vector<std::string> mArgumentsInformation;

  /**
   * @brief Store a string and get a reference to it
   * 
   * @param paString String to look for from the stored ones
   * @return reference to the stored string 
   */
  std::string& getArgumentString(std::string paString);

  /**
   * @brief Callback when iterating over the resources of the device. It return true if the iteration should continue,
   * false otherwise. 
   */
  typedef std::function<bool(CResource*, CFakeEventExecutionThread*)> ResourceIteratorCallback;

  /**
   * @brief Iterates over the resources of the device and calls the callback for each one
   * 
   * @param paCallback Function to call for each resource
   */
  void iterateResources(ResourceIteratorCallback paCallback);

  /**
   * @brief Given a resource and the full name to a port, get the EventEntry corresponding to the function block and port
   * 
   * @param paResource resource where to look for the EventEntry  
   * @param paDestination String reprensantion of the full name until the port ID separated by dots
   * @return the Event Entry if the function block and port id are found, std::nullopt otherwise
   */
  std::optional<TEventEntry> getEventEntry(CResource* paResource, std::string paDestination);

  void addRemoteControlMethod();

  void addTriggerNextEventMethod();

  void addAddBreakpointMethod();

  void addRemoveBreakpointMethod();

  static UA_StatusCode onSetRemoteControl(UA_Server*,
    const UA_NodeId*, void*,
    const UA_NodeId*, void*,
    const UA_NodeId*, void*,
    size_t, const UA_Variant* input,
    size_t, UA_Variant*);

  static UA_StatusCode onTriggerNextEvent(UA_Server*,
    const UA_NodeId*, void*,
    const UA_NodeId*, void* methodContext,
    const UA_NodeId*, void* objectContext,
    size_t, const UA_Variant*,
    size_t, UA_Variant*);

  static UA_StatusCode onAddBreakpoint(UA_Server*,
    const UA_NodeId*, void*,
    const UA_NodeId*, void* methodContext,
    const UA_NodeId*, void* objectContext,
    size_t, const UA_Variant* input,
    size_t, UA_Variant*);

  static UA_StatusCode onRemoveBreakpoint(UA_Server*,
    const UA_NodeId*, void*,
    const UA_NodeId*, void* methodContext,
    const UA_NodeId*, void* objectContext,
    size_t, const UA_Variant* input,
    size_t, UA_Variant*);
};
