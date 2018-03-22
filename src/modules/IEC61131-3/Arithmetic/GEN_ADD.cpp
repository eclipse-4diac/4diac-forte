/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Matthias Plasch, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "GEN_ADD.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_ADD_gen.cpp"
#endif

#include <anyhelper.h>
#include <ctype.h>
#include <stdio.h>
#include <forte_printer.h>

DEFINE_GENERIC_FIRMWARE_FB(GEN_ADD, g_nStringIdGEN_ADD)

const CStringDictionary::TStringId GEN_ADD::scm_anDataOutputNames[] = {
    g_nStringIdOUT };

const CStringDictionary::TStringId GEN_ADD::scm_anDataOutputTypeIds[] = {
    g_nStringIdANY_MAGNITUDE };

const TForteInt16 GEN_ADD::scm_anEIWithIndexes[] = { 0 };
const CStringDictionary::TStringId GEN_ADD::scm_anEventInputNames[] = {
    g_nStringIdREQ };

const TDataIOID GEN_ADD::scm_anEOWith[] = { 0, 255 };
const TForteInt16 GEN_ADD::scm_anEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId GEN_ADD::scm_anEventOutputNames[] = {
    g_nStringIdCNF };

GEN_ADD::GEN_ADD(const CStringDictionary::TStringId pa_nInstanceNameId,
    CResource *pa_poSrcRes) :
    CFunctionBlock(pa_poSrcRes, 0, pa_nInstanceNameId, 0, 0) {
}

GEN_ADD::~GEN_ADD() {
  delete[] m_anDataInputNames;
  delete[] m_anDataInputTypeIds;
  delete[] m_anEIWith;
}

void GEN_ADD::executeEvent(int pa_nEIID) {
  switch (pa_nEIID) {
  case scm_nEventREQID:

    anyMagnitudeFBHelper<GEN_ADD>(OUT().getDataTypeID(), *this);
    sendOutputEvent(scm_nEventCNFID);

    break;
  }
}

bool GEN_ADD::configureFB(const char *pa_acConfigString) {
  bool bRetVal = false;

  m_nConfiguredFBTypeNameId = CStringDictionary::getInstance().insert(
      pa_acConfigString);

  const char *pcPos = strrchr(pa_acConfigString, '_');

  if (0 != pcPos) {
    pcPos++;
    //we have an underscore and it is the first underscore after AND
    m_nDInputs = static_cast<int>(forte::core::util::strtoul(pcPos, 0, 10));
    DEVLOG_DEBUG("DIs: %d;\n", m_nDInputs);
  } else {
    return false;
  }

  if (m_nDInputs < 2) {

    return false;
  }

  //now the number of needed eventInputs and dataOutputs are available in the integer array
  //create the eventInputs
  if (m_nDInputs < CFunctionBlock::scm_nMaxInterfaceEvents) {

    //create the data inputs
    m_anDataInputNames = new CStringDictionary::TStringId[m_nDInputs];
    m_anDataInputTypeIds = new CStringDictionary::TStringId[m_nDInputs];

    char diNames[cg_nIdentifierLength] = { "IN" };

    for (int di = 0; di < m_nDInputs; di = di + 1) {
      forte_snprintf(&(diNames[2]), 5 - 2, "%i", di + 1);
      m_anDataInputNames[di] = CStringDictionary::getInstance().insert(diNames);
      m_anDataInputTypeIds[di] = g_nStringIdANY_MAGNITUDE;
    }

    //now create the WITH constructs...
    //the With-Indexes for the data variables
    m_anEIWith = new TDataIOID[m_nDInputs + 1]; //for inputs + '255' separators at the list end

    //in-withs
    for (int in_with = 0; in_with < m_nDInputs + 1; in_with = in_with + 1) {
      if (in_with == m_nDInputs) {
        //set end separator of with
        m_anEIWith[in_with] = 255;
      } else {
        m_anEIWith[in_with] = static_cast<TDataIOID>(in_with);
      }
    }

    //create the interface Specification
    SFBInterfaceSpecforGenerics *pstInterfaceSpec =
        new SFBInterfaceSpecforGenerics(1, scm_anEventInputNames, m_anEIWith,
            scm_anEIWithIndexes, 1, scm_anEventOutputNames, scm_anEOWith,
            scm_anEOWithIndexes, static_cast<TForteUInt8>(m_nDInputs), m_anDataInputNames,
            m_anDataInputTypeIds, 1, scm_anDataOutputNames,
            scm_anDataOutputTypeIds);

    TForteByte *acFBConnData = new TForteByte[genFBConnDataSize(
        pstInterfaceSpec->m_nNumEOs, pstInterfaceSpec->m_nNumDIs,
        pstInterfaceSpec->m_nNumDOs)];
    TForteByte *acFBVarsData = new TForteByte[genFBVarsDataSize(
        pstInterfaceSpec->m_nNumDIs, pstInterfaceSpec->m_nNumDOs)];

    setupFBInterface(pstInterfaceSpec, acFBConnData, acFBVarsData, true);
    bRetVal = true;
  }
  return bRetVal;
}
