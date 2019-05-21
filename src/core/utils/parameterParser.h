/*******************************************************************************
 * Copyright (c) 2016, 2017 ACIN - fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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
     * Creates the parser object with a defined numbers of parameters
     * @param paParameters Pointer to the string to be parsed. It remains untouched during the life of the object, since the string is copied. A null pointer is treated as an empty string
     * @param paAmountOfParameters Amount of parameter to look for
     * @param paSeparator Separator of the parameters
     */
    CParameterParser(const char* paParameters, size_t paAmountOfParameters, const char paSeparator);

    /**
     * Creates the parser object with undefined numbers of parameters
     * @param paParameters Pointer to the string to be parsed. It remains untouched during the life of the object, since the string is copied. A null pointer is treated as an empty string
     * @param paSeparator Separator of the parameters
     */
    CParameterParser(const char* paParameters, const char paSeparator);

    /**
     * Parse the string using the separator passed in the constructor
     * Leading and trailing blank spaces in each parameter are ignored, unless the separator is actually a blank space
     * If the string has less parameters than the amount specified in the constructor, the parsed amount of parameters is stored
     * If the string has more parameters than the amount specified in the constructor, the the specified amount is stored
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
    const size_t mMaxAmountOfParameters;
    size_t mActualAmountOfParameters;
    const bool mAmountUndefined;
    const char mSeparator;

    void moveToPositionOfFirstNonWhiteSpaceCharacter(char** paParsePosition);
    void saveStartPositionForParameterSubstring(char* paParsePosition);
    void moveToPositionOfNextParameterSeparatorOrEndOfString(char** paParsePosition);
    void trimTrailingWhiteSpacesOfParameterSubstring(char* paParsePosition, bool isSpace);

    //we don't want that parser to be copied or assigned therefore
    //the copy constructor and assignment operator are declared private
    //but not implemented avoiding cppchecks warnings too
    CParameterParser(const CParameterParser&);
    CParameterParser& operator =(const CParameterParser &);
};

#endif /* SRC_CORE_UTILS_PARAMETERPARSER_H_ */
