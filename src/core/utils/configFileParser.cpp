/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Jose Cabral - initial implementation
 *******************************************************************************/

#include "configFileParser.h"
#include "../../arch/devlog.h"
#include "parameterParser.h"

CConfigFileParser::CConfigFileParser(const std::string &paFileLocation) {
  mConfigFile = fopen(paFileLocation.c_str(), "r");
  if(0 != mConfigFile) {
    DEVLOG_INFO("[CConfigFileParser]: Configuration file %s opened\n", paFileLocation.c_str());
  } else {
    DEVLOG_ERROR("[CConfigFileParser]: Could not open configuration file %s\n", paFileLocation.c_str());
  }
}

CConfigFileParser::~CConfigFileParser() {
  if(0 != mConfigFile) {
    DEVLOG_INFO("[CConfigFileParser]: Closing configuration file\n");
    fclose(mConfigFile);
  }
}

CConfigFileParser::ParseResult CConfigFileParser::parseNextLine(std::pair<std::string, std::string> &paResult) {
  ParseResult retVal = eFileNotOpened;
  if(mConfigFile) {
    char lineBuf[scmLineBuffer];
    if(0 != fgets(lineBuf, scmLineBuffer, mConfigFile)) {
      if(0 == strcmp(lineBuf, "\n")) {
        retVal = eEmptyLine;
      } else {
        CParameterParser lineParser(lineBuf, scmSeparator, scmNumberOfElementsInLine);
        if(scmNumberOfElementsInLine == lineParser.parseParameters()) {
          paResult.first = lineParser[0];
          paResult.second = lineParser[1];
          retVal = eOk;
        } else {
          retVal = eWrongLine;
          DEVLOG_ERROR("[CConfigFileParser]: The line %s is wrongly formated\n", lineBuf);
        }
      }
    } else {
      if(feof(mConfigFile)) {
        retVal = eEndOfFile;
      } else {
        retVal = eInternalError;
        DEVLOG_ERROR("[CConfigFileParser]: There was an internal problem reading the configuration file\n");
      }
    }
  } else {
    DEVLOG_ERROR("[CConfigFileParser]: Cannot parse next line because the configuration file is not opened\n");
  }

  return retVal;
}

bool CConfigFileParser::lookForKeyValueInFile(CConfigFileParser &paFileParse, const std::string &paKey, const std::string &paValue, bool &paFound) {

  bool retVal = true;
  bool moreLinesToRead = true;
  paFound = false;

  while(moreLinesToRead) {
    std::pair < std::string, std::string > resultPair;

    switch(paFileParse.parseNextLine(resultPair)){
      case CConfigFileParser::eOk:
        if(0 == resultPair.first.compare(paKey) && 0 == resultPair.second.compare(paValue)) {
          paFound = true;
          moreLinesToRead = false;
        }
        break;
      case CConfigFileParser::eEmptyLine:
        //do nothing, keep reading
        break;
      case CConfigFileParser::eEndOfFile:
        moreLinesToRead = false;
        break;
      default:
        // CConfigFileParser::eWrongLine
        // CConfigFileParser::eFileNotOpened
        // CConfigFileParser::eInternalError
        retVal = false;
        moreLinesToRead = false;
        break;
    }
  }

  return retVal;
}
