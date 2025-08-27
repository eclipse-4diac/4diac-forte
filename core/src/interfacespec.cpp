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

#include "forte/interfacespec.h"
#include "forte/datatype.h"

using namespace forte::core::literals;

TPortId forte::getPortId(forte::core::StringId paPortNameId, std::span<const forte::core::StringId> paPortNames) {
  auto it = std::find(paPortNames.begin(), paPortNames.end(), paPortNameId);
  if (it == paPortNames.end()) {
    return cgInvalidPortId;
  }
  return static_cast<TPortId>(it - paPortNames.begin());
}

TEventID SFBInterfaceSpec::getEIID(forte::core::StringId paEINameId) const {
  return static_cast<TEventID>(forte::getPortId(paEINameId, mEINames));
}

TEventID SFBInterfaceSpec::getEOID(forte::core::StringId paEONameId) const {
  return static_cast<TEventID>(forte::getPortId(paEONameId, mEONames));
}

forte::core::StringId SFBInterfaceSpec::getEIType(TEventID paEIID) const {
  if (!mEITypeNames.empty()) {
    return mEITypeNames[paEIID];
  }
  return "Event"_STRID;
}

forte::core::StringId SFBInterfaceSpec::getEOType(TEventID paEOID) const {
  if (!mEOTypeNames.empty()) {
    return mEOTypeNames[paEOID];
  }
  return "Event"_STRID;
}

TPortId SFBInterfaceSpec::getDIID(forte::core::StringId paDINameId) const {
  return forte::getPortId(paDINameId, mDINames);
}

TPortId SFBInterfaceSpec::getDOID(forte::core::StringId paDONameId) const {
  return forte::getPortId(paDONameId, mDONames);
}

TPortId SFBInterfaceSpec::getDIOID(forte::core::StringId paDIONameId) const {
  return forte::getPortId(paDIONameId, mDIONames);
}

TPortId SFBInterfaceSpec::getPlugID(forte::core::StringId paPlugNameId) const {
  return forte::getPortId(paPlugNameId, mPlugNames);
}

TPortId SFBInterfaceSpec::getSocketID(forte::core::StringId paSocketNameId) const {
  return forte::getPortId(paSocketNameId, mSocketNames);
}
