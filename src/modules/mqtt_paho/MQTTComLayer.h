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


#ifndef MQTTCOMLAYER_H_
#define MQTTCOMLAYER_H_

#include "comlayer.h"
extern "C" {
	#include <MQTTAsync.h>
}

#define QOS 0

//fbdk[].mqtt[tcp://localhost:1883, ClientID, Topic]

using namespace forte::com_infra;

class MQTTComLayer: public forte::com_infra::CComLayer{
public:
	MQTTComLayer(CComLayer* pa_poUpperLayer, CBaseCommFB * pa_poFB);
	virtual ~MQTTComLayer();

	EComResponse sendData(void* pa_pvData, unsigned int pa_unSize);

	EComResponse recvData(const void *pa_pvData, unsigned int pa_unSize);

	EComResponse processInterrupt();

	char const* getTopicName() {
		return mTopicName;
	}

private:
	char* mTopicName;

    char dataBuffer[255];
    unsigned int bufferSize;
    EComResponse m_eInterruptResp;

	EComResponse openConnection(char* pa_acLayerParameter);
	void closeConnection();

	enum Parameters {
		Address,
		ClientID,
		Topic
	};
};

#endif /* MQTTCOMLAYER_H_ */
