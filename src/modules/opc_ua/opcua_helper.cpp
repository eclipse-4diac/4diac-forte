/*******************************************************************************
  * Copyright (c) 2016 fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Stefan Profanter
  *      - initial implementation
  *******************************************************************************/

#include "opcua_helper.h"
#include "convert_functions.h"

#define UA_String_to_char_alloc(str, chars) { \
        chars = static_cast<char*>(forte_malloc(str->length+1)); \
        memcpy(chars, str->data, str->length); \
        chars[str->length] = 0; \
    }

template<CIEC_ANY::EDataTypeID T_FORTE_E, typename T_FORTE_T, typename T_C>
bool map_convert_get(const CIEC_ANY *src, void *dst) {
  if (src->getDataTypeID() != T_FORTE_E)
    return false;
  T_C *dstType = static_cast<T_C *>(dst);
  *dstType = *(T_FORTE_T *) (src);
  return true;
};

template<CIEC_ANY::EDataTypeID T_FORTE_E, typename T_FORTE_T, typename T_C>
bool map_convert_set(const void *src, CIEC_ANY *dst) {
  if (dst->getDataTypeID() != T_FORTE_E)
    return false;
  T_FORTE_T *dstType = static_cast<T_FORTE_T *>(dst);
  *dstType = *(T_C *) (src);
  return true;
};

#define MAP_INSERT_CONVERT(T_FORTE_E, T_FORTE_T, T_UA, T_C) \
    { \
        T_UA, \
        &map_convert_get<T_FORTE_E, T_FORTE_T, T_C>, \
        &map_convert_set<T_FORTE_E, T_FORTE_T, T_C> \
    } \

#define MAP_INSERT_CONVERT_SPECIFIC(T_FORTE_T, T_UA) \
    { \
        T_UA, \
        &map_convert_get_##T_FORTE_T, \
        &map_convert_set_##T_FORTE_T \
    } \



bool map_convert_get_CIEC_DATE(const CIEC_ANY *src, void *dst) {
  if (src->getDataTypeID() != CIEC_ANY::e_DATE)
    return false;
  UA_DateTime *dstType = static_cast<UA_DateTime *>(dst);
  *dstType = DATE_TO_DT(*(CIEC_DATE *) (src));
  return true;
}

bool map_convert_set_CIEC_DATE(const void *src, CIEC_ANY *dst) {
  if (dst->getDataTypeID() != CIEC_ANY::e_DATE)
    return false;
  CIEC_DATE *dstType = static_cast<CIEC_DATE *>(dst);
  *dstType = DT_TO_DATE(*(TForteUInt64 *) ((const UA_DateTime *) (src)));
  return true;
}

bool map_convert_get_CIEC_TIME_OF_DAY(const CIEC_ANY *src, void *) {
  if (src->getDataTypeID() != CIEC_ANY::e_TIME_OF_DAY)
    return false;
  //TODO how convert TOD to DT?
  return false;
}

bool map_convert_set_CIEC_TIME_OF_DAY(const void *, CIEC_ANY *dst) {
  if (dst->getDataTypeID() != CIEC_ANY::e_TIME_OF_DAY)
    return false;
  //TODO how convert UA_DateTime to TOD?
  return false;
}


bool map_convert_get_CIEC_STRING(const CIEC_ANY *src, void *dst) {
  if (src->getDataTypeID() != CIEC_ANY::e_STRING)
    return false;
  UA_String *dstType = static_cast<UA_String *>(dst);
  *dstType = UA_String_fromChars(static_cast<const CIEC_STRING *>(src)->getValue());
  return true;
}

bool map_convert_set_CIEC_STRING(const void *src, CIEC_ANY *dst) {
  if (dst->getDataTypeID() != CIEC_ANY::e_STRING)
    return false;
  CIEC_STRING *dstType = static_cast<CIEC_STRING *>(dst);

  const UA_String *str = static_cast<const UA_String *>(src);
  char *chars;
  UA_String_to_char_alloc(str, chars);
  dstType->fromString(chars);
  forte_free(chars);
  return true;
}

bool map_convert_get_CIEC_WSTRING(const CIEC_ANY *src, void *dst) {
  if (src->getDataTypeID() != CIEC_ANY::e_WSTRING)
    return false;
  UA_TypeConvert conv = COPC_UA_Helper::mapForteTypeIdToOpcUa[CIEC_ANY::e_STRING];
  CIEC_STRING str = WSTRING_TO_STRING(*static_cast<const CIEC_WSTRING *>(src));
  return conv.get(&str, dst);
}

bool map_convert_set_CIEC_WSTRING(const void *src, CIEC_ANY *dst) {
  if (dst->getDataTypeID() != CIEC_ANY::e_WSTRING)
    return false;
  CIEC_WSTRING *dstType = static_cast<CIEC_WSTRING *>(dst);

  const UA_String *str = static_cast<const UA_String *>(src);
  char *chars;
  UA_String_to_char_alloc(str, chars);
  dstType->fromString(chars);
  forte_free(chars);

  return true;
}

/**
 * ATTENTION:
 * This array has to match the enum CIEC_ANY::EDataTypeID.
 * Current maximum index is WSTRING
 */
const UA_TypeConvert COPC_UA_Helper::mapForteTypeIdToOpcUa[CIEC_ANY::e_WSTRING + 1] = {
    { // dummy for e_ANY
      NULL,
      NULL,
      NULL
    },
    MAP_INSERT_CONVERT(CIEC_ANY::e_BOOL, CIEC_BOOL, &UA_TYPES[UA_TYPES_BOOLEAN], UA_Boolean),
    MAP_INSERT_CONVERT(CIEC_ANY::e_SINT, CIEC_SINT, &UA_TYPES[UA_TYPES_SBYTE], UA_SByte),
    MAP_INSERT_CONVERT(CIEC_ANY::e_INT, CIEC_INT, &UA_TYPES[UA_TYPES_INT16], UA_Int16),
    MAP_INSERT_CONVERT(CIEC_ANY::e_DINT, CIEC_DINT, &UA_TYPES[UA_TYPES_INT32], UA_Int32),
    MAP_INSERT_CONVERT(CIEC_ANY::e_LINT, CIEC_LINT, &UA_TYPES[UA_TYPES_INT64], UA_Int64),
    MAP_INSERT_CONVERT(CIEC_ANY::e_USINT, CIEC_USINT, &UA_TYPES[UA_TYPES_BYTE], UA_Byte),
    MAP_INSERT_CONVERT(CIEC_ANY::e_UINT, CIEC_UINT, &UA_TYPES[UA_TYPES_UINT16], UA_UInt16),
    MAP_INSERT_CONVERT(CIEC_ANY::e_UDINT, CIEC_UDINT, &UA_TYPES[UA_TYPES_UINT32], UA_UInt32),
    MAP_INSERT_CONVERT(CIEC_ANY::e_ULINT, CIEC_ULINT, &UA_TYPES[UA_TYPES_UINT64], UA_UInt64),
    MAP_INSERT_CONVERT(CIEC_ANY::e_BYTE, CIEC_BYTE, &UA_TYPES[UA_TYPES_BYTE], UA_Byte),
    MAP_INSERT_CONVERT(CIEC_ANY::e_WORD, CIEC_WORD, &UA_TYPES[UA_TYPES_UINT16], UA_UInt16),
    MAP_INSERT_CONVERT(CIEC_ANY::e_DWORD, CIEC_DWORD, &UA_TYPES[UA_TYPES_UINT32], UA_UInt32),
    MAP_INSERT_CONVERT(CIEC_ANY::e_LWORD, CIEC_LWORD, &UA_TYPES[UA_TYPES_UINT64], UA_UInt64),
    MAP_INSERT_CONVERT_SPECIFIC(CIEC_DATE, &UA_TYPES[UA_TYPES_DATETIME]),
    MAP_INSERT_CONVERT_SPECIFIC(CIEC_TIME_OF_DAY, &UA_TYPES[UA_TYPES_DATETIME]),
    MAP_INSERT_CONVERT(CIEC_ANY::e_DATE_AND_TIME, CIEC_DATE_AND_TIME, &UA_TYPES[UA_TYPES_DATETIME], UA_DateTime),
    MAP_INSERT_CONVERT(CIEC_ANY::e_TIME, CIEC_TIME, &UA_TYPES[UA_TYPES_DOUBLE], UA_Int64),
    MAP_INSERT_CONVERT(CIEC_ANY::e_REAL, CIEC_REAL, &UA_TYPES[UA_TYPES_FLOAT], UA_Float),
    MAP_INSERT_CONVERT(CIEC_ANY::e_LREAL, CIEC_LREAL, &UA_TYPES[UA_TYPES_DOUBLE], UA_Double),
    MAP_INSERT_CONVERT_SPECIFIC(CIEC_STRING, &UA_TYPES[UA_TYPES_STRING]),
    MAP_INSERT_CONVERT_SPECIFIC(CIEC_WSTRING, &UA_TYPES[UA_TYPES_STRING])
};


