/*******************************************************************************
 * Copyright (c) 2016, 2017 ACIN - fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Martin Melik Merkumians - initial API and implementation and/or initial documentation
 * Jose Cabral - Move to a new file and make the amount of parameter a variable
 *******************************************************************************/

#include "parameterParser.h"
#include <string.h>
#include <cctype>

CParameterParser::CParameterParser(const char* paParameters, const char paSeparator, size_t paExpectedNumParams) :
    mSeparator(paSeparator) {
  mParameterLocations.reserve(paExpectedNumParams);
  if (paParameters != nullptr)
  { // make a copy of the parameter string for processing
    mParameters = new char[strlen(paParameters) + 1]();
    strcpy(mParameters, paParameters);
      } else { // if nullptr provide valid empty size-1 char array
        mParameters = new char[1]();
      }
}

CParameterParser::~CParameterParser() {
  delete[] mParameters;
}

size_t CParameterParser::parseParameters() {
  char* parsePosition = mParameters;
  bool endOfString = ('\0' == *parsePosition);
  if(endOfString) { //empty string
    saveStartPositionForParameterSubstring(parsePosition);
  } else { //non empty string
    while(!endOfString) {
      if(' ' != mSeparator) {
        moveToPositionOfFirstNonWhiteSpaceCharacter(&parsePosition);
      }
      saveStartPositionForParameterSubstring(parsePosition);
      moveToPositionOfNextParameterSeparatorOrEndOfString(&parsePosition);
      endOfString = ('\0' == *parsePosition);
      if(mParameterLocations.back() < parsePosition) { //avoid going backwards when not needed
        trimTrailingWhiteSpacesOfParameterSubstring(parsePosition, ' ' == mSeparator);
      } else { //mParameterLocations[i] == parsePosition. parsePosition cannot be behind
        *parsePosition = '\0';
      }
      if(!endOfString) {
        parsePosition++;
      }
    }
  }
  return mParameterLocations.size();
}

void CParameterParser::moveToPositionOfFirstNonWhiteSpaceCharacter(char** paParsePosition) const {
  while(0 != isspace(**paParsePosition)) {
    ++(*paParsePosition);
  }
}

void CParameterParser::saveStartPositionForParameterSubstring(char* paParsePosition) {
  mParameterLocations.push_back(paParsePosition);
}

void CParameterParser::moveToPositionOfNextParameterSeparatorOrEndOfString(char** paParsePosition) const {
  while(mSeparator != **paParsePosition && '\0' != **paParsePosition) {
    ++(*paParsePosition);
  }
}

void CParameterParser::trimTrailingWhiteSpacesOfParameterSubstring(char* paParsePosition, bool isSpace) const {
  char* backTraceCharacter = paParsePosition - 1;
  if(!isSpace) {
    while(0 != isspace(*backTraceCharacter)) {
      --backTraceCharacter;
    }
  }
  backTraceCharacter++;
  *backTraceCharacter = '\0';
}

const char* CParameterParser::operator [](const size_t paIndex) {
  const char* result = (paIndex < mParameterLocations.size()) ? mParameterLocations[paIndex] : nullptr;
  return result;
}
