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

#include "forte/eclipse4diac/signalprocessing/FIELDBUS_SIGNAL_gcf.h"

#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_word.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::signalprocessing {
  namespace {
    constexpr std::string_view TypeHash =
        "v1:SHA3-512:721f067bab158051f9496f4a7a754a0892c1de3ff18b2e95192ea2f5b791b092aa5a50e00fc78d7e3bc70e73fb3b56ac0397051c71f79545a0571418a3a18a2a"sv;
  }

  DEFINE_FIRMWARE_GLOBAL_CONST(FORTE_FIELDBUS_SIGNAL, "eclipse4diac::signalprocessing::FIELDBUS_SIGNAL"_STRID, TypeHash)

  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_REVERSE = 0_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_DISABLED = 0_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_DISENGAGED = 0_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_FORWARD = 1_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_ENABLED = 1_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_ENGAGED = 1_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_ERROR_INDI_2Bit = 2_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_NOT_AVAILABLE_2Bit = 3_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_DONT_CARE_2bit = 3_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_ERROR_INDI_3Bit = 6_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_NOT_AVAILABLE_3Bit = 7_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_DONT_CARE_3bit = 7_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_ERROR_INDI_4bit = 14_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_NOT_AVAILABLE_4bit = 15_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_DONT_CARE_4bit = 15_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_VALID_SIGNAL_B = 250_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_ERROR_INDI_B = 254_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_NOT_AVAILABLE_B = 255_BYTE;
  const CIEC_BYTE FORTE_FIELDBUS_SIGNAL::var_DONT_CARE_B = 255_BYTE;
  const CIEC_WORD FORTE_FIELDBUS_SIGNAL::var_VALID_SIGNAL_W = 64255_WORD;
  const CIEC_WORD FORTE_FIELDBUS_SIGNAL::var_ERROR_INDI_W = 65024_WORD;
  const CIEC_WORD FORTE_FIELDBUS_SIGNAL::var_NOT_AVAILABLE_W = 65280_WORD;
  const CIEC_WORD FORTE_FIELDBUS_SIGNAL::var_ERROR_INDI_WM = 65279_WORD;
  const CIEC_WORD FORTE_FIELDBUS_SIGNAL::var_NOT_AVAILABLE_WM = 65535_WORD;
  const CIEC_WORD FORTE_FIELDBUS_SIGNAL::var_DONT_CARE_W = 65535_WORD;
  const CIEC_DWORD FORTE_FIELDBUS_SIGNAL::var_VALID_SIGNAL_DW = 4211081215_DWORD;
  const CIEC_DWORD FORTE_FIELDBUS_SIGNAL::var_ERROR_INDI_DW = 4261412864_DWORD;
  const CIEC_DWORD FORTE_FIELDBUS_SIGNAL::var_NOT_AVAILABLE_DW = 4278190080_DWORD;
  const CIEC_DWORD FORTE_FIELDBUS_SIGNAL::var_ERROR_INDI_DWM = 4278190079_DWORD;
  const CIEC_DWORD FORTE_FIELDBUS_SIGNAL::var_NOT_AVAILABLE_DM = 4294967295_DWORD;
} // namespace forte::eclipse4diac::signalprocessing
