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

#include "core/util/parameterParser.h"

namespace forte {
  namespace ads {
    CAdsProcessInterface::CAdsProcessInterface(forte::core::CFBContainer &paContainer, const SFBInterfaceSpec& paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId) :
        CProcessInterfaceBase(paContainer, paInterfaceSpec, paInstanceNameId), mAdsHandle(0) {
    }

    bool CAdsProcessInterface::initialise(bool paIsInput, CEventChainExecutionThread *const paECET) {
      CParameterParser adsParameters(PARAMS().getValue(), ':', 2);
      if(2 != adsParameters.parseParameters()) {
        STATUS() = CIEC_STRING("PARAMS could not be parsed");
        return false;
      }
      mFriendlyAdsServerName = adsParameters[0];
      mAdsVariableName = adsParameters[1];

      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      if(0 != connection){
        uint32_t handle = 0;
        const long handleStatus = AdsSyncReadWriteReqEx2(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_HNDBYNAME, 0, sizeof(handle), &handle, static_cast<uint32_t>(mAdsVariableName.size()), mAdsVariableName.c_str(), 0);
        if(handleStatus){
          DEVLOG_ERROR("Create handle for %s failed with code 0x%x\n", mAdsVariableName, handleStatus);
          STATUS() = CIEC_STRING("Handle could not be created!");
          return false;
        }
        mAdsHandle = handle;
        return true;
      }
      else{
        DEVLOG_ERROR("Connection could not be found!\n");
        STATUS() = CIEC_STRING("Connection could not be found!");
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
        STATUS() = CIEC_STRING("Write Pin malfunction\n");
        DEVLOG_ERROR("Write Pin malfunction\n");
        return false;
      }
      STATUS() = CIEC_STRING("OK");
      return true;
    }

    bool CAdsProcessInterface::readPin(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      uint32_t bytesRead;
      uint8_t buffer;
      const long status = AdsSyncReadReqEx2(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(buffer), &buffer, &bytesRead);
      if(status){
        IN_X() = CIEC_BOOL(false);
        STATUS() = CIEC_STRING("Read Pin malfunction");
        DEVLOG_ERROR("Read Pin malfunction\n");
        return false;
      }
      STATUS() = CIEC_STRING("OK");
      IN_X() = CIEC_BOOL(buffer);
      return true;
    }

    bool CAdsProcessInterface::writeByte(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      const long status = AdsSyncWriteReqEx(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(TForteWord), OUT_B().getDataPtr());
      if(status){
        STATUS() = CIEC_STRING("Write Byte malfunction\n");
        DEVLOG_ERROR("Write Byte malfunction\n");
        return false;
      }
      STATUS() = CIEC_STRING("OK");
      return true;
    }

    bool CAdsProcessInterface::readByte(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      uint32_t bytesRead;
      uint8_t buffer;
      const long status = AdsSyncReadReqEx2(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(buffer), &buffer, &bytesRead);
      if(status){
        IN_B() = CIEC_BYTE(0);
        STATUS() = CIEC_STRING("Read Byte malfunction");
        DEVLOG_ERROR("Read Byte malfunction\n");
        return false;
      }
      IN_B() = CIEC_BYTE(buffer);
      STATUS() = CIEC_STRING("OK");
      return true;
    }

    bool CAdsProcessInterface::writeWord(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      const long status = AdsSyncWriteReqEx(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(TForteWord), OUT_W().getDataPtr());
      if(status){
        STATUS() = CIEC_STRING("Write Word malfunction\n");
        DEVLOG_ERROR("Write Word malfunction\n");
        return false;
      }
      STATUS() = CIEC_STRING("OK");
      return true;
    }

    bool CAdsProcessInterface::readWord(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      uint32_t bytesRead;
      uint16_t buffer;
      const long status = AdsSyncReadReqEx2(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(buffer), &buffer, &bytesRead);
      if(status){
        IN_W() = CIEC_WORD(0);
        STATUS() = CIEC_STRING("Read Word malfunction");
        DEVLOG_ERROR("Read Word malfunction\n");
        return false;
      }
      IN_W() = CIEC_WORD(static_cast<TForteWord>(buffer));
      STATUS() = CIEC_STRING("OK");
      return true;
    }

    bool CAdsProcessInterface::writeDWord(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      const long status = AdsSyncWriteReqEx(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(TForteDWord), OUT_D().getDataPtr());
      if(status){
        STATUS() = CIEC_STRING("Write DWord malfunction\n");
        DEVLOG_ERROR("Write DWord malfunction\n");
        return false;
      }
      STATUS() = CIEC_STRING("OK");
      return true;
    }

    bool CAdsProcessInterface::readDWord(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      uint32_t bytesRead;
      uint32_t buffer;
      const long status = AdsSyncReadReqEx2(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(buffer), &buffer, &bytesRead);
      if(status){
        IN_D() = CIEC_DWORD(0);
        STATUS() = CIEC_STRING("Read DWORD malfunction");
        DEVLOG_ERROR("Read DWORD malfunction\n");
        return false;
      }
      IN_D() = CIEC_DWORD(static_cast<TForteDWord>(buffer));
      STATUS() = CIEC_STRING("OK");
      return true;
    }

    bool CAdsProcessInterface::writeLWord(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      const long status = AdsSyncWriteReqEx(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(TForteDWord), OUT_L().getDataPtr());
      if(status){
        STATUS() = CIEC_STRING("Write DWord malfunction\n");
        DEVLOG_ERROR("Write DWord malfunction\n");
        return false;
      }
      STATUS() = CIEC_STRING("OK");
      return true;
    }

    bool CAdsProcessInterface::readLWord(){
      CAdsConnection *connection = CAdsConnectionManager::getInstance().getConnection(mFriendlyAdsServerName);
      uint32_t bytesRead;
      uint64_t buffer;
      const long status = AdsSyncReadReqEx2(connection->getPort(), connection->getRemoteDevice(), ADSIGRP_SYM_VALBYHND, mAdsHandle, sizeof(buffer), &buffer, &bytesRead);
      if(status){
        IN_L() = CIEC_LWORD(0);
        STATUS() = CIEC_STRING("Read LWORD malfunction");
        DEVLOG_ERROR("Read LWORD malfunction\n");
        return false;
      }
      IN_L() = CIEC_LWORD(buffer);
      STATUS() = CIEC_STRING("OK");
      return true;
    }

    CAdsProcessInterface::~CAdsProcessInterface(){
      if(0 != mAdsHandle){
        deinitialise();
      }
    }
  }
}
