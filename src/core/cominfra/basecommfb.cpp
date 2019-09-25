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

const char * const CBaseCommFB::scm_sResponseTexts[] = { "OK", "INVALID_ID", "TERMINATED", "INVALID_OBJECT", "DATA_TYPE_ERROR", "INHIBITED", "NO_SOCKET", "SEND_FAILED", "RECV_FAILED" };

CBaseCommFB::CBaseCommFB(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes, forte::com_infra::EComServiceType pa_eCommServiceType) :
    CGenFunctionBlock<CEventSourceFB>(pa_poSrcRes, pa_nInstanceNameId), m_eCommServiceType(pa_eCommServiceType), m_poTopOfComStack(0) {
  memset(m_apoInterruptQueue, 0, sizeof(m_apoInterruptQueue)); //TODO change this to  m_apoInterruptQueue{0} in the extended list when fully switching to C++11
  setEventChainExecutor(getResource().getResourceEventExecution());
  m_unComInterruptQueueCount = 0;
}

CBaseCommFB::~CBaseCommFB() {
  closeConnection();

  if(0 != m_pstInterfaceSpec) {
    //Free the memory allocated for the interface, only do this if we dynamically created it (i.e., getManagesFBData is true)
    delete[](m_pstInterfaceSpec->m_anEIWith);
    delete[](m_pstInterfaceSpec->m_anEOWith);
    delete[](m_pstInterfaceSpec->m_aunDINames);
    delete[](m_pstInterfaceSpec->m_aunDIDataTypeNames);
    delete[](m_pstInterfaceSpec->m_aunDONames);
    delete[](m_pstInterfaceSpec->m_aunDODataTypeNames);
  }
}

EMGMResponse CBaseCommFB::changeFBExecutionState(EMGMCommandType pa_unCommand) {
  EMGMResponse retVal = CEventSourceFB::changeFBExecutionState(pa_unCommand);
  if ((e_RDY == retVal) && (cg_nMGM_CMD_Kill == pa_unCommand)) {
    //when we are killed we'll close the connection so that it can safely be opened again after an reset
    closeConnection();
  }
  return retVal;
}

EComResponse CBaseCommFB::openConnection() {
  EComResponse retVal;
  if (0 == m_poTopOfComStack) {
    // Get the ID
    char *commID;
    if (0 == strchr(ID().getValue(), ']')) {
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
  CComLayer *newLayer = 0;
  CComLayer *previousLayer = 0; // Reference to the previous layer as it needs to set the bottom layer
  char *layerParams = 0;
  while('\0' != *commID) { // Loop until reaching the end of the ID
    retVal = e_InitInvalidId;
    char * layerID = extractLayerIdAndParams(&commID, &layerParams); // Get the next layer's ID and parameters

    if(0 != layerID && '\0' != *layerID) { // If well formated ID, keep going
    // Create the new layer
      newLayer = CComLayersManager::createCommunicationLayer(layerID, previousLayer, this);
      if(0 != newLayer) { // If the layer could be created, keep going
        if(0 == m_poTopOfComStack) {
          m_poTopOfComStack = newLayer; // Assign the newly created layer to the FB
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
  if (m_poTopOfComStack != 0) {
    m_poTopOfComStack->closeConnection();
    delete m_poTopOfComStack; // this will close the whole communication stack
    m_poTopOfComStack = 0;
  }
}

void CBaseCommFB::interruptCommFB(CComLayer *pa_poComLayer) {
  if (cg_unCommunicationInterruptQueueSize > m_unComInterruptQueueCount) {
    m_apoInterruptQueue[m_unComInterruptQueueCount] = pa_poComLayer;
    m_unComInterruptQueueCount++;
  }
  else {
    //TODO to many interrupts received issue error msg
  }
}

char *CBaseCommFB::extractLayerIdAndParams(char **paRemainingID, char **paLayerParams) {
  char *layerID = 0;
  if('\0' != **paRemainingID) {
    char *possibleLayerId = *paRemainingID;
    *paRemainingID = forte::core::util::lookForNonEscapedChar(paRemainingID, '[', '\\');
    if(0 != *paRemainingID) {
      ++*paRemainingID;
      char *possibleLayerParams = *paRemainingID;
      *paRemainingID = forte::core::util::lookForNonEscapedChar(paRemainingID, ']', '\\');
      if(0 != *paRemainingID) { //both [ and ] were found so the ID is correct
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
