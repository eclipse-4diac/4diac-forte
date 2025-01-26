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

#include "opcua_local_handler.h"
#include "core/mgmcmd.h"

#include <vector>
#include <string>

class CDevice;

class OPCUA_MGR {
public:

  OPCUA_MGR(CDevice& paUaDevice);
  
  virtual ~OPCUA_MGR();
  
  EMGMResponse initialize();
  
  bool isInitialized();

  static void initArgument(UA_Argument& paArgument, int paTypeId, char* paName, char* paDescription);

  struct MethodInformation {
    std::string mMethodName;
    std::string mDisplayName;
    std::string mDescription;
    std::vector<UA_Argument> mInArguments;
    std::vector<UA_Argument> mOutArguments;
    UA_MethodCallback mCallback;
    void* mNodeContext;
  };

  void addExtraMgmMethod(const MethodInformation& paMethod);

  void addExtraResourceMethod(const MethodInformation& paMethod);

private:

  static char smEmptyLocale[];

  /* Management and Resource Type */
  static char smMgmtType[];
  static char smMgmtName[];
  static char smResType[];

  /* Write Device */
  static char smWriteDevMethodName[];
  static char smWriteDevArgName[];
  static char smWriteDevArgDescription[];
  static char smWriteDevAttrDisplayName[];
  static char smWriteDevAttrDescription[];

  /* Start Device */
  static char smStartDevMethodName[];
  static char smStartDevAttrDisplayName[];
  static char smStartDevAttrDescription[];

  /* Stop Device */
  static char smStopDevMethodName[];
  static char smStopDevAttrDisplayName[];
  static char smStopDevAttrDescription[];

  /* Reset Device */
  static char smResetDevMethodName[];
  static char smResetDevAttrDisplayName[];
  static char smResetDevAttrDescription[];

  /* Kill Device */
  static char smKillDevMethodName[];
  static char smKillDevAttrDisplayName[];
  static char smKillDevAttrDescription[];

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

  /* Write Resource */
  static char smWriteResMethodName[];
  static char smWriteResArg1Name[];
  static char smWriteResArg1Description[];
  static char smWriteResArg2Name[];
  static char smWriteResArg2Description[];
  static char smWriteResAttrDisplayName[];
  static char smWriteResAttrDescription[];

  /* Start Resource */
  static char smStartResMethodName[];
  static char smStartResArgName[];
  static char smStartResArgDescription[];
  static char smStartResAttrDisplayName[];
  static char smStartResAttrDescription[];

  /* Stop Resource */
  static char smStopResMethodName[];
  static char smStopResArgName[];
  static char smStopResArgDescription[];
  static char smStopResAttrDisplayName[];
  static char smStopResAttrDescription[];

  /* Reset Resource */
  static char smResetResMethodName[];
  static char smResetResArgName[];
  static char smResetResArgDescription[];
  static char smResetResAttrDisplayName[];
  static char smResetResAttrDescription[];

  /* Kill Resource */
  static char smKillResMethodName[];
  static char smKillResArgName[];
  static char smKillResArgDescription[];
  static char smKillResAttrDisplayName[];
  static char smKillResAttrDescription[];

  /* Delete Resource */
  static char smDeleteResMethodName[];
  static char smDeleteResArgName[];
  static char smDeleteResArgDescription[];
  static char smDeleteResAttrDisplayName[];
  static char smDeleteResAttrDescription[];

  /* Create FB */
  static char smCreateFBMethodName[];
  static char smCreateFBArg1Name[];
  static char smCreateFBArg1Description[];
  static char smCreateFBArg2Name[];
  static char smCreateFBArg2Description[];
  static char smCreateFBAttrDisplayName[];
  static char smCreateFBAttrDescription[];

  /* Write FB */
  static char smWriteFBMethodName[];
  static char smWriteFBArg1Name[];
  static char smWriteFBArg1Description[];
  static char smWriteFBArg2Name[];
  static char smWriteFBArg2Description[];
  static char smWriteFBAttrDisplayName[];
  static char smWriteFBAttrDescription[];

  /* Start FB */
  static char smStartFBMethodName[];
  static char smStartFBArg1Name[];
  static char smStartFBArg1Description[];
  static char smStartFBAttrDisplayName[];
  static char smStartFBAttrDescription[];

  /* Stop FB */
  static char smStopFBMethodName[];
  static char smStopFBArg1Name[];
  static char smStopFBArg1Description[];
  static char smStopFBAttrDisplayName[];
  static char smStopFBAttrDescription[];

  /* Reset FB */
  static char smResetFBMethodName[];
  static char smResetFBArg1Name[];
  static char smResetFBArg1Description[];
  static char smResetFBAttrDisplayName[];
  static char smResetFBAttrDescription[];

  /* Kill FB */
  static char smKillFBMethodName[];
  static char smKillFBArg1Name[];
  static char smKillFBArg1Description[];
  static char smKillFBAttrDisplayName[];
  static char smKillFBAttrDescription[];

  /* Delete FB */
  static char smDeleteFBMethodName[];
  static char smDeleteFBArg1Name[];
  static char smDeleteFBArg1Description[];
  static char smDeleteFBAttrDisplayName[];
  static char smDeleteFBAttrDescription[];

  /* Create Connection */
  static char smCreateConnMethodName[];
  static char smCreateConnArg1Name[];
  static char smCreateConnArg1Description[];
  static char smCreateConnArg2Name[];
  static char smCreateConnArg2Description[];
  static char smCreateConnAttrDisplayName[];
  static char smCreateConnAttrDescription[];

  /* Delete Connection */
  static char smDeleteConnMethodName[];
  static char smDeleteConnArg1Name[];
  static char smDeleteConnArg1Description[];
  static char smDeleteConnArg2Name[];
  static char smDeleteConnArg2Description[];
  static char smDeleteConnAttrDisplayName[];
  static char smDeleteConnAttrDescription[];

  /* Query Resources */
  static char smQueryResourcesMethodName[];
  static char smQueryResourcesDisplayName[];
  static char smQueryResourcesDescription[];
  static char smQueryResourcesOutArgName[];
  static char smQueryResourcesOutArgDescription[];

/* FORTE Monitoring */
#ifdef FORTE_SUPPORT_MONITORING

  /* Add Watch */
  static char smAddWatchMethodName[];
  static char smAddWatchArgName[];
  static char smAddWatchArgDescription[];
  static char smAddWatchAttrDisplayName[];
  static char smAddWatchAttrDescription[];

  /* Read Watches */
  static char smReadWatchesMethodName[];
  static char smReadWatchesDisplayName[];
  static char smReadWatchesDescription[];
  static char smReadWatchesOutArgName[];
  static char smReadWatchesOutArgDescription[];

  /* Remove Watch */
  static char smRemoveWatchMethodName[];
  static char smRemoveWatchArgName[];
  static char smRemoveWatchArgDescription[];
  static char smRemoveWatchAttrDisplayName[];
  static char smRemoveWatchAttrDescription[];

  /* Trigger Event */
  static char smTriggerEventMethodName[];
  static char smTriggerEventArgName[];
  static char smTriggerEventArgDescription[];
  static char smTriggerEventAttrDisplayName[];
  static char smTriggerEventAttrDescription[];
  static const char* scmTriggerEventParam;

  /* Force Value */
  static char smForceValueMethodName[];
  static char smForceValueArg1Name[];
  static char smForceValueArg1Description[];
  static char smForceValueArg2Name[];
  static char smForceValueArg2Description[];
  static char smForceValueAttrDisplayName[];
  static char smForceValueAttrDescription[];

  /* Clear Force */
  static char smClearForceMethodName[];
  static char smClearForceArgName[];
  static char smClearForceArgDescription[];
  static char smClearForceAttrDisplayName[];
  static char smClearForceAttrDescription[];

#endif // FORTE_SUPPORT_MONITORING

  static const std::map<EMGMResponse, UA_StatusCode> scResponseMap;

  static const UA_UInt16 smNamespaces[];

  CDevice& mUaDevice;
  
  COPC_UA_Local_Handler& mUaHandler;

  forte::core::SManagementCMD mCommand;
  
  UA_NodeId mMgmtTypeId;
  
  UA_NodeId mMgmtNodeId;
  
  UA_NodeId mResourceTypeId;

  std::map<std::string, UA_NodeId> resourceNodeMap;

  std::vector<MethodInformation> mExtraMgmMethods;

  std::vector<MethodInformation> mExtraResourceMethods;
  
  /**** OPCUA Methods ****/
  
  EMGMResponse createIEC61499MgmtObject(UA_Server* paServer);
  
  EMGMResponse defineIEC61499MgmtObjectType(UA_Server* paServer);
  
  EMGMResponse addMgmtObjectInstance();
  
  EMGMResponse createIEC61499ResourceObjectType(UA_Server* paServer);
  
  EMGMResponse defineIEC61499ResourceObjectType(UA_Server* paServer);
  
  EMGMResponse addResourceObjectInstance(char* name, UA_NodeId& paNodeId, UA_NodeId paParentNodeId);

  EMGMResponse removeResourceObjectInstance(std::string resourceName);

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

  EMGMResponse addWriteDeviceMethod(UA_Server* paServer);
  static UA_StatusCode onWriteDevice(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addWriteResourceMethod(UA_Server* paServer);
  static UA_StatusCode onWriteResource(UA_Server* server,
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

  EMGMResponse addStartDeviceMethod(UA_Server* paServer);
  static UA_StatusCode onStartDevice(UA_Server* server,
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

  EMGMResponse addStartFBMethod(UA_Server* paServer);
  static UA_StatusCode onStartFB(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addStopDeviceMethod(UA_Server* paServer);
  static UA_StatusCode onStopDevice(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addStopResourceMethod(UA_Server* paServer);
  static UA_StatusCode onStopResource(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addStopFBMethod(UA_Server* paServer);
  static UA_StatusCode onStopFB(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addResetDeviceMethod(UA_Server* paServer);
  static UA_StatusCode onResetDevice(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addResetResourceMethod(UA_Server* paServer);
  static UA_StatusCode onResetResource(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addResetFBMethod(UA_Server* paServer);
  static UA_StatusCode onResetFB(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addKillDeviceMethod(UA_Server* paServer);
  static UA_StatusCode onKillDevice(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addKillResourceMethod(UA_Server* paServer);
  static UA_StatusCode onKillResource(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addKillFBMethod(UA_Server* paServer);
  static UA_StatusCode onKillFB(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addDeleteResourceMethod(UA_Server* paServer);
  static UA_StatusCode onDeleteResource(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addDeleteFBMethod(UA_Server* paServer);
  static UA_StatusCode onDeleteFB(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addDeleteConnectionMethod(UA_Server* paServer);
  static UA_StatusCode onDeleteConnection(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addQueryResourcesMethod(UA_Server* paServer);
  static UA_StatusCode onQueryResources(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

/* FORTE Monitoring */

#ifdef FORTE_SUPPORT_MONITORING

  EMGMResponse addAddWatchMethod(UA_Server* paServer);
  static UA_StatusCode onAddWatch(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addReadWatchesMethod(UA_Server* paServer);
  static UA_StatusCode onReadWatches(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);    

  EMGMResponse addRemoveWatchMethod(UA_Server* paServer);
  static UA_StatusCode onRemoveWatch(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addTriggerEventMethod(UA_Server* paServer);
  static UA_StatusCode onTriggerEvent(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addForceValueMethod(UA_Server* paServer);
  static UA_StatusCode onForceValue(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

  EMGMResponse addClearForceMethod(UA_Server* paServer);
  static UA_StatusCode onClearForce(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionHandle,
    const UA_NodeId* methodId, void* methodContext,
    const UA_NodeId* objectId, void* objectContext,
    size_t inputSize, const UA_Variant* input,
    size_t outputSize, UA_Variant* output);

#endif // FORTE_SUPPORT_MONITORING

/* Helpers */

  EMGMResponse addMethodNode(UA_Server* paServer, char* paMethodName, UA_NodeId paParentNodeId, 
    UA_MethodAttributes paAttr, const UA_Argument *paInArgs, size_t paInArgSize, const UA_Argument *paOutArgs, 
    size_t paOutArgSize, UA_MethodCallback paCallback, void* nodeContext = nullptr);

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

  static void parseDestinationName(const std::string& paDestination, std::vector<std::string>& paTarget);

  EMGMResponse addExtraMethods(UA_Server* paServer, UA_NodeId paParentNodeId, std::vector<MethodInformation>& paMethods);
};