/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH, fortiss GmbH
 *                      2018 Johannes Kepler University
 *               2023 Martin Erich Jobst
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
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "GEN_ADD.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_ADD_gen.cpp"
#endif

#include <ifSpecBuilder.h>
#include <ctype.h>
#include <stdio.h>
#include <forte_printer.h>

DEFINE_GENERIC_FIRMWARE_FB(GEN_ADD, g_nStringIdGEN_ADD)

GEN_ADD::GEN_ADD(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CGenFunctionBlock<CFunctionBlock>(pa_poSrcRes, pa_nInstanceNameId), m_nDInputs(0){
}

GEN_ADD::~GEN_ADD(){
}

void GEN_ADD::executeEvent(TEventID paEIID){
  switch (paEIID){
    case scm_nEventREQID:
      if(m_nDInputs) {
        var_OUT() = var_IN(0);
        for (size_t i = 1; i < m_nDInputs; ++i) {
          var_OUT() = std::visit([](auto &&paOUT, auto &&paIN) -> CIEC_ANY_MAGNITUDE_VARIANT {
              using T = std::decay_t<decltype(paOUT)>;
              using U = std::decay_t<decltype(paIN)>;
              using deductedType = typename forte::core::mpl::get_add_operator_result_type<T, U>::type;
              if constexpr (!std::is_same<deductedType, forte::core::mpl::NullType>::value) {
                return func_ADD(paOUT, paIN);
              }
              DEVLOG_ERROR("Adding incompatible types %s and %s\n",
                           CStringDictionary::getInstance().get(paOUT.getTypeNameID()),
                           CStringDictionary::getInstance().get(paIN.getTypeNameID()));
              return paOUT;
          }, var_OUT(), var_IN(i));
        }
      }
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}

bool GEN_ADD::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec){
  const char *pcPos = strrchr(paConfigString, '_');

  if(nullptr != pcPos){
    pcPos++;
    //we have an underscore and it is the first underscore after AND
    m_nDInputs = static_cast<unsigned int>(forte::core::util::strtoul(pcPos, nullptr, 10));
    DEVLOG_DEBUG("DIs: %d;\n", m_nDInputs);
  }
  else{
    return false;
  }

  if(m_nDInputs < 2){
    return false;
  }

  //now the number of needed eventInputs and dataOutputs are available in the integer array
  //create the eventInputs

  static const std::array<CStringDictionary::TStringId, 1>anEventInputNames = { g_nStringIdREQ };
  static const std::array<CStringDictionary::TStringId, 1>anEventOutputNames = { g_nStringIdCNF };
  static const std::array<CStringDictionary::TStringId, 1>anDataOutputNames = { g_nStringIdOUT };
  static const std::array<CStringDictionary::TStringId, 1>anDataOutputTypeIds = { g_nStringIdANY_MAGNITUDE };
  static constexpr std::array<TDataIOID, 2> anEOWith = { 0, scmWithListDelimiter };
  static constexpr std::array<TForteInt16, 1> anEOWithIndexes = { 0 };

  forte::core::util::CIfSpecBuilder isb;
  isb.m_oEI.setStaticEvents(anEventInputNames);
  isb.m_oEO.setStaticEvents(anEventOutputNames);
  auto DIRange = isb.m_oDI.addDataRange("IN", m_nDInputs, g_nStringIdANY_MAGNITUDE);
  isb.m_oDO.setStaticData(anDataOutputNames, anDataOutputTypeIds);
  isb.m_oIWith.bindRange(isb.m_oEI[g_nStringIdREQ], DIRange);
  isb.m_oOWith.setStaticBindings(anEOWith, anEOWithIndexes);

  return isb.build(m_oIfSpecStorage, paInterfaceSpec);
}
