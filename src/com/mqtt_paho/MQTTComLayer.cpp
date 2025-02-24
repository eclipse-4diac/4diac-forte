/*******************************************************************************
 * Copyright (c) 2013, 2023 ACIN
 *                          Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Martin Melik Merkumians - initial API and implementation and/or initial documentation
 *                         - Change CIEC_STRING to std::string
 *******************************************************************************/

#include "MQTTComLayer.h"
#include "core/util/parameterParser.h"
#include "MQTTHandler.h"
#include "MQTTClient.h"
#include "commfb.h"
#include <string>

using namespace forte::com_infra;

MQTTComLayer::MQTTComLayer(CComLayer* paUpperLayer, CBaseCommFB* pFB) : CComLayer(paUpperLayer, pFB),
mUsedBuffer(0), mInterruptResp(e_Nothing) {
  memset(mDataBuffer, 0, mBufferSize); //TODO change this to  dataBuffer{0} in the extended list when fully switching to C++11
}

MQTTComLayer::~MQTTComLayer() = default;

EComResponse MQTTComLayer::sendData(void* paData, unsigned int paSize) {
  if (mClient == nullptr) {
    return e_ProcessDataSendFailed;
  }
  int errorCode = mClient->sendData(paData, paSize, mTopicName.c_str());
  if (0 != errorCode) {
    return e_ProcessDataSendFailed;
  }
  //while(deliveredtoken != token);
  return e_ProcessDataOk;
}

EComResponse MQTTComLayer::recvData(const void* paData, unsigned int paSize) {
  if (paSize > mBufferSize) {
    paSize = mBufferSize; //Rest of the message is discarded
  }
  memcpy(mDataBuffer, paData, paSize);
  mUsedBuffer = paSize;
  mInterruptResp = e_ProcessDataOk;
  mFb->interruptCommFB(this);
  return mInterruptResp;
}

EComResponse MQTTComLayer::processInterrupt() {
  if (e_ProcessDataOk == mInterruptResp) {
    if ((0 < mUsedBuffer) && (nullptr != mTopLayer)) {
      mInterruptResp = mTopLayer->recvData(mDataBuffer, mUsedBuffer);
      mUsedBuffer = 0;
    }
  }
  return mInterruptResp;
}

EComResponse MQTTComLayer::openConnection(char* paLayerParameter) {
  EComResponse eRetVal = e_InitInvalidId;
  CParameterParser parser(paLayerParameter, ',', mNoOfParameters);
  if (mNoOfParameters == parser.parseParameters()) {
    mTopicName = parser[Topic];
    if (MQTTHandler::eRegisterLayerSucceeded ==
      getExtEvHandler<MQTTHandler>().registerLayer(parser[Address], parser[ClientID], this)) {
      if (mClient != nullptr) {
        eRetVal = e_InitOk;
      }
    }

    switch (mFb->getComServiceType()) {
    case e_Server:
      // TODO: Not implemented yet
      eRetVal = e_InitTerminated;
      break;
    case e_Client:
      // TODO: Not implemented yet
      eRetVal = e_InitTerminated;
      break;
    case e_Publisher:
      //is handled via sendData
      break;
    case e_Subscriber:
      //handled inside the register layer function in the Handler
      break;
    }
  }
  return eRetVal;
}

void MQTTComLayer::closeConnection() {
  getExtEvHandler<MQTTHandler>().unregisterLayer(this);
}
