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
 *   Alois Zoitl  - extracted from funclboc.h
 *******************************************************************************/
#pragma once

#include "core/event.h"
#include "core/stringid.h"
#include <span>

using TDataIOID = TPortId; //!< \ingroup CORE Type for holding an data In- or output ID

/*!\ingroup CORE\brief Structure to hold all the data for specifying a function block interface.
 */
struct SFBInterfaceSpec {
    std::span<const forte::core::StringId> mEINames; //!< List of the event input names
    std::span<const forte::core::StringId> mEITypeNames; //!< List of the event input types
    std::span<const forte::core::StringId> mEONames; //!< List of the event output names
    std::span<const forte::core::StringId> mEOTypeNames; //!< List of the event output types
    std::span<const forte::core::StringId> mDINames; //!< List of the data input names
    std::span<const forte::core::StringId> mDONames; //!< List of the data output names
    std::span<const forte::core::StringId> mDIONames; //!< List of the data inout names
    std::span<const forte::core::StringId> mSocketNames;
    std::span<const forte::core::StringId> mPlugNames;

    size_t getNumEIs() const {
      return mEINames.size();
    }

    size_t getNumEOs() const {
      return mEONames.size();
    }

    size_t getNumDIs() const {
      return mDINames.size();
    }

    size_t getNumDOs() const {
      return mDONames.size();
    }

    size_t getNumDIOs() const {
      return mDIONames.size();
    }

    size_t getNumSockets() const {
      return mSocketNames.size();
    }

    size_t getNumPlugs() const {
      return mPlugNames.size();
    }

    /*!\brief Get the ID of a specific event input of the FB.
     *
     * \param paEINameId   StringId to the event input name.
     * \return The ID of the event input or cgInvalidEventID.
     */
    TEventID getEIID(forte::core::StringId paEINameId) const;

    /*!\brief Get the ID of a specific event output of the FB.
     *
     * \param paEONameId string id to the event output name.
     * \return The ID of the event output or cgInvalidEventID.
     */
    TEventID getEOID(forte::core::StringId paEONameId) const;

    /*! \brief Gets the EventTypeID of a specific input event
     * \param paEIID  StringId of the event name.
     * \return Returns the EventTypeID of a specific input event
     */
    forte::core::StringId getEIType(TEventID paEIID) const;

    /*! \brief Gets the EventTypeID of a specific output event
     * \param paEOID  StringId of the event name.
     * \return Returns the EventTypeID of a specific output event
     */
    forte::core::StringId getEOType(TEventID paEOID) const;

    /*! \brief Gets the index of the mDINames array of a specific data output of a FB
     *
     * \param paDINameId  StringId of the data input name.
     * \return Returns index of the Data Input Array of a FB
     */
    TPortId getDIID(forte::core::StringId paDINameId) const;

    /*! \brief Gets the index of the mDONames array of a specific data output of a FB
     * \param paDONameId  StringId of the data input name.
     * \return Returns index of the Data Output Array of a FB
     */
    TPortId getDOID(forte::core::StringId paDONameId) const;

    /*! \brief Gets the index of the mDONames array of a specific data output of a FB
     * \param paDONameId  StringId of the data input name.
     * \return Returns index of the Data Output Array of a FB
     */
    TPortId getDIOID(forte::core::StringId paDIONameId) const;

    TPortId getPlugID(forte::core::StringId paPlugNameId) const;

    TPortId getSocketID(forte::core::StringId paSocketNameId) const;
};

namespace forte {
  TPortId getPortId(forte::core::StringId paPortNameId, std::span<const forte::core::StringId> paPortNames);
}
