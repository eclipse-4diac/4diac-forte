/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
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
#include <open62541.h>
#include <string>

/**
 * Parse the configuration file for OPC UA clients. It supports the following keys:
 *   endpoint (string)
 *   username (string)
 *   password (string)
 *   certificate (path to file, only if encryption is enabled)
 *   privateKey (path to file, only if encryption is enabled)
 *   securityMode (integer, only if encryption is enabled. 1: None; 2:Sign; 3: Sign and encrypt)
 *   securityPolicy (string URI, for example http://opcfoundation.org/UA/SecurityPolicy#Basic256, only if encryption is enabled)
 *
 *   The parser looks for an endpoint, and only after it matches the one passed as argument, it starts storing the following information, and
 *   it will keep reading after another endpoint other the end of file is found
 */
class CUA_ClientConfigFileParser {
  public:

    /**
     * Encapsulation for the result used by the class when parsing the configuration file
     */
    class UA_ConfigFromFile {
      public:
        UA_ConfigFromFile(UA_ClientConfig &paClientConfig, std::string &paUsername, std::string &paPassword) :
            mClientConfig(paClientConfig), mUsername(paUsername), mPassword(paPassword) {
        }
        UA_ClientConfig &mClientConfig;
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
    static bool loadConfig(const std::string &paFileLocation, const std::string &paEndpoint, UA_ConfigFromFile &paResult);

  private:
    explicit CUA_ClientConfigFileParser();
    virtual ~CUA_ClientConfigFileParser();

    /**
     * Allowed keys in the configuration file. If a new key is to be added, it should go before eKeyUnknown
     */
    enum UA_KeyType {
      eEndpoint,
      eUsername,
      ePassword,
#ifdef UA_ENABLE_ENCRYPTION
      eCertificate,
      ePrivateKey,
      eSecurityMode,
      eSecurityPolicy,
#endif //UA_ENABLE_ENCRYPTION
      eKeyUnknown,
    };

    /**
     * String representations of the possible keys
     */
    static const char *const mKeyNames[eKeyUnknown];

#ifdef UA_ENABLE_ENCRYPTION
    /**
     * Load a file into a byte string. Used for certifcates and privated key files
     * @param paFileLocation Path to the file to be loaded
     * @param paResult Place to store the result
     * @return True if no error occurred, false otherwise
     */
    static bool loadFileIntoBytestring(const std::string &paFileLocation, UA_ByteString &paResult);
#endif //UA_ENABLE_ENCRYPTION
};

#endif /* SRC_MODULES_OPC_UA_OPCUA_CLIENT_CONFIG_PARSER_H_ */
