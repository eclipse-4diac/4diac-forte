/*******************************************************************************
 * Copyright (c) 2011 nxtControl GmbH, ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Stanislav Meduna, Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _UNICODE_UTILS_H_
#define _UNICODE_UTILS_H_

#include "../../arch/datatype.h"

/*!\ingroup COREDTS  CUnicodeUtilities is a collection of utility methods managing Unicode processing.
 */

/*!\ingroup CORE \defgroup COREDTS Core Data Types
 * \brief Unicode processing.
 *
 */

class CUnicodeUtilities {
  public:
    static const TForteUInt32 scmBOMMarker;
    static const TForteUInt32 scmBOMMarkerSwapped;

    /*! \brief Parse a UTF-8 codepoint
    *
    *   \param paCodepoint  Codepoint to parse
    *   \param paValue  Parsed value
    *   \return Number of bytes consumed or -1 for error
    */
    static int parseUTF8Codepoint(const TForteByte *paCodepoint, TForteUInt32 &paValue);

    /*! \brief Encode a UTF-8 codepoint
    *
    *   \param paCodepoint  Pointer to the codepoint (0 for no output)
    *   \param paBufferSize  Size of the provided buffer.
    *   \param paValue  Input value
    *   \return Number of bytes produced or -1 for error
    */
    static int encodeUTF8Codepoint(TForteByte *paCodepoint, unsigned int paBufferSize, TForteUInt32 paValue);

    /*! \brief Parse a UTF-16 codepoint
    *
    *   \param paCodepoint  Codepoint to parse
    *   \param paValue  Parsed value
    *   \param paLittleEndian  Endianess to use
    *   \return Number of bytes consumed or -1 for error
    */
    static int parseUTF16Codepoint(const TForteByte *paCodepoint, TForteUInt32 &paValue, bool paLittleEndian);

    /*! \brief Encode a UTF-16 codepoint
    *
    *   \param paCodepoint  Pointer to the codepoint (0 for no output)
    *   \param paBufferSize  Size of the provided buffer.
    *   \param paValue  Input value
    *   \param paLittleEndian  Endianess to use
    *   \return Number of bytes produced or -1 for error
    */
    static int encodeUTF16Codepoint(TForteByte *paCodepoint, unsigned int paBufferSize, TForteUInt32 paValue, bool paLittleEndian);

    /*! \brief Check whether a given string is a valid UTF-8 string
    *
    *   \param paString  String to check
    *   \param paLength  Length of the string to check (use -1 for null-terminated strings)
    *   \param paMaxWidth Returns the max. width of the codepoint (BOM is not considered).
    *           7 ... IRV (ASCII)
    *           8 ... Row 00 only
    *          16 ... Basic Multilingual Plane only (UCS-2)
    *          21 ... Rest of UTF-8 (and UTF-16) representable codepoints
    *   \return Number of codepoints in string or -1 for invalid input
    */
    static int checkUTF8(const char *paValue, int paLength, unsigned int &paMaxWidth);
};

#endif /*_UNICODE_UTILS_H_*/
