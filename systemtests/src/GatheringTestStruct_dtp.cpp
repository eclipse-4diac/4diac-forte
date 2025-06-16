/*******************************************************************************
 * Copyright (c) 2026 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Jobst - initial implementation
 *******************************************************************************/

#include "GatheringTestStruct_dtp.h"

using namespace forte::literals;

namespace forte::systemtest {
  DEFINE_FIRMWARE_DATATYPE(GatheringTestStruct, "GatheringTestStruct"_STRID);

  const StringId CIEC_GatheringTestStruct::scmElementNames[] = {"VAR1"_STRID, "VAR2"_STRID, "VAR3"_STRID};

  CIEC_GatheringTestStruct::CIEC_GatheringTestStruct() :
      CIEC_STRUCT(),
      var_VAR1(0_BOOL),
      var_VAR2(0_INT),
      var_VAR3(""_STRING) {
  }

  CIEC_GatheringTestStruct::CIEC_GatheringTestStruct(const CIEC_BOOL &paVAR1,
                                                     const CIEC_INT &paVAR2,
                                                     const CIEC_STRING &paVAR3) :
      CIEC_STRUCT(),
      var_VAR1(paVAR1),
      var_VAR2(paVAR2),
      var_VAR3(paVAR3) {
  }

  StringId CIEC_GatheringTestStruct::getStructTypeNameID() const {
    return "GatheringTestStruct"_STRID;
  }

  void CIEC_GatheringTestStruct::setValue(const CIEC_ANY &paValue) {
    if (paValue.getDataTypeID() == e_STRUCT) {
      auto &otherStruct = static_cast<const CIEC_STRUCT &>(paValue);
      if ("GatheringTestStruct"_STRID == otherStruct.getStructTypeNameID()) {
        operator=(static_cast<const CIEC_GatheringTestStruct &>(paValue));
      }
    }
  }

  CIEC_ANY *CIEC_GatheringTestStruct::getMember(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_VAR1;
      case 1: return &var_VAR2;
      case 2: return &var_VAR3;
    }
    return nullptr;
  }

  const CIEC_ANY *CIEC_GatheringTestStruct::getMember(const size_t paIndex) const {
    switch (paIndex) {
      case 0: return &var_VAR1;
      case 1: return &var_VAR2;
      case 2: return &var_VAR3;
    }
    return nullptr;
  }
} // namespace forte::systemtest
