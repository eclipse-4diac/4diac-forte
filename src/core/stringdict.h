/*******************************************************************************
 * Copyright (c) 2008, 2023 nxtControl GmbH, ACIN, fortiss GmbH,
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Stanislav Meduna, Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *    Martin Jobst
 *      - add string functions accepting a size parameter
 *******************************************************************************/
#ifndef _CStringDictionary_H_
#define _CStringDictionary_H_

#include "forte_config.h"
#include "core/util/singlet.h"
#include "datatype.h"

#include <limits>

/**\ingroup CORE\brief Manages a dictionary of strings that can be referenced by ids
 *
 * Manages a dictionary of strings that can be referenced by ids
 */
// cppcheck-suppress noConstructor
class CStringDictionary{
  DECLARE_SINGLETON(CStringDictionary)
public:
  typedef size_t TStringId;

  //!\brief Invalid string id
  static const TStringId scmInvalidStringId = std::numeric_limits<TStringId>::max();


  /*!\brief Get a string from the dictionary
   *
   * \param paId String id
   * \return the found string or 0 if not found
   */
  const char *get(TStringId paId);

  /*!\brief Insert a string into the dictionary
   *
   * \param paStr String to be inserted
   * \return id of the inserted string (or the existing one if the dictionary already has it)
   */
  TStringId insert(const char *paStr);

  /*!\brief Insert a string into the dictionary
   *
   * \param paStr String to be searched for (need not be '\0' terminated)
   * \param paStrSize Size of the string (excluding any terminating '\0' character)
   * \return id of the inserted string (or the existing one if the dictionary already has it)
   */
  TStringId insert(const char *paStr, size_t paStrSize);

  /*!\brief Retrieve the Id of a given string if it is already in the dictionary
   *
   * \param paStr String to be searched for
   * \return id of the string (or scmInvalidStringId if it is not in the dictionary)
   */
  TStringId getId(const char *paStr) const{
    unsigned int nIdx;
    return findEntry(paStr, nIdx);
  }

  /*!\brief Retrieve the Id of a given string if it is already in the dictionary
   *
   * \param paStr String to be searched for (need not be '\0' terminated)
   * \param paStrSize Size of the string (excluding any terminating '\0' character)
   * \return id of the string (or scmInvalidStringId if it is not in the dictionary)
   */
  TStringId getId(const char *paStr, size_t paStrSize) const{
    unsigned int nIdx;
    return findEntry(paStr, paStrSize, nIdx);
  }
private:
  //!\brief Remove all dictionary entries
  void clear();

  // Find an exact match or place to be the new index
  TStringId findEntry(const char *paStr, unsigned int &paIdx) const;
  TStringId findEntry(const char *paStr, size_t paStrSize, unsigned int &paIdx) const;

  // Reallocate the buffer
  bool reallocateStringIdBuf(unsigned int paNewMaxNrOfStrings);
  bool reallocateStringBuf(size_t paNewBufSize);

  // Insert the string at the specified position
  TStringId insertAt(const char *paStr, unsigned int paIdx, size_t paLen);

  // Get an address
  const char *getStringAddress(TStringId paId) const {
    return mStringBufAddr + paId;
  };

  char *getStringAddress(TStringId paId) {
    return mStringBufAddr + paId;
  };

  //!Buffer for the String Ids. The Ids are sorted according to their values they are pointing at
  TStringId *mStringIdBufAddr;

  //! Buffer for the strings
  char *mStringBufAddr;

  // Size of the allocated space
  size_t mStringBufSize;

  // Maximum number of strings we can hold (size of the StringIdBufer)
  unsigned int mMaxNrOfStrings;

  // Number of strings we are actually holding
  unsigned int mNrOfStrings;

  // Next string gets written here
  TStringId mNextString;

#ifdef FORTE_STRING_DICT_FIXED_MEMORY
  static TStringId scmIdList[cgStringDictInitialMaxNrOfStrings];
  static char scmConstStringBuf[cgStringDictInitialStringBufSize];
#else
  static const TStringId scmIdList[];
  static const char scmConstStringBuf[];
#endif
};

#endif // _CStringDictionary_H_
