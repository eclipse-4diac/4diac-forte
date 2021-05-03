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

#ifndef SRC_MODULES_OPC_UA_OPCUA_CLIENT_CONFIG_PARSER_H_
#define SRC_MODULES_OPC_UA_OPCUA_CLIENT_CONFIG_PARSER_H_

#include "utils/configFileParser.h"
#include <core/datatypes/forte_string.h>
#include <string>

/**
 * Parse the configuration file for the mqtt client. It supports the following keys:
 *   endpoint (string)
 *   username (string)
 *   password (string)
 *
 *   The parser looks for an endpoint, and only after it matches the one passed as argument, it starts storing the following information, and
 *   it will keep reading after another endpoint other the end of file is found
 */
class CMQTTClientConfigFileParser {
  public:

    /**
     * Encapsulation for the result used by the class when parsing the configuration file
     */
    class MQTTConfigFromFile {
      public:
        MQTTConfigFromFile(std::string &paUsername, std::string &paPassword) :
            mUsername(paUsername), mPassword(paPassword) {
        }
        std::string &mUsername;
        std::string &mPassword;
    };

    /**
     * Reads a configuration file, parse it and store the data
     * @param paFileLocation Path to file to be parsed
     * @param paEndpoint Endpoint of the client to be looked for
     * @param paResult Place to store the results
     * @return True if no error occurred, false otherwise
     */
    static bool loadConfig(std::string &paFileLocation, std::string &paEndpoint, MQTTConfigFromFile &paResult);

  private:
    explicit CMQTTClientConfigFileParser();
    virtual ~CMQTTClientConfigFileParser();

    /**
     * Allowed key in the configuration file for the mqtt clients
     */
    enum MQTTConfigKeys {
      eEndoint, /**< eEndoint */
      eUsername,/**< eUsername */
      ePassword,/**< ePassword */
      eUnknown /**< eUnknown */
    };

    /**
     * Strings for the allowed keys in the configuration file for the mqtt clients
     */
    static const char *const mConfigKeysNames[eUnknown];

};

#endif /* SRC_MODULES_OPC_UA_OPCUA_CLIENT_CONFIG_PARSER_H_ */
