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

//raw[].mqtt[tcp://localhost:1883, ClientID, Topic]

using namespace forte::com_infra;

class MQTTComLayer: public forte::com_infra::CComLayer{
public:
	MQTTComLayer(CComLayer* paUpperLayer, CBaseCommFB * paFB);
	virtual ~MQTTComLayer();

	EComResponse sendData(void* paData, unsigned int paSize);

	EComResponse recvData(const void *paData, unsigned int paSize);

	EComResponse processInterrupt();

	char const* getTopicName() {
		return mTopicName;
	}

private:
	char* mTopicName;

  static const unsigned int mNoOfParameters = 3;
  static const unsigned int mBufferSize = 255;

  char mDataBuffer[mBufferSize];
  unsigned int mUsedBuffer;
  EComResponse mInterruptResp;

	EComResponse openConnection(char* paLayerParameter);
	void closeConnection();

	enum Parameters {
		Address,
		ClientID,
		Topic
	};

};

#endif /* MQTTCOMLAYER_H_ */
