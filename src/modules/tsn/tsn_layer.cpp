/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH.
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Ben Schneider - Initial contribution; vlan and prio configuration to support tsn for pub/sub
 *******************************************************************************/

#include <sys/socket.h>
#include <net/if.h>
#include "tsn_layer.h"
#include "../../arch/devlog.h"
#include "commfb.h"
#include <stdio.h>
#include "parameterParser.h"
#include "string_utils.h"

using namespace forte::com_infra;

CTSNLayer::CTSNLayer(CComLayer* paUpperLayer, CBaseCommFB* paBaseCommFB) :
    CIPComLayer(paUpperLayer, paBaseCommFB){
}

CTSNLayer::~CTSNLayer(){
}

EComResponse CTSNLayer::openConnection(char *paLayerParameter){
  EComResponse eRetVal = e_InitInvalidId;

  // complete ID for publisher: fbdk[].tsn[<ip>:<port>:<vlan_id>:<prio>] e.g., fbdk[].tsn[239.1.0.1:48401:3:5]
  CParameterParser parser(paLayerParameter, ':', scmNumParameters);

  if(scmNumParameters != parser.parseParameters()){
    DEVLOG_ERROR("[TSN Layer] Wrong parameters (%s)\n", paLayerParameter);
  }
  else{

    unsigned int vlanPriority = static_cast<unsigned int>(forte::core::util::strtoul(parser[3], 0, 10));
    char * dstIPAddress = const_cast<char*>(parser[0]);
    TForteUInt16 nPort = static_cast<TForteUInt16>(forte::core::util::strtoul(parser[1], 0, 10));

    CIPComSocketHandler::TSocketDescriptor nSockDes = CIPComSocketHandler::scmInvalidSocketDescriptor;
    m_eConnectionState = e_Connected;

    switch (m_poFb->getComServiceType()){
      case e_Server:
        DEVLOG_ERROR("[TSN LAYER] TSN layer does not support server FBs. Use the default "
            "ip layer without vlan parameters instead.\n");
        break;
      case e_Client:
        DEVLOG_ERROR("[TSN LAYER] TSN layer does not support client FBs. Use the default "
            "ip layer without vlan parameters instead.\n");
        break;
      case e_Publisher:
        nSockDes = mSocketID = CIPComSocketHandler::openUDPSendPort(dstIPAddress, nPort, &mDestAddr);
        eRetVal = setVLANIDForSocket(parser[2]);
        if(e_InitOk == eRetVal){
          eRetVal = setVLANPriorityForSocket(vlanPriority);
        }
        break;
      case e_Subscriber:
        DEVLOG_ERROR("[TSN LAYER] TSN layer does not support subscriber FBs. Use the default "
            "ip layer without vlan parameters instead.\n");
        break;
    }

    if(CIPComSocketHandler::scmInvalidSocketDescriptor != nSockDes && e_InitOk == eRetVal){
      if(e_Publisher != m_poFb->getComServiceType()){
        //Publishers should not be registered for receiving data
        getExtEvHandler<CIPComSocketHandler>().addComCallback(nSockDes, this);
      }
      eRetVal = e_InitOk;
    }
    else{
      m_eConnectionState = e_Disconnected;
    }
  }
  return eRetVal;
}

EComResponse CTSNLayer::setVLANPriorityForSocket(unsigned int paVlanPriority){
  EComResponse eRetVal = e_InitInvalidId;
  if (scmMaxVLANPrio < paVlanPriority){
    DEVLOG_ERROR("[TSN Layer] VLAN priority %u out of range (0 - 7)\n", paVlanPriority);
    return eRetVal;
  }
  if(0 == setsockopt(mSocketID, SOL_SOCKET, SO_PRIORITY, &paVlanPriority, sizeof(paVlanPriority))){
    eRetVal = e_InitOk;
  }
  else{
    DEVLOG_ERROR("[TSN LAYER] Setting user space priority %u of socket not possible \n", paVlanPriority);
  }
  return eRetVal;
}

EComResponse CTSNLayer::setVLANIDForSocket(const char* paId){
  EComResponse eRetVal = e_InitInvalidId;

  unsigned int id = static_cast<unsigned int>(forte::core::util::strtoul(paId, 0, 10));
  if(scmMinVLANID <= id && scmMaxVLANID >= id){
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    CIEC_STRING interfaceName = "eth0.";
    interfaceName.append(paId);

    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), interfaceName.getValue());

    if(0 == setsockopt(mSocketID, SOL_SOCKET, SO_BINDTODEVICE, (void *) &ifr, sizeof(ifr))){
      eRetVal = e_InitOk;
    }
    else{
      DEVLOG_ERROR("[TSN LAYER] binding to interface %s not possible \n", interfaceName.getValue());
    }
  }
  else{
    DEVLOG_ERROR("[TSN Layer] Invalid VLAN ID %u out of range (1 - 4095)\n", id);
  }

  return eRetVal;
}
