/*******************************************************************************
 * Copyright (c) 2015-2016 Florian Froschermeier <florian.froschermeier@tum.de>,
 *               fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Florian Froschermeier
 *      - initial integration of the OPC-UA protocol
 *    Stefan Profanter
 *      - refactoring and adaption to new concept
 *    Jose Cabral:
 *      - refactoring to cleaner architecture
 *******************************************************************************/

#ifndef SRC_MODULES_OPC_UA_OPCUA_LAYER_H_
#define SRC_MODULES_OPC_UA_OPCUA_LAYER_H_

#include "../../core/cominfra/comlayer.h"
#include "opcua_helper.h"

class COPC_UA_HandlerAbstract;
class CActionInfo;

class COPC_UA_Layer : public forte::com_infra::CComLayer {
  public:

    COPC_UA_Layer(forte::com_infra::CComLayer *paUpperLayer, forte::com_infra::CBaseCommFB *paComFB);

    virtual ~COPC_UA_Layer();

    forte::com_infra::EComResponse sendData(void *paData, unsigned int paSize);

    forte::com_infra::EComResponse recvData(const void *paData, unsigned int paSize);

    virtual forte::com_infra::EComResponse processInterrupt();

    void triggerNewEvent();

  private:

    /**
     * Response for the processInterrupt() method
     */
    forte::com_infra::EComResponse mInterruptResp;

    COPC_UA_HandlerAbstract* mHandler;

    CActionInfo* mActionInfo;

    forte::com_infra::EComResponse openConnection(char *paLayerParameter);

    void closeConnection();

    bool getRemoteType(CIEC_ANY **paResult, const SConnectionPoint& paRemoteConnectionPoint, bool paIsSD) const;

    bool checkFanOutTypes(const CDataConnection& paPortConnection, CIEC_ANY **paRemoteType) const;

    bool getPortConnectionInfo(unsigned int paPortIndex, bool paIsSD, const COPC_UA_Helper::UA_TypeConvert **paResult) const;

    bool storeTypesFromInterface(CSinglyLinkedList<COPC_UA_Helper::UA_TypeConvert *>& paConverters);

    // These are needed because if many subscription are present in one FB, and all of them are updated, we'll get one external event for each, when
    //only one is needed.
    bool mDataAlreadyPresent;
    CSyncObject mDataAlreadyPresentMutex;
    void setDataAlreadyPresentRead(bool paDataRead);
    bool getDataAlreadyPresentRead();

};

#endif /* SRC_MODULES_OPC_UA_OPCUA_LAYER_H_ */

