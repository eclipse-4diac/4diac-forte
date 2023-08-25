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
const unsigned int cgBoolSerSize = 1;
const TForteByte cgBoolFalse = 0x40;
const TForteByte cgBoolTrue = 0x41;

//BYTE
const unsigned int cgByteSerSize = 2;
const TForteByte cgByte0[2] = {0x51, 0};
const TForteByte cgByte12[2] = {0x51, 12};
const TForteByte cgByte128[2] = {0x51, 128};
const TForteByte cgByte255[2] = {0x51, 255};

//WORD
const unsigned int cgWordSerSize = 3;
const TForteByte cgWord0[3] = {0x52, 0, 0};
const TForteByte cgWord255[3] = {0x52, 0, 255};
const TForteByte cgWord256[3] = {0x52, 1, 0};
const TForteByte cgWord65535[3] = {0x52, 0xFF, 0xFF};
const TForteByte cgWord40396[3] = {0x52, 0x9D, 0xCC};

//DWORD
const unsigned int cgDWordSerSize = 5;
const TForteByte cgDWord0[5] = {0x53, 0, 0, 0, 0};
const TForteByte cgDWord255[5] = {0x53, 0, 0, 0, 0xFF};
const TForteByte cgDWord256[5] = {0x53, 0, 0, 1, 0};
const TForteByte cgDWord65535[5] = {0x53, 0, 0, 0xFF, 0xFF};
const TForteByte cgDWord65536[5] = {0x53, 0, 1, 0, 0};
const TForteByte cgDWord4294967295[5] = {0x53, 0xFF, 0xFF, 0xFF, 0xFF};
const TForteByte cgDWord690586453[5] = {0x53, 0x29, 0x29, 0x83, 0x55};

//LWORD
const unsigned int cgLWordSerSize = 9;
const TForteByte cgLWord0[9] = {0x54, 0, 0, 0, 0, 0, 0, 0, 0};
const TForteByte cgLWord255[9] = {0x54, 0, 0, 0, 0, 0, 0, 0, 0xFF};
const TForteByte cgLWord256[9] = {0x54, 0, 0, 0, 0, 0, 0, 1, 0};
const TForteByte cgLWord65535[9] = {0x54, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF};
const TForteByte cgLWord65536[9] = {0x54, 0, 0, 0, 0, 0, 1, 0, 0};
const TForteByte cgLWord4294967295[9] = {0x54, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF};
const TForteByte cgLWord4294967296[9] = {0x54, 0, 0, 0, 1, 0, 0, 0, 0};
const TForteByte cgLWord18446744073709551615[9] = {0x54, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const TForteByte cgLWord3544923647067170873[9] =  {0x54, 0x31, 0x32, 0x19, 0x46, 0x73, 0x52, 0x28, 0x39};

//USINT
const unsigned int cgUSIntSerSize = 2;
const TForteByte cgUSInt0[2] = {0x46, 0};
const TForteByte cgUSInt12[2] = {0x46, 12};
const TForteByte cgUSInt128[2] = {0x46, 128};
const TForteByte cgUSInt255[2] = {0x46, 255};

//UINT
const unsigned int cgUIntSerSize = 3;
const TForteByte cgUInt0[3] = {0x47, 0, 0};
const TForteByte cgUInt255[3] = {0x47, 0, 255};
const TForteByte cgUInt256[3] = {0x47, 1, 0};
const TForteByte cgUInt65535[3] = {0x47, 0xFF, 0xFF};
const TForteByte cgUInt40396[3] = {0x47, 0x9D, 0xCC};

//UDINT
const unsigned int cgUDIntSerSize = 5;
const TForteByte cgUDInt0[5] = {0x48, 0, 0, 0, 0};
const TForteByte cgUDInt255[5] = {0x48, 0, 0, 0, 0xFF};
const TForteByte cgUDInt256[5] = {0x48, 0, 0, 1, 0};
const TForteByte cgUDInt65535[5] = {0x48, 0, 0, 0xFF, 0xFF};
const TForteByte cgUDInt65536[5] = {0x48, 0, 1, 0, 0};
const TForteByte cgUDInt4294967295[5] = {0x48, 0xFF, 0xFF, 0xFF, 0xFF};
const TForteByte cgUDInt690586453[5] = {0x48, 0x29, 0x29, 0x83, 0x55};

//ULINT
const unsigned int cgULIntSerSize = 9;
const TForteByte cgULInt0[9] = {0x49, 0, 0, 0, 0, 0, 0, 0, 0};
const TForteByte cgULInt255[9] = {0x49, 0, 0, 0, 0, 0, 0, 0, 0xFF};
const TForteByte cgULInt256[9] = {0x49, 0, 0, 0, 0, 0, 0, 1, 0};
const TForteByte cgULInt65535[9] = {0x49, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF};
const TForteByte cgULInt65536[9] = {0x49, 0, 0, 0, 0, 0, 1, 0, 0};
const TForteByte cgULInt4294967295[9] = {0x49, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF};
const TForteByte cgULInt4294967296[9] = {0x49, 0, 0, 0, 1, 0, 0, 0, 0};
const TForteByte cgULInt18446744073709551615[9] = {0x49, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const TForteByte cgULInt3544923647067170873[9] =  {0x49, 0x31, 0x32, 0x19, 0x46, 0x73, 0x52, 0x28, 0x39};

//SINT
const unsigned int cgSIntSerSize = 2;
const TForteByte cgSInt0[2] = {0x42, 0};
const TForteByte cgSIntm128[2] = {0x42, 0x80};
const TForteByte cgSInt127[2] = {0x42, 127};
const TForteByte cgSIntm90[2] = {0x42, 0xA6};
const TForteByte cgSInt90[2] = {0x42, 0x5A };

//INT
const unsigned int cgIntSerSize = 3;
const TForteByte cgInt0[3] = {0x43, 0, 0};
const TForteByte cgIntm128[3] = {0x43, 0xff, 0x80};
const TForteByte cgIntm129[3] = {0x43, 0xff, 0x7F};
const TForteByte cgInt127[3] = {0x43, 0, 127};
const TForteByte cgInt128[3] = {0x43, 0, 128};
const TForteByte cgIntm32768[3] = {0x43, 0x80, 0x00};
const TForteByte cgInt32767[3] = {0x43, 0x7F, 0xFF};
const TForteByte cgInt10934[3] = {0x43, 0x2A, 0xB6};
const TForteByte cgIntm10934[3] = {0x43, 0xD5, 0x4A};

//DINT
const unsigned int cgDIntSerSize = 5;
const TForteByte cgDInt0[5] = {0x44, 0, 0, 0, 0};
const TForteByte cgDIntm128[5] = {0x44, 0xff, 0xff, 0xff, 0x80};
const TForteByte cgDIntm129[5] = {0x44, 0xff, 0xff, 0xff, 0x7F};
const TForteByte cgDInt127[5] = {0x44, 0, 0, 0, 127};
const TForteByte cgDInt128[5] = {0x44, 0, 0, 0, 128};
const TForteByte cgDIntm32768[5] = {0x44, 0xff, 0xff, 0x80, 0x00};
const TForteByte cgDIntm32769[5] = {0x44, 0xff, 0xff, 0x7F, 0xFF};
const TForteByte cgDInt32767[5] = {0x44, 0, 0, 0x7F, 0xFF};
const TForteByte cgDInt32768[5] = {0x44, 0, 0, 0x80, 0x00};
const TForteByte cgDIntm2147483648[5] = {0x44, 0x80, 0, 0x0, 0x0};
const TForteByte cgDInt2147483647[5] = {0x44, 0x7F, 0xFF, 0xFF, 0xFF};
const TForteByte cgDInt800058586[5] = {0x44, 0x2F, 0xAF, 0xEC, 0xDA};
const TForteByte cgDIntm800058586[5] = {0x44, 0xD0, 0x50, 0x13, 0x26};

//LINT
const unsigned int cgLIntSerSize = 9;
const TForteByte cgLInt0[9] = {0x45, 0, 0, 0, 0, 0, 0, 0, 0};
const TForteByte cgLIntm128[9] = {0x45, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80};
const TForteByte cgLIntm129[9] = {0x45, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7F};
const TForteByte cgLInt127[9] = {0x45, 0, 0, 0, 0, 0, 0, 0, 127};
const TForteByte cgLInt128[9] = {0x45, 0, 0, 0, 0, 0, 0, 0, 128};
const TForteByte cgLIntm32768[9] = {0x45, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00};
const TForteByte cgLIntm32769[9] = {0x45, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7F, 0xFF};
const TForteByte cgLInt32767[9] = {0x45, 0, 0, 0, 0, 0, 0, 0x7F, 0xFF};
const TForteByte cgLInt32768[9] = {0x45, 0, 0, 0, 0, 0, 0, 0x80, 0x00};
const TForteByte cgLIntm2147483648[9] = {0x45, 0xff, 0xff, 0xff, 0xff, 0x80, 0, 0x0, 0x0};
const TForteByte cgLIntm2147483649[9] = {0x45, 0xff, 0xff, 0xff, 0xff, 0x7F, 0xFF, 0xFF, 0xFF};
const TForteByte cgLInt2147483647[9] = {0x45, 0, 0, 0, 0, 0x7F, 0xFF, 0xFF, 0xFF};
const TForteByte cgLInt2147483648[9] = {0x45, 0, 0, 0, 0, 0x80, 0, 0x0, 0x0};
const TForteByte cgLIntm9223372036854775808[9] = {0x45, 0x80, 0, 0x0, 0x0, 0, 0, 0, 0};
const TForteByte cgLInt9223372036854775807[9] = {0x45, 0x7F, 0xFF, 0xFF, 0xFF, 0xff, 0xff, 0xff, 0xff};
const TForteByte cgLInt800058586[9] = {0x45, 0, 0, 0, 0, 0x2F, 0xAF, 0xEC, 0xDA};
const TForteByte cgLIntm800058586[9] = {0x45, 0xff, 0xff, 0xff, 0xff, 0xD0, 0x50, 0x13, 0x26};

//REAL
const unsigned int cgRealSerSize = 5;
const TForteByte cgReal0[5] = {0x4A, 0, 0, 0, 0};
const TForteByte cgReal2_2874e6[5] = {0x4A, 0x4a, 0x0b, 0x9c, 0xa0};
const TForteByte cgRealm6_2587em4[5] = {0x4A, 0xba, 0x24, 0x11, 0x6d};
const TForteByte cgReal1_0Em37[5] = {0x4A, 0x02, 0x08, 0x1c, 0xea};
const TForteByte cgReal36_0[5] = {0x4A, 0x42, 0x10, 0x00, 0x00};

//LREAL
const unsigned int cgLRealSerSize = 9;
const TForteByte cgLReal0[9] = {0x4B, 0, 0, 0, 0, 0, 0, 0, 0};
const TForteByte cgLReal2_28743e6[9] = {0x4B, 0x41, 0x41, 0x73, 0xa3, 0x00, 0x00, 0x00, 0x00};
const TForteByte cgLRealm6_2587em4[9] = {0x4B, 0xbf, 0x44, 0x82, 0x2d, 0x97, 0x3c, 0xf2, 0xfb};
const TForteByte cgLReal1_0Em37[9] = {0x4B, 0x38, 0x41, 0x03, 0x9d, 0x42, 0x8a, 0x8b, 0x8f};
const TForteByte cgLReal36_0[9] = {0x4B, 0x40, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//STRING
const unsigned int cgStringEmptySerSize = 3;
const TForteByte cgStringEmpty[3] = {0x50, 0, 0};
const unsigned int cgStringHalloWorldSerSize = 13;
const TForteByte cgStringHalloWorld[13] = {0x50, 0, 0xA, 'H', 'a', 'l', 'l', 'o', 'W', 'o', 'r', 'l', 'd'};

//WSTRING
const unsigned int cgWStringEmptySerSize = 3;
const TForteByte cgWStringEmpty[3] = {0x55, 0, 0 };
const unsigned int cgWStringHalloWorldSerSize = 23;
const TForteByte cgWStringHalloWorld[23] = {0x55, 0, 0xA, 0, 'H', 0, 'a', 0, 'l', 0, 'l', 0, 'o', 0, 'W', 0, 'o', 0, 'r', 0, 'l', 0, 'd'};
const unsigned int cgWStringNihongoSerSize = 9;
const TForteByte cgWStringNihongo[9] = { 0x55, 0, 3, 0x65, 0xe5, 0x67, 0x2c, 0x8a, 0x9e };
const TForteByte cgWStringNihongoUTF8[10] = { 0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC, 0xE8, 0xAA, 0x9E, 0 };

//TIME
const unsigned int cgTimeSerSize = 9;
const TForteByte cgTime0[9] = {0x4C, 0, 0, 0, 0, 0, 0, 0, 0};
const TForteByte cgTime3000ms[9] = {0x4C, 0, 0, 0, 0, 0, 0x2D, 0xC6, 0xC0};
const TForteByte cgTime3s22ms[9] = {0x4C, 0, 0, 0, 0, 0, 0x2e, 0x1c, 0xb0};


//ARRAY tests
const unsigned int cgBOOL5SerSize = 8;
const TForteByte cgArrayBool10011[] = {0x76, 0x0, 0x5, 0x41, 0x40, 0x40, 0x41, 0x41};
const TForteByte cgArrayBool01010[] = {0x76, 0x0, 0x5, 0x40, 0x41, 0x40, 0x41, 0x40};

const unsigned int cgSINT4SerSize = 8;
const TForteByte cgArraySINTm128_127_0_m90[] = {0x76, 0x0, 0x4, 0x42, 0x80, 127, 0, 0xA6};
const TForteByte cgArraySINTm90_90_127_0[] = {0x76, 0x0, 0x4, 0x42, 0xA6, 0x5A, 127, 0x0};

const unsigned int cgString2SerSize = 18;
const TForteByte cgArrayStringEmptyHalloWorld[] = {0x76, 0x00, 0x02, 0x50, 0, 0, 0, 0xA, 'H', 'a', 'l', 'l', 'o', 'W', 'o', 'r', 'l', 'd'};


