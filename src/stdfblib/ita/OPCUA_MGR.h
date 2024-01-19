/*******************************************************************************
 * Copyright (c) 2022, 2024 Markus Meingast, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Markus Meingast - initial implementation
 *******************************************************************************/

#pragma once

#include "commfb.h"
#include "opcua_local_handler.h"
#include <vector>

class OPCUA_DEV;

class OPCUA_MGR {
public:

  OPCUA_MGR(OPCUA_DEV& paUaDevice);
  
  virtual ~OPCUA_MGR();
  
  EMGMResponse initialize();
  
  bool isInitialized();
private:

  static const std::map<EMGMResponse, UA_StatusCode> scResponseMap;

  static char smEmptyLocale[];
  static char smMgmtType[];
  static char smMgmtName[];
  static char smResType[];

  static const UA_UInt16 smNamespaces[];

  OPCUA_DEV& mUaDevice;
  
  COPC_UA_Local_Handler& mUaHandler;
  
  UA_NodeId mMgmtTypeId;
  
  UA_NodeId mMgmtNodeId;
  
  UA_NodeId mResourceTypeId;
  
  /**** OPCUA Methods ****/
  
  EMGMResponse createIEC61499MgmtObject(UA_Server* paServer);
  
  EMGMResponse defineIEC61499MgmtObjectType(UA_Server* paServer);
  
  EMGMResponse addMgmtObjectInstance();
  
  EMGMResponse createIEC61499ResourceObjectType(UA_Server* paServer);
  
  EMGMResponse defineIEC61499ResourceObjectType(UA_Server* paServer);
  
  EMGMResponse addResourceObjectInstance(char* name, UA_NodeId& paNodeId, UA_NodeId paParentNodeId);
};