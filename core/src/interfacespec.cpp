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

using namespace forte::literals;

namespace forte {

  namespace {
    StringId getPortNameId(TPortId paPortId, std::span<const StringId> paPortNames) {
      if (paPortId >= paPortNames.size()) {
        return {};
      }
      return paPortNames[paPortId];
    }
  } // namespace

  TPortId getPortId(StringId paPortNameId, std::span<const StringId> paPortNames) {
    auto it = std::find(paPortNames.begin(), paPortNames.end(), paPortNameId);
    if (it == paPortNames.end()) {
      return cgInvalidPortId;
    }
    return static_cast<TPortId>(it - paPortNames.begin());
  }

  TEventID SFBInterfaceSpec::getEIID(StringId paEINameId) const {
    return static_cast<TEventID>(getPortId(paEINameId, mEINames));
  }

  TEventID SFBInterfaceSpec::getEOID(StringId paEONameId) const {
    return static_cast<TEventID>(getPortId(paEONameId, mEONames));
  }

  StringId SFBInterfaceSpec::getEIType(TEventID paEIID) const {
    if (!mEITypeNames.empty()) {
      return mEITypeNames[paEIID];
    }
    return "Event"_STRID;
  }

  StringId SFBInterfaceSpec::getEOType(TEventID paEOID) const {
    if (!mEOTypeNames.empty()) {
      return mEOTypeNames[paEOID];
    }
    return "Event"_STRID;
  }

  TPortId SFBInterfaceSpec::getDIID(StringId paDINameId) const {
    return getPortId(paDINameId, mDINames);
  }

  TPortId SFBInterfaceSpec::getDOID(StringId paDONameId) const {
    return getPortId(paDONameId, mDONames);
  }

  TPortId SFBInterfaceSpec::getDIOID(StringId paDIONameId) const {
    return getPortId(paDIONameId, mDIONames);
  }

  TPortId SFBInterfaceSpec::getPlugID(StringId paPlugNameId) const {
    return getPortId(paPlugNameId, mPlugNames);
  }

  TPortId SFBInterfaceSpec::getSocketID(StringId paSocketNameId) const {
    return getPortId(paSocketNameId, mSocketNames);
  }

  StringId SFBInterfaceSpec::getEINameId(TPortId paPortId) const {
    return getPortNameId(paPortId, mEINames);
  }

  StringId SFBInterfaceSpec::getEONameId(TPortId paPortId) const {
    return getPortNameId(paPortId, mEONames);
  }

  StringId SFBInterfaceSpec::getDINameId(TPortId paPortId) const {
    return getPortNameId(paPortId, mDONames);
  }

  StringId SFBInterfaceSpec::getDONameId(TPortId paPortId) const {
    return getPortNameId(paPortId, mDONames);
  }

  StringId SFBInterfaceSpec::getDIONameId(TPortId paPortId) const {
    return getPortNameId(paPortId, mDIONames);
  }

  StringId SFBInterfaceSpec::getSocketNameId(TPortId paPortId) const {
    return getPortNameId(paPortId, mSocketNames);
  }

  StringId SFBInterfaceSpec::getPlugNameId(TPortId paPortId) const {
    return getPortNameId(paPortId, mPlugNames);
  }

} // namespace forte
