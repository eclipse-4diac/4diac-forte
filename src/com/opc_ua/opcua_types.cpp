/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Jose Cabral - initial implementation and rework communication infrastructure
 *******************************************************************************/

#include "com/opc_ua/opcua_helper.h"
#include "generated/opcua_types.h"

const UA_DataType *COPC_UA_Helper::getExternalOPCUATypeFromAny(const CIEC_ANY &paAnyType) {
  CIEC_ANY::EDataTypeID typeId = paAnyType.getDataTypeID();
  if (CIEC_ANY::e_STRUCT == typeId) {
    forte::core::StringId typeOfStructure = static_cast<const CIEC_STRUCT &>(paAnyType).getStructTypeNameID();
    for (size_t i = 0; i < sizeof(scmExternalMapForteTypeIdToOpcUa) / sizeof(UA_TypeConvert_external); i++) {
      if (scmExternalMapForteTypeIdToOpcUa[i].mStringId == typeOfStructure) {
        return scmExternalMapForteTypeIdToOpcUa[i].mType;
      }
    }
  }
  return 0;
}
