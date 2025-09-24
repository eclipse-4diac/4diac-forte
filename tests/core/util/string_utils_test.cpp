/*******************************************************************************
 * Copyright (c) 2013, 2025 fortiss GmbH, Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *   Alois Zoitl - migrated data type toString to std::string
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "forte/util/string_utils.h"
#include <errno.h>
#include <stdlib.h>
#include <array>

namespace forte::util::test {
  BOOST_AUTO_TEST_SUITE(CIEC_ARRAY_function_test)

  BOOST_AUTO_TEST_CASE(isDigit_Test) {

    for (char i = 0; i < '0'; i++) {
      BOOST_CHECK(!forte::util::isDigit(i));
    }

    BOOST_CHECK(forte::util::isDigit('0'));
    BOOST_CHECK(forte::util::isDigit('1'));
    BOOST_CHECK(forte::util::isDigit('2'));
    BOOST_CHECK(forte::util::isDigit('3'));
    BOOST_CHECK(forte::util::isDigit('4'));
    BOOST_CHECK(forte::util::isDigit('5'));
    BOOST_CHECK(forte::util::isDigit('6'));
    BOOST_CHECK(forte::util::isDigit('7'));
    BOOST_CHECK(forte::util::isDigit('8'));
    BOOST_CHECK(forte::util::isDigit('9'));

    for (int i = '9' + 1; i <= 255; i++) {
      BOOST_CHECK(!forte::util::isDigit((char) i));
    }
  }

  BOOST_AUTO_TEST_CASE(isAtoFChar) {
    BOOST_CHECK(forte::util::isAtoFChar('a'));
    BOOST_CHECK(forte::util::isAtoFChar('b'));
    BOOST_CHECK(forte::util::isAtoFChar('c'));
    BOOST_CHECK(forte::util::isAtoFChar('d'));
    BOOST_CHECK(forte::util::isAtoFChar('e'));
    BOOST_CHECK(forte::util::isAtoFChar('f'));

    BOOST_CHECK(forte::util::isAtoFChar('A'));
    BOOST_CHECK(forte::util::isAtoFChar('B'));
    BOOST_CHECK(forte::util::isAtoFChar('C'));
    BOOST_CHECK(forte::util::isAtoFChar('D'));
    BOOST_CHECK(forte::util::isAtoFChar('E'));
    BOOST_CHECK(forte::util::isAtoFChar('F'));

    BOOST_CHECK(!forte::util::isAtoFChar('g'));
    BOOST_CHECK(!forte::util::isAtoFChar('a' - 1));

    BOOST_CHECK(!forte::util::isAtoFChar('G'));
    BOOST_CHECK(!forte::util::isAtoFChar('A' - 1));

    BOOST_CHECK(!forte::util::isAtoFChar(0));
    BOOST_CHECK(!forte::util::isAtoFChar('\0'));
    BOOST_CHECK(!forte::util::isAtoFChar('\n'));
    BOOST_CHECK(!forte::util::isAtoFChar('\r'));
    BOOST_CHECK(!forte::util::isAtoFChar('\t'));
    BOOST_CHECK(!forte::util::isAtoFChar('-'));
    BOOST_CHECK(!forte::util::isAtoFChar('x'));
    BOOST_CHECK(!forte::util::isAtoFChar('X'));
    BOOST_CHECK(!forte::util::isAtoFChar('/'));
    BOOST_CHECK(!forte::util::isAtoFChar('<'));
    BOOST_CHECK(!forte::util::isAtoFChar('>'));

    BOOST_CHECK(!forte::util::isAtoFChar((char) 255));
  }

  BOOST_AUTO_TEST_CASE(isHexDigit) {
    BOOST_CHECK(forte::util::isHexDigit('0'));
    BOOST_CHECK(forte::util::isHexDigit('1'));
    BOOST_CHECK(forte::util::isHexDigit('2'));
    BOOST_CHECK(forte::util::isHexDigit('3'));
    BOOST_CHECK(forte::util::isHexDigit('4'));
    BOOST_CHECK(forte::util::isHexDigit('5'));
    BOOST_CHECK(forte::util::isHexDigit('6'));
    BOOST_CHECK(forte::util::isHexDigit('7'));
    BOOST_CHECK(forte::util::isHexDigit('8'));
    BOOST_CHECK(forte::util::isHexDigit('9'));

    BOOST_CHECK(forte::util::isHexDigit('a'));
    BOOST_CHECK(forte::util::isHexDigit('b'));
    BOOST_CHECK(forte::util::isHexDigit('c'));
    BOOST_CHECK(forte::util::isHexDigit('d'));
    BOOST_CHECK(forte::util::isHexDigit('e'));
    BOOST_CHECK(forte::util::isHexDigit('f'));

    BOOST_CHECK(forte::util::isHexDigit('A'));
    BOOST_CHECK(forte::util::isHexDigit('B'));
    BOOST_CHECK(forte::util::isHexDigit('C'));
    BOOST_CHECK(forte::util::isHexDigit('D'));
    BOOST_CHECK(forte::util::isHexDigit('E'));
    BOOST_CHECK(forte::util::isHexDigit('F'));

    // some significant negative test cases

    BOOST_CHECK(!forte::util::isHexDigit('g'));
    BOOST_CHECK(!forte::util::isHexDigit('a' - 1));

    BOOST_CHECK(!forte::util::isHexDigit('G'));
    BOOST_CHECK(!forte::util::isHexDigit('A' - 1));

    BOOST_CHECK(!forte::util::isHexDigit('0' - 1));
    BOOST_CHECK(!forte::util::isHexDigit('9' + 1));

    BOOST_CHECK(!forte::util::isHexDigit(0));
    BOOST_CHECK(!forte::util::isHexDigit('\0'));
    BOOST_CHECK(!forte::util::isHexDigit('\n'));
    BOOST_CHECK(!forte::util::isHexDigit('\r'));
    BOOST_CHECK(!forte::util::isHexDigit('\t'));
    BOOST_CHECK(!forte::util::isHexDigit('-'));
    BOOST_CHECK(!forte::util::isHexDigit('x'));
    BOOST_CHECK(!forte::util::isHexDigit('X'));
    BOOST_CHECK(!forte::util::isHexDigit('/'));
    BOOST_CHECK(!forte::util::isHexDigit('<'));
    BOOST_CHECK(!forte::util::isHexDigit('>'));

    BOOST_CHECK(!forte::util::isHexDigit((char) 255));
  }

  BOOST_AUTO_TEST_CASE(charDigitToInt) {

    BOOST_CHECK_EQUAL(0, forte::util::charDigitToInt('0'));
    BOOST_CHECK_EQUAL(1, forte::util::charDigitToInt('1'));
    BOOST_CHECK_EQUAL(2, forte::util::charDigitToInt('2'));
    BOOST_CHECK_EQUAL(3, forte::util::charDigitToInt('3'));
    BOOST_CHECK_EQUAL(4, forte::util::charDigitToInt('4'));
    BOOST_CHECK_EQUAL(5, forte::util::charDigitToInt('5'));
    BOOST_CHECK_EQUAL(6, forte::util::charDigitToInt('6'));
    BOOST_CHECK_EQUAL(7, forte::util::charDigitToInt('7'));
    BOOST_CHECK_EQUAL(8, forte::util::charDigitToInt('8'));
    BOOST_CHECK_EQUAL(9, forte::util::charDigitToInt('9'));
  }

  BOOST_AUTO_TEST_CASE(charAtoFToInt) {

    BOOST_CHECK_EQUAL(10, forte::util::charAtoFToInt('a'));
    BOOST_CHECK_EQUAL(10, forte::util::charAtoFToInt('A'));

    BOOST_CHECK_EQUAL(11, forte::util::charAtoFToInt('b'));
    BOOST_CHECK_EQUAL(11, forte::util::charAtoFToInt('B'));

    BOOST_CHECK_EQUAL(12, forte::util::charAtoFToInt('c'));
    BOOST_CHECK_EQUAL(12, forte::util::charAtoFToInt('C'));

    BOOST_CHECK_EQUAL(13, forte::util::charAtoFToInt('d'));
    BOOST_CHECK_EQUAL(13, forte::util::charAtoFToInt('D'));

    BOOST_CHECK_EQUAL(14, forte::util::charAtoFToInt('e'));
    BOOST_CHECK_EQUAL(14, forte::util::charAtoFToInt('E'));

    BOOST_CHECK_EQUAL(15, forte::util::charAtoFToInt('f'));
    BOOST_CHECK_EQUAL(15, forte::util::charAtoFToInt('F'));
  }

  BOOST_AUTO_TEST_CASE(charHexDigitToInt) {

    BOOST_CHECK_EQUAL(0, forte::util::charHexDigitToInt('0'));
    BOOST_CHECK_EQUAL(1, forte::util::charHexDigitToInt('1'));
    BOOST_CHECK_EQUAL(2, forte::util::charHexDigitToInt('2'));
    BOOST_CHECK_EQUAL(3, forte::util::charHexDigitToInt('3'));
    BOOST_CHECK_EQUAL(4, forte::util::charHexDigitToInt('4'));
    BOOST_CHECK_EQUAL(5, forte::util::charHexDigitToInt('5'));
    BOOST_CHECK_EQUAL(6, forte::util::charHexDigitToInt('6'));
    BOOST_CHECK_EQUAL(7, forte::util::charHexDigitToInt('7'));
    BOOST_CHECK_EQUAL(8, forte::util::charHexDigitToInt('8'));
    BOOST_CHECK_EQUAL(9, forte::util::charHexDigitToInt('9'));

    BOOST_CHECK_EQUAL(10, forte::util::charHexDigitToInt('a'));
    BOOST_CHECK_EQUAL(10, forte::util::charHexDigitToInt('A'));

    BOOST_CHECK_EQUAL(11, forte::util::charHexDigitToInt('b'));
    BOOST_CHECK_EQUAL(11, forte::util::charHexDigitToInt('B'));

    BOOST_CHECK_EQUAL(12, forte::util::charHexDigitToInt('c'));
    BOOST_CHECK_EQUAL(12, forte::util::charHexDigitToInt('C'));

    BOOST_CHECK_EQUAL(13, forte::util::charHexDigitToInt('d'));
    BOOST_CHECK_EQUAL(13, forte::util::charHexDigitToInt('D'));

    BOOST_CHECK_EQUAL(14, forte::util::charHexDigitToInt('e'));
    BOOST_CHECK_EQUAL(14, forte::util::charHexDigitToInt('E'));

    BOOST_CHECK_EQUAL(15, forte::util::charHexDigitToInt('f'));
    BOOST_CHECK_EQUAL(15, forte::util::charHexDigitToInt('F'));
  }

  void strtolWrongInputCheck(const char *paString, long int paResult, unsigned int paErrorPos) {
    char *pacEndptr;
    BOOST_CHECK_EQUAL(paResult, forte::util::strtol(paString, &pacEndptr, 10));
    BOOST_CHECK_EQUAL(paString[paErrorPos], *pacEndptr);
  }

  //*assumes that overflow will happen when the last digit is parsed
  void strtolOverflowCheck(const char *paString, long int paResult) {
    strtolWrongInputCheck(paString, paResult, static_cast<unsigned int>(strlen(paString) - 1));
    BOOST_CHECK_EQUAL(ERANGE, errno);
  }

  BOOST_AUTO_TEST_CASE(strtol) {
    char *pacEndptr;
    const unsigned int unTestData = 12;
    std::string sTestData10[unTestData] = {"0",    "255",  "256",  "65535",  "65536",  "2147483647",
                                           "-255", "-256", "-257", "-65536", "-65537", "-2147483648"};
    std::string sTestData16[unTestData] = {"0x0",   "0xFF",   "0x100",  "0xFFFF",   "0x10000",  "0x7FFFFFFF",
                                           "-0xFF", "-0x100", "-0x101", "-0x10000", "-0x10001", "-0x80000000"};
    long int sTestDataResult[unTestData] = {0,    255,  256,  65535,  65536,  2147483647,
                                            -255, -256, -257, -65536, -65537, -2147483648};

    for (unsigned int i = 0; i < unTestData; i++) {
      BOOST_CHECK_EQUAL(sTestDataResult[i], forte::util::strtol(sTestData10[i].c_str(), &pacEndptr, 10));
      BOOST_CHECK(nullptr != pacEndptr);
      if (nullptr != pacEndptr) {
        BOOST_CHECK_EQUAL('\0', *pacEndptr);
      }
    }

    for (unsigned int i = 0; i < unTestData; i++) {
      BOOST_CHECK_EQUAL(sTestDataResult[i], forte::util::strtol(sTestData16[i].c_str(), &pacEndptr, 16));
      BOOST_CHECK(nullptr != pacEndptr);
      if (nullptr != pacEndptr) {
        BOOST_CHECK_EQUAL('\0', *pacEndptr);
      }
    }

    strtolOverflowCheck("2147483648", 2147483647);
    strtolOverflowCheck("2147483649", 2147483647);
    strtolOverflowCheck("2147483650", 2147483647);
    strtolOverflowCheck("2147483651", 2147483647);
    strtolOverflowCheck("2147483652", 2147483647);
    strtolOverflowCheck("2147483653", 2147483647);
    strtolOverflowCheck("2147483654", 2147483647);
    strtolOverflowCheck("2147483655", 2147483647);
    strtolOverflowCheck("2147483656", 2147483647);
    strtolOverflowCheck("2147483657", 2147483647);
    strtolOverflowCheck("2147483658", 2147483647);
    strtolOverflowCheck("2147483659", 2147483647);
    strtolOverflowCheck("2147483660", 2147483647);
    strtolOverflowCheck("2147483661", 2147483647);
    strtolOverflowCheck("2147483662", 2147483647);

    strtolOverflowCheck("-2147483649", -2147483648);
    strtolOverflowCheck("-2147483650", -2147483648);
    strtolOverflowCheck("-2147483651", -2147483648);
    strtolOverflowCheck("-2147483652", -2147483648);
    strtolOverflowCheck("-2147483653", -2147483648);

    strtolWrongInputCheck("hansi", 0, 0);
    strtolWrongInputCheck("3267x23", 3267, 4);
    strtolWrongInputCheck("-74219x52", -74219, 6);
  }

  void strtoulWrongInputCheck(const char *paString, unsigned long int paResult, unsigned int paErrorPos, int base) {
    char *pacEndptr;
    BOOST_CHECK_EQUAL(paResult, forte::util::strtoul(paString, &pacEndptr, base));
    BOOST_CHECK_EQUAL(paString[paErrorPos], *pacEndptr);
  }

  //*assumes that overflow will happen when the last digit is parsed
  void strtoulOverflowCheck(const char *paString, unsigned long int paResult, int base) {
    strtoulWrongInputCheck(paString, paResult, static_cast<unsigned int>(strlen(paString) - 1), base);
    BOOST_CHECK_EQUAL(ERANGE, errno);
  }

  BOOST_AUTO_TEST_CASE(strtoul) {
    char *pacEndptr;
    const unsigned int unTestData = 7;
    std::string sTestData10[unTestData] = {"0", "255", "256", "65535", "65536", "2147483647", "4294967295"};
    std::string sTestData16[unTestData] = {"0x0", "0xFF", "0x100", "0xFFFF", "0x10000", "0x7FFFFFFF", "0xFFFFFFFF"};
    unsigned long int sTestDataResult[unTestData] = {0, 255, 256, 65535, 65536, 2147483647, 4294967295};

    for (unsigned int i = 0; i < unTestData; i++) {
      BOOST_CHECK_EQUAL(sTestDataResult[i], forte::util::strtoul(sTestData10[i].c_str(), &pacEndptr, 10));
      BOOST_CHECK(nullptr != pacEndptr);
      if (nullptr != pacEndptr) {
        BOOST_CHECK_EQUAL('\0', *pacEndptr);
      }
    }

    for (unsigned int i = 0; i < unTestData; i++) {
      BOOST_CHECK_EQUAL(sTestDataResult[i], forte::util::strtoul(sTestData16[i].c_str(), &pacEndptr, 16));
      BOOST_CHECK(nullptr != pacEndptr);
      if (nullptr != pacEndptr) {
        BOOST_CHECK_EQUAL('\0', *pacEndptr);
      }
    }

    strtoulOverflowCheck("4294967296", 4294967295, 10);
    strtoulOverflowCheck("4294967297", 4294967295, 10);
    strtoulOverflowCheck("4294967298", 4294967295, 10);
    strtoulOverflowCheck("4294967299", 4294967295, 10);
    strtoulOverflowCheck("4294967300", 4294967295, 10);
    strtoulOverflowCheck("0xFFFFFFFF1", 4294967295, 16);
    strtoulOverflowCheck("0xFFFFFFF3a", 4294967295, 16);

    strtoulWrongInputCheck("hansi", 0, 0, 10);
    strtoulWrongInputCheck("3267x23", 3267, 4, 10);
    strtoulWrongInputCheck("-74219x52", 0, 0, 10);

    strtoulWrongInputCheck("0x74x219x52", 116, 4, 16);
    strtoulWrongInputCheck("0x0x74x219x52", 0, 3, 16);
  }

  void strtollWrongInputCheck(const char *paString, long long int paResult, unsigned int paErrorPos) {
    char *pacEndptr;
    BOOST_CHECK_EQUAL(paResult, forte::util::strtoll(paString, &pacEndptr, 10));
    BOOST_CHECK_EQUAL(paString[paErrorPos], *pacEndptr);
  }

  //*assumes that overflow will happen when the last digit is parsed
  void strtollOverflowCheck(const char *paString, long long int paResult) {
    strtollWrongInputCheck(paString, paResult, static_cast<unsigned int>(strlen(paString) - 1));
    BOOST_CHECK_EQUAL(ERANGE, errno);
  }

  BOOST_AUTO_TEST_CASE(strtoll) {
    char *pacEndptr;
    const unsigned int unTestData = 19;
    std::string sTestData10[unTestData] = {"0",
                                           "255",
                                           "256",
                                           "65535",
                                           "65536",
                                           "2147483647",
                                           "2147483648",
                                           "4294967296",
                                           "4294967297",
                                           "9223372036854775807",
                                           "-255",
                                           "-256",
                                           "-257",
                                           "-65536",
                                           "-65537",
                                           "-2147483648",
                                           "-4294967296",
                                           "-4294967297",
                                           "-9223372036854775808"};
    std::string sTestData16[unTestData] = {"0x0",
                                           "0xFF",
                                           "0x100",
                                           "0xFFFF",
                                           "0x10000",
                                           "0x7FFFFFFF",
                                           "0x80000000",
                                           "0x100000000",
                                           "0x100000001",
                                           "0x7FFFFFFFFFFFFFFF",
                                           "-0xFF",
                                           "-0x100",
                                           "-0x101",
                                           "-0x10000",
                                           "-0x10001",
                                           "-0x80000000",
                                           "-0x100000000",
                                           "-0x100000001",
                                           "-0x8000000000000000"};
    long long int sTestDataResult[unTestData] = {0,
                                                 255,
                                                 256,
                                                 65535,
                                                 65536,
                                                 2147483647,
                                                 2147483648,
                                                 4294967296,
                                                 4294967297,
                                                 9223372036854775807LL,
                                                 -255,
                                                 -256,
                                                 -257,
                                                 -65536,
                                                 -65537,
                                                 -2147483648LL,
                                                 -4294967296LL,
                                                 -4294967297LL,
                                                 -9223372036854775807LL - 1LL};

    for (unsigned int i = 0; i < unTestData; i++) {
      BOOST_CHECK_EQUAL(sTestDataResult[i], forte::util::strtoll(sTestData10[i].c_str(), &pacEndptr, 10));
      BOOST_CHECK(nullptr != pacEndptr);
      if (nullptr != pacEndptr) {
        BOOST_CHECK_EQUAL('\0', *pacEndptr);
      }
    }

    for (unsigned int i = 0; i < unTestData; i++) {
      BOOST_CHECK_EQUAL(sTestDataResult[i], forte::util::strtoll(sTestData16[i].c_str(), &pacEndptr, 16));
      BOOST_CHECK(nullptr != pacEndptr);
      if (nullptr != pacEndptr) {
        BOOST_CHECK_EQUAL('\0', *pacEndptr);
      }
    }

    strtollOverflowCheck("9223372036854775808", 9223372036854775807LL);
    strtollOverflowCheck("9223372036854775809", 9223372036854775807LL);
    strtollOverflowCheck("9223372036854775810", 9223372036854775807LL);
    strtollOverflowCheck("9223372036854775811", 9223372036854775807LL);

    strtollOverflowCheck("-9223372036854775809", -9223372036854775807LL - 1LL);
    strtollOverflowCheck("-9223372036854775810", -9223372036854775807LL - 1LL);
    strtollOverflowCheck("-9223372036854775811", -9223372036854775807LL - 1LL);
    strtollOverflowCheck("-9223372036854775812", -9223372036854775807LL - 1LL);

    strtollWrongInputCheck("hansi", 0, 0);
    strtollWrongInputCheck("3267x23", 3267, 4);
    strtollWrongInputCheck("-74219x52", -74219, 6);
  }

  void strtoullWrongInputCheck(const char *paString, unsigned long long int paResult, unsigned int paErrorPos) {
    char *pacEndptr;
    BOOST_CHECK_EQUAL(paResult, forte::util::strtoull(paString, &pacEndptr, 10));
    BOOST_CHECK_EQUAL(paString[paErrorPos], *pacEndptr);
  }

  //*assumes that overflow will happen when the last digit is parsed
  void strtoullOverflowCheck(const char *paString, unsigned long long int paResult) {
    strtoullWrongInputCheck(paString, paResult, static_cast<unsigned int>(strlen(paString) - 1));
    BOOST_CHECK_EQUAL(ERANGE, errno);
  }

  BOOST_AUTO_TEST_CASE(strtoull) {
    char *pacEndptr;
    const unsigned int unTestData = 11;
    std::string sTestData10[unTestData] = {"0",
                                           "255",
                                           "256",
                                           "65535",
                                           "65536",
                                           "2147483647",
                                           "2147483648",
                                           "4294967296",
                                           "4294967297",
                                           "9223372036854775807",
                                           "18446744073709551615"};
    std::string sTestData16[unTestData] = {"0x0",
                                           "0xFF",
                                           "0x100",
                                           "0xFFFF",
                                           "0x10000",
                                           "0x7FFFFFFF",
                                           "0x80000000",
                                           "0x100000000",
                                           "0x100000001",
                                           "0x7FFFFFFFFFFFFFFF",
                                           "0xFFFFFFFFFFFFFFFF"};
    unsigned long long int sTestDataResult[unTestData] = {0,
                                                          255,
                                                          256,
                                                          65535,
                                                          65536,
                                                          2147483647,
                                                          2147483648,
                                                          4294967296,
                                                          4294967297,
                                                          9223372036854775807LL,
                                                          18446744073709551615ULL};

    for (unsigned int i = 0; i < unTestData; i++) {
      BOOST_CHECK_EQUAL(sTestDataResult[i], forte::util::strtoull(sTestData10[i].c_str(), &pacEndptr, 10));
      BOOST_CHECK(nullptr != pacEndptr);
      if (nullptr != pacEndptr) {
        BOOST_CHECK_EQUAL('\0', *pacEndptr);
      }
    }

    for (unsigned int i = 0; i < unTestData; i++) {
      BOOST_CHECK_EQUAL(sTestDataResult[i], forte::util::strtoull(sTestData16[i].c_str(), &pacEndptr, 16));
      BOOST_CHECK(nullptr != pacEndptr);
      if (nullptr != pacEndptr) {
        BOOST_CHECK_EQUAL('\0', *pacEndptr);
      }
    }

    strtoullOverflowCheck("18446744073709551616", 18446744073709551615ULL);
    strtoullOverflowCheck("18446744073709551617", 18446744073709551615ULL);
    strtoullOverflowCheck("18446744073709551618", 18446744073709551615ULL);
    strtoullOverflowCheck("18446744073709551619", 18446744073709551615ULL);
    strtoullOverflowCheck("18446744073709551620", 18446744073709551615ULL);

    strtoullWrongInputCheck("hansi", 0, 0);
    strtoullWrongInputCheck("3267x23", 3267, 4);
    strtoullWrongInputCheck("-74219x52", 0, 0);
  }

  const auto sNonEscapedData = std::array{
      "",      "A",    "AB", // no escaped chars
      "\"",    "'",    "&",     "<",      ">", // single examples
      "\"\"",  "''",   "&&",    "<<",     ">>", // double examples
      "A\"",   "A'",   "A&",    "A<",     "A>", // one char before
      "\"A",   "'A",   "&A",    "<A",     ">A", // one char after
      "A\"B",  "A'B",  "A&B",   "A<B",    "A>B", // one char after and before
      "\"A\"", "'A'",  "&A&",   "<A<",    ">A>", // one char in the middle
      "\"'",   "\"'&", "\"'&<", "\"'&<>", "\"A'B&C<D>EFGH" // mixed escaped chars
  };

  const auto sExtraSize = std::array{0,  0,  0, // no escaped chars
                                     5,  5,  4,  3,  3, // single examples
                                     10, 10, 8,  6,  6, // double examples
                                     5,  5,  4,  3,  3, // one char before
                                     5,  5,  4,  3,  3, // one char after
                                     5,  5,  4,  3,  3, // one char after and before
                                     10, 10, 8,  6,  6, // one char in the middle
                                     10, 14, 17, 20, 20}; // mixed escaped chars

  const auto sEscapedData = std::array<std::string, sNonEscapedData.size()>{
      "",
      "A",
      "AB", // no escaped chars
      "&quot;",
      "&apos;",
      "&amp;",
      "&lt;",
      "&gt;", // single examples
      "&quot;&quot;",
      "&apos;&apos;",
      "&amp;&amp;",
      "&lt;&lt;",
      "&gt;&gt;", // double examples
      "A&quot;",
      "A&apos;",
      "A&amp;",
      "A&lt;",
      "A&gt;", // one char before
      "&quot;A",
      "&apos;A",
      "&amp;A",
      "&lt;A",
      "&gt;A", // one char after
      "A&quot;B",
      "A&apos;B",
      "A&amp;B",
      "A&lt;B",
      "A&gt;B", // one char after and before
      "&quot;A&quot;",
      "&apos;A&apos;",
      "&amp;A&amp;",
      "&lt;A&lt;",
      "&gt;A&gt;", // one char in the middle
      "&quot;&apos;",
      "&quot;&apos;&amp;",
      "&quot;&apos;&amp;&lt;",
      "&quot;&apos;&amp;&lt;&gt;",
      "&quot;A&apos;B&amp;C&lt;D&gt;EFGH" // mixed escaped chars
  };

  BOOST_AUTO_TEST_CASE(getExtraSizeForEscapedChars) {
    for (size_t i = 0; i < sizeof(sNonEscapedData) / sizeof(const char *); i++) {
      BOOST_CHECK_EQUAL(forte::util::getExtraSizeForXMLEscapedChars(sNonEscapedData[i]), sExtraSize[i]);
    }
  }

  BOOST_AUTO_TEST_CASE(transformNonEscapedToEscapedXMLText) {
    std::string toTest;
    for (size_t i = 0; i < sNonEscapedData.size(); i++) {
      toTest = sNonEscapedData[i];
      util::transformNonEscapedToEscapedXMLText(toTest, 0);
      BOOST_CHECK_EQUAL(toTest, sEscapedData[i]);
    }
  }

  BOOST_AUTO_TEST_CASE(transformEscapedXMLToNonEscapedText) {
    char toTest[50];
    for (size_t i = 0; i < sNonEscapedData.size(); i++) {
      memset(toTest, 0, 50);
      memcpy(toTest, sEscapedData[i].c_str(), sEscapedData[i].length());
      BOOST_CHECK_EQUAL(forte::util::transformEscapedXMLToNonEscapedText(toTest), sExtraSize[i]);
      BOOST_CHECK_EQUAL(0, strcmp(toTest, sNonEscapedData[i]));
    }
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::util::test
