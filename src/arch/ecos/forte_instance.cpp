/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *  Tarik Terzimehic - make OPC UA server port setable from the command line
 *******************************************************************************/

#include "forte_instance.h"
#include <forte_printer.h>

#include "fortenew.h"
#include "forte_architecture.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../stdfblib/ita/RMT_DEV.h"

#include "../utils/mainparam_utils.h"

unsigned const int cgForteDefaultPort = 61499;

/*!\brief Check if the correct endianess has been configured.
 *
 * If the right endianess is not set this function will end FORTE.
 */
bool checkEndianess();

void createDev(const char *paMGRID, TForteInstance* paResultDevice);

void forteGlobalInitialize(){
  CForteArchitecture::initialize();
}

void forteGlobalDeinitialize(){
  CForteArchitecture::deinitialize();
}

int forteStartInstance(unsigned int paPort, TForteInstance* paResultDevice){

  if(65535 < paPort){
    return FORTE_WRONG_PARAMETERS;
  }

  if(0 == paPort){
    paPort = cgForteDefaultPort;
  }

  char progName[] = "forte";
  char flag[] = "-c";
  char address[17] = "localhost:";
  char port[6];
  forte_snprintf(port, 6, "%u", paPort);
  strcat(address, port);

  char* arguments[] = { progName, flag, address };
  return forteStartInstanceGeneric(3, arguments, paResultDevice);
}

int forteStartInstanceGeneric(int argc, char *arg[], TForteInstance* paResultDevice){

  if(!CForteArchitecture::isInitialized()){
    return FORTE_ARCHITECTURE_NOT_READY;
  }

  if(0 == paResultDevice){
    return FORTE_WRONG_PARAMETERS;
  }

  if(0 != *paResultDevice){
    return FORTE_DEVICE_ALREADY_STARTED;
  }

  if(!checkEndianess()){
    return FORTE_WRONG_ENDIANESS;
  }

  const char *pIpPort = parseCommandLineArguments(argc, arg);
  if((0 != strlen(pIpPort)) && (nullptr != strchr(pIpPort, ':'))){
    createDev(pIpPort, paResultDevice);
  }
  else{ //! If needed call listHelp() to list the help for FORTE
    return FORTE_WRONG_PARAMETERS;
  }

  return FORTE_OK;
}

void forteStopInstance(int paSig, TForteInstance paResultDevice){
  if(!CForteArchitecture::isInitialized()){
    return;
  }
  (void) paSig;
  RMT_DEV *poDev = static_cast<RMT_DEV*>(paResultDevice);
  if(0 != poDev){
    poDev->changeFBExecutionState(EMGMCommandType::Kill);
    poDev->awaitShutdown();
    DEVLOG_INFO("FORTE finished\n");
    delete poDev;
  }
}

/*!\brief Creates the Device-Object
 * \param paMGRID A string containing IP and Port like [IP]:[Port]
 * \param The result
 */
void createDev(const char *paMGRID, TForteInstance* paResultDevice){
  CDevice *dev = CDevice::createDev(paMGRID);
  poDev->startDevice();
  *paResultDevice = poDev;
  DEVLOG_INFO("FORTE is up and running\n");
}

bool checkEndianess(){
  volatile TForteInt16 i = 1;
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
