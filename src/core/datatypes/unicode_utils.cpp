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

const TForteUInt32 CUnicodeUtilities::scm_unBOMMarker = 0xfeff;
const TForteUInt32 CUnicodeUtilities::scm_unBOMMarkerSwapped = 0xfffe;

/* RFC 3629
   Char. number range  |        UTF-8 octet sequence
      (hexadecimal)    |              (binary)
   --------------------+---------------------------------------------
   0000 0000-0000 007F | 0xxxxxxx
   0000 0080-0000 07FF | 110xxxxx 10xxxxxx
   0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
   0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
*/
int CUnicodeUtilities::parseUTF8Codepoint(const TForteByte *pa_pacCodepoint, TForteUInt32 &pa_rValue) {
  if ((pa_pacCodepoint[0] & 0x80) == 0) {
    pa_rValue = (pa_pacCodepoint[0] & 0x7f);
    return 1;
  } else if ((pa_pacCodepoint[0] & 0xe0) == 0xc0) {
    if((pa_pacCodepoint[1] & 0xc0) != 0x80) {
      return -1;
    }
    pa_rValue = ((pa_pacCodepoint[0] & 0x1f) << 6) |
                (pa_pacCodepoint[1] & 0x3f);
    return 2;
  } else if ((pa_pacCodepoint[0] & 0xf0) == 0xe0) {
    if ((pa_pacCodepoint[1] & 0xc0) != 0x80 ||
        (pa_pacCodepoint[2] & 0xc0) != 0x80) {
      return -1;
    }
    pa_rValue = ((pa_pacCodepoint[0] & 0x0f) << 12) |
                ((pa_pacCodepoint[1] & 0x3f) <<  6) |
                (pa_pacCodepoint[2] & 0x3f);
    return 3;
  } else if ((pa_pacCodepoint[0] & 0xf8) == 0xf0) {
    if ((pa_pacCodepoint[1] & 0xc0) != 0x80 ||
        (pa_pacCodepoint[2] & 0xc0) != 0x80 ||
        (pa_pacCodepoint[3] & 0xc0) != 0x80) {
      return -1;
    }
    pa_rValue = ((pa_pacCodepoint[0] & 0x07) << 18) |
                ((pa_pacCodepoint[1] & 0x3f) << 12) |
                ((pa_pacCodepoint[2] & 0x3f) <<  6) |
                (pa_pacCodepoint[3] & 0x3f);
    return 4;
  }

  pa_rValue = 0;
  return -1;
}

int CUnicodeUtilities::encodeUTF8Codepoint(TForteByte *pa_pacCodepoint, unsigned int pa_nBufferSize, TForteUInt32 pa_nValue) {
  if (pa_nValue < 0x80) {
    if (pa_pacCodepoint) {
      if(pa_nBufferSize < 1) {
        return -1;
      }
      pa_pacCodepoint[0] = (TForteByte) pa_nValue;
    }
    return 1;
  } else if (pa_nValue < 0x800) {
    if (pa_pacCodepoint) {
      if(pa_nBufferSize < 2) {
        return -1;
      }
      pa_pacCodepoint[0] = (TForteByte) (0xc0 | ((pa_nValue >> 6) & 0x1f));
      pa_pacCodepoint[1] = (TForteByte) (0x80 | ((pa_nValue     ) & 0x3f));
    }
    return 2;
  } else if (pa_nValue < 0x10000) {
    if (pa_pacCodepoint) {
      if(pa_nBufferSize < 3) {
        return -1;
      }
      pa_pacCodepoint[0] = (TForteByte) (0xe0 | ((pa_nValue >> 12) & 0x1f));
      pa_pacCodepoint[1] = (TForteByte) (0x80 | ((pa_nValue >>  6) & 0x3f));
      pa_pacCodepoint[2] = (TForteByte) (0x80 | ((pa_nValue      ) & 0x3f));
    }
    return 3;
  } else if (pa_nValue < 0x110000) {
    if (pa_pacCodepoint) {
      if(pa_nBufferSize < 4) {
        return -1;
      }
      pa_pacCodepoint[0] = (TForteByte) (0xf0 | ((pa_nValue >> 18) & 0x1f));
      pa_pacCodepoint[1] = (TForteByte) (0x80 | ((pa_nValue >> 12) & 0x3f));
      pa_pacCodepoint[2] = (TForteByte) (0x80 | ((pa_nValue >>  6) & 0x3f));
      pa_pacCodepoint[3] = (TForteByte) (0x80 | ((pa_nValue      ) & 0x3f));
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
int CUnicodeUtilities::parseUTF16Codepoint(const TForteByte *pa_pacCodepoint, TForteUInt32 &pa_rValue, bool pa_bLittleEndian) {
  TForteUInt16 nFirstWord = static_cast<TForteUInt16>(pa_bLittleEndian ?
                                                        (pa_pacCodepoint[0] | (pa_pacCodepoint[1] << 8)) :
                                                        ((pa_pacCodepoint[0] << 8) | pa_pacCodepoint[1]));

  if ((nFirstWord & 0xfc00) == 0xd800) {
    TForteUInt16 nSecondWord = static_cast<TForteUInt16>(pa_bLittleEndian ?
      (pa_pacCodepoint[2] | (pa_pacCodepoint[3] << 8)) :
      ((pa_pacCodepoint[2] << 8) | pa_pacCodepoint[3]));

    if((nSecondWord & 0xfc00) != 0xdc00) {
      return -1;
    }

    pa_rValue = 0x10000 + (((nFirstWord & 0x3ff) << 10) | (nSecondWord & 0x3ff));

    return 4;
  }

  pa_rValue = nFirstWord;

  return 2;
}


int CUnicodeUtilities::encodeUTF16Codepoint(TForteByte *pa_pacCodepoint, unsigned int pa_nBufferSize, TForteUInt32 pa_nValue, bool pa_bLittleEndian) {
  if((pa_nValue >= 0xd800 && pa_nValue < 0xe000) || pa_nValue >= 0x110000) {
    return -1;
  }

  if (pa_nValue < 0x10000) {
    if (pa_pacCodepoint) {
      if(pa_nBufferSize < 2) {
        return -1;
      }
      if (pa_bLittleEndian) {
        pa_pacCodepoint[0] = (TForteByte) ((pa_nValue     ) & 0xff);
        pa_pacCodepoint[1] = (TForteByte) ((pa_nValue >> 8) & 0xff);
      } else {
        pa_pacCodepoint[0] = (TForteByte) ((pa_nValue >> 8) & 0xff);
        pa_pacCodepoint[1] = (TForteByte) ((pa_nValue     ) & 0xff);
      }
    }
    return 2;
  } else {
    if (pa_pacCodepoint) {
      TForteUInt32 nVal = pa_nValue - 0x10000;
      TForteUInt16 nHighSurrogate = static_cast<TForteUInt16>(0xd800 | ((nVal >> 10) & 0x3ff));
      TForteUInt16 nLowSurrogate  = static_cast<TForteUInt16>(0xdc00 | ((nVal      ) & 0x3ff));

      if(pa_nBufferSize < 4) {
        return -1;
      }

      if (pa_bLittleEndian) {
        pa_pacCodepoint[0] = (TForteByte) ((nHighSurrogate     ) & 0xff);
        pa_pacCodepoint[1] = (TForteByte) ((nHighSurrogate >> 8) & 0xff);
        pa_pacCodepoint[2] = (TForteByte) ((nLowSurrogate      ) & 0xff);
        pa_pacCodepoint[3] = (TForteByte) ((nLowSurrogate  >> 8) & 0xff);
      } else {
        pa_pacCodepoint[0] = (TForteByte) ((nHighSurrogate >> 8) & 0xff);
        pa_pacCodepoint[1] = (TForteByte) ((nHighSurrogate     ) & 0xff);
        pa_pacCodepoint[2] = (TForteByte) ((nLowSurrogate  >> 8) & 0xff);
        pa_pacCodepoint[3] = (TForteByte) ((nLowSurrogate      ) & 0xff);
      }
    }
    return 4;
  }
}

int CUnicodeUtilities::checkUTF8(const char *pa_pacValue, int pa_nLength, unsigned int &pa_rnMaxWidth) {
  const TForteByte *pRunner = (const TForteByte *) pa_pacValue;
  size_t nRemLen = (pa_nLength == -1) ? strlen(pa_pacValue) : (size_t) pa_nLength;
  TForteUInt32 nCodepoint;
  int nRetVal = 0;
  size_t i = 0;
  pa_rnMaxWidth = 7;

  while (i < nRemLen) {
    int nRes = parseUTF8Codepoint(pRunner, nCodepoint);
    if (nRes < 0 || nRes + i > nRemLen) {
      return -1;
    }
    if(nCodepoint != CUnicodeUtilities::scm_unBOMMarker) {
      if(nCodepoint >= 0x10000) {
        pa_rnMaxWidth = 21;
      } else if(nCodepoint >= 0x100 && pa_rnMaxWidth < 16) {
        pa_rnMaxWidth = 16;
      } else if(nCodepoint >= 0x80 && pa_rnMaxWidth < 8) {
        pa_rnMaxWidth = 8;
      }
    }

    i += nRes;
    pRunner += nRes;
    ++nRetVal;
  }

  return nRetVal;
}
