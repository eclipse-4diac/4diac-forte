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

CParameterParser::CParameterParser(const char* paParameters, size_t paAmountOfParameters, const char paSeparator) :
    mParameters(paParameters), mMaxAmountOfParameters(paAmountOfParameters), mActualAmountOfParameters(0), mAmountUndefined(false), mSeparator(paSeparator) {
  mParameterLocations.reserve(paAmountOfParameters);
}

CParameterParser::CParameterParser(const char* paParameters, const char paSeparator) :
    mParameters(paParameters), mMaxAmountOfParameters(0), mActualAmountOfParameters(0), mAmountUndefined(true), mSeparator(paSeparator) {
}

size_t CParameterParser::parseParameters() {
  char* parsePosition = mParameters.getValue();
  size_t i = 0;
  if(0 < mMaxAmountOfParameters || mAmountUndefined) {
    if('\0' == *parsePosition){ //empty string
      saveStartPositionForParameterSubstring(parsePosition);
      i++;
    }else{ //non empty string
      for(i = 0; i < mMaxAmountOfParameters || mAmountUndefined; i++) {
        if(' ' != mSeparator){
          moveToPositionOfFirstNonWhiteSpaceCharacter(&parsePosition);
        }
        saveStartPositionForParameterSubstring(parsePosition);
        moveToPositionOfNextParameterSeparatorOrEndOfString (&parsePosition);
        bool endOfString = ('\0' == *parsePosition);
        if(mParameterLocations[i] < parsePosition) { //avoid going backwards when not needed
          trimTrailingWhiteSpacesOfParameterSubstring(parsePosition, ' ' == mSeparator);
        } else { //mParameterLocations[i] == parsePosition. parsePosition cannot be behind
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
  mActualAmountOfParameters = i;
  return i;
}

void CParameterParser::moveToPositionOfFirstNonWhiteSpaceCharacter(char** paParsePosition) {
  while (0 != isspace(**paParsePosition)) {
    ++(*paParsePosition);
  }
}

void CParameterParser::saveStartPositionForParameterSubstring(char* paParsePosition) {
  mParameterLocations.push_back(paParsePosition);
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

const char* CParameterParser::operator [](const size_t paIndex) {
  const char* result = (paIndex < mActualAmountOfParameters) ? mParameterLocations[paIndex] : 0;
  return result;
}
