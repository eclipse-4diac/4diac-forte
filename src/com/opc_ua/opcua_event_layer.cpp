/*******************************************************************************
 * Copyright (c) 2024 Primetals Technologies Austria GmbH
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

#include "opcua_event_layer.h"
#include "../../core/utils/parameterParser.h"
#include "../../core/cominfra/basecommfb.h"
#include "opcua_local_handler.h"

using namespace forte::com_infra;

char COPC_UA_Event_Layer::smEmptyString[] = "";

COPC_UA_Event_Layer::COPC_UA_Event_Layer(CComLayer *paUpperLayer, CBaseCommFB *paComFB) :
  CComLayer(paUpperLayer, paComFB), mHandler(nullptr) {

}

COPC_UA_Event_Layer::~COPC_UA_Event_Layer() = default;


EComResponse COPC_UA_Event_Layer::openConnection(char *paLayerParameter) {
  EComResponse eRetVal = e_InitTerminated;
  CParameterParser parser(paLayerParameter, ',', 1);
  size_t nrOfParams = parser.parseParameters();
  if(nrOfParams != scmNumberOfParameters) {
    DEVLOG_ERROR("[OPC UA EVENT LAYER]: Too many layer arguments! Number of arguments: %d", nrOfParams);
    return eRetVal;
  }
  char* eventTypeName = new char[strlen(parser[0]) + 1];
  strncpy(eventTypeName, parser[0], strlen(parser[0]));
  eventTypeName[strlen(parser[0])] = '\0';

  mHandler = static_cast<COPC_UA_HandlerAbstract*>(&getExtEvHandler<COPC_UA_Local_Handler>());
  COPC_UA_Local_Handler* localHandler = static_cast<COPC_UA_Local_Handler*>(mHandler);
  localHandler->enableHandler();
  UA_NodeId eventType;
  UA_StatusCode status = addNewEventType(localHandler->getUAServer(), eventType, eventTypeName);
  if(status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA EVENT LAYER]: Failed to add EventType. Status: %s", UA_StatusCode_name(status));
    return eRetVal;
  }
  return e_InitOk;
}

void COPC_UA_Event_Layer::closeConnection() {
  // TODO
}

EComResponse COPC_UA_Event_Layer::recvData(const void *paData, unsigned int paSize) {
  return e_ProcessDataOk;
}

EComResponse COPC_UA_Event_Layer::sendData(void *paData, unsigned int paSize) {
  // TODO
  return e_ProcessDataOk;
}

EComResponse COPC_UA_Event_Layer::processInterrupt() {
  // TODO
  return e_ProcessDataOk;
}

UA_StatusCode COPC_UA_Event_Layer::addNewEventType(UA_Server *server, UA_NodeId &paEventType, char* eventTypeName) {
    UA_ObjectTypeAttributes attr = UA_ObjectTypeAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT(smEmptyString, eventTypeName);
    return UA_Server_addObjectTypeNode(server, UA_NODEID_NULL,
                                       UA_NODEID_NUMERIC(0, UA_NS0ID_BASEEVENTTYPE),
                                       UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                       UA_QUALIFIEDNAME(0, eventTypeName),
                                       attr, NULL, &paEventType);
}