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

#ifndef SRC_CORE_DATATYPES_CONVERT_TIMEOFDAYTOCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_TIMEOFDAYTOCONVERTFUNCTIONS_H_

//********************************************************************************************
//   L/TOD_TO_*  functions
//********************************************************************************************

inline const CIEC_TIME_OF_DAY func_LTOD_TO_TOD(const CIEC_LTIME_OF_DAY &paValue) {
  return CIEC_TIME_OF_DAY(static_cast<CIEC_LTIME_OF_DAY::TValueType>(paValue));
}

inline const CIEC_LTIME_OF_DAY func_TOD_TO_LTOD(const CIEC_TIME_OF_DAY &paValue) {
  return CIEC_ANY::cast<CIEC_TIME_OF_DAY>(paValue);
}

#endif /* SRC_CORE_DATATYPES_CONVERT_TIMEOFDAYTOCONVERTFUNCTIONS_H_ */