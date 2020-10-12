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

#ifndef SRC_CORE_UTILS_CONFIGFILEPARSER_H_
#define SRC_CORE_UTILS_CONFIGFILEPARSER_H_

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <utility> // needed for std::pair

/**
 * Helper class to parse configuration files. For now, all lines must be formed as key=value or empty lines
 */
class CConfigFileParser {
  public:

    /**
     * Possible results that can be returned when parsing a line
     */
    enum ParseResult {
      eOk, //!< eOk No problem occurred
      eEmptyLine, //!< eEmptyLine An empty line was found
      eEndOfFile, //!< eEndOfFile The end of the file was found
      eWrongLine, //!< eWrongLine A wrongly formed line was found
      eFileNotOpened, //!< eFileNotOpened The file is not opened
      eInternalError, //!< eInternalError An internal error occurred
    };

    /**
     * Constructor
     * @param paFileLocation location of the configuration file to be parsed
     */
    explicit CConfigFileParser(const std::string &paFileLocation);

    /**
     * Destructor
     */
    virtual ~CConfigFileParser();

    /**
     * Parse the next line of the file. The key and value are stored in the passed parameter
     * @param paResult Place to store the key and value
     * @return Result of the parsing
     */
    ParseResult parseNextLine(std::pair<std::string, std::string> &paResult);

    /**
     * Looks for a key and value in the file that matches the values passed as parameter
     * @param paFileParse Parser object used to look for the key/value
     * @param paKey Key to look for in the file
     * @param paValue Value to look for
     * @param paFound True if the key/value was found, false otherwise
     * @return True if no error occurred, false otherwise. If the key is found but no the value, it's still a valid result and paFound is false
     */
    static bool lookForKeyValueInFile(CConfigFileParser &paFileParse, const std::string &paKey, const std::string &paValue, bool &paFound);

  private:

    /**
     * File to be parsed
     */
    FILE *mConfigFile;

    static const size_t scmLineBuffer = 256;
    static const char scmSeparator = '=';
    static const size_t scmNumberOfElementsInLine = 2;

};

#endif /* SRC_CORE_UTILS_CONFIGFILEPARSER_H_ */
