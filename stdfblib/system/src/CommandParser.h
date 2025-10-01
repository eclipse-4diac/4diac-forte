/*******************************************************************************
 * Copyright (c) 2024, 2025 Jose Cabral
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/mgmcmd.h"
#include "forte/mgmcmdstruct.h"
#include "forte/datatypes/forte_string.h"

#include <string>

namespace forte {
  class CDevice;

  namespace iec61499::system {

    class CommandParser {

      public:
        CommandParser(CDevice &paDevice);

        /**
         * @brief Parse and executes a command on a destination in a device
         *
         * @param paDest destination where to executed the command
         * @param paCommand the command to be executed
         * @param paDevice device where to execute the command
         * @return EMGMResponse response of the execution of the command
         */
        EMGMResponse parseAndExecuteMGMCommand(const char *const paDest, char *paCommand);

        /*! \brief Generate a response string according to the previous executed command
         *
         * @param paResponse generated response, the given string is used to reduce memory load on the system
         */
        void generateResponse(CIEC_STRING &aResponse);

      private:
        SManagementCMD mCommand;

        EMGMResponse mLastResponse{EMGMResponse::InvalidObject};

        CDevice &mDevice;

        /*! \brief Parse the given request header to determine the ID and the requested command
         *
         * \param paRequestString data of the request
         * \param paCommand the command structure for holding command information
         * \return pointer to the next part of the command zero on error
         */
        char *parseRequest(char *paRequestString);

        /*! \brief Parse the given request that is left after parsing the header to parse FB data
         *
         * \param paRequestPartLeft  data of the request that has been left after parsing the header
         * \param paCommand the command structure for holding command information
         * \return true if the FB data could be parsed
         */
        bool parseFBData(char *paRequestPartLeft);

#ifdef FORTE_DYNAMIC_TYPE_LOAD

        /*! \brief Parse the given request that is left after parsing the header to parse FB or Adapter type
         *
         * \param paRequestPartLeft  data of the request that has been left after parsing the header
         * \param paCommand the command structure for holding command information
         * \param pa_requestType the type that should be searched
         * \return true if the FB type could be parsed
         */
        bool parseXType(char *paRequestPartLeft, const char *paRequestType);
#endif // FORTE_DYNAMIC_TYPE_LOAD

        /*! \brief Parse the given request that is left after parsing the header to parse connection data
         *
         * \param paRequestPartLeft   data of the request that has been left after parsing the header
         * \param paCommand the command structure for holding command information
         * \return true if the connection data could be parsed
         */
        bool parseConnectionData(char *paRequestPartLeft);
        bool parseWriteConnectionData(char *paRequestPartLeft);

        void parseCreateData(char *paRequestPartLeft);
        void parseDeleteData(char *paRequestPartLeft);
        //! Check if an FB is given for a state change command (i.e., START, STOP, KILL, RESET)
        void parseAdditionalStateCommandData(char *paRequestPartLeft);
        void parseReadData(char *paRequestPartLeft);
        void parseWriteData(char *paRequestPartLeft);

        void parseQueryData(char *paRequestPartLeft);
        void parseQueryTypes(std::string_view paRequestPartLeft,
                             EMGMCommandType paSingleQueryCMD,
                             EMGMCommandType paListQueryCMD);
        void generateQueryResponse(CIEC_STRING &paResponse);

        /*! \brief parse a hierarchical identifier list
         *
         * The identifiers are separated  by '.' and the end character for the list is '\"'
         *
         * @param paIdentifierStart pointer to the start of the identifier that will be parsed
         * @param paIdentifier identifier vector where to write the parsed identifiers to
         * @return number of bytes used from the character array or -1 if the identifier could not be parsed
         */
        int parseIdentifier(char *paIdentifierStart, TNameIdentifier &paIdentifier);

        /*! \brief Parse the name of the type
         *
         * @param paCmdString string containing the type name that will be parsed
         * @param paIdentifier identifier vector where to write the parsed identifiers to
         * @param paTypeHash  buffer for storing the type hash if present
         * @return number of bytes used from the character array or -1 if the identifier could not be parsed
         */
        int parseTypeName(const std::string_view paCmdString, TNameIdentifier &paIdentifier, std::string &paTypeHash);

        bool parseMonitoringData(char *paRequestPartLeft);
        void generateMonitorResponse(CIEC_STRING &paResponse);

        /*! \brief Generate a short response string according to the previous executed command
         *
         * @param paResponse generated response, the given string is used to reduce memory load on the system
         */
        void generateShortResponse(CIEC_STRING &paResponse);

        /*! \brief Generate a response string according to the previous executed command
         *
         * @param paResponse generated response, the given string is used to reduce memory load on the system
         */
        void generateLongResponse(CIEC_STRING &paResponse);

        void appendIdentifierName(CIEC_STRING &paDest, TNameIdentifier &paIdentifier);
    };

  } // namespace iec61499::system
} // namespace forte
