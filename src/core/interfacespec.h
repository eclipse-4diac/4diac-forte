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

#include "event.h"
#include "stringdict.h"
#include <span>

using TDataIOID = TPortId; //!< \ingroup CORE Type for holding an data In- or output ID

/*!\ingroup CORE\brief Structure to hold all data of adapters instantiated in the function block.
 */
struct SAdapterInstanceDef {
    CStringDictionary::TStringId mAdapterTypeNameID; //!< Adapter type name
    CStringDictionary::TStringId mAdapterNameID; //!< Adapter instance name
    bool mIsPlug; //!< Flag for distinction of adapter nature (plug/socket)
};

/*!\ingroup CORE\brief Structure to hold all the data for specifying a function block interface.
 */
struct SFBInterfaceSpec {
    TEventID mNumEIs; //!< Number of event inputs
    const CStringDictionary::TStringId *mEINames; //!< List of the event input names
    const CStringDictionary::TStringId *mEITypeNames; //!< List of the event input types
    const TDataIOID
        *mEIWith; //!< Input WITH reference list. This list contains an array of input data ids. For each input event
                  //!< the associated data inputs are listed. The start for each input event is specified in the
                  //!< mEIWithIndexes field. The end is defined by the value scmWithListDelimiter.
    const TForteInt16
        *mEIWithIndexes; //!< Index list for each input event. This list gives for each input event an entry in the
                         //!< mEIWith. Input events are numbered starting from 0. if the input event has no assciated
                         //!< data inputs -1 is the entry at this event inputs postion.
    TEventID mNumEOs; //!< Number of event outputs
    const CStringDictionary::TStringId *mEONames; //!< List of the event output names
    const CStringDictionary::TStringId *mEOTypeNames; //!< List of the event output types
    const TDataIOID
        *mEOWith; //!< Output WITH reference list. This list contains an array of output data ids. For each output event
                  //!< the associated data outputs are listed. The start for each output event is specified in the
                  //!< mEOWithIndexes field. The end is defined by the value scmWithListDelimiter.
    const TForteInt16
        *mEOWithIndexes; //!< Index list for each output event. This list gives for each output event an entry in the
                         //!< mEOWith. Output events are numbered starting from 0. If the output event has no assciated
                         //!< data outputs -1 is the entry at this event outputs postion. Additionally at the postion
                         //!< mNumEOs in this list an index to an own list in the mEOWith list is stored specifying all
                         //!< output data port that are not associated with any output event. That values will be
                         //!< updated on every FB invocation.
    TPortId mNumDIs; //!< Number of data inputs
    const CStringDictionary::TStringId *mDINames; //!< List of the data input names
    const CStringDictionary::TStringId *mDIDataTypeNames; //!< List of the data type names for the data inputs
    TPortId mNumDOs; //!< Number of data outputs
    const CStringDictionary::TStringId *mDONames; //!< List of the data output names
    const CStringDictionary::TStringId *mDODataTypeNames; //!< List of the data type names for the data outputs
    TPortId mNumDIOs; //!< Number of data inouts
    const CStringDictionary::TStringId *mDIONames; //!< List of the data inout names
    TPortId mNumAdapters; //!< Number of Adapters
    const SAdapterInstanceDef *mAdapterInstanceDefinition; //!< List of adapter instances

    std::span<const CStringDictionary::TStringId> mSocketNames;
    std::span<const CStringDictionary::TStringId> mPlugNames;

    /*!\brief Get the ID of a specific event input of the FB.
     *
     * \param paEINameId   StringId to the event input name.
     * \return The ID of the event input or cgInvalidEventID.
     */
    TEventID getEIID(CStringDictionary::TStringId paEINameId) const;

    /*!\brief Get the ID of a specific event output of the FB.
     *
     * \param paEONameId string id to the event output name.
     * \return The ID of the event output or cgInvalidEventID.
     */
    TEventID getEOID(CStringDictionary::TStringId paEONameId) const;

    /*! \brief Gets the EventTypeID of a specific input event
     * \param paEIID  StringId of the event name.
     * \return Returns the EventTypeID of a specific input event
     */
    CStringDictionary::TStringId getEIType(TEventID paEIID) const;

    /*! \brief Gets the EventTypeID of a specific output event
     * \param paEOID  StringId of the event name.
     * \return Returns the EventTypeID of a specific output event
     */
    CStringDictionary::TStringId getEOType(TEventID paEOID) const;

    /*! \brief Gets the index of the mDINames array of a specific data output of a FB
     *
     * \param paDINameId  StringId of the data input name.
     * \return Returns index of the Data Input Array of a FB
     */
    TPortId getDIID(CStringDictionary::TStringId paDINameId) const;

    /*! \brief Gets the index of the mDONames array of a specific data output of a FB
     * \param paDONameId  StringId of the data input name.
     * \return Returns index of the Data Output Array of a FB
     */
    TPortId getDOID(CStringDictionary::TStringId paDONameId) const;

    /*! \brief Gets the index of the mDONames array of a specific data output of a FB
     * \param paDONameId  StringId of the data input name.
     * \return Returns index of the Data Output Array of a FB
     */
    TPortId getDIOID(CStringDictionary::TStringId paDIONameId) const;

    TPortId getPlugID(CStringDictionary::TStringId paPlugNameId) const;

    TPortId getSocketID(CStringDictionary::TStringId paSocketNameId) const;
};
