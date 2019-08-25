/*******************************************************************************
 * Copyright (c) 2013, 2014 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Martin Melik Merkumians - initial API and implementation and/or initial documentation
 *******************************************************************************/


#ifndef MQTTCOMLAYER_H_
#define MQTTCOMLAYER_H_

#include "comlayer.h"
#include "../../core/datatypes/forte_string.h"
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

  char const* getTopicName() const {
    return mTopicName.getValue();
  }

private:
  CIEC_STRING mTopicName;

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
