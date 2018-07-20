/*******************************************************************************
 * Copyright (c) 2016, 2017 ACIN - fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 * Martin Melik Merkumians - initial API and implementation and/or initial documentation
 * Jose Cabral - Move to a new file and make the amount of parameter a variable
 *******************************************************************************/

#include "parameterParser.h"
#include <string.h>
#include <cctype>

CParameterParser::CParameterParser(const char* paParameters, unsigned int paAmountOfParameters, const char paSeparator) :
  mParameters(paParameters), mAmountOfParameters(paAmountOfParameters), mSeparator(paSeparator){
  mParameterLocations = new char*[paAmountOfParameters];
  memset(mParameterLocations, 0, sizeof(char*)*paAmountOfParameters);
}

CParameterParser::~CParameterParser(){
  if(0 != mParameterLocations){
    delete[] mParameterLocations;
  }
}

unsigned int CParameterParser::parseParameters() {
  char* parsePosition = mParameters.getValue();
  unsigned int i = 0;
  if(0 < mAmountOfParameters){
    if('\0' == *parsePosition){ //empty string
      saveStartPositionForParameterSubstring(i++, parsePosition);
    }else{ //non empty string
      for(i = 0; i < mAmountOfParameters; i++){
        if(' ' != mSeparator){
          moveToPositionOfFirstNonWhiteSpaceCharacter (&parsePosition);
        }
        saveStartPositionForParameterSubstring(i, parsePosition);
        moveToPositionOfNextParameterSeparatorOrEndOfString (&parsePosition);
        bool endOfString = ('\0' == *parsePosition);
        if(mParameterLocations[i] < parsePosition){ //avoid going bakcwards when not needed
          trimTrailingWhiteSpacesOfParameterSubstring(parsePosition, ' ' == mSeparator);
        }else if(mParameterLocations[i] == parsePosition){
          *parsePosition = '\0';
        }
        if(endOfString){
          i++;
          break;
        }
        parsePosition++;
      }
    }
  }
  return i;
}

void CParameterParser::moveToPositionOfFirstNonWhiteSpaceCharacter(char** paParsePosition) {
  while (0 != isspace(**paParsePosition)) {
    ++(*paParsePosition);
  }
}

void CParameterParser::saveStartPositionForParameterSubstring(int paParameterNumber, char* paParsePosition) {
  mParameterLocations[paParameterNumber] = paParsePosition;
}

void CParameterParser::moveToPositionOfNextParameterSeparatorOrEndOfString(char** paParsePosition) {
  while (mSeparator != **paParsePosition && '\0' != **paParsePosition) {
    ++(*paParsePosition);
  }
}

void CParameterParser::trimTrailingWhiteSpacesOfParameterSubstring(char* paParsePosition, bool isSpace) {
  char* backTraceCharacter = paParsePosition - 1;
  if(!isSpace){
    while(0 != isspace(*backTraceCharacter)){
      --backTraceCharacter;
    }
  }
  backTraceCharacter++;
  *backTraceCharacter = '\0';
}

const char* CParameterParser::operator [](const int& paIndex) {
  const char* result = (paIndex < static_cast<int>(mAmountOfParameters)) ? mParameterLocations[paIndex] : 0;
  return result;
}
