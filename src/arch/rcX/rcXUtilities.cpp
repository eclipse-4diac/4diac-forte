/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "rcXUtilities.h"

void getRandomString(char* pa_destString, const TForteUInt32 pa_length){
  for(TForteUInt32 i = 0; i < pa_length; ++i){
    TForteInt32 randomNumber = rand() % 62; //26 lower case, 26 upper case, 10 numbers
    if(randomNumber < 26){ //first 26 numbers are for the lower cases
      pa_destString[i] = static_cast<char>('a' + randomNumber);
    }
    else if(randomNumber < 52){//the second 26 numbers are for the upper cases
      pa_destString[i] = static_cast<char>('A' + randomNumber - 26);
    }
    else{ //the last 10 for the numbers
      pa_destString[i] = static_cast<char>('0' + randomNumber - 52);
    }
  }
  pa_destString[pa_length] = 0;
}

