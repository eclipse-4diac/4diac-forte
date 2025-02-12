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

#include "open62541.h"

#include <vector>
#include <memory>
#include <future> // to indicate that the this device should die 

class MultiDevice;
class OPCUA_MGR;

class MultiMGR {
public:

  MultiMGR(MultiDevice& paDevice, OPCUA_MGR& paOpcuaMgr);
  ~MultiMGR() = default;

  bool initialize();

private:

  MultiDevice& mDevice;

  OPCUA_MGR& mOpcuaMgr;

  // For restart signal coming from the OPCUA server
  // killing the device will wait for the opcua handler to finish before returning
  // so it the method callback will block.
  // The killing of the device (and the opcua server in the handler) is made asynchonously.
  // the last future referencing the shared state with the promise of the async will block untill
  // the async finishes, so we move the future into this class outside of the opcua callback,
  // so the callback does not block. 
  std::future<void> mRestartSignalHandler;

  // the arguments strings are passed by pointer and not stored, so they should leave until
  // the methods are created
  std::vector<std::string> mArgumentsInformation;

  std::string& getArgumentString(std::string paString);

  void addRestartDeviceMethod();

  void addSetDefaultDeviceMethod();

  static UA_StatusCode onRestartDevice(UA_Server*,
    const UA_NodeId*, void*,
    const UA_NodeId*, void* methodContext,
    const UA_NodeId*, void*,
    size_t, const UA_Variant*,
    size_t, UA_Variant*);

  static UA_StatusCode onSetDefaultDevice(UA_Server*,
    const UA_NodeId*, void*,
    const UA_NodeId*, void*,
    const UA_NodeId*, void*,
    size_t, const UA_Variant* input,
    size_t, UA_Variant*);

};
