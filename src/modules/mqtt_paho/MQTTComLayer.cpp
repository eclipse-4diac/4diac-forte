/*******************************************************************************
 * Copyright (c) 2013, 2014 ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 * Martin Melik Merkumians - initial API and implementation and/or initial documentation
 *******************************************************************************/


#include "MQTTComLayer.h"
#include "MQTTParameterParser.h"
#include "MQTTHandler.h"
#include "commfb.h"
#include <unistd.h>

using namespace forte::com_infra;

MQTTComLayer::MQTTComLayer(CComLayer* pa_poUpperLayer, CCommFB * pa_poFB) : CComLayer(pa_poUpperLayer, pa_poFB),
    mTopicName(0), bufferSize(0), m_eInterruptResp(e_Nothing){
  memset(dataBuffer, 0, sizeof(dataBuffer)); //TODO change this to  dataBuffer{0} in the extended list when fully switching to C++11
	// TODO Auto-generated constructor stub

}

MQTTComLayer::~MQTTComLayer() {
	if(mTopicName){
	  delete mTopicName;
	}
}

EComResponse MQTTComLayer::sendData(void* pa_pvData, unsigned int pa_unSize) {
	MQTTAsync_message message = MQTTAsync_message_initializer;
	message.payload = pa_pvData;
	message.payloadlen = pa_unSize;
	message.qos = QOS;
	message.retained = 0;
	int errorCode = MQTTAsync_sendMessage(MQTTHandler::getInstance().getClient(), mTopicName, &message, NULL);
	if (0 != errorCode) {
		return e_ProcessDataSendFailed;
	}
	//while(deliveredtoken != token);
	return e_ProcessDataOk;
}

EComResponse MQTTComLayer::recvData(const void* pa_pvData,
		unsigned int pa_unSize) {
	memcpy(dataBuffer, pa_pvData, pa_unSize);
	bufferSize = pa_unSize;
	m_eInterruptResp = e_ProcessDataOk;
	m_poFb->interruptCommFB(this);
	return m_eInterruptResp;
}

EComResponse MQTTComLayer::processInterrupt() {
	if(e_ProcessDataOk == m_eInterruptResp) {
		if((0 < bufferSize) && (0 != m_poTopLayer)) {
			m_eInterruptResp = m_poTopLayer->recvData(dataBuffer, bufferSize);
			bufferSize = 0;
			//MQTTHandler::getInstance().mqttMessageProcessed();
		}
	}

	return m_eInterruptResp;
}

EComResponse MQTTComLayer::openConnection(char* pa_acLayerParameter) {
	EComResponse eRetVal = e_InitInvalidId;
	MQTTParameterParser parser(pa_acLayerParameter);
	parser.setSeparator(',');
	if(3 == parser.parseParameters()){
		mTopicName = new char[(strlen(parser[Topic]) + 1)];
		mTopicName = strcpy(mTopicName, parser[Topic]);
		if( MQTTHandler::eRegisterLayerSucceeded ==
				(MQTTHandler::getInstance()).registerLayer(parser[Address], parser[ClientID], this)) {
			eRetVal = e_InitOk;
		}
		else eRetVal = e_InitInvalidId;

		switch (m_poFb->getComServiceType()){
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
		  MQTTHandler::addLayerToSubscribe(this);
			break;
		}
	}

	return eRetVal;
}

void MQTTComLayer::closeConnection() {
	MQTTHandler::getInstance().unregisterLayer(this);
}
