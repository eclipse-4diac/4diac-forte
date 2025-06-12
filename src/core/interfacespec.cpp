/*******************************************************************************
 * Copyright (c) 2005, 2025 ACIN, Profactor GmbH, nxtControl GmbH, fortiss GmbH,
 *                          Johannes Kepler University, Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Gunnar Grabmaier, Smodic Rene,
 *   Gerhard Ebenhofer, Michael Hofmann, Martin Melik Merkumians, Ingo Hegny,
 *   Stanislav Meduna, Patrick Smejkal,
 *                - initial implementation and rework communication infrastructure
 *   Alois Zoitl  - introduced new CGenFB class for better handling generic FBs
 *   Martin Jobst - account for data type size in FB initialization
 *   Alois Zoitl  - extracted from funclboc.h / funcllboc.cpp
 *******************************************************************************/

#include "interfacespec.h"
#include "datatype.h"

USE_STRING_ID(Event);

namespace {

  TPortId getPortId(CStringDictionary::TStringId paPortNameId,
                    std::span<const CStringDictionary::TStringId> paPortNames) {
    auto it = std::find(paPortNames.begin(), paPortNames.end(), paPortNameId);
    if (it == paPortNames.end()) {
      return cgInvalidPortId;
    }
    return static_cast<TPortId>(it - paPortNames.begin());
  }

} // namespace

TEventID SFBInterfaceSpec::getEIID(CStringDictionary::TStringId paEINameId) const {
  return static_cast<TEventID>(getPortId(paEINameId, mEINames));
}

TEventID SFBInterfaceSpec::getEOID(CStringDictionary::TStringId paEONameId) const {
  return static_cast<TEventID>(getPortId(paEONameId, mEONames));
}

CStringDictionary::TStringId SFBInterfaceSpec::getEIType(TEventID paEIID) const {
  if (!mEITypeNames.empty()) {
    return mEITypeNames[paEIID];
  }
  return STRID(Event);
}

CStringDictionary::TStringId SFBInterfaceSpec::getEOType(TEventID paEOID) const {
  if (!mEOTypeNames.empty()) {
    return mEOTypeNames[paEOID];
  }
  return STRID(Event);
}

TPortId SFBInterfaceSpec::getDIID(CStringDictionary::TStringId paDINameId) const {
  return getPortId(paDINameId, mDINames);
}

TPortId SFBInterfaceSpec::getDOID(CStringDictionary::TStringId paDONameId) const {
  return getPortId(paDONameId, mDONames);
}

TPortId SFBInterfaceSpec::getDIOID(CStringDictionary::TStringId paDIONameId) const {
  return getPortId(paDIONameId, mDIONames);
}

TPortId SFBInterfaceSpec::getPlugID(CStringDictionary::TStringId paPlugNameId) const {
  return getPortId(paPlugNameId, mPlugNames);
}

TPortId SFBInterfaceSpec::getSocketID(CStringDictionary::TStringId paSocketNameId) const {
  return getPortId(paSocketNameId, mSocketNames);
}
