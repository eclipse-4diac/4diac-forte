/*******************************************************************************
 * Copyright (c)2018 Johannes Kepler University
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "genbitbase.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "genbitbase_gen.cpp"
#endif

#include <ctype.h>
#include <stdio.h>
#include <forte_printer.h>


const CStringDictionary::TStringId CGenBitBase::scmDataOutputNames[] = { g_nStringIdOUT };
const CStringDictionary::TStringId CGenBitBase::scmDataOutputTypeIds[] = {g_nStringIdANY_BIT };

const TForteInt16 CGenBitBase::scmEIWithIndexes[] = { 0 };
const CStringDictionary::TStringId CGenBitBase::scmEventInputNames[] = {g_nStringIdREQ };

const TDataIOID CGenBitBase::scmEOWith[] = { 0, 255 };
const TForteInt16 CGenBitBase::scmEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId CGenBitBase::scmEventOutputNames[] = { g_nStringIdCNF };

CGenBitBase::CGenBitBase(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CGenFunctionBlock<CFunctionBlock>(paSrcRes, paInstanceNameId), mDataInputNames(0), mDataInputTypeIds(0), mEIWith(0){
}

CGenBitBase::~CGenBitBase(){
  delete[] mDataInputNames;
  delete[] mDataInputTypeIds;
  delete[] mEIWith;
}

bool CGenBitBase::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
  const char *pcPos = strrchr(paConfigString, '_');

  if (0 != pcPos) {
    pcPos++;
    //we have an underscore and it is the first underscore after AND
    paInterfaceSpec.m_nNumDIs = static_cast<TForteUInt8>(forte::core::util::strtoul(pcPos, 0, 10));
    DEVLOG_DEBUG("DIs: %d;\n", paInterfaceSpec.m_nNumDIs);
  } else {
    return false;
  }

  if (paInterfaceSpec.m_nNumDIs < 2) {
    return false;
  }

  //now the number of needed eventInputs and dataOutputs are available in the integer array
  //create the eventInputs
  if (paInterfaceSpec.m_nNumDIs < CFunctionBlock::scm_nMaxInterfaceEvents) {

    //create the data inputs
    mDataInputNames = new CStringDictionary::TStringId[paInterfaceSpec.m_nNumDIs];
    mDataInputTypeIds = new CStringDictionary::TStringId[paInterfaceSpec.m_nNumDIs];

    char diNames[cg_nIdentifierLength] = { "IN" };

    for (size_t di = 0; di < paInterfaceSpec.m_nNumDIs; ++di) {
      forte_snprintf(&(diNames[2]), 5 - 2, "%i", di + 1);
      mDataInputNames[di] = CStringDictionary::getInstance().insert(diNames);
      mDataInputTypeIds[di] = g_nStringIdANY_BIT;
    }

    //now create the WITH constructs...
    //the With-Indexes for the data variables
    mEIWith = new TDataIOID[paInterfaceSpec.m_nNumDIs + 1]; //for inputs + '255' separators at the list end

    //in-withs
    for (size_t in_with = 0; in_with < paInterfaceSpec.m_nNumDIs; ++in_with) {
      mEIWith[in_with] = static_cast<TDataIOID>(in_with);
    }
    mEIWith[paInterfaceSpec.m_nNumDIs] = scmWithListDelimiter;

    //setup the interface Specification
    paInterfaceSpec.m_nNumEIs = 1;
    paInterfaceSpec.m_aunEINames = scmEventInputNames;
    paInterfaceSpec.m_anEIWith = mEIWith;
    paInterfaceSpec.m_anEIWithIndexes = scmEIWithIndexes;
    paInterfaceSpec.m_nNumEOs = 1;
    paInterfaceSpec.m_aunEONames = scmEventOutputNames;
    paInterfaceSpec.m_anEOWith = scmEOWith;
    paInterfaceSpec.m_anEOWithIndexes = scmEOWithIndexes;
    paInterfaceSpec.m_aunDINames = mDataInputNames;
    paInterfaceSpec.m_aunDIDataTypeNames = mDataInputTypeIds;
    paInterfaceSpec.m_nNumDOs = 1;
    paInterfaceSpec.m_aunDONames = scmDataOutputNames;
    paInterfaceSpec.m_aunDODataTypeNames = scmDataOutputTypeIds;
    return true;
  }
  return false;
}
