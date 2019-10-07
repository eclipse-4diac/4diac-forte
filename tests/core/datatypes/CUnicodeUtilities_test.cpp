/*******************************************************************************
 * Copyright (c) 2011, 2015 nxtControl, ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Stanislav Meduna, Ingo Hegny, Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>

#include "../../../src/core/datatypes/unicode_utils.h"

BOOST_AUTO_TEST_SUITE(CUnicodeUtilities_function_test)

BOOST_AUTO_TEST_CASE(CUnicodeUtilities_parseEncode8)
{
  int nRet;
  TForteUInt32 nValue;

  const TForteByte cBOM[] = { 0xEF, 0xBB, 0xBF };
  const TForteByte cASCII1[] = { 0 };
  const TForteByte cASCII2[] = { 'A' };
  const TForteByte cASCII3[] = { 0x7f };
  const TForteByte cUpper1[] = { 0xc2, 0xa2 };
  const TForteByte cUpper2[] = { 0xe2, 0x82, 0xac };
  const TForteByte cUpper3[] = { 0xf0, 0xa4, 0xad, 0xa2 };
  const TForteByte cInvalid1[] = { 0x80 };
  const TForteByte cInvalid2[] = { 0xfe, 0x80, 0x80, 0x80, 0x80, 0x80 };
  TForteByte cBuffer[32];

  nRet = CUnicodeUtilities::parseUTF8Codepoint(cBOM, nValue);
  BOOST_CHECK_EQUAL(nRet, 3);
  BOOST_CHECK_EQUAL(nValue, CUnicodeUtilities::scm_unBOMMarker);

  nRet = CUnicodeUtilities::parseUTF8Codepoint(cASCII1, nValue);
  BOOST_CHECK_EQUAL(nRet, 1);
  BOOST_CHECK_EQUAL(nValue, 0);
  nRet = CUnicodeUtilities::parseUTF8Codepoint(cASCII2, nValue);
  BOOST_CHECK_EQUAL(nRet, 1);
  BOOST_CHECK_EQUAL(nValue, 'A');
  nRet = CUnicodeUtilities::parseUTF8Codepoint(cASCII3, nValue);
  BOOST_CHECK_EQUAL(nRet, 1);
  BOOST_CHECK_EQUAL(nValue, 0x7f);

  nRet = CUnicodeUtilities::parseUTF8Codepoint(cUpper1, nValue);
  BOOST_CHECK_EQUAL(nRet, 2);
  BOOST_CHECK_EQUAL(nValue, 0xa2);
  nRet = CUnicodeUtilities::parseUTF8Codepoint(cUpper2, nValue);
  BOOST_CHECK_EQUAL(nRet, 3);
  BOOST_CHECK_EQUAL(nValue, 0x20ac);
  nRet = CUnicodeUtilities::parseUTF8Codepoint(cUpper3, nValue);
  BOOST_CHECK_EQUAL(nRet, 4);
  BOOST_CHECK_EQUAL(nValue, 0x24b62);

  nRet = CUnicodeUtilities::parseUTF8Codepoint(cInvalid1, nValue);
  BOOST_CHECK_EQUAL(nRet, -1);
  nRet = CUnicodeUtilities::parseUTF8Codepoint(cInvalid2, nValue);
  BOOST_CHECK_EQUAL(nRet, -1);

  nRet = CUnicodeUtilities::encodeUTF8Codepoint(cBuffer, 3, CUnicodeUtilities::scm_unBOMMarker);
  BOOST_CHECK_EQUAL(nRet, 3);
  BOOST_CHECK(! memcmp(cBOM, cBuffer, 3));

  nRet = CUnicodeUtilities::encodeUTF8Codepoint(cBuffer, 1, 0);
  BOOST_CHECK_EQUAL(nRet, 1);
  BOOST_CHECK(! memcmp(cASCII1, cBuffer, 1));
  nRet = CUnicodeUtilities::encodeUTF8Codepoint(cBuffer, 1, 'A');
  BOOST_CHECK_EQUAL(nRet, 1);
  BOOST_CHECK(! memcmp(cASCII2, cBuffer, 1));
  nRet = CUnicodeUtilities::encodeUTF8Codepoint(cBuffer, 1, 0x7f);
  BOOST_CHECK_EQUAL(nRet, 1);
  BOOST_CHECK(! memcmp(cASCII3, cBuffer, 1));

  nRet = CUnicodeUtilities::encodeUTF8Codepoint(cBuffer, 2, 0xa2);
  BOOST_CHECK_EQUAL(nRet, 2);
  BOOST_CHECK(! memcmp(cUpper1, cBuffer, 1));
  nRet = CUnicodeUtilities::encodeUTF8Codepoint(cBuffer, 3, 0x20ac);
  BOOST_CHECK_EQUAL(nRet, 3);
  BOOST_CHECK(! memcmp(cUpper2, cBuffer, 1));
  nRet = CUnicodeUtilities::encodeUTF8Codepoint(cBuffer, 4, 0x24b62);
  BOOST_CHECK_EQUAL(nRet, 4);
  BOOST_CHECK(! memcmp(cUpper3, cBuffer, 1));

  cBuffer[0] = '\0';
  nRet = CUnicodeUtilities::encodeUTF8Codepoint(0, 0, 0x7f);
  BOOST_CHECK_EQUAL(nRet, 1);
  BOOST_CHECK_EQUAL(cBuffer[0], 0);
  nRet = CUnicodeUtilities::encodeUTF8Codepoint(0, 0, 0xa2);
  BOOST_CHECK_EQUAL(nRet, 2);
  BOOST_CHECK_EQUAL(cBuffer[0], 0);
  nRet = CUnicodeUtilities::encodeUTF8Codepoint(0, 0, 0x20ac);
  BOOST_CHECK_EQUAL(nRet, 3);
  BOOST_CHECK_EQUAL(cBuffer[0], 0);
  nRet = CUnicodeUtilities::encodeUTF8Codepoint(0, 0, 0x24b62);
  BOOST_CHECK_EQUAL(nRet, 4);
  BOOST_CHECK_EQUAL(cBuffer[0], 0);


  nRet = CUnicodeUtilities::encodeUTF8Codepoint(cBuffer, 0, 0x7f);
  BOOST_CHECK_EQUAL(nRet, -1);
  nRet = CUnicodeUtilities::encodeUTF8Codepoint(cBuffer, 1, 0xa2);
  BOOST_CHECK_EQUAL(nRet, -1);
  nRet = CUnicodeUtilities::encodeUTF8Codepoint(cBuffer, 2, 0x20ac);
  BOOST_CHECK_EQUAL(nRet, -1);
  nRet = CUnicodeUtilities::encodeUTF8Codepoint(cBuffer, 3, 0x24b62);
  BOOST_CHECK_EQUAL(nRet, -1);
}

BOOST_AUTO_TEST_CASE(CUnicodeUtilities_parseEncode16)
{
  int nRet;
  TForteUInt32 nValue;

  const TForteByte cBOMBE[] = { 0xFE, 0xFF };
  const TForteByte cBOMLE[] = { 0xFF, 0xFE };
  const TForteByte cTest1BE[] = { 0x00, 0x7a };
  const TForteByte cTest1LE[] = { 0x7a, 0x00 };
  const TForteByte cTest2BE[] = { 0x6c, 0x34 };
  const TForteByte cTest2LE[] = { 0x34, 0x6c };
  const TForteByte cTest3BE[] = { 0xd8, 0x00, 0xdc, 0x00 };
  const TForteByte cTest3LE[] = { 0x00, 0xd8, 0x00, 0xdc };
  const TForteByte cTest4BE[] = { 0xd8, 0x34, 0xdd, 0x1e };
  const TForteByte cTest4LE[] = { 0x34, 0xd8, 0x1e, 0xdd };
  const TForteByte cTest5BE[] = { 0xdb, 0xff, 0xdf, 0xfd };
  const TForteByte cTest5LE[] = { 0xff, 0xdb, 0xfd, 0xdf };
  TForteByte cBuffer[32];

  nRet = CUnicodeUtilities::parseUTF16Codepoint(cBOMBE, nValue, false);
  BOOST_CHECK_EQUAL(nRet, 2);
  BOOST_CHECK_EQUAL(nValue, CUnicodeUtilities::scm_unBOMMarker);
  nRet = CUnicodeUtilities::parseUTF16Codepoint(cBOMLE, nValue, false);
  BOOST_CHECK_EQUAL(nRet, 2);
  BOOST_CHECK_EQUAL(nValue, CUnicodeUtilities::scm_unBOMMarkerSwapped);
  nRet = CUnicodeUtilities::parseUTF16Codepoint(cBOMBE, nValue, true);
  BOOST_CHECK_EQUAL(nRet, 2);
  BOOST_CHECK_EQUAL(nValue, CUnicodeUtilities::scm_unBOMMarkerSwapped);
  nRet = CUnicodeUtilities::parseUTF16Codepoint(cBOMLE, nValue, true);
  BOOST_CHECK_EQUAL(nRet, 2);
  BOOST_CHECK_EQUAL(nValue, CUnicodeUtilities::scm_unBOMMarker);

  nRet = CUnicodeUtilities::parseUTF16Codepoint(cTest1BE, nValue, false);
  BOOST_CHECK_EQUAL(nRet, 2);
  BOOST_CHECK_EQUAL(nValue, 0x7a);
  nRet = CUnicodeUtilities::parseUTF16Codepoint(cTest1LE, nValue, true);
  BOOST_CHECK_EQUAL(nRet, 2);
  BOOST_CHECK_EQUAL(nValue, 0x7a);

  nRet = CUnicodeUtilities::parseUTF16Codepoint(cTest2BE, nValue, false);
  BOOST_CHECK_EQUAL(nRet, 2);
  BOOST_CHECK_EQUAL(nValue, 0x6c34);
  nRet = CUnicodeUtilities::parseUTF16Codepoint(cTest2LE, nValue, true);
  BOOST_CHECK_EQUAL(nRet, 2);
  BOOST_CHECK_EQUAL(nValue, 0x6c34);

  nRet = CUnicodeUtilities::parseUTF16Codepoint(cTest3BE, nValue, false);
  BOOST_CHECK_EQUAL(nRet, 4);
  BOOST_CHECK_EQUAL(nValue, 0x10000);
  nRet = CUnicodeUtilities::parseUTF16Codepoint(cTest3LE, nValue, true);
  BOOST_CHECK_EQUAL(nRet, 4);
  BOOST_CHECK_EQUAL(nValue, 0x10000);

  nRet = CUnicodeUtilities::parseUTF16Codepoint(cTest4BE, nValue, false);
  BOOST_CHECK_EQUAL(nRet, 4);
  BOOST_CHECK_EQUAL(nValue, 0x1d11e);
  nRet = CUnicodeUtilities::parseUTF16Codepoint(cTest4LE, nValue, true);
  BOOST_CHECK_EQUAL(nRet, 4);
  BOOST_CHECK_EQUAL(nValue, 0x1d11e);

  nRet = CUnicodeUtilities::parseUTF16Codepoint(cTest5BE, nValue, false);
  BOOST_CHECK_EQUAL(nRet, 4);
  BOOST_CHECK_EQUAL(nValue, 0x10fffd);
  nRet = CUnicodeUtilities::parseUTF16Codepoint(cTest5LE, nValue, true);
  BOOST_CHECK_EQUAL(nRet, 4);
  BOOST_CHECK_EQUAL(nValue, 0x10fffd);

  nRet = CUnicodeUtilities::encodeUTF16Codepoint(cBuffer, 2, 0x7a, false);
  BOOST_CHECK_EQUAL(nRet, 2);
  BOOST_CHECK(! memcmp(cTest1BE, cBuffer, 1));
  nRet = CUnicodeUtilities::encodeUTF16Codepoint(cBuffer, 2, 0x7a, true);
  BOOST_CHECK_EQUAL(nRet, 2);
  BOOST_CHECK(! memcmp(cTest1LE, cBuffer, 1));

  nRet = CUnicodeUtilities::encodeUTF16Codepoint(cBuffer, 2, 0x6c34, false);
  BOOST_CHECK_EQUAL(nRet, 2);
  BOOST_CHECK(! memcmp(cTest2BE, cBuffer, 1));
  nRet = CUnicodeUtilities::encodeUTF16Codepoint(cBuffer, 2, 0x6c34, true);
  BOOST_CHECK_EQUAL(nRet, 2);
  BOOST_CHECK(! memcmp(cTest2LE, cBuffer, 1));

  nRet = CUnicodeUtilities::encodeUTF16Codepoint(cBuffer, 4, 0x10000, false);
  BOOST_CHECK_EQUAL(nRet, 4);
  BOOST_CHECK(! memcmp(cTest3BE, cBuffer, 1));
  nRet = CUnicodeUtilities::encodeUTF16Codepoint(cBuffer, 4, 0x10000, true);
  BOOST_CHECK_EQUAL(nRet, 4);
  BOOST_CHECK(! memcmp(cTest3LE, cBuffer, 1));

  nRet = CUnicodeUtilities::encodeUTF16Codepoint(cBuffer, 4, 0x1d11e, false);
  BOOST_CHECK_EQUAL(nRet, 4);
  BOOST_CHECK(! memcmp(cTest4BE, cBuffer, 1));
  nRet = CUnicodeUtilities::encodeUTF16Codepoint(cBuffer, 4, 0x1d11e, true);
  BOOST_CHECK_EQUAL(nRet, 4);
  BOOST_CHECK(! memcmp(cTest4LE, cBuffer, 1));

  nRet = CUnicodeUtilities::encodeUTF16Codepoint(cBuffer, 4, 0x10fffd, false);
  BOOST_CHECK_EQUAL(nRet, 4);
  BOOST_CHECK(! memcmp(cTest5BE, cBuffer, 1));
  nRet = CUnicodeUtilities::encodeUTF16Codepoint(cBuffer, 4, 0x10fffd, true);
  BOOST_CHECK_EQUAL(nRet, 4);
  BOOST_CHECK(! memcmp(cTest5LE, cBuffer, 1));

  nRet = CUnicodeUtilities::encodeUTF16Codepoint(0, 0, 0x6c34, false);
  BOOST_CHECK_EQUAL(nRet, 2);
  nRet = CUnicodeUtilities::encodeUTF16Codepoint(0, 0, 0x10fffd, false);
  BOOST_CHECK_EQUAL(nRet, 4);

  cBuffer[0] = '\0';
  nRet = CUnicodeUtilities::encodeUTF16Codepoint(cBuffer, 1, 0x6c34, false);
  BOOST_CHECK_EQUAL(nRet, -1);
  BOOST_CHECK_EQUAL(cBuffer[0], 0);
  nRet = CUnicodeUtilities::encodeUTF16Codepoint(cBuffer, 3, 0x10fffd, false);
  BOOST_CHECK_EQUAL(nRet, -1);
  BOOST_CHECK_EQUAL(cBuffer[0], 0);
}

BOOST_AUTO_TEST_CASE(CUnicodeUtilities_checkUTF8)
{
  int nRet;

  const TForteByte cASCII1[] = { 0 };
  const TForteByte cASCII2[] = { 'A', 0 };
  const TForteByte cASCII3[] = { 0x7f, 0 };
  const TForteByte cUpper1[] = { 0xc2, 0xa2, 0 };
  const TForteByte cUpper2[] = { 0xe2, 0x82, 0xac, 0 };
  const TForteByte cUpper3[] = { 0xf0, 0xa4, 0xad, 0xa2, 0 };
  const TForteByte cSeq[] = { 0x7f, 0xc2, 0xa2, 0xe2, 0x82, 0xac, 0xf0, 0xa4, 0xad, 0xa2, 0x7f, 0 };
  unsigned int nWidth;

  nWidth = 0;
  nRet = CUnicodeUtilities::checkUTF8((const char *) cASCII1, -1, nWidth);
  BOOST_CHECK_EQUAL(nRet, 0);
  BOOST_CHECK_EQUAL(nWidth, 7);

  nWidth = 0;
  nRet = CUnicodeUtilities::checkUTF8((const char *) cASCII2, -1, nWidth);
  BOOST_CHECK_EQUAL(nRet, 1);
  BOOST_CHECK_EQUAL(nWidth, 7);

  nWidth = 0;
  nRet = CUnicodeUtilities::checkUTF8((const char *) cASCII3, -1, nWidth);
  BOOST_CHECK_EQUAL(nRet, 1);
  BOOST_CHECK_EQUAL(nWidth, 7);

  nWidth = 0;
  nRet = CUnicodeUtilities::checkUTF8((const char *) cUpper1, -1, nWidth);
  BOOST_CHECK_EQUAL(nRet, 1);
  BOOST_CHECK_EQUAL(nWidth, 8);

  nWidth = 0;
  nRet = CUnicodeUtilities::checkUTF8((const char *) cUpper2, -1, nWidth);
  BOOST_CHECK_EQUAL(nRet, 1);
  BOOST_CHECK_EQUAL(nWidth, 16);

  nWidth = 0;
  nRet = CUnicodeUtilities::checkUTF8((const char *) cUpper3, -1, nWidth);
  BOOST_CHECK_EQUAL(nRet, 1);
  BOOST_CHECK_EQUAL(nWidth, 21);

  nWidth = 0;
  nRet = CUnicodeUtilities::checkUTF8((const char *) cSeq, 1, nWidth);
  BOOST_CHECK_EQUAL(nRet, 1);
  BOOST_CHECK_EQUAL(nWidth, 7);
  nWidth = 0;
  nRet = CUnicodeUtilities::checkUTF8((const char *) cSeq, 2, nWidth);
  BOOST_CHECK_EQUAL(nRet, -1);
  nWidth = 0;
  nRet = CUnicodeUtilities::checkUTF8((const char *) cSeq, 3, nWidth);
  BOOST_CHECK_EQUAL(nRet, 2);
  BOOST_CHECK_EQUAL(nWidth, 8);
  nWidth = 0;
  nRet = CUnicodeUtilities::checkUTF8((const char *) cSeq, 4, nWidth);
  BOOST_CHECK_EQUAL(nRet, -1);
  nWidth = 0;
  nRet = CUnicodeUtilities::checkUTF8((const char *) cSeq, 5, nWidth);
  BOOST_CHECK_EQUAL(nRet, -1);
  nWidth = 0;
  nRet = CUnicodeUtilities::checkUTF8((const char *) cSeq, 6, nWidth);
  BOOST_CHECK_EQUAL(nRet, 3);
  BOOST_CHECK_EQUAL(nWidth, 16);
  nWidth = 0;
  nRet = CUnicodeUtilities::checkUTF8((const char *) cSeq, 7, nWidth);
  BOOST_CHECK_EQUAL(nRet, -1);
  nWidth = 0;
  nRet = CUnicodeUtilities::checkUTF8((const char *) cSeq, 8, nWidth);
  BOOST_CHECK_EQUAL(nRet, -1);
  nWidth = 0;
  nRet = CUnicodeUtilities::checkUTF8((const char *) cSeq, 9, nWidth);
  BOOST_CHECK_EQUAL(nRet, -1);
  nWidth = 0;
  nRet = CUnicodeUtilities::checkUTF8((const char *) cSeq, 10, nWidth);
  BOOST_CHECK_EQUAL(nRet, 4);
  BOOST_CHECK_EQUAL(nWidth, 21);
  nWidth = 0;
  nRet = CUnicodeUtilities::checkUTF8((const char *) cSeq, 11, nWidth);
  BOOST_CHECK_EQUAL(nRet, 5);
  BOOST_CHECK_EQUAL(nWidth, 21);
}


BOOST_AUTO_TEST_SUITE_END()
