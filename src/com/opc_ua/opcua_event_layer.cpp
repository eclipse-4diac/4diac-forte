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
  // TODO
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