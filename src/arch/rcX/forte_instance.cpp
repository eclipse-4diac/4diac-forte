/*******************************************************************************
 * Copyright (c) 2017-2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte_instance.h"

#include "fortenew.h"
#include "forte_architecture.h"
#include <stdio.h>
#include <string>
#include "../../stdfblib/ita/RMT_DEV.h"

#include "../utils/mainparam_utils.h"

unsigned int forte_default_port = 61499;

/*!\brief Check if the correct endianess has been configured.
 *
 * If the right endianess is not set this function will end FORTE.
 */
bool checkEndianess();

void createDev(const char *pa_acMGRID, TForteInstance* pa_resultDevice);

void forteGlobalInitialize(void){
  CForteArchitecture::initialize();
}

void forteGlobalDeinitialize(void){
  CForteArchitecture::deinitialize();
}

int forteStartInstance(unsigned int pa_port, TForteInstance* pa_resultDevice){

  if (65535 < pa_port){
    return FORTE_WRONG_PARAMETERS;
  }

  if (0 == pa_port){
    pa_port = forte_default_port;
  }

  char flag[] = "-c";
  char address[16] = "localhost:";
  char port[6];
  sprintf(port, "%u", pa_port);
  strcat(address, port);

  char* arguments[] = {flag, address};
  return forteStartInstanceGeneric(2, arguments, pa_resultDevice);
}


int forteStartInstanceGeneric(int argc, char *arg[], TForteInstance* pa_resultDevice){

  if(!CForteArchitecture::isInitialized()){
    return FORTE_ARCHITECTURE_NOT_READY;
  }

  if(0 == pa_resultDevice){
    return FORTE_WRONG_PARAMETERS;
  }

  if (0 != *pa_resultDevice){
    return FORTE_DEVICE_ALREADY_STARTED;
  }

  if (!checkEndianess()){
    return FORTE_WRONG_ENDIANESS;
  }

  const char *pIpPort = parseCommandLineArguments(argc, arg);
  if((0 != strlen(pIpPort)) && (NULL != strchr(pIpPort, ':'))){
    createDev(pIpPort, pa_resultDevice);
  }
  else{ //! If needed call listHelp() to list the help for FORTE
    return FORTE_WRONG_PARAMETERS;
  }

  return FORTE_OK;
}

void forteStopInstance(int pa_nSig, TForteInstance pa_resultDevice){
  if(!CForteArchitecture::isInitialized()){
    return;
  }
  (void) pa_nSig;
  RMT_DEV *poDev = static_cast<RMT_DEV*>(pa_resultDevice);
  if(0 != poDev){
    poDev->changeFBExecutionState(cg_nMGM_CMD_Kill);
    poDev->MGR.joinResourceThread();
    DEVLOG_INFO("FORTE finished\n");
    delete poDev;
  }
}

/*!\brief Creates the Device-Object
 * \param pa_acMGRID A string containing IP and Port like [IP]:[Port]
 * \param The result
 */
void createDev(const char *pa_acMGRID, TForteInstance* pa_resultDevice){
  RMT_DEV *poDev = new RMT_DEV;
  poDev->setMGR_ID(pa_acMGRID);
  poDev->startDevice();
  *pa_resultDevice = poDev;
  DEVLOG_INFO("FORTE is up and running\n");
}

bool checkEndianess(){
  int i = 1;
  char *p = (char *) &i;
  if(p[0] == 1){
    //we are on a little endian platform
#ifdef FORTE_BIG_ENDIAN
    DEVLOG_ERROR("Wrong endianess configured! You are on a little endian platform and have configured big endian!\n");
    return false;
#endif
  }
  else{
    //we are on a big endian platform
#ifdef FORTE_LITTLE_ENDIAN
    DEVLOG_ERROR("Wrong endianess configured! You are on a big endian platform and have configured little endian!\n");
    return false;
#endif
  }

  return true;
}
