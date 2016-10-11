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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <open62541.h>
#pragma GCC diagnostic pop
#include <map>
#include <functional>
#include "../../core/datatypes/forte_any.h"

struct UA_TypeConvert {
	const UA_DataType *type;
	bool (*get)(const CIEC_ANY *src, void* dst);
};

class COPC_UA_Helper {
public:
	static const std::map<CIEC_ANY::EDataTypeID, UA_TypeConvert> mapForteTypeIdToOpcUa;
	static const std::map<CStringDictionary::TStringId, const UA_TypeConvert*> mapForteStringIdToOpcUa;

private:


	static std::map<CIEC_ANY::EDataTypeID, UA_TypeConvert> getTypeMappingTypeId();
	static std::map<CStringDictionary::TStringId, const UA_TypeConvert*> getTypeMappingStringId();
};

#endif //FORTE_OPCUA_HELPER_H
