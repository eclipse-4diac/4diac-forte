/*******************************************************************************
 * Copyright (c) 2006-2014 ACIN, Profactor GmbH, fortiss GmbH
 *     2018 Johannes Kepler University, 2018 TU Wien/ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Rene Smodic, Alois Zoitl, Michael Hofmann, Martin Melik Merkumians,
 *    Patrick Smejkal
 *      - initial implementation and rework communication infrastructure
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *    Martin Melik Merkumians - removes usage of unsecure function
 *******************************************************************************/

#include "basecommfb.h"
#include "comlayer.h"
#include "comlayersmanager.h"
#include "../resource.h"
#include "../../arch/fortenew.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <devlog.h>
#include "core/utils/string_utils.h"

using namespace forte::com_infra;

const char * const CBaseCommFB::scmResponseTexts[] = { "OK", "INVALID_ID", "TERMINATED", "INVALID_OBJECT", "DATA_TYPE_ERROR", "INHIBITED", "NO_SOCKET", "SEND_FAILED", "RECV_FAILED" };

CBaseCommFB::CBaseCommFB(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes, forte::com_infra::EComServiceType paCommServiceType) :
    CGenFunctionBlock<CEventSourceFB>(paSrcRes, paInstanceNameId), mCommServiceType(paCommServiceType), mTopOfComStack(nullptr) {
  memset(mInterruptQueue, 0, sizeof(mInterruptQueue)); //TODO change this to  mInterruptQueue{0} in the extended list when fully switching to C++11
  setEventChainExecutor(getResource().getResourceEventExecution());
  mComInterruptQueueCount = 0;
}

CBaseCommFB::~CBaseCommFB() {
  closeConnection();

  if(nullptr != mInterfaceSpec) {
    //Free the memory allocated for the interface, only do this if we dynamically created it (i.e., getManagesFBData is true)
    delete[](mInterfaceSpec->mDINames);
    delete[](mInterfaceSpec->mDIDataTypeNames);
    delete[](mInterfaceSpec->mDONames);
    delete[](mInterfaceSpec->mDODataTypeNames);
  }
}

EMGMResponse CBaseCommFB::changeFBExecutionState(EMGMCommandType paCommand) {
  EMGMResponse retVal = CEventSourceFB::changeFBExecutionState(paCommand);
  if ((EMGMResponse::Ready == retVal) && (EMGMCommandType::Kill == paCommand)) {
    //when we are killed we'll close the connection so that it can safely be opened again after an reset
    closeConnection();
  }
  return retVal;
}

EComResponse CBaseCommFB::openConnection() {
  EComResponse retVal;
  if (nullptr == mTopOfComStack) {
    // Get the ID
    char *commID;
    if (nullptr == strchr(ID().getValue(), ']')) {
      commID = getDefaultIDString(ID().getValue());
    }
    else {
      size_t commIdLength = strlen(ID().getValue()) + 1;
      commID = new char[commIdLength];
      memcpy(commID, ID().getValue(), commIdLength);
      commID[commIdLength - 1] = '\0';
    }

    // If the ID is empty return an error
    if ('\0' == *commID) {
      retVal = e_InitInvalidId;
    }
    else {
      retVal = createComstack(commID);
      // If any error is going to be returned, delete the layers that were created
      if (e_InitOk != retVal) {
        closeConnection();
      }
    }
    delete[] commID;
  }
  else {
    // If the connection was already opened return ok
    retVal = e_InitOk;
  }
  return retVal;
}

EComResponse CBaseCommFB::createComstack(char *commID) {
  EComResponse retVal = e_InitInvalidId;
  CComLayer *newLayer = nullptr;
  CComLayer *previousLayer = nullptr; // Reference to the previous layer as it needs to set the bottom layer
  char *layerParams = nullptr;
  while('\0' != *commID) { // Loop until reaching the end of the ID
    retVal = e_InitInvalidId;
    char * layerID = extractLayerIdAndParams(&commID, &layerParams); // Get the next layer's ID and parameters

    if(nullptr != layerID && '\0' != *layerID) { // If well formated ID, keep going
    // Create the new layer
      newLayer = CComLayersManager::createCommunicationLayer(layerID, previousLayer, this);
      if(nullptr != newLayer) { // If the layer could be created, keep going
        if(nullptr == mTopOfComStack) {
          mTopOfComStack = newLayer; // Assign the newly created layer to the FB
        }

        previousLayer = newLayer; // Update the previous layer reference
        retVal = newLayer->openConnection(layerParams); // Open the layer connection
      }
    }

    if(e_InitOk != retVal) {  // If it was not opened correctly return the error
      break;
    }
  }
  return retVal;
}


void CBaseCommFB::closeConnection() {
  if (mTopOfComStack != nullptr) {
    mTopOfComStack->closeConnection();
    delete mTopOfComStack; // this will close the whole communication stack
    mTopOfComStack = nullptr;
  }
}

void CBaseCommFB::interruptCommFB(CComLayer *paComLayer) {
  if (cgCommunicationInterruptQueueSize > mComInterruptQueueCount) {
    mInterruptQueue[mComInterruptQueueCount] = paComLayer;
    mComInterruptQueueCount++;
  }
  else {
    //TODO to many interrupts received issue error msg
  }
}

char *CBaseCommFB::extractLayerIdAndParams(char **paRemainingID, char **paLayerParams) {
  char *layerID = nullptr;
  if('\0' != **paRemainingID) {
    char *possibleLayerId = *paRemainingID;
    *paRemainingID = forte::core::util::lookForNonEscapedChar(paRemainingID, '[', '\\');
    if(nullptr != *paRemainingID) {
      ++*paRemainingID;
      char *possibleLayerParams = *paRemainingID;
      *paRemainingID = forte::core::util::lookForNonEscapedChar(paRemainingID, ']', '\\');
      if(nullptr != *paRemainingID) { //both [ and ] were found so the ID is correct
        ++*paRemainingID;
        if('\0' != **paRemainingID) {
          ++*paRemainingID;
        }

        layerID = possibleLayerId;
        *paLayerParams = possibleLayerParams;
        forte::core::util::removeEscapedSigns(paLayerParams, '\\');
      } else {
        DEVLOG_ERROR("[CBaseCommFB]: No valid closing bracket was found\n");
      }
    } else {
      DEVLOG_ERROR("[CBaseCommFB]: No valid opening bracket was found\n");
    }
  } else {
    DEVLOG_ERROR("[CBaseCommFB]: The id of the layer is empty\n");
  }
  return layerID;
}

char *CBaseCommFB::buildIDString(const char *paPrefix, const char *paIDRoot, const char *paSuffix) {
  size_t idStringLength = strlen(paPrefix) + strlen(paIDRoot) + strlen(paSuffix) + 1;
  char *RetVal = new char[idStringLength];
  snprintf(RetVal, idStringLength, "%s%s%s", paPrefix, paIDRoot, paSuffix);
  return RetVal;
}
