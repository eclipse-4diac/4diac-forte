/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH, fortiss GmbH
 *                      2018 Johannes Kepler University
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
 *******************************************************************************/

#include "GEN_XOR.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_XOR_gen.cpp"
#endif

#include <anyhelper.h>

DEFINE_GENERIC_FIRMWARE_FB(GEN_XOR, g_nStringIdGEN_XOR)

GEN_XOR::GEN_XOR(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CGenBitBase(paInstanceNameId, paSrcRes){
}

GEN_XOR::~GEN_XOR() {
}

void GEN_XOR::executeEvent(int paEIID) {
  switch (paEIID) {
  case scm_nEventREQID:

    anyBitFBHelper<GEN_XOR>(st_OUT().getDataTypeID(), *this);
    sendOutputEvent(scm_nEventCNFID);

    break;
  }
}

