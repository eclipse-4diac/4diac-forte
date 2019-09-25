/*******************************************************************************
 * Copyright (c) 2013 - 2014 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "../../../src/core/utils/string_utils.h"
#include <errno.h>
#include <stdlib.h>

BOOST_AUTO_TEST_SUITE(CIEC_ARRAY_function_test)

  BOOST_AUTO_TEST_CASE(isDigit_Test){

    for(char i = 0; i < '0'; i++){
      BOOST_CHECK(!forte::core::util::isDigit(i));
    }

    BOOST_CHECK(forte::core::util::isDigit('0'));
    BOOST_CHECK(forte::core::util::isDigit('1'));
    BOOST_CHECK(forte::core::util::isDigit('2'));
    BOOST_CHECK(forte::core::util::isDigit('3'));
    BOOST_CHECK(forte::core::util::isDigit('4'));
    BOOST_CHECK(forte::core::util::isDigit('5'));
    BOOST_CHECK(forte::core::util::isDigit('6'));
    BOOST_CHECK(forte::core::util::isDigit('7'));
    BOOST_CHECK(forte::core::util::isDigit('8'));
    BOOST_CHECK(forte::core::util::isDigit('9'));

    for(int i = '9' + 1; i <= 255; i++){
      BOOST_CHECK(!forte::core::util::isDigit((char )i));
    }
  }

  BOOST_AUTO_TEST_CASE(isAtoFChar){
    BOOST_CHECK(forte::core::util::isAtoFChar('a'));
    BOOST_CHECK(forte::core::util::isAtoFChar('b'));
    BOOST_CHECK(forte::core::util::isAtoFChar('c'));
    BOOST_CHECK(forte::core::util::isAtoFChar('d'));
    BOOST_CHECK(forte::core::util::isAtoFChar('e'));
    BOOST_CHECK(forte::core::util::isAtoFChar('f'));

    BOOST_CHECK(forte::core::util::isAtoFChar('A'));
    BOOST_CHECK(forte::core::util::isAtoFChar('B'));
    BOOST_CHECK(forte::core::util::isAtoFChar('C'));
    BOOST_CHECK(forte::core::util::isAtoFChar('D'));
    BOOST_CHECK(forte::core::util::isAtoFChar('E'));
    BOOST_CHECK(forte::core::util::isAtoFChar('F'));

    BOOST_CHECK(!forte::core::util::isAtoFChar('g'));
    BOOST_CHECK(!forte::core::util::isAtoFChar('a' - 1));

    BOOST_CHECK(!forte::core::util::isAtoFChar('G'));
    BOOST_CHECK(!forte::core::util::isAtoFChar('A' - 1));

    BOOST_CHECK(!forte::core::util::isAtoFChar(0));
    BOOST_CHECK(!forte::core::util::isAtoFChar('\0'));
    BOOST_CHECK(!forte::core::util::isAtoFChar('\n'));
    BOOST_CHECK(!forte::core::util::isAtoFChar('\r'));
    BOOST_CHECK(!forte::core::util::isAtoFChar('\t'));
    BOOST_CHECK(!forte::core::util::isAtoFChar('-'));
    BOOST_CHECK(!forte::core::util::isAtoFChar('x'));
    BOOST_CHECK(!forte::core::util::isAtoFChar('X'));
    BOOST_CHECK(!forte::core::util::isAtoFChar('/'));
    BOOST_CHECK(!forte::core::util::isAtoFChar('<'));
    BOOST_CHECK(!forte::core::util::isAtoFChar('>'));

    BOOST_CHECK(!forte::core::util::isAtoFChar((char )255));
  }

  BOOST_AUTO_TEST_CASE(isHexDigit){
    BOOST_CHECK(forte::core::util::isHexDigit('0'));
    BOOST_CHECK(forte::core::util::isHexDigit('1'));
    BOOST_CHECK(forte::core::util::isHexDigit('2'));
    BOOST_CHECK(forte::core::util::isHexDigit('3'));
    BOOST_CHECK(forte::core::util::isHexDigit('4'));
    BOOST_CHECK(forte::core::util::isHexDigit('5'));
    BOOST_CHECK(forte::core::util::isHexDigit('6'));
    BOOST_CHECK(forte::core::util::isHexDigit('7'));
    BOOST_CHECK(forte::core::util::isHexDigit('8'));
    BOOST_CHECK(forte::core::util::isHexDigit('9'));

    BOOST_CHECK(forte::core::util::isHexDigit('a'));
    BOOST_CHECK(forte::core::util::isHexDigit('b'));
    BOOST_CHECK(forte::core::util::isHexDigit('c'));
    BOOST_CHECK(forte::core::util::isHexDigit('d'));
    BOOST_CHECK(forte::core::util::isHexDigit('e'));
    BOOST_CHECK(forte::core::util::isHexDigit('f'));

    BOOST_CHECK(forte::core::util::isHexDigit('A'));
    BOOST_CHECK(forte::core::util::isHexDigit('B'));
    BOOST_CHECK(forte::core::util::isHexDigit('C'));
    BOOST_CHECK(forte::core::util::isHexDigit('D'));
    BOOST_CHECK(forte::core::util::isHexDigit('E'));
    BOOST_CHECK(forte::core::util::isHexDigit('F'));

    //some significant negative test cases

    BOOST_CHECK(!forte::core::util::isHexDigit('g'));
    BOOST_CHECK(!forte::core::util::isHexDigit('a' - 1));

    BOOST_CHECK(!forte::core::util::isHexDigit('G'));
    BOOST_CHECK(!forte::core::util::isHexDigit('A' - 1));

    BOOST_CHECK(!forte::core::util::isHexDigit('0' - 1));
    BOOST_CHECK(!forte::core::util::isHexDigit('9' + 1));

    BOOST_CHECK(!forte::core::util::isHexDigit(0));
    BOOST_CHECK(!forte::core::util::isHexDigit('\0'));
    BOOST_CHECK(!forte::core::util::isHexDigit('\n'));
    BOOST_CHECK(!forte::core::util::isHexDigit('\r'));
    BOOST_CHECK(!forte::core::util::isHexDigit('\t'));
    BOOST_CHECK(!forte::core::util::isHexDigit('-'));
    BOOST_CHECK(!forte::core::util::isHexDigit('x'));
    BOOST_CHECK(!forte::core::util::isHexDigit('X'));
    BOOST_CHECK(!forte::core::util::isHexDigit('/'));
    BOOST_CHECK(!forte::core::util::isHexDigit('<'));
    BOOST_CHECK(!forte::core::util::isHexDigit('>'));

    BOOST_CHECK(!forte::core::util::isHexDigit((char )255));
  }

  BOOST_AUTO_TEST_CASE(charDigitToInt){

    BOOST_CHECK_EQUAL(0, forte::core::util::charDigitToInt('0'));
    BOOST_CHECK_EQUAL(1, forte::core::util::charDigitToInt('1'));
    BOOST_CHECK_EQUAL(2, forte::core::util::charDigitToInt('2'));
    BOOST_CHECK_EQUAL(3, forte::core::util::charDigitToInt('3'));
    BOOST_CHECK_EQUAL(4, forte::core::util::charDigitToInt('4'));
    BOOST_CHECK_EQUAL(5, forte::core::util::charDigitToInt('5'));
    BOOST_CHECK_EQUAL(6, forte::core::util::charDigitToInt('6'));
    BOOST_CHECK_EQUAL(7, forte::core::util::charDigitToInt('7'));
    BOOST_CHECK_EQUAL(8, forte::core::util::charDigitToInt('8'));
    BOOST_CHECK_EQUAL(9, forte::core::util::charDigitToInt('9'));
  }

  BOOST_AUTO_TEST_CASE(charAtoFToInt){

    BOOST_CHECK_EQUAL(10, forte::core::util::charAtoFToInt('a'));
    BOOST_CHECK_EQUAL(10, forte::core::util::charAtoFToInt('A'));

    BOOST_CHECK_EQUAL(11, forte::core::util::charAtoFToInt('b'));
    BOOST_CHECK_EQUAL(11, forte::core::util::charAtoFToInt('B'));

    BOOST_CHECK_EQUAL(12, forte::core::util::charAtoFToInt('c'));
    BOOST_CHECK_EQUAL(12, forte::core::util::charAtoFToInt('C'));

    BOOST_CHECK_EQUAL(13, forte::core::util::charAtoFToInt('d'));
    BOOST_CHECK_EQUAL(13, forte::core::util::charAtoFToInt('D'));

    BOOST_CHECK_EQUAL(14, forte::core::util::charAtoFToInt('e'));
    BOOST_CHECK_EQUAL(14, forte::core::util::charAtoFToInt('E'));

    BOOST_CHECK_EQUAL(15, forte::core::util::charAtoFToInt('f'));
    BOOST_CHECK_EQUAL(15, forte::core::util::charAtoFToInt('F'));
  }

  BOOST_AUTO_TEST_CASE(charHexDigitToInt){

    BOOST_CHECK_EQUAL(0, forte::core::util::charHexDigitToInt('0'));
    BOOST_CHECK_EQUAL(1, forte::core::util::charHexDigitToInt('1'));
    BOOST_CHECK_EQUAL(2, forte::core::util::charHexDigitToInt('2'));
    BOOST_CHECK_EQUAL(3, forte::core::util::charHexDigitToInt('3'));
    BOOST_CHECK_EQUAL(4, forte::core::util::charHexDigitToInt('4'));
    BOOST_CHECK_EQUAL(5, forte::core::util::charHexDigitToInt('5'));
    BOOST_CHECK_EQUAL(6, forte::core::util::charHexDigitToInt('6'));
    BOOST_CHECK_EQUAL(7, forte::core::util::charHexDigitToInt('7'));
    BOOST_CHECK_EQUAL(8, forte::core::util::charHexDigitToInt('8'));
    BOOST_CHECK_EQUAL(9, forte::core::util::charHexDigitToInt('9'));

    BOOST_CHECK_EQUAL(10, forte::core::util::charHexDigitToInt('a'));
    BOOST_CHECK_EQUAL(10, forte::core::util::charHexDigitToInt('A'));

    BOOST_CHECK_EQUAL(11, forte::core::util::charHexDigitToInt('b'));
    BOOST_CHECK_EQUAL(11, forte::core::util::charHexDigitToInt('B'));

    BOOST_CHECK_EQUAL(12, forte::core::util::charHexDigitToInt('c'));
    BOOST_CHECK_EQUAL(12, forte::core::util::charHexDigitToInt('C'));

    BOOST_CHECK_EQUAL(13, forte::core::util::charHexDigitToInt('d'));
    BOOST_CHECK_EQUAL(13, forte::core::util::charHexDigitToInt('D'));

    BOOST_CHECK_EQUAL(14, forte::core::util::charHexDigitToInt('e'));
    BOOST_CHECK_EQUAL(14, forte::core::util::charHexDigitToInt('E'));

    BOOST_CHECK_EQUAL(15, forte::core::util::charHexDigitToInt('f'));
    BOOST_CHECK_EQUAL(15, forte::core::util::charHexDigitToInt('F'));
  }

  void strtolWrongInputCheck(const char* pa_acString, long int pa_nResult, unsigned int pa_unErrorPos){
    char *pacEndptr;
    BOOST_CHECK_EQUAL(pa_nResult, forte::core::util::strtol(pa_acString, &pacEndptr, 10));
    BOOST_CHECK_EQUAL(pa_acString[pa_unErrorPos], *pacEndptr);
  }

  //*assumes that overflow will happen when the last digit is parsed
  void strtolOverflowCheck(const char* pa_acString, long int pa_nResult){
    strtolWrongInputCheck(pa_acString, pa_nResult, static_cast<unsigned int>(strlen(pa_acString) - 1));
    BOOST_CHECK_EQUAL(ERANGE, errno);
  }

  BOOST_AUTO_TEST_CASE(strtol){
    char *pacEndptr;
    const unsigned int unTestData = 12;
    std::string sTestData10[unTestData] = { "0", "255", "256", "65535", "65536", "2147483647", "-255", "-256", "-257", "-65536", "-65537", "-2147483648" };
    std::string sTestData16[unTestData] = { "0x0", "0xFF", "0x100", "0xFFFF", "0x10000", "0x7FFFFFFF", "-0xFF", "-0x100", "-0x101", "-0x10000", "-0x10001", "-0x80000000" };
    long int sTestDataResult[unTestData] = { 0, 255, 256, 65535, 65536, 2147483647, -255, -256, -257, -65536, -65537, -2147483648 };

    for(unsigned int i = 0; i < unTestData; i++){
      BOOST_CHECK_EQUAL(sTestDataResult[i], forte::core::util::strtol(sTestData10[i].c_str(), &pacEndptr, 10));
      BOOST_CHECK(0 != pacEndptr);
      if(0 != pacEndptr){
        BOOST_CHECK_EQUAL('\0', *pacEndptr);
      }
    }

    for(unsigned int i = 0; i < unTestData; i++){
      BOOST_CHECK_EQUAL(sTestDataResult[i], forte::core::util::strtol(sTestData16[i].c_str(), &pacEndptr, 16));
      BOOST_CHECK(0 != pacEndptr);
      if(0 != pacEndptr){
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

  void strtoulWrongInputCheck(const char* pa_acString, unsigned long int pa_unResult, unsigned int pa_unErrorPos, int base){
    char *pacEndptr;
    BOOST_CHECK_EQUAL(pa_unResult, forte::core::util::strtoul(pa_acString, &pacEndptr, base));
    BOOST_CHECK_EQUAL(pa_acString[pa_unErrorPos], *pacEndptr);
  }

  //*assumes that overflow will happen when the last digit is parsed
  void strtoulOverflowCheck(const char* pa_acString, unsigned long int pa_unResult, int base){
    strtoulWrongInputCheck(pa_acString, pa_unResult, static_cast<unsigned int>(strlen(pa_acString) - 1), base);
    BOOST_CHECK_EQUAL(ERANGE, errno);
  }

  BOOST_AUTO_TEST_CASE(strtoul){
    char *pacEndptr;
    const unsigned int unTestData = 7;
    std::string sTestData10[unTestData] = { "0", "255", "256", "65535", "65536", "2147483647", "4294967295" };
    std::string sTestData16[unTestData] = { "0x0", "0xFF", "0x100", "0xFFFF", "0x10000", "0x7FFFFFFF", "0xFFFFFFFF" };
    unsigned long int sTestDataResult[unTestData] = { 0, 255, 256, 65535, 65536, 2147483647, 4294967295 };

    for(unsigned int i = 0; i < unTestData; i++){
      BOOST_CHECK_EQUAL(sTestDataResult[i], forte::core::util::strtoul(sTestData10[i].c_str(), &pacEndptr, 10));
      BOOST_CHECK(0 != pacEndptr);
      if(0 != pacEndptr){
        BOOST_CHECK_EQUAL('\0', *pacEndptr);
      }
    }

    for(unsigned int i = 0; i < unTestData; i++){
      BOOST_CHECK_EQUAL(sTestDataResult[i], forte::core::util::strtoul(sTestData16[i].c_str(), &pacEndptr, 16));
      BOOST_CHECK(0 != pacEndptr);
      if(0 != pacEndptr){
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

#ifdef FORTE_USE_64BIT_DATATYPES
  void strtollWrongInputCheck(const char* pa_acString, long long int pa_nResult, unsigned int pa_unErrorPos){
    char *pacEndptr;
    BOOST_CHECK_EQUAL(pa_nResult, forte::core::util::strtoll(pa_acString, &pacEndptr, 10));
    BOOST_CHECK_EQUAL(pa_acString[pa_unErrorPos], *pacEndptr);
  }

  //*assumes that overflow will happen when the last digit is parsed
  void strtollOverflowCheck(const char* pa_acString, long long int pa_nResult){
    strtollWrongInputCheck(pa_acString, pa_nResult, static_cast<unsigned int>(strlen(pa_acString) - 1));
    BOOST_CHECK_EQUAL(ERANGE, errno);
  }

  BOOST_AUTO_TEST_CASE(strtoll){
    char *pacEndptr;
    const unsigned int unTestData = 19;
    std::string sTestData10[unTestData] ={"0", "255", "256", "65535", "65536", "2147483647", "2147483648", "4294967296", "4294967297", "9223372036854775807",
      "-255", "-256", "-257", "-65536", "-65537", "-2147483648", "-4294967296", "-4294967297", "-9223372036854775808"};
    std::string sTestData16[unTestData] ={"0x0", "0xFF", "0x100", "0xFFFF", "0x10000", "0x7FFFFFFF", "0x80000000", "0x100000000", "0x100000001", "0x7FFFFFFFFFFFFFFF",
      "-0xFF", "-0x100", "-0x101", "-0x10000", "-0x10001", "-0x80000000", "-0x100000000", "-0x100000001", "-0x8000000000000000"
    };
    long long int sTestDataResult[unTestData] ={0, 255, 256, 65535, 65536, 2147483647, 2147483648, 4294967296, 4294967297, 9223372036854775807LL,
      -255, -256, -257, -65536, -65537, -2147483648LL, -4294967296LL, -4294967297LL, -9223372036854775807LL - 1LL};

    for(unsigned int i = 0; i < unTestData; i++){
      BOOST_CHECK_EQUAL(sTestDataResult[i], forte::core::util::strtoll(sTestData10[i].c_str(), &pacEndptr, 10));
      BOOST_CHECK(0 != pacEndptr);
      if(0 != pacEndptr){
        BOOST_CHECK_EQUAL('\0', *pacEndptr);
      }
    }

    for(unsigned int i = 0; i < unTestData; i++){
      BOOST_CHECK_EQUAL(sTestDataResult[i], forte::core::util::strtoll(sTestData16[i].c_str(), &pacEndptr, 16));
      BOOST_CHECK(0 != pacEndptr);
      if(0 != pacEndptr){
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

  void strtoullWrongInputCheck(const char* pa_acString, unsigned long long int pa_unResult, unsigned int pa_unErrorPos){
    char *pacEndptr;
    BOOST_CHECK_EQUAL(pa_unResult, forte::core::util::strtoull(pa_acString, &pacEndptr, 10));
    BOOST_CHECK_EQUAL(pa_acString[pa_unErrorPos], *pacEndptr);
  }

  //*assumes that overflow will happen when the last digit is parsed
  void strtoullOverflowCheck(const char* pa_acString, unsigned long long int pa_unResult){
    strtoullWrongInputCheck(pa_acString, pa_unResult, static_cast<unsigned int>(strlen(pa_acString) - 1));
    BOOST_CHECK_EQUAL(ERANGE, errno);
  }

  BOOST_AUTO_TEST_CASE(strtoull){
    char *pacEndptr;
    const unsigned int unTestData = 11;
    std::string sTestData10[unTestData] ={"0", "255", "256", "65535", "65536", "2147483647", "2147483648", "4294967296", "4294967297", "9223372036854775807",
      "18446744073709551615"};
    std::string sTestData16[unTestData] ={"0x0", "0xFF", "0x100", "0xFFFF", "0x10000", "0x7FFFFFFF", "0x80000000", "0x100000000", "0x100000001", "0x7FFFFFFFFFFFFFFF",
      "0xFFFFFFFFFFFFFFFF"};
    unsigned long long int sTestDataResult[unTestData] ={0, 255, 256, 65535, 65536, 2147483647, 2147483648, 4294967296, 4294967297, 9223372036854775807LL,
      18446744073709551615ULL};

    for(unsigned int i = 0; i < unTestData; i++){
      BOOST_CHECK_EQUAL(sTestDataResult[i], forte::core::util::strtoull(sTestData10[i].c_str(), &pacEndptr, 10));
      BOOST_CHECK(0 != pacEndptr);
      if(0 != pacEndptr){
        BOOST_CHECK_EQUAL('\0', *pacEndptr);
      }
    }

    for(unsigned int i = 0; i < unTestData; i++){
      BOOST_CHECK_EQUAL(sTestDataResult[i], forte::core::util::strtoull(sTestData16[i].c_str(), &pacEndptr, 16));
      BOOST_CHECK(0 != pacEndptr);
      if(0 != pacEndptr){
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
#endif

  const char* sNonEscapedData[] = {
      "", "A", "AB", //no escaped chars
      "\"", "'", "&",  "<", ">", //single examples
      "\"\"", "''", "&&",  "<<", ">>", //double examples
      "A\"", "A'", "A&",  "A<", "A>", //one char before
      "\"A", "'A", "&A",  "<A", ">A",  //one char after
      "A\"B", "A'B", "A&B",  "A<B", "A>B", //one char after and before
      "\"A\"", "'A'", "&A&",  "<A<", ">A>", //one char in the middle
      "\"'", "\"'&", "\"'&<",  "\"'&<>", "\"A'B&C<D>EFGH" //mixed escaped chars
  };

  unsigned int sExtraSize[] = {
      0, 0, 0, //no escaped chars
      5, 5, 4, 3, 3, //single examples
      10, 10, 8, 6, 6, //double examples
      5, 5, 4, 3, 3, //one char before
      5, 5, 4, 3, 3, //one char after
      5, 5, 4, 3, 3, //one char after and before
      10, 10, 8, 6, 6, //one char in the middle
      10, 14, 17, 20, 20  }; //mixed escaped chars

  const char* sEscapedData[] = {
      "", "A", "AB", //no escaped chars
      "&quot;", "&apos;", "&amp;",  "&lt;", "&gt;",  //single examples
      "&quot;&quot;", "&apos;&apos;", "&amp;&amp;",  "&lt;&lt;", "&gt;&gt;", //double examples
      "A&quot;", "A&apos;", "A&amp;",  "A&lt;", "A&gt;", //one char before
      "&quot;A", "&apos;A", "&amp;A",  "&lt;A", "&gt;A",  //one char after
      "A&quot;B", "A&apos;B", "A&amp;B",  "A&lt;B", "A&gt;B",  //one char after and before
      "&quot;A&quot;", "&apos;A&apos;", "&amp;A&amp;",  "&lt;A&lt;", "&gt;A&gt;", //one char in the middle
      "&quot;&apos;", "&quot;&apos;&amp;", "&quot;&apos;&amp;&lt;", "&quot;&apos;&amp;&lt;&gt;", "&quot;A&apos;B&amp;C&lt;D&gt;EFGH" //mixed escaped chars
   };

    BOOST_AUTO_TEST_CASE(getExtraSizeForEscapedChars){
      for(size_t i = 0; i < sizeof(sNonEscapedData) / sizeof(const char*); i++){
        BOOST_CHECK_EQUAL(forte::core::util::getExtraSizeForXMLEscapedChars(sNonEscapedData[i]), sExtraSize[i]);
      }
    }

    BOOST_AUTO_TEST_CASE(transformNonEscapedToEscapedXMLText){
      char toTest[50];
      for(size_t i = 0; i < sizeof(sNonEscapedData) / sizeof(const char*); i++){
        memset(toTest, 0, 50);
        memcpy(toTest, sNonEscapedData[i], strlen(sNonEscapedData[i]));
        BOOST_CHECK_EQUAL(forte::core::util::transformNonEscapedToEscapedXMLText(toTest), sExtraSize[i]);
        BOOST_CHECK_EQUAL(0, strcmp(toTest, sEscapedData[i]));
      }
    }

    BOOST_AUTO_TEST_CASE(transformEscapedXMLToNonEscapedText){
      char toTest[50];
      for(size_t i = 0; i < sizeof(sNonEscapedData) / sizeof(const char*); i++){
        memset(toTest, 0, 50);
        memcpy(toTest, sEscapedData[i], strlen(sEscapedData[i]));
        BOOST_CHECK_EQUAL(forte::core::util::transformEscapedXMLToNonEscapedText(toTest), sExtraSize[i]);
        BOOST_CHECK_EQUAL(0, strcmp(toTest, sNonEscapedData[i]));
      }
    }

  BOOST_AUTO_TEST_SUITE_END()
