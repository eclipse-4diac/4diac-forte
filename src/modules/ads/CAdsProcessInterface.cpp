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
 *    Martin Melik-Merkumians - adds Byte and LWord functions
 *******************************************************************************/

#include <stdint.h>

#include "CAdsProcessInterface.h"

#include "CAdsConnectionManager.h"

#include "../../core/utils/parameterParser.h"

namespace forte {
  namespace ads {
    CAdsProcessInterface::CAdsProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
        CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData), mAdsHandle(0) {
    }

    bool CAdsProcessInterface::initialise(bool){
      CParameterParser adsParameters(PARAMS().getValue(), ':', 2);
      if(2 != adsParameters.parseParameters()) {
        STATUS() = "PARAMS could not be parsed";
        return false;
      }
      mFriendlyAdsServerName = adsParameters[0];
      mAdsVariableName = adsParameters[1];

      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      if(0 != connection){
        uint32_t handle = 0;
        const long handleStatus = AdsSyncReadWriteReqEx2(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_HNDBYNAME, 0, sizeof(handle), &handle, mAdsVariableName.size(), mAdsVariableName.c_str(), 0);
        if(handleStatus){
          DEVLOG_ERROR("Create handle for %s failed with code 0x%x\n", mAdsVariableName, handleStatus);
          STATUS() = "Handle could not be created!";
          return false;
        }
        mAdsHandle = handle;
        return true;
      }
      else{
        DEVLOG_ERROR("Connection could not be found!\n");
        STATUS() = "Connection could not be found!";
        return false;
      }
    }

    bool CAdsProcessInterface::deinitialise(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      const long releaseHandle = AdsSyncWriteReqEx(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_RELEASEHND, 0, sizeof(mAdsHandle), &mAdsHandle);
      if(releaseHandle){
        DEVLOG_ERROR("Release handle 0x%x failed with code 0x%x\n", mAdsHandle, releaseHandle);
        return false;
      }
      mAdsHandle = 0;
      return true;
    }

    bool CAdsProcessInterface::writePin(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      const long status = AdsSyncWriteReqEx(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(TForteByte), OUT_X().getDataPtr());
      if(status){
        STATUS() = "Write Pin malfunction\n";
        DEVLOG_ERROR("Write Pin malfunction\n");
        return false;
      }
      STATUS() = "OK";
      return true;
    }

    bool CAdsProcessInterface::readPin(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      uint32_t bytesRead;
      uint_fast8_t buffer;
      const long status = AdsSyncReadReqEx2(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(buffer), &buffer, &bytesRead);
      if(status){
        IN_X() = false;
        STATUS() = "Read Pin malfunction";
        DEVLOG_ERROR("Read Pin malfunction\n");
        return false;
      }
      STATUS() = "OK";
      IN_X() = buffer;
      return true;
    }

    bool CAdsProcessInterface::writeByte(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      const long status = AdsSyncWriteReqEx(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(TForteWord), OUT_B().getDataPtr());
      if(status){
        STATUS() = "Write Byte malfunction\n";
        DEVLOG_ERROR("Write Byte malfunction\n");
        return false;
      }
      STATUS() = "OK";
      return true;
    }

    bool CAdsProcessInterface::readByte(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      uint32_t bytesRead;
      uint_fast8_t buffer;
      const long status = AdsSyncReadReqEx2(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(buffer), &buffer, &bytesRead);
      if(status){
        IN_B() = 0;
        STATUS() = "Read Byte malfunction";
        DEVLOG_ERROR("Read Byte malfunction\n");
        return false;
      }
      IN_B() = buffer;
      STATUS() = "OK";
      return true;
    }

    bool CAdsProcessInterface::writeWord(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      const long status = AdsSyncWriteReqEx(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(TForteWord), OUT_W().getDataPtr());
      if(status){
        STATUS() = "Write Word malfunction\n";
        DEVLOG_ERROR("Write Word malfunction\n");
        return false;
      }
      STATUS() = "OK";
      return true;
    }

    bool CAdsProcessInterface::readWord(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      uint32_t bytesRead;
      uint_fast16_t buffer;
      const long status = AdsSyncReadReqEx2(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(buffer), &buffer, &bytesRead);
      if(status){
        IN_W() = 0;
        STATUS() = "Read Word malfunction";
        DEVLOG_ERROR("Read Word malfunction\n");
        return false;
      }
      IN_W() = buffer;
      STATUS() = "OK";
      return true;
    }

    bool CAdsProcessInterface::writeDWord(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      const long status = AdsSyncWriteReqEx(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(TForteDWord), OUT_D().getDataPtr());
      if(status){
        STATUS() = "Write DWord malfunction\n";
        DEVLOG_ERROR("Write DWord malfunction\n");
        return false;
      }
      STATUS() = "OK";
      return true;
    }

    bool CAdsProcessInterface::readDWord(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      uint32_t bytesRead;
      uint_fast32_t buffer;
      const long status = AdsSyncReadReqEx2(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(buffer), &buffer, &bytesRead);
      if(status){
        IN_D() = 0;
        STATUS() = "Read DWORD malfunction";
        DEVLOG_ERROR("Read DWORD malfunction\n");
        return false;
      }
      IN_D() = buffer;
      STATUS() = "OK";
      return true;
    }

#ifdef FORTE_USE_64BIT_DATATYPES
    bool CAdsProcessInterface::writeLWord(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      const long status = AdsSyncWriteReqEx(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(TForteDWord), OUT_L().getDataPtr());
      if(status){
        STATUS() = "Write DWord malfunction\n";
        DEVLOG_ERROR("Write DWord malfunction\n");
        return false;
      }
      STATUS() = "OK";
      return true;
    }

    bool CAdsProcessInterface::readLWord(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      uint32_t bytesRead;
      uint_fast64_t buffer;
      const long status = AdsSyncReadReqEx2(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(buffer), &buffer, &bytesRead);
      if(status){
        IN_L() = 0;
        STATUS() = "Read DWORD malfunction";
        DEVLOG_ERROR("Read DWORD malfunction\n");
        return false;
      }
      IN_L() = buffer;
      STATUS() = "OK";
      return true;
    }
#endif /* FORTE_USE_64BIT_DATATYPES */

    CAdsProcessInterface::~CAdsProcessInterface(){
      if(0 != mAdsHandle){
        deinitialise();
      }
    }
  }
}
