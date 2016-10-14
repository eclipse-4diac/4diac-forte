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
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "opcua_helper_gen.cpp"
#endif

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP

# include "opcua_helper_gen.cpp"

#endif

#include "convert_functions.h"

#define MAP_INSERT_FORTE_TO_UA(T_FORTE_E, T_FORTE_T, T_UA, T_C) \
 ( \
    { \
        map[T_FORTE_E] = UA_TypeConvert { \
            type: T_UA, \
            get: [](const CIEC_ANY *src, void *dst) { \
                if (src->getDataTypeID() != T_FORTE_E) \
                    return false; \
                T_C *dstType = static_cast<T_C*>(dst); \
                *dstType = *(T_FORTE_T*)(src); \
                return true; \
            }, \
            set: [](const void *src, CIEC_ANY *dst) { \
                if (dst->getDataTypeID() != T_FORTE_E) \
                    return false; \
                T_FORTE_T *dstType = static_cast<T_FORTE_T*>(dst); \
                *dstType = *(T_C*)(src); \
                return true; \
            } \
        }; \
    } \
 )

#define UA_String_to_char_alloc(str, chars) \
 ( \
    { \
        chars = static_cast<char*>(forte_malloc(str->length+1)); \
        memcpy(chars, str->data, str->length); \
        chars[str->length] = 0; \
    } \
 )

std::map<CIEC_ANY::EDataTypeID, UA_TypeConvert> COPC_UA_Helper::getTypeMappingTypeId() {
	std::map<CIEC_ANY::EDataTypeID, UA_TypeConvert> map;

	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_BOOL, CIEC_BOOL, &UA_TYPES[UA_TYPES_BOOLEAN], UA_Boolean);
	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_SINT, CIEC_SINT, &UA_TYPES[UA_TYPES_SBYTE], UA_SByte);
	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_SINT, CIEC_SINT, &UA_TYPES[UA_TYPES_SBYTE], UA_SByte);
	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_INT, CIEC_INT, &UA_TYPES[UA_TYPES_INT16], UA_Int16);
	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_DINT, CIEC_DINT, &UA_TYPES[UA_TYPES_INT32], UA_Int32);
	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_LINT, CIEC_LINT, &UA_TYPES[UA_TYPES_INT64], UA_Int64);
	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_USINT, CIEC_USINT, &UA_TYPES[UA_TYPES_BYTE], UA_Byte);
	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_UINT, CIEC_UINT, &UA_TYPES[UA_TYPES_UINT16], UA_UInt16);
	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_UDINT, CIEC_UDINT, &UA_TYPES[UA_TYPES_UINT32], UA_UInt32);
	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_ULINT, CIEC_ULINT, &UA_TYPES[UA_TYPES_UINT64], UA_UInt64);
	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_BYTE, CIEC_BYTE, &UA_TYPES[UA_TYPES_BYTE], UA_Byte);
	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_WORD, CIEC_WORD, &UA_TYPES[UA_TYPES_UINT16], UA_UInt16);
	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_DWORD, CIEC_DWORD, &UA_TYPES[UA_TYPES_UINT32], UA_UInt32);
	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_LWORD, CIEC_LWORD, &UA_TYPES[UA_TYPES_UINT64], UA_UInt64);

	map[CIEC_ANY::e_DATE] = UA_TypeConvert {
			type: &UA_TYPES[UA_TYPES_DATETIME],
			get: [](const CIEC_ANY *src, void *dst) {
				if (src->getDataTypeID() != CIEC_ANY::e_DATE)
					return false;
				UA_DateTime *dstType = static_cast<UA_DateTime *>(dst);
				*dstType = DATE_TO_DT(*(CIEC_DATE *) (src));
				return true;
			},
			set: [](const void *src, CIEC_ANY *dst) {
				if (dst->getDataTypeID() != CIEC_ANY::e_DATE)
					return false;
				CIEC_DATE *dstType = static_cast<CIEC_DATE *>(dst);
				*dstType = DT_TO_DATE(*(TForteUInt64 *) ((const UA_DateTime *) (src)));
				return true;
			}
	};

	map[CIEC_ANY::e_TIME_OF_DAY] = UA_TypeConvert {
			type: &UA_TYPES[UA_TYPES_DATETIME],
			get: [](const CIEC_ANY *src, __attribute__((unused)) void *dst) {
				if (src->getDataTypeID() != CIEC_ANY::e_TIME_OF_DAY)
					return false;
				//TODO how convert TOD to DT?
				return false;
			},
			set: [](const __attribute__((unused)) void *src, CIEC_ANY *dst) {
				if (dst->getDataTypeID() != CIEC_ANY::e_TIME_OF_DAY)
					return false;
				//TODO how convert UA_DateTime to TOD?
				return false;
			}
	};

	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_DATE_AND_TIME, CIEC_DATE_AND_TIME, &UA_TYPES[UA_TYPES_DATETIME], UA_DateTime);
	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_TIME, CIEC_TIME, &UA_TYPES[UA_TYPES_DOUBLE], UA_Int64);
	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_REAL, CIEC_REAL, &UA_TYPES[UA_TYPES_FLOAT], UA_Float);
	MAP_INSERT_FORTE_TO_UA(CIEC_ANY::e_LREAL, CIEC_LREAL, &UA_TYPES[UA_TYPES_DOUBLE], UA_Double);

	map[CIEC_ANY::e_STRING] = UA_TypeConvert {
			type: &UA_TYPES[UA_TYPES_STRING],
			get: [](const CIEC_ANY *src, void *dst) {
				if (src->getDataTypeID() != CIEC_ANY::e_STRING)
					return false;
				UA_String *dstType = static_cast<UA_String *>(dst);
				*dstType = UA_String_fromChars(static_cast<const CIEC_STRING *>(src)->getValue());
				return true;
			},
			set: [](const void *src, CIEC_ANY *dst) {
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
	};

	map[CIEC_ANY::e_WSTRING] = UA_TypeConvert {
			type: &UA_TYPES[UA_TYPES_STRING],
			get: [](const CIEC_ANY *src, void *dst) {
				if (src->getDataTypeID() != CIEC_ANY::e_WSTRING)
					return false;
				UA_TypeConvert conv = COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_STRING)->second;
				CIEC_STRING str = WSTRING_TO_STRING(*static_cast<const CIEC_WSTRING *>(src));
				return conv.get(&str, dst);
			},
			set: [](const void *src, CIEC_ANY *dst) {
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
	};

	return map;
}


std::map<CStringDictionary::TStringId, const UA_TypeConvert *> COPC_UA_Helper::getTypeMappingStringId() {
	std::map<CStringDictionary::TStringId, const UA_TypeConvert *> map;

	map[g_nStringIdBOOL] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_BOOL)->second;
	map[g_nStringIdSINT] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_SINT)->second;
	map[g_nStringIdINT] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_INT)->second;
	map[g_nStringIdDINT] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_DINT)->second;
	map[g_nStringIdLINT] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_LINT)->second;
	map[g_nStringIdUSINT] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_USINT)->second;
	map[g_nStringIdUINT] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_UINT)->second;
	map[g_nStringIdUDINT] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_UDINT)->second;
	map[g_nStringIdULINT] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_ULINT)->second;
	map[g_nStringIdBYTE] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_BYTE)->second;
	map[g_nStringIdWORD] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_WORD)->second;
	map[g_nStringIdDWORD] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_DWORD)->second;
	map[g_nStringIdLWORD] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_LWORD)->second;
	map[g_nStringIdDATE] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_DATE)->second;
	map[g_nStringIdTIME_OF_DAY] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_TIME_OF_DAY)->second;
	map[g_nStringIdDATE_AND_TIME] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_DATE_AND_TIME)->second;
	map[g_nStringIdTIME] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_TIME)->second;
	map[g_nStringIdREAL] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_REAL)->second;
	map[g_nStringIdLREAL] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_LREAL)->second;
	map[g_nStringIdSTRING] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_STRING)->second;
	map[g_nStringIdWSTRING] = &COPC_UA_Helper::mapForteTypeIdToOpcUa.find(CIEC_ANY::e_WSTRING)->second;

	return map;
}


const std::map<CIEC_ANY::EDataTypeID, UA_TypeConvert> COPC_UA_Helper::mapForteTypeIdToOpcUa = COPC_UA_Helper::getTypeMappingTypeId();
const std::map<CStringDictionary::TStringId, const UA_TypeConvert *> COPC_UA_Helper::mapForteStringIdToOpcUa = COPC_UA_Helper::getTypeMappingStringId();
