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
    CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId) {
}

void GEN_ARRAY2ARRAY::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:

      OUT_Array().setValue(IN_Array());

      sendOutputEvent(scmEventCNFID, paECET);

      break;
  }
}

void GEN_ARRAY2ARRAY::readInputData(TEventID) {
  readData(0, *mDIs[0], mDIConns[0]);
}

void GEN_ARRAY2ARRAY::writeOutputData(TEventID) {
  writeData(1 + 0, *mDOs[0], mDOConns[0]);
}

bool GEN_ARRAY2ARRAY::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
  const char *dNumberPos = strchr(paConfigString, '_');

  if (nullptr != dNumberPos) {
    ++dNumberPos;
    // get position of a second underscore
    const char *dTypePos = strchr(dNumberPos, '_');

    if (nullptr != dTypePos) {
      // there is a number and a data type of inputs within the typename
      mArrayLength = static_cast<unsigned int>(forte::core::util::strtoul(dNumberPos, nullptr, 10));
      m_ValueTypeID = CStringDictionary::getId(++dTypePos);
    } else {
      m_ValueTypeID = CStringDictionary::scmInvalidStringId;
      mArrayLength = 0;
    }
  } else {
    return false;
  }

  if (m_ValueTypeID != CStringDictionary::scmInvalidStringId && mArrayLength >= 1) {
    // create data input type
    mDataInputTypeIds[0] = STRID(ARRAY);
    mDataInputTypeIds[1] = mArrayLength;
    mDataInputTypeIds[2] = m_ValueTypeID;

    // create data output type
    mDataOutputTypeIds[0] = STRID(ARRAY);
    mDataOutputTypeIds[1] = mArrayLength;
    mDataOutputTypeIds[2] = m_ValueTypeID;

    // create the interface Specification
    paInterfaceSpec.mNumEIs = 1;
    paInterfaceSpec.mEINames = scmEventInputNames;
    paInterfaceSpec.mNumEOs = 1;
    paInterfaceSpec.mEONames = scmEventOutputNames;
    paInterfaceSpec.mNumDIs = 1;
    paInterfaceSpec.mDINames = scmDataInputNames;
    paInterfaceSpec.mDIDataTypeNames = mDataInputTypeIds.data();
    paInterfaceSpec.mNumDOs = 1;
    paInterfaceSpec.mDONames = scmDataOutputNames;
    paInterfaceSpec.mDODataTypeNames = mDataOutputTypeIds.data();
    return true;
  }

  return false;
}
