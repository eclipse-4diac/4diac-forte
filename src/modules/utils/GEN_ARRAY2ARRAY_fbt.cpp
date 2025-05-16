/*******************************************************************************
 * Copyright (c) 2014, 2023 Profactor GmbH, fortiss GmbH
 *                          Johannes Kepler University
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *    Martin Jobst - add generic readInputData and writeOutputData
 *******************************************************************************/
#include "GEN_ARRAY2ARRAY_fbt.h"
#include "string_utils.h"

USE_STRING_ID(ARRAY);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(GEN_ARRAY2ARRAY);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

DEFINE_GENERIC_FIRMWARE_FB(GEN_ARRAY2ARRAY, STRID(GEN_ARRAY2ARRAY))

const CStringDictionary::TStringId GEN_ARRAY2ARRAY::scmDataInputNames[] = {STRID(IN)};
const CStringDictionary::TStringId GEN_ARRAY2ARRAY::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId GEN_ARRAY2ARRAY::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId GEN_ARRAY2ARRAY::scmEventInputTypeIds[] = {STRID(Event)};

const CStringDictionary::TStringId GEN_ARRAY2ARRAY::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId GEN_ARRAY2ARRAY::scmEventOutputTypeIds[] = {STRID(Event)};

GEN_ARRAY2ARRAY::GEN_ARRAY2ARRAY(const CStringDictionary::TStringId paInstanceNameId,
                                 forte::core::CFBContainer &paContainer) :
    CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_IN) {
}

void GEN_ARRAY2ARRAY::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID: sendOutputEvent(scmEventCNFID, paECET); break;
  }
}

void GEN_ARRAY2ARRAY::readInputData(TEventID) {
  readData(0, var_IN, conn_IN);
}

void GEN_ARRAY2ARRAY::writeOutputData(TEventID) {
  writeData(1 + 0, var_IN, conn_OUT);
}

bool GEN_ARRAY2ARRAY::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
  const char *dNumberPos = strchr(paConfigString, '_');

  if (dNumberPos == nullptr) {
    return false;
  }
  ++dNumberPos;
  // get position of a second underscore
  const char *dTypePos = strchr(dNumberPos, '_');

  if (dTypePos == nullptr) {
    return false;
  }

  // there is a number and a data type of inputs within the typename
  TForteUInt16 arrayLength = static_cast<TForteUInt16>(forte::core::util::strtoul(dNumberPos, nullptr, 10));
  auto valueTypeID = CStringDictionary::getId(++dTypePos);

  if (arrayLength == 0) {
    return false;
  }

  // create data input type
  var_IN.setup(arrayLength, valueTypeID);

  conn_OUT.getValue().setup(arrayLength, valueTypeID);

  // create the interface Specification
  paInterfaceSpec.mNumEIs = 1;
  paInterfaceSpec.mEINames = scmEventInputNames;
  paInterfaceSpec.mNumEOs = 1;
  paInterfaceSpec.mEONames = scmEventOutputNames;
  paInterfaceSpec.mNumDIs = 1;
  paInterfaceSpec.mDINames = scmDataInputNames;
  paInterfaceSpec.mNumDOs = 1;
  paInterfaceSpec.mDONames = scmDataOutputNames;
  return true;
}

CEventConnection *GEN_ARRAY2ARRAY::getEOConUnchecked(TPortId paEONum) {
  return (paEONum == 0) ? &conn_CNF : nullptr;
}

CIEC_ANY *GEN_ARRAY2ARRAY::getDI(size_t paIndex) {
  return (paIndex == 0) ? &var_IN : nullptr;
}

CIEC_ANY *GEN_ARRAY2ARRAY::getDO(size_t paIndex) {
  return (paIndex == 0) ? &var_IN : nullptr;
}

CDataConnection **GEN_ARRAY2ARRAY::getDIConUnchecked(const TPortId paIndex) {
  return (paIndex == 0) ? &conn_IN : nullptr;
}

CDataConnection *GEN_ARRAY2ARRAY::getDOConUnchecked(TPortId paDONum) {
  return (paDONum == 0) ? &conn_OUT : nullptr;
}
