/*******************************************************************************
 * Copyright (c) 2020 fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial implementation
 *******************************************************************************/

#include "MQTTClientConfigParser.h"

#include "arch/devlog.h"

const char *const CMQTTClientConfigFileParser::mConfigKeysNames[] = {
  "endpoint",
  "username",
  "password" };

bool CMQTTClientConfigFileParser::loadConfig(std::string &paFileLocation, std::string &paEndpoint, MQTTConfigFromFile &paResult) {
  bool retVal = true;

  CConfigFileParser configFileParser(paFileLocation);
  bool endpointFound = false;
  std::string endpointKey = mConfigKeysNames[eEndoint];

  if(CConfigFileParser::lookForKeyValueInFile(configFileParser, endpointKey, paEndpoint, endpointFound)) {
    if(endpointFound) {

      bool moreLinesToRead = true;
      while(moreLinesToRead) {
        std::pair<std::string, std::string> resultPair;

        switch(configFileParser.parseNextLine(resultPair)){
          case CConfigFileParser::eOk:
            if(0 == resultPair.first.compare(mConfigKeysNames[eEndoint])) {
              moreLinesToRead = false;
            } else if(0 == resultPair.first.compare(mConfigKeysNames[eUsername])) {
              paResult.mUsername = resultPair.second;
            } else if(0 == resultPair.first.compare(mConfigKeysNames[ePassword])) {
              paResult.mPassword = resultPair.second;
            }
            else {
              DEVLOG_WARNING("[CMQTTClientConfigFileParser]: They %s was not recognized so it will be omitted\n", resultPair.first.c_str());
            }
            break;
          case CConfigFileParser::eEmptyLine:
            //do nothing, keep reading
            break;
          case CConfigFileParser::eEndOfFile:
            moreLinesToRead = false;
            break;
          case CConfigFileParser::eWrongLine:
          case CConfigFileParser::eFileNotOpened:
          case CConfigFileParser::eInternalError:
          default:
            retVal = false;
            moreLinesToRead = false;
            break;
        }
      }
    } else { //if the endpoint is not found, configuration is initialize as default
      DEVLOG_INFO("[CMQTTClientConfigFileParser]: No entry for endpoint %s was found in file %s\n", paEndpoint.c_str(), paFileLocation.c_str());
    }
  } else { //if lookForEndpointInFile fails, the errors was already logged
    retVal = false;
  }

  return retVal;
}

