/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 *** 
 *** FORTE Language Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: LED_COLOURS_gcf
 *************************************************************************/

#ifndef _LED_COLOURS_GCF_H_
#define _LED_COLOURS_GCF_H_

#include "forte_uint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"



enum LED_COLOURS {
  c_LED_RED     = 1,
  c_LED_YELLOW  = 2,
  c_LED_ORANGE  = 3,
  c_LED_GREEN   = 4,
  c_LED_BLUE    = 5,
  c_LED_WHITE   = 6,
  c_LED_MAGENTA = 7,
  c_LED_CYAN    = 8,

};


extern const CIEC_UINT st_global_LED_RED;
extern const CIEC_UINT st_global_LED_YELLOW;
extern const CIEC_UINT st_global_LED_ORANGE;
extern const CIEC_UINT st_global_LED_GREEN;
extern const CIEC_UINT st_global_LED_BLUE;
extern const CIEC_UINT st_global_LED_WHITE;
extern const CIEC_UINT st_global_LED_MAGENTA;
extern const CIEC_UINT st_global_LED_CYAN;

#endif // _LED_COLOURS_GCF_H_
