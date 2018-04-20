/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Monika Wenger
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "ForteBootFileLoader.h"
#include "../../arch/devlog.h"
#include "../../../src/core/datatypes/forte_string.h"
#include "IBootFileCallback.h"
#include <mgmcmd.h>
#include <mgmcmdstruct.h>
#include "../../core/device.h"

ForteBootFileLoader::ForteBootFileLoader(IBootFileCallback &paCallback) : mCallback(paCallback){
  openBootFile(0);
}

ForteBootFileLoader::ForteBootFileLoader(IBootFileCallback &paCallback, CIEC_STRING &paBootFileName) : mCallback(paCallback){
  openBootFile(&paBootFileName);
}

ForteBootFileLoader::~ForteBootFileLoader() {
  if(0 != bootfile){
    DEVLOG_INFO("Closing bootfile\n");
    fclose(bootfile);
  }
}

bool ForteBootFileLoader::openBootFile(CIEC_STRING* paBootFileName){
  bool retVal = false;
  CIEC_STRING bootFileName;
  if(0 != paBootFileName){
     bootFileName = *paBootFileName;
   }else{
     // select provided or default boot file name
     char * envBootFileName = getenv("FORTE_BOOT_FILE");
     if(0 != envBootFileName){
       DEVLOG_INFO("Using provided bootfile location: %s\n", envBootFileName);
       bootFileName = envBootFileName;
     }
     else{
       DEVLOG_INFO("Using default bootfile location: %s\n", FORTE_BOOT_FILE_LOCATION);
       bootFileName = FORTE_BOOT_FILE_LOCATION;
     }
   }

  // check if we finally have a boot file name
  if("" == bootFileName){
    DEVLOG_INFO("No bootfile specified and no default bootfile configured during build\n");
  }else{
    bootfile = fopen(bootFileName.getValue(), "r");
    if(0 != bootfile){
      DEVLOG_INFO("Boot file %s opened\n", bootFileName.getValue());
      retVal = true;
    }
    else{
      if(0 != getenv("FORTE_BOOT_FILE_FAIL_MISSING")){
        DEVLOG_ERROR("Boot file %s could not be opened and FORTE_BOOT_FILE_FAIL_MISSING is set. Failing...\n", bootFileName.getValue());
        exit(2);
      }
      else{
        DEVLOG_INFO("Boot file %s could not be opened. Skipping...\n", bootFileName.getValue());
      }
    }
  }
  return retVal;
}

LoadBootResult ForteBootFileLoader::loadBootFile(){
  LoadBootResult eResp = FILE_NOT_OPENED;
  if(0 != bootfile){
    //we could open the file try to load it
    int nLineCount = 1;
    eResp = LOAD_RESULT_OK;
    CIEC_STRING line;
    while(readLine(line)){
      char *cmdStart = strchr(line.getValue(), ';');
      if(0 == cmdStart){
        eResp = MISSING_COLON;
        DEVLOG_ERROR("Boot file line does not contain separating ';'. Line: %d\n", nLineCount);
        break;
      }
      *cmdStart = '\0';
      cmdStart++;
      if(!mCallback.executeCommand(line.getValue(), cmdStart)){
        //command was not successful
        DEVLOG_ERROR("Boot file command could not be executed. Line: %d: %s\n", nLineCount, cmdStart);
        eResp = EXTERNAL_ERROR;
        break;
      }
      nLineCount++;
    }
  }else{
    DEVLOG_ERROR("Loading cannot proceed because the boot file is no opened\n");
  }
  return eResp;
}

bool ForteBootFileLoader::readLine(CIEC_STRING &line){
  const unsigned int size = 100;
  line.clear();
  char acLineBuf[size];
  do{
    if(0 != fgets(acLineBuf, size, bootfile)){
      line.append(acLineBuf);
    }else{
      return 0 != line.length();
    }
  }while(!hasCommandEnded(line));
  return true;
}

bool ForteBootFileLoader::hasCommandEnded(const CIEC_STRING &line) const{
  return (!strcmp(line.getValue() + line.length() - 11, "</Request>\n")
      || !strcmp(line.getValue() + line.length() - 3, "/>\n"));
}
