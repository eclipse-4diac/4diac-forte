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

namespace forte::iec61499::system {

  namespace detail {
    class CommandScanner {
      public:
        CommandScanner(std::string_view paRemaining);

        void skipWhiteSpace();
        bool consume(std::string_view paToConsume);
        std::string_view takeUntil(char paDelimiter);
        char peek() const;
        bool empty() const;

      private:
        std::string_view mRemaining;
    };
  } // namespace detail

  class CommandParser {

    public:
      CommandParser(SManagementCMD &paCommand);

      /**
       * @brief Parse a command
       *
       * @param paDest destination where to executed the command
       * @param paCommand the command to be executed
       * @param paDevice device where to execute the command
       * @return EMGMResponse response of the execution of the command
       */
      EMGMResponse parseMGMCommand(std::string_view paDest, std::string_view paCommand);

      /*! \brief Generate a response string according to the previous executed command
       *
       * @param paResponseText generated response, the given string is used to reduce memory load on the system
       * @param paResponse response provided by the parser or device management
       */
      void generateResponse(CIEC_STRING &paResponseText, EMGMResponse paResponse);

    private:
      SManagementCMD &mCommand;

      /*! \brief Parse the given request header to determine the ID and the requested command
       *
       * \param paScanner reference to the scanner with the command string to parse
       * \return EMGMResponse::Ready if a request tag could be correctly parsed else an according error response
       */
      EMGMResponse parseRequest(detail::CommandScanner &paScanner);

      /*! \brief Parse the given request that is left after parsing the header to parse FB data
       *
       * \param paScanner reference to the scanner with the command string to parse
       * \return EMGMResponse::Ready if FB data could be parsed
       */
      EMGMResponse parseFBData(detail::CommandScanner &paScanner);

      /*! \brief Parse the given request that is left after parsing the header to parse connection data
       *
       * \param paScanner reference to the scanner with the connection string to parse
       * \return EMGMResponse::Ready if connection data could be parsed
       */
      EMGMResponse parseConnectionData(detail::CommandScanner &paScanner);
      EMGMResponse parseConnectionContent(detail::CommandScanner &paScanner);

      EMGMResponse parseCreateData(detail::CommandScanner &paScanner);
      EMGMResponse parseDeleteData(detail::CommandScanner &paScanner);
      //! Check if an FB is given for a state change command (i.e., START, STOP, KILL, RESET)
      EMGMResponse parseAdditionalStateCommandData(detail::CommandScanner &paScanner);
      EMGMResponse parseReadData(detail::CommandScanner &paScanner);
      EMGMResponse parseWriteData(detail::CommandScanner &paScanner);

      EMGMResponse parseQueryData(detail::CommandScanner &paScanner);
      EMGMResponse parseQueryTypes(detail::CommandScanner &paScanner,
                                   EMGMCommandType paSingleQueryCMD,
                                   EMGMCommandType paListQueryCMD);
      void generateQueryResponse(CIEC_STRING &paResponse);

      /*! \brief parse a hierarchical identifier list
       *
       * The identifiers are separated  by '.'
       *
       * @param paIdentifierString string value of the identifier
       * @param paIdentifier identifier vector where to write the parsed identifiers to
       * @return EMGMResponse::Ready if all segments could be parsed and added to the name identifier
       */
      EMGMResponse parseIdentifier(std::string_view paIdentifierString, TNameIdentifier &paIdentifier);

      /*! \brief Parse the name of the type
       *
       * @param paTypeString string of the type name
       * @param paIdentifier identifier vector where to write the parsed identifiers to
       * @param paTypeHash  buffer for storing the type hash if present
       * @return true when type name is correctly parsed and added to the identifier
       */
      bool parseTypeName(const std::string_view paTypeString, TNameIdentifier &paIdentifier, std::string &paTypeHash);

      EMGMResponse parseMonitoringData(detail::CommandScanner &paScanner);
      void generateMonitorResponse(CIEC_STRING &paResponseText, EMGMResponse paResponse);

      /*! \brief Generate a short response string according to the previous executed command
       *
       * @param paResponseText generated response, the given string is used to reduce memory load on the system
       * @param paResponse response provided by the parser or device management
       */
      void generateShortResponse(CIEC_STRING &paResponseText, EMGMResponse paResponse);

      /*! \brief Generate a response string according to the previous executed command
       *
       * @param paResponseText generated response, the given string is used to reduce memory load on the system
       * @param paResponse response provided by the parser or device management
       */
      void generateLongResponse(CIEC_STRING &paResponseText, EMGMResponse paResponse);

      void appendIdentifierName(CIEC_STRING &paDest, TNameIdentifier &paIdentifier);
  };

} // namespace forte::iec61499::system
