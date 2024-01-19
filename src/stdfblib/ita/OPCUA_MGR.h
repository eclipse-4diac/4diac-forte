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

  static char smEmptyLocale[];

  /* Management and Resource Type */
  static char smMgmtType[];
  static char smMgmtName[];
  static char smResType[];

  /* Create Resource */
  static char smCreateResMethodName[];
  static char smCreateResArg1Name[];
  static char smCreateResArg1Description[];
  static char smCreateResArg2Name[];
  static char smCreateResArg2Description[];
  static char smCreateResOutArgName[];
  static char smCreateResOutArgDescription[];
  static char smCreateResAttrDisplayName[];
  static char smCreateResAttrDescription[];

  /* Create FB */
  static char smCreateFBMethodName[];
  static char smCreateFBArg1Name[];
  static char smCreateFBArg1Description[];
  static char smCreateFBArg2Name[];
  static char smCreateFBArg2Description[];
  static char smCreateFBAttrDisplayName[];
  static char smCreateFBAttrDescription[];

  /* Create Connection */
  static char smCreateConnMethodName[];
  static char smCreateConnArg1Name[];
  static char smCreateConnArg1Description[];
  static char smCreateConnArg2Name[];
  static char smCreateConnArg2Description[];
  static char smCreateConnAttrDisplayName[];
  static char smCreateConnAttrDescription[];

  /* Write FB */
  static char smWriteFBMethodName[];
  static char smWriteFBArg1Name[];
  static char smWriteFBArg1Description[];
  static char smWriteFBArg2Name[];
  static char smWriteFBArg2Description[];
  static char smWriteFBAttrDisplayName[];
  static char smWriteFBAttrDescription[];

  /* Start Resource */
  static char smStartResMethodName[];
  static char smStartResArgName[];
  static char smStartResArgDescription[];
  static char smStartResAttrDisplayName[];
  static char smStartResAttrDescription[];

  static const std::map<EMGMResponse, UA_StatusCode> scResponseMap;

  static const UA_UInt16 smNamespaces[];

  OPCUA_DEV& mUaDevice;
  
  COPC_UA_Local_Handler& mUaHandler;

  forte::core::SManagementCMD mCommand;
  
  UA_NodeId mMgmtTypeId;
  
  UA_NodeId mMgmtNodeId;
  
  UA_NodeId mResourceTypeId;

  std::map<std::string, UA_NodeId> resourceNodeMap;
  
  /**** OPCUA Methods ****/
  
  EMGMResponse createIEC61499MgmtObject(UA_Server* paServer);
  
  EMGMResponse defineIEC61499MgmtObjectType(UA_Server* paServer);
  
  EMGMResponse addMgmtObjectInstance();
  
  EMGMResponse createIEC61499ResourceObjectType(UA_Server* paServer);
  
  EMGMResponse defineIEC61499ResourceObjectType(UA_Server* paServer);
  
  EMGMResponse addResourceObjectInstance(char* name, UA_NodeId& paNodeId, UA_NodeId paParentNodeId);

  EMGMResponse addCreateResourceMethod(UA_Server* paServer);
  static UA_StatusCode onCreateResource(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);
  
  EMGMResponse addCreateFBMethod(UA_Server* paServer);
  static UA_StatusCode onCreateFB(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);
  
  EMGMResponse addCreateConnectionMethod(UA_Server* paServer);
  static UA_StatusCode onCreateConnection(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addWriteFBMethod(UA_Server* paServer);
  static UA_StatusCode onWriteFB(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addStartResourceMethod(UA_Server* paServer);
  static UA_StatusCode onStartResource(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

/* Helpers */

  EMGMResponse addMethodNode(UA_Server* paServer, char* paMethodName, UA_NodeId paParentNodeId, 
    UA_MethodAttributes paAttr, const UA_Argument *paInArgs, size_t paInArgSize, const UA_Argument *paOutArgs, 
    size_t paOutArgSize, UA_MethodCallback paCallback);

  void initArgument(UA_Argument& paArgument, int paTypeId, char* paName, char* paDescription);

  UA_MethodAttributes createAttribute(char* paDisplayName, char* paDescription);

  UA_StatusCode addReference(UA_Server* paServer, UA_NodeId& paMethodId);

  void clearMGMCommand();

  void setMGMCommand(EMGMCommandType paCMD, CStringDictionary::TStringId paDestination,
    const char* paAdditionalParams, const char* paFirstParam, 
    const char* paSecondParam);

  void setMGMCommand(EMGMCommandType paCMD, CStringDictionary::TStringId paDestination,
    const char* paAdditionalParams, std::vector<std::string>& paFirstParam, 
    std::vector<std::string> paSecondParam = std::vector<std::string>());

  static std::string getInputValue(UA_String paInput);

  static void parseDestinationName(std::string& paDestination, std::vector<std::string>& paTarget);
};