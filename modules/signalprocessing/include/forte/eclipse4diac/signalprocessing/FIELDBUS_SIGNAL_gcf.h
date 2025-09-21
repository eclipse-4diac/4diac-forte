/*************************************************************************
 *** Copyright (c) 2023 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: FIELDBUS_SIGNAL_gcf
 *************************************************************************/

#pragma once

#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_word.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::signalprocessing {
  class FORTE_FIELDBUS_SIGNAL final {
      DECLARE_FIRMWARE_GLOBAL_CONST()
    public:
      static const CIEC_BYTE var_REVERSE;
      static const CIEC_BYTE var_DISABLED;
      static const CIEC_BYTE var_DISENGAGED;
      static const CIEC_BYTE var_FORWARD;
      static const CIEC_BYTE var_ENABLED;
      static const CIEC_BYTE var_ENGAGED;
      static const CIEC_BYTE var_ERROR_INDI_2Bit;
      static const CIEC_BYTE var_NOT_AVAILABLE_2Bit;
      static const CIEC_BYTE var_DONT_CARE_2bit;
      static const CIEC_BYTE var_ERROR_INDI_3Bit;
      static const CIEC_BYTE var_NOT_AVAILABLE_3Bit;
      static const CIEC_BYTE var_DONT_CARE_3bit;
      static const CIEC_BYTE var_ERROR_INDI_4bit;
      static const CIEC_BYTE var_NOT_AVAILABLE_4bit;
      static const CIEC_BYTE var_DONT_CARE_4bit;
      static const CIEC_BYTE var_VALID_SIGNAL_B;
      static const CIEC_BYTE var_ERROR_INDI_B;
      static const CIEC_BYTE var_NOT_AVAILABLE_B;
      static const CIEC_BYTE var_DONT_CARE_B;
      static const CIEC_WORD var_VALID_SIGNAL_W;
      static const CIEC_WORD var_ERROR_INDI_W;
      static const CIEC_WORD var_NOT_AVAILABLE_W;
      static const CIEC_WORD var_ERROR_INDI_WM;
      static const CIEC_WORD var_NOT_AVAILABLE_WM;
      static const CIEC_WORD var_DONT_CARE_W;
      static const CIEC_DWORD var_VALID_SIGNAL_DW;
      static const CIEC_DWORD var_ERROR_INDI_DW;
      static const CIEC_DWORD var_NOT_AVAILABLE_DW;
      static const CIEC_DWORD var_ERROR_INDI_DWM;
      static const CIEC_DWORD var_NOT_AVAILABLE_DM;

      FORTE_FIELDBUS_SIGNAL() = delete;
  };
} // namespace forte::eclipse4diac::signalprocessing
