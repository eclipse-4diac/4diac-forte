/*******************************************************************************
 * Copyright (c) 2011 ACIN and nxtControl
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Stanislav Meduna - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "../../../src/arch/datatype.h"


//Testdata
//BOOL
const unsigned int cg_unBoolSerSize = 1;
const TForteByte cg_abBoolFalse = 0x40;
const TForteByte cg_abBoolTrue = 0x41;

//BYTE
const unsigned int cg_unByteSerSize = 2;
const TForteByte cg_abByte0[2] = {0x51, 0};
const TForteByte cg_abByte12[2] = {0x51, 12};
const TForteByte cg_abByte128[2] = {0x51, 128};
const TForteByte cg_abByte255[2] = {0x51, 255};

//WORD
const unsigned int cg_unWordSerSize = 3;
const TForteByte cg_abWord0[3] = {0x52, 0, 0};
const TForteByte cg_abWord255[3] = {0x52, 0, 255};
const TForteByte cg_abWord256[3] = {0x52, 1, 0};
const TForteByte cg_abWord65535[3] = {0x52, 0xFF, 0xFF};
const TForteByte cg_abWord40396[3] = {0x52, 0x9D, 0xCC};

//DWORD
const unsigned int cg_unDWordSerSize = 5;
const TForteByte cg_abDWord0[5] = {0x53, 0, 0, 0, 0};
const TForteByte cg_abDWord255[5] = {0x53, 0, 0, 0, 0xFF};
const TForteByte cg_abDWord256[5] = {0x53, 0, 0, 1, 0};
const TForteByte cg_abDWord65535[5] = {0x53, 0, 0, 0xFF, 0xFF};
const TForteByte cg_abDWord65536[5] = {0x53, 0, 1, 0, 0};
const TForteByte cg_abDWord4294967295[5] = {0x53, 0xFF, 0xFF, 0xFF, 0xFF};
const TForteByte cg_abDWord690586453[5] = {0x53, 0x29, 0x29, 0x83, 0x55};

#ifdef FORTE_USE_64BIT_DATATYPES
//LWORD
const unsigned int cg_unLWordSerSize = 9;
const TForteByte cg_abLWord0[9] = {0x54, 0, 0, 0, 0, 0, 0, 0, 0};
const TForteByte cg_abLWord255[9] = {0x54, 0, 0, 0, 0, 0, 0, 0, 0xFF};
const TForteByte cg_abLWord256[9] = {0x54, 0, 0, 0, 0, 0, 0, 1, 0};
const TForteByte cg_abLWord65535[9] = {0x54, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF};
const TForteByte cg_abLWord65536[9] = {0x54, 0, 0, 0, 0, 0, 1, 0, 0};
const TForteByte cg_abLWord4294967295[9] = {0x54, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF};
const TForteByte cg_abLWord4294967296[9] = {0x54, 0, 0, 0, 1, 0, 0, 0, 0};
const TForteByte cg_abLWord18446744073709551615[9] = {0x54, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const TForteByte cg_abLWord3544923647067170873[9] =  {0x54, 0x31, 0x32, 0x19, 0x46, 0x73, 0x52, 0x28, 0x39};
#endif

//USINT
const unsigned int cg_unUSIntSerSize = 2;
const TForteByte cg_abUSInt0[2] = {0x46, 0};
const TForteByte cg_abUSInt12[2] = {0x46, 12};
const TForteByte cg_abUSInt128[2] = {0x46, 128};
const TForteByte cg_abUSInt255[2] = {0x46, 255};

//UINT
const unsigned int cg_unUIntSerSize = 3;
const TForteByte cg_abUInt0[3] = {0x47, 0, 0};
const TForteByte cg_abUInt255[3] = {0x47, 0, 255};
const TForteByte cg_abUInt256[3] = {0x47, 1, 0};
const TForteByte cg_abUInt65535[3] = {0x47, 0xFF, 0xFF};
const TForteByte cg_abUInt40396[3] = {0x47, 0x9D, 0xCC};

//UDINT
const unsigned int cg_unUDIntSerSize = 5;
const TForteByte cg_abUDInt0[5] = {0x48, 0, 0, 0, 0};
const TForteByte cg_abUDInt255[5] = {0x48, 0, 0, 0, 0xFF};
const TForteByte cg_abUDInt256[5] = {0x48, 0, 0, 1, 0};
const TForteByte cg_abUDInt65535[5] = {0x48, 0, 0, 0xFF, 0xFF};
const TForteByte cg_abUDInt65536[5] = {0x48, 0, 1, 0, 0};
const TForteByte cg_abUDInt4294967295[5] = {0x48, 0xFF, 0xFF, 0xFF, 0xFF};
const TForteByte cg_abUDInt690586453[5] = {0x48, 0x29, 0x29, 0x83, 0x55};

#ifdef FORTE_USE_64BIT_DATATYPES
//ULINT
const unsigned int cg_unULIntSerSize = 9;
const TForteByte cg_abULInt0[9] = {0x49, 0, 0, 0, 0, 0, 0, 0, 0};
const TForteByte cg_abULInt255[9] = {0x49, 0, 0, 0, 0, 0, 0, 0, 0xFF};
const TForteByte cg_abULInt256[9] = {0x49, 0, 0, 0, 0, 0, 0, 1, 0};
const TForteByte cg_abULInt65535[9] = {0x49, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF};
const TForteByte cg_abULInt65536[9] = {0x49, 0, 0, 0, 0, 0, 1, 0, 0};
const TForteByte cg_abULInt4294967295[9] = {0x49, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF};
const TForteByte cg_abULInt4294967296[9] = {0x49, 0, 0, 0, 1, 0, 0, 0, 0};
const TForteByte cg_abULInt18446744073709551615[9] = {0x49, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const TForteByte cg_abULInt3544923647067170873[9] =  {0x49, 0x31, 0x32, 0x19, 0x46, 0x73, 0x52, 0x28, 0x39};
#endif


//SINT
const unsigned int cg_unSIntSerSize = 2;
const TForteByte cg_abSInt0[2] = {0x42, 0};
const TForteByte cg_abSIntm128[2] = {0x42, 0x80};
const TForteByte cg_abSInt127[2] = {0x42, 127};
const TForteByte cg_abSIntm90[2] = {0x42, 0xA6};
const TForteByte cg_abSInt90[2] = {0x42, 0x5A };

//INT
const unsigned int cg_unIntSerSize = 3;
const TForteByte cg_abInt0[3] = {0x43, 0, 0};
const TForteByte cg_abIntm128[3] = {0x43, 0xff, 0x80};
const TForteByte cg_abIntm129[3] = {0x43, 0xff, 0x7F};
const TForteByte cg_abInt127[3] = {0x43, 0, 127};
const TForteByte cg_abInt128[3] = {0x43, 0, 128};
const TForteByte cg_abIntm32768[3] = {0x43, 0x80, 0x00};
const TForteByte cg_abInt32767[3] = {0x43, 0x7F, 0xFF};
const TForteByte cg_abInt10934[3] = {0x43, 0x2A, 0xB6};
const TForteByte cg_abIntm10934[3] = {0x43, 0xD5, 0x4A};

//DINT
const unsigned int cg_unDIntSerSize = 5;
const TForteByte cg_abDInt0[5] = {0x44, 0, 0, 0, 0};
const TForteByte cg_abDIntm128[5] = {0x44, 0xff, 0xff, 0xff, 0x80};
const TForteByte cg_abDIntm129[5] = {0x44, 0xff, 0xff, 0xff, 0x7F};
const TForteByte cg_abDInt127[5] = {0x44, 0, 0, 0, 127};
const TForteByte cg_abDInt128[5] = {0x44, 0, 0, 0, 128};
const TForteByte cg_abDIntm32768[5] = {0x44, 0xff, 0xff, 0x80, 0x00};
const TForteByte cg_abDIntm32769[5] = {0x44, 0xff, 0xff, 0x7F, 0xFF};
const TForteByte cg_abDInt32767[5] = {0x44, 0, 0, 0x7F, 0xFF};
const TForteByte cg_abDInt32768[5] = {0x44, 0, 0, 0x80, 0x00};
const TForteByte cg_abDIntm2147483648[5] = {0x44, 0x80, 0, 0x0, 0x0};
const TForteByte cg_abDInt2147483647[5] = {0x44, 0x7F, 0xFF, 0xFF, 0xFF};
const TForteByte cg_abDInt800058586[5] = {0x44, 0x2F, 0xAF, 0xEC, 0xDA};
const TForteByte cg_abDIntm800058586[5] = {0x44, 0xD0, 0x50, 0x13, 0x26};

#ifdef FORTE_USE_64BIT_DATATYPES
//LINT
const unsigned int cg_unLIntSerSize = 9;
const TForteByte cg_abLInt0[9] = {0x45, 0, 0, 0, 0, 0, 0, 0, 0};
const TForteByte cg_abLIntm128[9] = {0x45, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80};
const TForteByte cg_abLIntm129[9] = {0x45, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7F};
const TForteByte cg_abLInt127[9] = {0x45, 0, 0, 0, 0, 0, 0, 0, 127};
const TForteByte cg_abLInt128[9] = {0x45, 0, 0, 0, 0, 0, 0, 0, 128};
const TForteByte cg_abLIntm32768[9] = {0x45, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00};
const TForteByte cg_abLIntm32769[9] = {0x45, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7F, 0xFF};
const TForteByte cg_abLInt32767[9] = {0x45, 0, 0, 0, 0, 0, 0, 0x7F, 0xFF};
const TForteByte cg_abLInt32768[9] = {0x45, 0, 0, 0, 0, 0, 0, 0x80, 0x00};
const TForteByte cg_abLIntm2147483648[9] = {0x45, 0xff, 0xff, 0xff, 0xff, 0x80, 0, 0x0, 0x0};
const TForteByte cg_abLIntm2147483649[9] = {0x45, 0xff, 0xff, 0xff, 0xff, 0x7F, 0xFF, 0xFF, 0xFF};
const TForteByte cg_abLInt2147483647[9] = {0x45, 0, 0, 0, 0, 0x7F, 0xFF, 0xFF, 0xFF};
const TForteByte cg_abLInt2147483648[9] = {0x45, 0, 0, 0, 0, 0x80, 0, 0x0, 0x0};
const TForteByte cg_abLIntm9223372036854775808[9] = {0x45, 0x80, 0, 0x0, 0x0, 0, 0, 0, 0};
const TForteByte cg_abLInt9223372036854775807[9] = {0x45, 0x7F, 0xFF, 0xFF, 0xFF, 0xff, 0xff, 0xff, 0xff};
const TForteByte cg_abLInt800058586[9] = {0x45, 0, 0, 0, 0, 0x2F, 0xAF, 0xEC, 0xDA};
const TForteByte cg_abLIntm800058586[9] = {0x45, 0xff, 0xff, 0xff, 0xff, 0xD0, 0x50, 0x13, 0x26};
#endif

#ifdef FORTE_USE_REAL_DATATYPE
//REAL
const unsigned int cg_unRealSerSize = 5;
const TForteByte cg_abReal0[5] = {0x4A, 0, 0, 0, 0};
const TForteByte cg_abReal2_2874e6[5] = {0x4A, 0x4a, 0x0b, 0x9c, 0xa0};
const TForteByte cg_abRealm6_2587em4[5] = {0x4A, 0xba, 0x24, 0x11, 0x6d};
const TForteByte cg_abReal1_0Em37[5] = {0x4A, 0x02, 0x08, 0x1c, 0xea};
const TForteByte cg_abReal36_0[5] = {0x4A, 0x42, 0x10, 0x00, 0x00};

#ifdef FORTE_USE_64BIT_DATATYPES
//LREAL
const unsigned int cg_unLRealSerSize = 9;
const TForteByte cg_abLReal0[9] = {0x4B, 0, 0, 0, 0, 0, 0, 0, 0};
const TForteByte cg_abLReal2_28743e6[9] = {0x4B, 0x41, 0x41, 0x73, 0xa3, 0x00, 0x00, 0x00, 0x00};
const TForteByte cg_abLRealm6_2587em4[9] = {0x4B, 0xbf, 0x44, 0x82, 0x2d, 0x97, 0x3c, 0xf2, 0xfb};
const TForteByte cg_abLReal1_0Em37[9] = {0x4B, 0x38, 0x41, 0x03, 0x9d, 0x42, 0x8a, 0x8b, 0x8f};
const TForteByte cg_abLReal36_0[9] = {0x4B, 0x40, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif
#endif

//STRING
const unsigned int cg_unStringEmptySerSize = 3;
const TForteByte cg_abStringEmpty[3] = {0x50, 0, 0};
const unsigned int cg_unStringHalloWorldSerSize = 13;
const TForteByte cg_abStringHalloWorld[13] = {0x50, 0, 0xA, 'H', 'a', 'l', 'l', 'o', 'W', 'o', 'r', 'l', 'd'};

//WSTRING
const unsigned int cg_unWStringEmptySerSize = 3;
const TForteByte cg_abWStringEmpty[3] = {0x55, 0, 0 };
const unsigned int cg_unWStringHalloWorldSerSize = 23;
const TForteByte cg_abWStringHalloWorld[23] = {0x55, 0, 0xA, 0, 'H', 0, 'a', 0, 'l', 0, 'l', 0, 'o', 0, 'W', 0, 'o', 0, 'r', 0, 'l', 0, 'd'};
const unsigned int cg_unWStringNihongoSerSize = 9;
const TForteByte cg_abWStringNihongo[9] = { 0x55, 0, 3, 0x65, 0xe5, 0x67, 0x2c, 0x8a, 0x9e };
const TForteByte cg_abWStringNihongoUTF8[10] = { 0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC, 0xE8, 0xAA, 0x9E, 0 };

//TIME
const unsigned int cg_unTimeSerSize = 9;
const TForteByte cg_abTime0[9] = {0x4C, 0, 0, 0, 0, 0, 0, 0, 0};
const TForteByte cg_abTime3000ms[9] = {0x4C, 0, 0, 0, 0, 0, 0x2D, 0xC6, 0xC0};
const TForteByte cg_abTime3s22ms[9] = {0x4C, 0, 0, 0, 0, 0, 0x2e, 0x1c, 0xb0};


//ARRAY tests
const unsigned int cg_unBOOL5SerSize = 8;
const TForteByte cg_abArrayBool10011[] = {0x76, 0x0, 0x5, 0x41, 0x40, 0x40, 0x41, 0x41};
const TForteByte cg_abArrayBool01010[] = {0x76, 0x0, 0x5, 0x40, 0x41, 0x40, 0x41, 0x40};

const unsigned int cg_unSINT4SerSize = 8;
const TForteByte cg_abArraySINTm128_127_0_m90[] = {0x76, 0x0, 0x4, 0x42, 0x80, 127, 0, 0xA6};
const TForteByte cg_abArraySINTm90_90_127_0[] = {0x76, 0x0, 0x4, 0x42, 0xA6, 0x5A, 127, 0x0};

const unsigned int cg_unString2SerSize = 18;
const TForteByte cg_abArrayStringEmptyHalloWorld[] = {0x76, 0x00, 0x02, 0x50, 0, 0, 0, 0xA, 'H', 'a', 'l', 'l', 'o', 'W', 'o', 'r', 'l', 'd'};


