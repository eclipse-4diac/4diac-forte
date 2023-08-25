/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Gunnar Grabmaier, Thomas Strasser, Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _EVENT_H_
#define _EVENT_H_

#include "../arch/datatype.h"
#include "conn.h"


/*! \ingroup CORE\brief Datatype for holding the IDs of event inputs.
 *
 *  With the current implementation at max 255 event inputs per FB are possible.
 *  That should be enough.
 */
typedef size_t TEventID;


//!\ingroup CORE Constant for the invalid event input id
const TEventID cgInvalidEventID = cgInvalidPortId;
//!\ingroup CORE Constant for the external event input id
const TEventID cgExternalEventID = cgInvalidEventID - 1;

//!\brief With this marker events are anotated that are from the internals of a CFB to the interface of the CFB
const TPortId cgInternal2InterfaceMarker = cgInvalidPortId + 1;
static_assert((cgInternal2InterfaceMarker & (cgInternal2InterfaceMarker - 1)) == 0,
              "cgInternal2InterfaceMarker must be a power of 2");

const TPortId cgInternal2InterfaceRemovalMask = cgInvalidPortId;
static_assert((cgInternal2InterfaceRemovalMask & (cgInternal2InterfaceRemovalMask + 1)) == 0,
              "cgInternal2InterfaceRemovalMask must be a valid bitmask");

/*!\ingroup CORE \brief Structure to hold the information needed for delivering input events to FBs.
*/
typedef CConnectionPoint TEventEntry;

typedef TEventEntry *TEventEntryPtr;

#endif /*_EVENT_H_*/
