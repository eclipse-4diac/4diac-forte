/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#ifndef SRC_CORE_DATATYPES_CONVERT_LDATEANDTIMETOCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_LDATEANDTIMETOCONVERTFUNCTIONS_H_

//********************************************************************************************
//   LDT_TO_**  functions
//********************************************************************************************

inline const CIEC_DATE_AND_TIME func_LDT_TO_DT(const CIEC_LDATE_AND_TIME &paValue) {
  return CIEC_DATE_AND_TIME(static_cast<CIEC_LDATE_AND_TIME::TValueType>(paValue));
}

CIEC_LDATE func_LDT_TO_LDATE(const CIEC_LDATE_AND_TIME &paVal);

inline const CIEC_DATE func_LDT_TO_DATE(const CIEC_LDATE_AND_TIME &paValue) {
  return CIEC_DATE(static_cast<CIEC_LDATE::TValueType>(func_LDT_TO_LDATE(paValue)));
}

CIEC_LTIME_OF_DAY func_LDT_TO_LTOD(const CIEC_LDATE_AND_TIME &paValue);

inline const CIEC_TIME_OF_DAY func_LDT_TO_TOD(const CIEC_LDATE_AND_TIME &paValue) {
  return CIEC_TIME_OF_DAY(static_cast<CIEC_LTIME_OF_DAY::TValueType>(func_LDT_TO_LTOD(paValue)));
}

#endif /* SRC_CORE_DATATYPES_CONVERT_LDATEANDTIMETOCONVERTFUNCTIONS_H_ */
