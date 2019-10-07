/*******************************************************************************
 * Copyright (c) 2016 - 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Ben Schneider
 *      - initial implementation and documentation
 *******************************************************************************/

#ifndef CROSLAYER_H_
#define CROSLAYER_H_

#include <ros/ros.h>
#include <ros/service_callback_helper.h>
#include <topic_tools/shape_shifter.h>

#include <comlayer.h>
#include <extevhan.h>
#include <forte_thread.h>
#include <singlet.h>

#include <string>

class CROSLayer : public forte::com_infra::CComLayer{

  public:
    CROSLayer(forte::com_infra::CComLayer* pa_poUpperLayer, forte::com_infra::CBaseCommFB* pa_poComFB);
    virtual ~CROSLayer();

    virtual void closeConnection();

    virtual forte::com_infra::EComResponse sendData(void *pa_pvData, unsigned int pa_unSize);
    virtual forte::com_infra::EComResponse recvData(const void *pa_pvData, unsigned int pa_unSize);

    virtual forte::com_infra::EComResponse processInterrupt();

  private:
    virtual forte::com_infra::EComResponse openConnection(char *pa_acLayerParameter);

    forte::com_infra::EComServiceType m_eCommServiceType;
    forte::com_infra::EComResponse m_eInterruptResp;

    ros::NodeHandle m_Nh;
    ros::Publisher m_Pub;
    ros::Subscriber m_Sub;
    std::string m_TopicName;
    std::string m_TopicType;
    int m_NumRDs;
    int m_NumSDs;

    void handleReceivedValue(const boost::shared_ptr<const topic_tools::ShapeShifter>& pa_message);
};

#endif //_CROSLAYER_H_
