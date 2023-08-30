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
#include "GEN_STRUCT_MUX.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_STRUCT_MUX_gen.cpp"
#endif

#include "resource.h"
#include "criticalregion.h"

DEFINE_GENERIC_FIRMWARE_FB(GEN_STRUCT_MUX, g_nStringIdGEN_STRUCT_MUX);

const CStringDictionary::TStringId GEN_STRUCT_MUX::scmEventInputNames[] = { g_nStringIdREQ };
const CStringDictionary::TStringId GEN_STRUCT_MUX::scmEventOutputNames[] = { g_nStringIdCNF };

const CStringDictionary::TStringId GEN_STRUCT_MUX::scmDataOutputNames[] = { g_nStringIdOUT };

void GEN_STRUCT_MUX::executeEvent(TEventID paEIID) {
  if(scmEventREQID == paEIID) {
    for (size_t i = 0; i < st_OUT().getStructSize(); i++){
      st_OUT().getMember(i)->setValue(*getDI(static_cast<unsigned int>(i)));
    }
    sendOutputEvent(scmEventCNFID);
  }
}

GEN_STRUCT_MUX::GEN_STRUCT_MUX(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CGenFunctionBlock<CFunctionBlock>(paSrcRes, paInstanceNameId){
}

GEN_STRUCT_MUX::~GEN_STRUCT_MUX(){
  if(nullptr!= mInterfaceSpec){
    delete[](mInterfaceSpec->mDINames);
    delete[](mInterfaceSpec->mDIDataTypeNames);
    delete[](mInterfaceSpec->mDODataTypeNames);
  }
}

void GEN_STRUCT_MUX::readInputData(TEventID) {
  RES_DATA_CON_CRITICAL_REGION();
  for(TPortId i = 0; i < mInterfaceSpec->mNumDIs; ++i) {
    readData(i, *mDIs[i], mDIConns[i]);
  }
}

void GEN_STRUCT_MUX::writeOutputData(TEventID) {
  RES_DATA_CON_CRITICAL_REGION();
  writeData(0, *mDOs[0], mDOConns[0]);
}

bool GEN_STRUCT_MUX::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
  bool retval = false;
  CStringDictionary::TStringId structTypeNameId = getStructNameId(paConfigString);

  CIEC_ANY *data = CTypeLib::createDataTypeInstance(structTypeNameId, nullptr);

  if(nullptr != data) {
    if(data->getDataTypeID() == CIEC_ANY::e_STRUCT) {
      // we could find the struct
      CIEC_STRUCT *structInstance = static_cast<CIEC_STRUCT*>(data);

      size_t structSize = structInstance->getStructSize();
      if(structSize != 0 && structSize < cgInvalidPortId) { //the structure size must be non zero and less than cgInvalidPortId (maximum number of data input)
        CStringDictionary::TStringId *diDataTypeNames = new CStringDictionary::TStringId[calcStructTypeNameSize(*structInstance)];
        CStringDictionary::TStringId *diNames = new CStringDictionary::TStringId[structSize];
        CStringDictionary::TStringId *doDataTypeNames = new CStringDictionary::TStringId[1];

        paInterfaceSpec.mNumEIs = 1;
        paInterfaceSpec.mEINames = scmEventInputNames;
        paInterfaceSpec.mNumEOs = 1;
        paInterfaceSpec.mEONames = scmEventOutputNames;
        paInterfaceSpec.mNumDIs = structSize;
        paInterfaceSpec.mDINames = diNames;
        paInterfaceSpec.mDIDataTypeNames = diDataTypeNames;
        paInterfaceSpec.mNumDOs = 1;
        paInterfaceSpec.mDONames = scmDataOutputNames;
        paInterfaceSpec.mDODataTypeNames = doDataTypeNames;
        doDataTypeNames[0] = structTypeNameId;

        for(size_t i = 0; i < paInterfaceSpec.mNumDIs; i++) {
          CIEC_ANY &member = *structInstance->getMember(i);
          diNames[i] = structInstance->elementNames()[i];
          fillDataPointSpec(member, diDataTypeNames);
        }
        retval = true;
      } else {
        DEVLOG_ERROR("[GEN_STRUCT_MUX]: The structure %s has a size is not within range > 0 and < %u\n",
                     CStringDictionary::getInstance().get(structTypeNameId), cgInvalidPortId);
      }
    } else {
      DEVLOG_ERROR("[GEN_STRUCT_MUX]: data type is not a structure: %s\n", CStringDictionary::getInstance().get(structTypeNameId));
    }
    delete data;
  } else {
    DEVLOG_ERROR("[GEN_STRUCT_MUX]: Couldn't create structure of type: %s\n", CStringDictionary::getInstance().get(structTypeNameId));
  }
  return retval;
}


CStringDictionary::TStringId GEN_STRUCT_MUX::getStructNameId(const char *paConfigString) {
  const char *acPos = strchr(paConfigString, '_');
  if(nullptr != acPos){
    acPos++;
    acPos = strchr(acPos, '_');
    if(nullptr != acPos){
      acPos += 2;  //put the position one after the separating number
      return CStringDictionary::getInstance().getId(acPos);
    }
  }
  return CStringDictionary::scmInvalidStringId;
}

size_t GEN_STRUCT_MUX::calcStructTypeNameSize(CIEC_STRUCT &paStruct){
  size_t structSize = paStruct.getStructSize();
  size_t result = 0;
  for(size_t i = 0; i < structSize; i++) {
    result += getDataPointSpecSize(*paStruct.getMember(i));
  }
  return result;
}

