/*******************************************************************************
 * Copyright (c) 2013, 2024 ACIN, Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - refactored from F_MOVE
 *******************************************************************************/

#include "GEN_FORTE_F_MOVE_fbt.h"
#include <memory>
#include "core/eventconn.h"
#include "core/datatypes/forte_any.h"
#include "core/datatypes/forte_any_variant.h"
#include "core/stringdict.h"

USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(GEN_F_MOVE);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

DEFINE_GENERIC_FIRMWARE_FB(GEN_FORTE_F_MOVE, STRID(GEN_F_MOVE))

namespace {
  const auto cDataInputNames = std::array{STRID(IN)};
  const auto cDataOutputNames = std::array{STRID(OUT)};
  const auto cEventInputNames = std::array{STRID(REQ)};
  const auto cEventOutputNames = std::array{STRID(CNF)};
} // namespace

GEN_FORTE_F_MOVE::GEN_FORTE_F_MOVE(const CStringDictionary::TStringId paInstanceNameId,
                                   forte::core::CFBContainer &paContainer) :
    CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
    conn_CNF(*this, 0),
    conn_IN(nullptr) {
}

void GEN_FORTE_F_MOVE::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  if (paEIID == scmEventREQID) {
    // as in and out are managed in one var we only need to send the output event
    sendOutputEvent(scmEventCNFID, paECET);
  }
}

void GEN_FORTE_F_MOVE::readInputData(TEventID paEIID) {
  if (paEIID == scmEventREQID) {
    readData(0, *mIn, conn_IN);
  }
}

void GEN_FORTE_F_MOVE::writeOutputData(TEventID paEOID) {
  if (paEOID == scmEventCNFID) {
    writeData(1 + 0, *mIn, *conn_OUT);
  }
}

bool GEN_FORTE_F_MOVE::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
  if (strcmp(paConfigString, "F_MOVE") == 0) {
    mIn = std::make_unique<CIEC_ANY_VARIANT>();
  } else {
    CStringDictionary::TStringId dataTypeID = getDataTypeNameId(paConfigString);
    mIn = std::unique_ptr<CIEC_ANY>(forte::core::createDataTypeInstance(dataTypeID, nullptr));
    if (!mIn) {
      return false;
    }
  }

  conn_OUT = std::make_unique<CGenDataConnection>(*this, 0, *mIn);

  paInterfaceSpec.mEINames = cEventInputNames;
  paInterfaceSpec.mEONames = cEventOutputNames;
  paInterfaceSpec.mDINames = cDataInputNames;
  paInterfaceSpec.mDONames = cDataOutputNames;

  return true;
}

CStringDictionary::TStringId GEN_FORTE_F_MOVE::getDataTypeNameId(const char *paConfigString) {
  const char *acPos = strchr(paConfigString, '_');
  if (nullptr != acPos) {
    acPos++;
    acPos = strchr(acPos, '_');
    if (nullptr != acPos) {
      acPos += 2; // put the position one after the separating number
      return CStringDictionary::getId(acPos);
    }
  }
  return CStringDictionary::scmInvalidStringId;
}

CEventConnection *GEN_FORTE_F_MOVE::getEOConUnchecked(TPortId paEONum) {
  return (paEONum == 0) ? &conn_CNF : nullptr;
}

CIEC_ANY *GEN_FORTE_F_MOVE::getDI(size_t paIndex) {
  return (paIndex == 0) ? mIn.get() : nullptr;
}

CIEC_ANY *GEN_FORTE_F_MOVE::getDO(size_t paIndex) {
  return (paIndex == 0) ? mIn.get() : nullptr;
}

CDataConnection **GEN_FORTE_F_MOVE::getDIConUnchecked(const TPortId paIndex) {
  return (paIndex == 0) ? &conn_IN : nullptr;
}

CDataConnection *GEN_FORTE_F_MOVE::getDOConUnchecked(TPortId paDONum) {
  return (paDONum == 0) ? conn_OUT.get() : nullptr;
}
