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
 * Jose Cabral - Move to a new file and make it more generic
 *******************************************************************************/

#ifndef SRC_CORE_UTILS_PARAMETERPARSER_H_
#define SRC_CORE_UTILS_PARAMETERPARSER_H_

#include "../datatypes/forte_string.h"
#include <vector>

class CParameterParser{
  public:

    /**
     * Creates the parser object
     * @param paParameters Pointer to the string to be parsed. It remains untouched during the life of the object, since the string is copied. A null pointer is treated as an empty string
     * @param paSeparator Separator of the parameters
     * @param paExpectedNumParams Expected number of parameters if known. Not providing it, will still work, but performance is improved when provided
     */
    CParameterParser(const char* paParameters, const char paSeparator, size_t paExpectedNumParams = 0);

    /**
     * Parse the string using the separator passed in the constructor
     * Leading and trailing blank spaces in each parameter are ignored, unless the separator is actually a blank space
     * If the string starts with a separator, the first parameter will be considered an empty one
     * If the string ends with a separator, the last parameter will be considered an empty one
     * An empty string is seen as a one empty parameter
     * @return Number of stored parameters
     */
    size_t parseParameters();

    /**
     * Get the parameter at certain index
     * @param paIndex Index of the desired parameter
     * @return If the index is less than the amount of stored parameters, a pointer to the string, 0 otherwise
     */
    const char* operator[](const size_t paIndex);

  private:
    CIEC_STRING mParameters;
    std::vector<const char *> mParameterLocations;
    const char mSeparator;

    void moveToPositionOfFirstNonWhiteSpaceCharacter(char** paParsePosition) const;
    void saveStartPositionForParameterSubstring(char* paParsePosition);
    void moveToPositionOfNextParameterSeparatorOrEndOfString(char** paParsePosition) const;
    void trimTrailingWhiteSpacesOfParameterSubstring(char* paParsePosition, bool isSpace) const;

    //we don't want that parser to be copied or assigned therefore
    //the copy constructor and assignment operator are declared private
    //but not implemented avoiding cppchecks warnings too
    CParameterParser(const CParameterParser&);
    CParameterParser& operator =(const CParameterParser &);
};

#endif /* SRC_CORE_UTILS_PARAMETERPARSER_H_ */
