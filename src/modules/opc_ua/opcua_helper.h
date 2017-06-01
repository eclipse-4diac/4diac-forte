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

#ifndef FORTE_OPCUA_HELPER_H
#define FORTE_OPCUA_HELPER_H

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include <open62541.h>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include "forte_any.h"

struct UA_TypeConvert {
	const UA_DataType *type;

	bool (*get)(const CIEC_ANY *src, void *dst);

	bool (*set)(const void *src, CIEC_ANY *dst);
};

class COPC_UA_Helper {
public:
	/**
	 * Maps EDataTypeID to OPC UA data types
	 */
	static const UA_TypeConvert mapForteTypeIdToOpcUa[CIEC_ANY::e_WSTRING + 1];

	/**
	 * Check if given type ID is valid an within the range of the array.
	 * @param typeId the type ID to check
	 * @return true if valid
	 */
	static inline bool isTypeIdValid(CIEC_ANY::EDataTypeID typeId) {
		return typeId >= CIEC_ANY::e_BOOL && typeId <= CIEC_ANY::e_WSTRING;
	}

	/**
	 * Convert forte DataType (CIEC_...) to the corresponding OPC UA datatype.
	 *
	 * @param typeId type id of the forte datatype
	 * @param src source data
	 * @param dst the value of src will be converted and stored in dst. It has to be initialized before.
	 * @return false if type is not supported
	 */
	static inline bool convertToOpcUa_typeId(CIEC_ANY::EDataTypeID typeId, const CIEC_ANY *src, void *dst) {
		if (!COPC_UA_Helper::isTypeIdValid(typeId))
			return false;
		return COPC_UA_Helper::mapForteTypeIdToOpcUa[typeId].get(src, dst);
	}

	/**
	 * Convert OPC UA DataType to the corresponding forte DataType (CIEC_...).
	 *
	 * @param typeId type id of the forte datatype
	 * @param src source data
	 * @param dst the value of src will be converted and stored in dst. It has to be initialized before.
	 * @return false if type is not supported
	 */
	static inline bool convertFromUa_typeId(CIEC_ANY::EDataTypeID typeId, const void *src, CIEC_ANY *dst) {
		if (!COPC_UA_Helper::isTypeIdValid(typeId))
			return false;
		return COPC_UA_Helper::mapForteTypeIdToOpcUa[typeId].set(src, dst);
	}
};

#endif //FORTE_OPCUA_HELPER_H
