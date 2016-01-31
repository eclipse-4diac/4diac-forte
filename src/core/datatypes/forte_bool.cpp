/*******************************************************************************
  * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Rene Smodic,
  *    Gerhard Ebenhofer, Ingo Hegny, Monika Wenger
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include "forte_bool.h"
#include <stdlib.h>

DEFINE_FIRMWARE_DATATYPE(BOOL, g_nStringIdBOOL)

const TForteUInt16 CIEC_BOOL::scm_unMaxStringBufSize = 5;

int CIEC_BOOL::fromString(const char *pa_pacValue){
  int nRetVal = 0;

  if(0 == strncmp(pa_pacValue, "BOOL#", 5)){
    pa_pacValue += 5;
    nRetVal += 5;
  }

  if(pa_pacValue[0] == '1'){
    setTBOOL8(true);
    nRetVal += 1;
  }
  else{
    if(pa_pacValue[0] == '0'){
      setTBOOL8(false);
      nRetVal += 1;
    }
    else{
      if(((pa_pacValue[0] == 'T') || (pa_pacValue[0] == 't')) &&
         ((pa_pacValue[1] == 'R') || (pa_pacValue[1] == 'r')) &&
         ((pa_pacValue[2] == 'U') || (pa_pacValue[2] == 'u')) &&
         ((pa_pacValue[3] == 'E') || (pa_pacValue[3] == 'e'))){
        setTBOOL8(true);
        nRetVal += 4;
      }
      else{
        if(((pa_pacValue[0] == 'F') || (pa_pacValue[0] == 'f')) &&
           ((pa_pacValue[1] == 'A') || (pa_pacValue[1] == 'a')) &&
           ((pa_pacValue[2] == 'L') || (pa_pacValue[2] == 'l')) &&
           ((pa_pacValue[3] == 'S') || (pa_pacValue[3] == 's')) &&
           ((pa_pacValue[4] == 'E') || (pa_pacValue[4] == 'e'))){
          setTBOOL8(false);
          nRetVal += 5;
        }
        else{
          nRetVal = -1;
        }
      }
    }
  }
  return nRetVal;
}

int CIEC_BOOL::toString(char* pa_acValue, unsigned int pa_nBufferSize)  const {
  int nRetval = -1;
  if(pa_nBufferSize >= 6){
    if(getTBOOL8()){
      strcpy(pa_acValue, "TRUE");
      nRetval = 4;
    }
    else{
      strcpy(pa_acValue, "FALSE");
      nRetval = 5;
    }
  }
  return nRetval;
}
