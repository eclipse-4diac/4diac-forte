/*******************************************************************************
 * Copyright (c) 2018 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians - initial contribution
 *******************************************************************************/

#include "CAdsConnection.h"
#include "devlog.h"

namespace forte {
  namespace ads {

    CAdsConnection::CAdsConnection(const std::string& paAddr, const std::string& paRemoteIpOrHostName) :
        mRemoteIpV4OrHostName(paRemoteIpOrHostName), mPort(0){
      mRemoteDevice.netId = AmsNetId(paAddr);
      mRemoteDevice.port = AMSPORT_R0_PLC_TC3;
    }

    CAdsConnection::CAdsConnection(const std::string& paAddr, uint16_t paPort, const std::string& paRemoteIpOrHostName) :
        mRemoteIpV4OrHostName(paRemoteIpOrHostName), mPort(0){
      mRemoteDevice.netId = AmsNetId(paAddr);
      mRemoteDevice.port = paPort;
    }

    CAdsConnection::~CAdsConnection(){
      const long closeStatus = AdsPortCloseEx(mRemoteDevice.port);
      if(0 != closeStatus){
        DEVLOG_ERROR("Close ADS port failed with: %d\n", closeStatus);
      }
    }

    bool CAdsConnection::connect(){
      long status = AdsAddRoute(mRemoteDevice.netId, mRemoteIpV4OrHostName.c_str());
      if(!status){
        mPort = static_cast<uint16_t>(AdsPortOpenEx());
      }
      else{
        DEVLOG_ERROR("ADS connection could not be opened! Code: %d\n", status);
        return false;
      }
      if(0 == mPort){
        DEVLOG_ERROR("ADS port could not be opened!\n");
        return false;
      }
      return true;
    }

    uint16_t CAdsConnection::getPort() const{
      return mPort;
    }

    const AmsAddr* const CAdsConnection::getRemoteDevice() const{
      return &mRemoteDevice;
    }

  } /* namespace ads */
} /* namespace forte */
