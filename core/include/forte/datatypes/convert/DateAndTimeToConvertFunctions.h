/*******************************************************************************
 * Copyright (c) 2010-2019 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH,
 *               2022 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Ingo Hegny, Monika Wenger, Carolyn Oates, Patrick Smejkal,
 *    Matthias Plasch,
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik-Merkumians - fixes DT_TO_TOD
 *    Martin Melik-Merkumians - removes invalid casts, update implementation
 *     to use new cast function
 *******************************************************************************/

#ifndef SRC_CORE_DATATYPES_CONVERT_DATEANDTIMETOCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_DATEANDTIMETOCONVERTFUNCTIONS_H_

//********************************************************************************************
//   DT_TO_**  functions
//********************************************************************************************
inline const CIEC_LDATE_AND_TIME func_DT_TO_LDT(const CIEC_DATE_AND_TIME &paValue) {
  return CIEC_LDATE_AND_TIME(paValue);
}

CIEC_DATE func_DT_TO_DATE(const CIEC_DATE_AND_TIME &paVal);

CIEC_TIME_OF_DAY func_DT_TO_TOD(const CIEC_DATE_AND_TIME &paVal);

inline const CIEC_LTIME_OF_DAY func_DT_TO_LTOD(const CIEC_DATE_AND_TIME &paValue) {
  return CIEC_LTIME_OF_DAY(func_DT_TO_TOD(paValue));
}

#endif /* SRC_CORE_DATATYPES_CONVERT_DATEANDTIMETOCONVERTFUNCTIONS_H_ */
