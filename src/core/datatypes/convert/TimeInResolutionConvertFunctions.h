/*******************************************************************************
 * Copyright (c) 2019 TU Vienna/ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Martin Melik-Merkumians - adds non standard time to number conversions
 *******************************************************************************/

#ifndef SRC_CORE_DATATYPES_CONVERT_TIMEINRESOLUTIONCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_TIMEINRESOLUTIONCONVERTFUNCTIONS_H_

/*********************************************************************************************
 *   TIME_IN_<RESOLUTION>_TO_* functions - custom non-standard functions
 *********************************************************************************************/
#ifdef FORTE_USE_64BIT_DATATYPES

inline const CIEC_LINT TIME_IN_S_TO_LINT(const CIEC_TIME &paValue) {
  return CIEC_LINT(paValue.getInSeconds());
}

inline const CIEC_LINT TIME_IN_MS_TO_LINT(const CIEC_TIME &paValue) {
  return CIEC_LINT(paValue.getInMilliSeconds());
}

inline const CIEC_LINT TIME_IN_US_TO_LINT(const CIEC_TIME &paValue) {
  return CIEC_LINT(paValue.getInMicroSeconds());
}

inline const CIEC_LINT TIME_IN_NS_TO_LINT(const CIEC_TIME &paValue) {
  return CIEC_LINT(paValue.getInNanoSeconds());
}

#endif /* FORTE_USE_64BIT_DATATYPES */

#endif /* SRC_CORE_DATATYPES_CONVERT_TIMEINRESOLUTIONCONVERTFUNCTIONS_H_ */
