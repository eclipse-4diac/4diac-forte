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
#include "GEN_STRUCT_DEMUX.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_STRUCT_DEMUX_gen.cpp"
#endif
#include "GEN_STRUCT_MUX.h"
#include <stdio.h>
#include "GEN_STRUCT_DEMUX.h"

#include "resource.h"
#include "criticalregion.h"

DEFINE_GENERIC_FIRMWARE_FB(GEN_STRUCT_DEMUX, g_nStringIdGEN_STRUCT_DEMUX);

const CStringDictionary::TStringId GEN_STRUCT_DEMUX::scm_anEventInputNames[] = { g_nStringIdREQ };
const CStringDictionary::TStringId GEN_STRUCT_DEMUX::scm_anEventOutputNames[] = { g_nStringIdCNF };

const CStringDictionary::TStringId GEN_STRUCT_DEMUX::scm_anDataInputNames[] = { g_nStringIdIN };

void GEN_STRUCT_DEMUX::executeEvent(TEventID paEIID) {
  if(scm_nEventREQID == paEIID) {
    for (size_t i = 0; i < st_IN().getStructSize(); i++){
      getDO(static_cast<unsigned int>(i))->setValue(*st_IN().getMember(i));
    }
    sendOutputEvent(scm_nEventCNFID);
  }
}

GEN_STRUCT_DEMUX::GEN_STRUCT_DEMUX(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CGenFunctionBlock<CFunctionBlock>(paSrcRes, paInstanceNameId){
}

GEN_STRUCT_DEMUX::~GEN_STRUCT_DEMUX(){
  if(nullptr != mInterfaceSpec){
    delete[](mInterfaceSpec->m_aunDIDataTypeNames);
    delete[](mInterfaceSpec->m_aunDONames);
    delete[](mInterfaceSpec->m_aunDODataTypeNames);
  }
}

void GEN_STRUCT_DEMUX::readInputData(TEventID) {
  RES_DATA_CON_CRITICAL_REGION();
  readData(0, *mDIs[0], mDIConns[0]);
}

void GEN_STRUCT_DEMUX::writeOutputData(TEventID) {
  RES_DATA_CON_CRITICAL_REGION();
  for(TPortId i = 0; i < mInterfaceSpec->m_nNumDOs; ++i) {
    writeData(i, *mDOs[i], mDOConns[i]);
  }
}

bool GEN_STRUCT_DEMUX::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
  bool retval = false;
  CStringDictionary::TStringId structTypeNameId = GEN_STRUCT_MUX::getStructNameId(paConfigString);

  CIEC_ANY *data = CTypeLib::createDataTypeInstance(structTypeNameId, nullptr);

  if(nullptr != data) {
    if(data->getDataTypeID() == CIEC_ANY::e_STRUCT) {
      // we could find the struct
      CIEC_STRUCT *structInstance = static_cast<CIEC_STRUCT*>(data);

      size_t structSize = structInstance->getStructSize();
      if(structSize != 0 && structSize < cgInvalidPortId) { //the structure size must be non zero and less than cgInvalidPortId (maximum number of data outputs)
        CStringDictionary::TStringId *doDataTypeNames = new CStringDictionary::TStringId[GEN_STRUCT_MUX::calcStructTypeNameSize(*structInstance)];
        CStringDictionary::TStringId *doNames = new CStringDictionary::TStringId[structSize];
        CStringDictionary::TStringId *diDataTypeNames = new CStringDictionary::TStringId[1];

        paInterfaceSpec.m_nNumEIs = 1;
        paInterfaceSpec.m_aunEINames = scm_anEventInputNames;
        paInterfaceSpec.m_nNumEOs = 1;
        paInterfaceSpec.m_aunEONames = scm_anEventOutputNames;
        paInterfaceSpec.m_nNumDIs = 1;
        paInterfaceSpec.m_aunDINames = scm_anDataInputNames;
        paInterfaceSpec.m_aunDIDataTypeNames = diDataTypeNames;
        paInterfaceSpec.m_nNumDOs = structSize;
        paInterfaceSpec.m_aunDONames = doNames;
        paInterfaceSpec.m_aunDODataTypeNames = doDataTypeNames;
        diDataTypeNames[0] = structTypeNameId;

        for(size_t i = 0; i < structSize; ++i) {
          const CIEC_ANY &member = *structInstance->getMember(i);
          doNames[i] = structInstance->elementNames()[i];
          fillDataPointSpec(member, doDataTypeNames);
        }
        retval = true;
      } else {
        DEVLOG_ERROR("[GEN_STRUCT_DEMUX]: The structure %s has a size is not within range > 0 and < %u\n",
                     CStringDictionary::getInstance().get(structTypeNameId), cgInvalidPortId);
      }
    } else {
      DEVLOG_ERROR("[GEN_STRUCT_DEMUX]: data type is not a structure: %s\n", CStringDictionary::getInstance().get(structTypeNameId));
    }
    delete data;
  } else {
    DEVLOG_ERROR("[GEN_STRUCT_DEMUX]: Couldn't create structure of type: %s\n", CStringDictionary::getInstance().get(structTypeNameId));
  }
  return retval;
}




