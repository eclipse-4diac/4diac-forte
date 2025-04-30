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
    for (TPortId i = 0; i < st_OUT().getStructSize(); i++) {
      st_OUT().getMember(i)->setValue(*getDI(i));
    }
    sendOutputEvent(scmEventCNFID, paECET);
  }
}

GEN_STRUCT_MUX::GEN_STRUCT_MUX(const CStringDictionary::TStringId paInstanceNameId,
                               forte::core::CFBContainer &paContainer) :
    CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId) {
}

void GEN_STRUCT_MUX::setInitialValues() {
  CFunctionBlock::setInitialValues();
  copyStructValuesToInputs();
}

bool GEN_STRUCT_MUX::initialize() {
  if (CGenFunctionBlock::initialize()) {
    copyStructValuesToInputs();
    return true;
  }
  return false;
}

void GEN_STRUCT_MUX::copyStructValuesToInputs() {
  for (TPortId i = 0; i < st_OUT().getStructSize(); i++) {
    getDI(i)->setValue(st_OUT().getMember(i)->unwrap());
  }
}

void GEN_STRUCT_MUX::readInputData(TEventID) {
  for (TPortId i = 0; i < getFBInterfaceSpec().mNumDIs; ++i) {
    readData(i, *mDIs[i], mDIConns[i]);
  }
}

void GEN_STRUCT_MUX::writeOutputData(TEventID) {
  writeData(getFBInterfaceSpec().mNumDIs + 0, *mDOs[0], mDOConns[0]);
}

bool GEN_STRUCT_MUX::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {

  const auto structTypeNameId = getStructNameId(paConfigString);
  if (structTypeNameId == CStringDictionary::scmInvalidStringId) {
    DEVLOG_ERROR("[GEN_STRUCT_MUX]: Structure name for %s does not exist\n", paConfigString);
    return false;
  }

  std::unique_ptr<CIEC_ANY> data(CTypeLib::createDataTypeInstance(structTypeNameId, nullptr));

  if (nullptr == data) {
    DEVLOG_ERROR("[GEN_STRUCT_MUX]: Couldn't create structure of type: %s\n", CStringDictionary::get(structTypeNameId));
    return false;
  }

  if (data->getDataTypeID() != CIEC_ANY::e_STRUCT) {
    DEVLOG_ERROR("[GEN_STRUCT_MUX]: data type is not a structure: %s\n", CStringDictionary::get(structTypeNameId));
    return false;
  }

  // we could find the struct
  auto structInstance = static_cast<CIEC_STRUCT *>(data.get());

  const auto structSize = structInstance->getStructSize();
  if (structSize == 0 || structSize >= cgInvalidPortId) { // the structure size must be non zero and less than
                                                          // cgInvalidPortId (maximum number of data input)
    DEVLOG_ERROR("[GEN_STRUCT_MUX]: The structure %s has a size that is not within range > 0 and < %u\n",
                 CStringDictionary::get(structTypeNameId), cgInvalidPortId);
    return false;
  }

  mDiDataTypeNames = std::make_unique<CStringDictionary::TStringId[]>(calcStructTypeNameSize(*structInstance));
  mDiNames = std::make_unique<CStringDictionary::TStringId[]>(structSize);

  mDoDataTypeNames[0] = structTypeNameId;

  paInterfaceSpec.mNumEIs = 1;
  paInterfaceSpec.mEINames = scmEventInputNames;
  paInterfaceSpec.mEITypeNames = scmEventInputTypeIds;
  paInterfaceSpec.mNumEOs = 1;
  paInterfaceSpec.mEONames = scmEventOutputNames;
  paInterfaceSpec.mEOTypeNames = scmEventOutputTypeIds;
  paInterfaceSpec.mNumDIs = structSize;
  paInterfaceSpec.mDINames = mDiNames.get();
  paInterfaceSpec.mDIDataTypeNames = mDiDataTypeNames.get();
  paInterfaceSpec.mNumDOs = 1;
  paInterfaceSpec.mDONames = scmDataOutputNames;
  paInterfaceSpec.mDODataTypeNames = mDoDataTypeNames.data();

  auto diDataTypeNames = mDiDataTypeNames.get();
  for (decltype(paInterfaceSpec.mNumDIs) i = 0; i < paInterfaceSpec.mNumDIs; i++) {
    const auto &member = *structInstance->getMember(i);
    mDiNames[i] = structInstance->elementNames()[i];
    fillDataPointSpec(member, diDataTypeNames);
  }
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

size_t GEN_STRUCT_MUX::calcStructTypeNameSize(CIEC_STRUCT &paStruct) {
  size_t structSize = paStruct.getStructSize();
  size_t result = 0;
  for (size_t i = 0; i < structSize; i++) {
    result += getDataPointSpecSize(*paStruct.getMember(i));
  }
  return result;
}
