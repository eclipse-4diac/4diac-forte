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
 *    Stanislav Meduna, Ingo Hegny, Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "unicode_utils.h"
#include <string.h>

const TForteUInt32 CUnicodeUtilities::scmBOMMarker = 0xfeff;
const TForteUInt32 CUnicodeUtilities::scmBOMMarkerSwapped = 0xfffe;

/* RFC 3629
   Char. number range  |        UTF-8 octet sequence
      (hexadecimal)    |              (binary)
   --------------------+---------------------------------------------
   0000 0000-0000 007F | 0xxxxxxx
   0000 0080-0000 07FF | 110xxxxx 10xxxxxx
   0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
   0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
*/
int CUnicodeUtilities::parseUTF8Codepoint(const TForteByte *paCodepoint, TForteUInt32 &paValue) {
  if ((paCodepoint[0] & 0x80) == 0) {
    paValue = (paCodepoint[0] & 0x7f);
    return 1;
  } else if ((paCodepoint[0] & 0xe0) == 0xc0) {
    if((paCodepoint[1] & 0xc0) != 0x80) {
      return -1;
    }
    paValue = ((paCodepoint[0] & 0x1f) << 6) |
                (paCodepoint[1] & 0x3f);
    return 2;
  } else if ((paCodepoint[0] & 0xf0) == 0xe0) {
    if ((paCodepoint[1] & 0xc0) != 0x80 ||
        (paCodepoint[2] & 0xc0) != 0x80) {
      return -1;
    }
    paValue = ((paCodepoint[0] & 0x0f) << 12) |
                ((paCodepoint[1] & 0x3f) <<  6) |
                (paCodepoint[2] & 0x3f);
    return 3;
  } else if ((paCodepoint[0] & 0xf8) == 0xf0) {
    if ((paCodepoint[1] & 0xc0) != 0x80 ||
        (paCodepoint[2] & 0xc0) != 0x80 ||
        (paCodepoint[3] & 0xc0) != 0x80) {
      return -1;
    }
    paValue = ((paCodepoint[0] & 0x07) << 18) |
                ((paCodepoint[1] & 0x3f) << 12) |
                ((paCodepoint[2] & 0x3f) <<  6) |
                (paCodepoint[3] & 0x3f);
    return 4;
  }

  paValue = 0;
  return -1;
}

int CUnicodeUtilities::encodeUTF8Codepoint(TForteByte *paCodepoint, unsigned int paBufferSize, TForteUInt32 paValue) {
  if (paValue < 0x80) {
    if (paCodepoint) {
      if(paBufferSize < 1) {
        return -1;
      }
      paCodepoint[0] = (TForteByte) paValue;
    }
    return 1;
  } else if (paValue < 0x800) {
    if (paCodepoint) {
      if(paBufferSize < 2) {
        return -1;
      }
      paCodepoint[0] = (TForteByte) (0xc0 | ((paValue >> 6) & 0x1f));
      paCodepoint[1] = (TForteByte) (0x80 | ((paValue     ) & 0x3f));
    }
    return 2;
  } else if (paValue < 0x10000) {
    if (paCodepoint) {
      if(paBufferSize < 3) {
        return -1;
      }
      paCodepoint[0] = (TForteByte) (0xe0 | ((paValue >> 12) & 0x1f));
      paCodepoint[1] = (TForteByte) (0x80 | ((paValue >>  6) & 0x3f));
      paCodepoint[2] = (TForteByte) (0x80 | ((paValue      ) & 0x3f));
    }
    return 3;
  } else if (paValue < 0x110000) {
    if (paCodepoint) {
      if(paBufferSize < 4) {
        return -1;
      }
      paCodepoint[0] = (TForteByte) (0xf0 | ((paValue >> 18) & 0x1f));
      paCodepoint[1] = (TForteByte) (0x80 | ((paValue >> 12) & 0x3f));
      paCodepoint[2] = (TForteByte) (0x80 | ((paValue >>  6) & 0x3f));
      paCodepoint[3] = (TForteByte) (0x80 | ((paValue      ) & 0x3f));
    }
    return 4;
  }

  return -1;
}

/*
 * UTF-16 converts > 0xffff into two 16-bit code units, called a surrogate pair, by the following scheme:
 *
 * 0x10000 is subtracted from the code point, leaving a 20 bit number in the range 0..0xFFFFF.
 * The top ten bits (a number in the range 0..0x3FF) are added to 0xD800 to give the first code unit
 *   or high surrogate, which will be in the range 0xD800..0xDBFF.
 * The low ten bits (also in the range 0..0x3FF) are added to 0xDC00 to give the second
 *   code unit or low surrogate, which will be in the range 0xDC00..0xDFFF.
*/
int CUnicodeUtilities::parseUTF16Codepoint(const TForteByte *paCodepoint, TForteUInt32 &paValue, bool paLittleEndian) {
  TForteUInt16 nFirstWord = static_cast<TForteUInt16>(paLittleEndian ?
                                                        (paCodepoint[0] | (paCodepoint[1] << 8)) :
                                                        ((paCodepoint[0] << 8) | paCodepoint[1]));

  if ((nFirstWord & 0xfc00) == 0xd800) {
    TForteUInt16 nSecondWord = static_cast<TForteUInt16>(paLittleEndian ?
      (paCodepoint[2] | (paCodepoint[3] << 8)) :
      ((paCodepoint[2] << 8) | paCodepoint[3]));

    if((nSecondWord & 0xfc00) != 0xdc00) {
      return -1;
    }

    paValue = 0x10000 + (((nFirstWord & 0x3ff) << 10) | (nSecondWord & 0x3ff));

    return 4;
  }

  paValue = nFirstWord;

  return 2;
}


int CUnicodeUtilities::encodeUTF16Codepoint(TForteByte *paCodepoint, unsigned int paBufferSize, TForteUInt32 paValue, bool paLittleEndian) {
  if((paValue >= 0xd800 && paValue < 0xe000) || paValue >= 0x110000) {
    return -1;
  }

  if (paValue < 0x10000) {
    if (paCodepoint) {
      if(paBufferSize < 2) {
        return -1;
      }
      if (paLittleEndian) {
        paCodepoint[0] = (TForteByte) ((paValue     ) & 0xff);
        paCodepoint[1] = (TForteByte) ((paValue >> 8) & 0xff);
      } else {
        paCodepoint[0] = (TForteByte) ((paValue >> 8) & 0xff);
        paCodepoint[1] = (TForteByte) ((paValue     ) & 0xff);
      }
    }
    return 2;
  } else {
    if (paCodepoint) {
      TForteUInt32 nVal = paValue - 0x10000;
      TForteUInt16 nHighSurrogate = static_cast<TForteUInt16>(0xd800 | ((nVal >> 10) & 0x3ff));
      TForteUInt16 nLowSurrogate  = static_cast<TForteUInt16>(0xdc00 | ((nVal      ) & 0x3ff));

      if(paBufferSize < 4) {
        return -1;
      }

      if (paLittleEndian) {
        paCodepoint[0] = (TForteByte) ((nHighSurrogate     ) & 0xff);
        paCodepoint[1] = (TForteByte) ((nHighSurrogate >> 8) & 0xff);
        paCodepoint[2] = (TForteByte) ((nLowSurrogate      ) & 0xff);
        paCodepoint[3] = (TForteByte) ((nLowSurrogate  >> 8) & 0xff);
      } else {
        paCodepoint[0] = (TForteByte) ((nHighSurrogate >> 8) & 0xff);
        paCodepoint[1] = (TForteByte) ((nHighSurrogate     ) & 0xff);
        paCodepoint[2] = (TForteByte) ((nLowSurrogate  >> 8) & 0xff);
        paCodepoint[3] = (TForteByte) ((nLowSurrogate      ) & 0xff);
      }
    }
    return 4;
  }
}

int CUnicodeUtilities::checkUTF8(const char *paValue, int paLength, unsigned int &paMaxWidth) {
  const TForteByte *pRunner = (const TForteByte *) paValue;
  size_t nRemLen = (paLength == -1) ? strlen(paValue) : (size_t) paLength;
  TForteUInt32 nCodepoint;
  int nRetVal = 0;
  size_t i = 0;
  paMaxWidth = 7;

  while (i < nRemLen) {
    int nRes = parseUTF8Codepoint(pRunner, nCodepoint);
    if (nRes < 0 || nRes + i > nRemLen) {
      return -1;
    }
    if(nCodepoint != CUnicodeUtilities::scmBOMMarker) {
      if(nCodepoint >= 0x10000) {
        paMaxWidth = 21;
      } else if(nCodepoint >= 0x100 && paMaxWidth < 16) {
        paMaxWidth = 16;
      } else if(nCodepoint >= 0x80 && paMaxWidth < 8) {
        paMaxWidth = 8;
      }
    }

    i += nRes;
    pRunner += nRes;
    ++nRetVal;
  }

  return nRetVal;
}
