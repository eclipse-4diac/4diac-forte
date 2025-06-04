/*******************************************************************************
 * Copyright (c) 2020, 2023 Johannes Kepler University
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *   Martin Jobst - add generic readInputData and writeOutputData
 *******************************************************************************/
#include "GEN_STRUCT_MUX_fbt.h"

USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(GEN_STRUCT_MUX);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

DEFINE_GENERIC_FIRMWARE_FB(GEN_STRUCT_MUX, STRID(GEN_STRUCT_MUX));

const CStringDictionary::TStringId GEN_STRUCT_MUX::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId GEN_STRUCT_MUX::scmEventInputTypeIds[] = {STRID(Event)};
const CStringDictionary::TStringId GEN_STRUCT_MUX::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId GEN_STRUCT_MUX::scmEventOutputTypeIds[] = {STRID(Event)};

const CStringDictionary::TStringId GEN_STRUCT_MUX::scmDataOutputNames[] = {STRID(OUT)};

void GEN_STRUCT_MUX::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  if (scmEventREQID == paEIID) {
    // the input data elements are directly the members we don't need to explicitly copy them anymore
    sendOutputEvent(scmEventCNFID, paECET);
  }
}

GEN_STRUCT_MUX::GEN_STRUCT_MUX(const CStringDictionary::TStringId paInstanceNameId,
                               forte::core::CFBContainer &paContainer) :
    CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
    conn_CNF(*this, 0) {
}

void GEN_STRUCT_MUX::readInputData(TEventID) {
  for (TPortId i = 0; i < getFBInterfaceSpec().mNumDIs; ++i) {
    readData(i, *var_OUT->getMember(i), mGenDIConns[i]);
  }
}

void GEN_STRUCT_MUX::writeOutputData(TEventID) {
  writeData(getFBInterfaceSpec().mNumDIs + 0, *var_OUT, *conn_OUT);
}

bool GEN_STRUCT_MUX::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {

  const auto structTypeNameId = getStructNameId(paConfigString);
  if (structTypeNameId == CStringDictionary::scmInvalidStringId) {
    DEVLOG_ERROR("[GEN_STRUCT_MUX]: Structure name for %s does not exist\n", paConfigString);
    return false;
  }

  std::unique_ptr<CIEC_ANY> data(forte::core::createDataTypeInstance(structTypeNameId, nullptr));

  if (nullptr == data) {
    DEVLOG_ERROR("[GEN_STRUCT_MUX]: Couldn't create structure of type: %s\n", CStringDictionary::get(structTypeNameId));
    return false;
  }

  if (data->getDataTypeID() != CIEC_ANY::e_STRUCT) {
    DEVLOG_ERROR("[GEN_STRUCT_MUX]: data type is not a structure: %s\n", CStringDictionary::get(structTypeNameId));
    return false;
  }

  // we could find the struct
  var_OUT = std::unique_ptr<CIEC_STRUCT>(static_cast<CIEC_STRUCT *>(data.release()));

  const auto structSize = var_OUT->getStructSize();
  if (structSize == 0 || structSize >= cgInvalidPortId) { // the structure size must be non zero and less than
                                                          // cgInvalidPortId (maximum number of data input)
    DEVLOG_ERROR("[GEN_STRUCT_MUX]: The structure %s has a size that is not within range > 0 and < %u\n",
                 CStringDictionary::get(structTypeNameId), cgInvalidPortId);
    return false;
  }

  conn_OUT = std::make_unique<CGenDataConnection>(*this, 0, *var_OUT);

  paInterfaceSpec.mNumEIs = 1;
  paInterfaceSpec.mEINames = scmEventInputNames;
  paInterfaceSpec.mEITypeNames = scmEventInputTypeIds;
  paInterfaceSpec.mNumEOs = 1;
  paInterfaceSpec.mEONames = scmEventOutputNames;
  paInterfaceSpec.mEOTypeNames = scmEventOutputTypeIds;
  paInterfaceSpec.mNumDIs = structSize;
  paInterfaceSpec.mDINames = var_OUT->elementNames();
  paInterfaceSpec.mNumDOs = 1;
  paInterfaceSpec.mDONames = scmDataOutputNames;
  return true;
}

CStringDictionary::TStringId GEN_STRUCT_MUX::getStructNameId(std::string_view paConfigString) {
  size_t index = paConfigString.find('_');
  if (index != std::string::npos) {
    std::string_view nameId = paConfigString.substr(index + 1);
    index = nameId.find('_');
    if (index != std::string::npos) {
      nameId = nameId.substr(index + 2); // put the position one after the separating number
      return CStringDictionary::getId(nameId.data(), nameId.length());
    }
  }
  return CStringDictionary::scmInvalidStringId;
}

CEventConnection *GEN_STRUCT_MUX::getEOConUnchecked(TPortId paEONum) {
  return (paEONum == 0) ? &conn_CNF : nullptr;
}

CIEC_ANY *GEN_STRUCT_MUX::getDI(size_t paIndex) {
  return var_OUT->getMember(paIndex);
}

CIEC_ANY *GEN_STRUCT_MUX::getDO(size_t paIndex) {
  return (paIndex == 0) ? var_OUT.get() : nullptr;
}

CDataConnection *GEN_STRUCT_MUX::getDOConUnchecked(const TPortId paIndex) {
  return (paIndex == 0) ? conn_OUT.get() : nullptr;
}
