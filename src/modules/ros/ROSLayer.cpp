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

#include "ROSLayer.h"
#include "ROSManager.h"

#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Empty.h>
#include <commfb.h>

using namespace forte::com_infra;

CROSLayer::CROSLayer(CComLayer* pa_poUpperLayer, CBaseCommFB* pa_poComFB) :
    CComLayer(pa_poUpperLayer, pa_poComFB){

  m_eInterruptResp = e_ProcessDataOk;
  m_TopicName = "";
  m_TopicType = "";
  m_NumSDs = -1;
  m_NumRDs = -1;
}

CROSLayer::~CROSLayer(){

}

// initializes current layer
EComResponse CROSLayer::openConnection(char *pa_acLayerParameter){

  EComResponse retVal = e_InitOk;
  m_eCommServiceType = getCommFB()->getComServiceType();

  std::string layerParams = pa_acLayerParameter;

  // extract layerparams from ID input between the square brackets
  // example: ID = ros[/signal:std_msgs/Float64], layerparams = /signal:std_msgs/Float64
  int doublePoint = static_cast<int>(layerParams.find_last_of(":"));
  m_TopicName = layerParams.substr(0, doublePoint);

  if(e_Subscriber == m_eCommServiceType){

    m_NumRDs = getCommFB()->getNumRD();

    if(0 == m_NumRDs || 1 == m_NumRDs){
      m_Sub = m_Nh.subscribe < topic_tools::ShapeShifter > (m_TopicName, 100, &CROSLayer::handleReceivedValue, const_cast<CROSLayer*>(this));
    }
    else{
      DEVLOG_ERROR("[ROSLAYER] Subscribers with more than 1 RD output are not supported at the moment");
    }
  }
  else if(e_Publisher == m_eCommServiceType){
    m_NumSDs = getCommFB()->getNumSD();

    if(0 == m_NumSDs){
      m_Pub = m_Nh.advertise < std_msgs::Empty > (m_TopicName, 100);
    }
    else if(1 == m_NumSDs){

      m_TopicType = layerParams.substr(doublePoint + 1);

      if("std_msgs/Float64" == m_TopicType) {
        m_Pub = m_Nh.advertise < std_msgs::Float64 > (m_TopicName, 100);
      } else if("std_msgs/Int32" == m_TopicType) {
        m_Pub = m_Nh.advertise < std_msgs::Int32 > (m_TopicName, 100);
      } else if("std_msgs/Bool" == m_TopicType) {
        m_Pub = m_Nh.advertise < std_msgs::Bool > (m_TopicName, 100);
      } else if("std_msgs/String" == m_TopicType) {
        m_Pub = m_Nh.advertise < std_msgs::String > (m_TopicName, 100);
      } else {
        DEVLOG_ERROR("[ROSLAYER] Publisher could not be initialized: unknown topic type \n");
      }
     //FIXME successful initialization message also on error..
    }
    else{
      retVal = e_InitInvalidId;
      DEVLOG_ERROR("[ROSLAYER] Publishers with more than 1 SD intput are not supported at the moment. There are %d SD inputs configured", m_NumSDs);
    }
  }
  // We are a Service
  else if(e_Server == m_eCommServiceType){
    //TODO implement this
  }
  else if(e_Client == m_eCommServiceType){
    //TODO implement this
  }

  else{
    retVal = e_InitInvalidId;
    DEVLOG_ERROR("[ROSLAYER] SIFB could not be initialized with an appropriate ROS communication type: block type is neither Publisher nor Subscriber nor Server nor Client\n");
  }

  return retVal;
}

// callback for subscribers
void CROSLayer::handleReceivedValue(const boost::shared_ptr<const topic_tools::ShapeShifter>& pa_Message){

  // writing received values to RD output: getRDs() returns adress of Data from the RDs -> overwrite them
  CIEC_ANY *DataArray = getCommFB()->getRDs();

  m_TopicType = pa_Message->getDataType();

  if(0 == m_NumRDs){

  }
  else if(1 == m_NumRDs){

    if("std_msgs/Float64" == m_TopicType){
      boost::shared_ptr < std_msgs::Float64 > instantiated = pa_Message->instantiate<std_msgs::Float64>();
      double ROSValue = instantiated->data;
      *(CIEC_LREAL *) DataArray = ROSValue;
    }
    else if("std_msgs/Int32" == m_TopicType){
      boost::shared_ptr < std_msgs::Int32 > instantiated = pa_Message->instantiate<std_msgs::Int32>();
      int ROSValue = instantiated->data;
      *(CIEC_DINT *) DataArray = ROSValue;
    }
    else if("std_msgs/Bool" == m_TopicType){
      boost::shared_ptr < std_msgs::Bool > instantiated = pa_Message->instantiate<std_msgs::Bool>();
      bool ROSValue = instantiated->data;
      *(CIEC_BOOL *) DataArray = ROSValue;
    }
    else if("std_msgs/String" == m_TopicType){
      boost::shared_ptr < std_msgs::String > instantiated = pa_Message->instantiate<std_msgs::String>();
      std::string ROSValue = instantiated->data;

      int stringLength = static_cast<int>(std::strlen(ROSValue.c_str()));
      CIEC_STRING tmpString;
      tmpString.assign(ROSValue.c_str(), static_cast<TForteUInt16>(stringLength));

      *(CIEC_STRING *) DataArray = tmpString;
    }
    else{
      DEVLOG_ERROR("[ROSLAYER] Subscriber received a message with unknown type: %s \n", m_TopicType.c_str());
    }
  }
  else{
    // TODO implement this:
    DEVLOG_ERROR("[ROSLAYER] More than 1 RD outputs are not supported at the Moment \n");
  }

  getCommFB()->interruptCommFB(this);

  getExtEvHandler<CROSManager>().startChain(this->getCommFB());
}

void CROSLayer::closeConnection(){
  m_Nh.shutdown();
}

//TODO use sendData parameters instead of e.g., getCommFB()->getSDs()
EComResponse CROSLayer::sendData(void *, unsigned int){
  EComResponse RetVal = e_ProcessDataOk;

  CIEC_ANY *DataArray = getCommFB()->getSDs();

  // process SD inputs
  if(0 == m_NumSDs){ //Publish 0
    std_msgs::Empty ROSValue;
    m_Pub.publish(ROSValue);
  }
  // Publish 1
  else if(1 == m_NumSDs){
    switch (DataArray->getDataTypeID()){
      case CIEC_ANY::e_BOOL: {
        std_msgs::Bool ROSValue;
        bool FORTEValue = (*(CIEC_BOOL *) DataArray);

        ROSValue.data = FORTEValue;
        m_Pub.publish(ROSValue);
      }
        break;
      case CIEC_ANY::e_LREAL: {
        std_msgs::Float64 ROSValue;

        double FORTEValue = *(CIEC_LREAL *) DataArray;
        ROSValue.data = FORTEValue;

        m_Pub.publish(ROSValue);
      }
        break;
      case CIEC_ANY::e_DINT: {
        std_msgs::Int32 ROSValue;

        int FORTEValue = *(CIEC_DINT *) DataArray;
        ROSValue.data = FORTEValue;

        m_Pub.publish(ROSValue);
      }
        break;
      case CIEC_ANY::e_STRING: {
        std_msgs::String ROSValue;

        CIEC_STRING ciecString = *(CIEC_STRING *) DataArray;
        const char * buf = ciecString.getValue();
        std::size_t len = std::strlen(buf);
        std::string tmpString(buf, 0, len);

        ROSValue.data = tmpString;
        m_Pub.publish(ROSValue);
      }
        break;
      case CIEC_ANY::e_ANY:
      case CIEC_ANY::e_SINT:
      case CIEC_ANY::e_INT:
      case CIEC_ANY::e_USINT:
      case CIEC_ANY::e_UINT:
      case CIEC_ANY::e_BYTE:
      case CIEC_ANY::e_WORD:
      case CIEC_ANY::e_LINT:
      case CIEC_ANY::e_UDINT:
      case CIEC_ANY::e_ULINT:
      case CIEC_ANY::e_DWORD:
      case CIEC_ANY::e_LWORD:
      case CIEC_ANY::e_REAL:
      case CIEC_ANY::e_WSTRING:
      case CIEC_ANY::e_DATE:
      case CIEC_ANY::e_DATE_AND_TIME:
      case CIEC_ANY::e_TIME_OF_DAY:
      case CIEC_ANY::e_TIME:
      case CIEC_ANY::e_DerivedData:
      case CIEC_ANY::e_DirectlyDerivedData:
      case CIEC_ANY::e_EnumeratedData:
      case CIEC_ANY::e_SubrangeData:
      case CIEC_ANY::e_ARRAY:
      case CIEC_ANY::e_STRUCT:
      case CIEC_ANY::e_External:
      case CIEC_ANY::e_Max:
        DEVLOG_ERROR("[ROSLAYER] Publisher with unsupported data type");
        break;
    }
  }
  // Publish n for n > 1
  else{
    DEVLOG_ERROR("[ROSLAYER] Publisher with more than 1 SD input is not supported at the moment");
  }
  return RetVal;
}

EComResponse CROSLayer::recvData(const void *, unsigned int){
  return e_ProcessDataOk;
}

EComResponse CROSLayer::processInterrupt(){
  //we don't need to do anything here (only 1 layer)
  return m_eInterruptResp;
}
