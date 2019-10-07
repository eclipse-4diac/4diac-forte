/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Rene Smodic,
 *    Gerhard Ebenhofer, Ingo Hegny, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte_bool.h"
#include <stdlib.h>

DEFINE_FIRMWARE_DATATYPE(BOOL, g_nStringIdBOOL)

int CIEC_BOOL::fromString(const char *paValue){
  int nRetVal = 0;

  if(0 == strncmp(paValue, "BOOL#", 5)){
    paValue += 5;
    nRetVal += 5;
  }

  if(paValue[0] == '1'){
    setTBOOL8(true);
    nRetVal += 1;
  }
  else{
    if(paValue[0] == '0'){
      setTBOOL8(false);
      nRetVal += 1;
    }
    else{
      if(((paValue[0] == 'T') || (paValue[0] == 't')) &&
         ((paValue[1] == 'R') || (paValue[1] == 'r')) &&
         ((paValue[2] == 'U') || (paValue[2] == 'u')) &&
         ((paValue[3] == 'E') || (paValue[3] == 'e'))){
        setTBOOL8(true);
        nRetVal += 4;
      }
      else{
        if(((paValue[0] == 'F') || (paValue[0] == 'f')) &&
           ((paValue[1] == 'A') || (paValue[1] == 'a')) &&
           ((paValue[2] == 'L') || (paValue[2] == 'l')) &&
           ((paValue[3] == 'S') || (paValue[3] == 's')) &&
           ((paValue[4] == 'E') || (paValue[4] == 'e'))){
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

int CIEC_BOOL::toString(char* paValue, size_t paBufferSize) const {
  int nRetval = -1;
  if(paBufferSize >= csmStringBufferSize[e_BOOL]){
    if(getTBOOL8()){
      strncpy(paValue, "TRUE", paBufferSize);
      nRetval = 4;
    }
    else{
      strncpy(paValue, "FALSE", paBufferSize);
      nRetval = 5;
    }
    paValue[paBufferSize - 1] = '\0';
  }
  return nRetval;
}
