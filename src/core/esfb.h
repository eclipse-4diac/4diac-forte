/*******************************************************************************
 * Copyright (c) 2005 - 2014 Profactor GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl, Gerhard Ebenhofer
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _ESFB_H_
#define _ESFB_H_

#include "funcbloc.h"

/*!\ingroup CORE\brief Base-Class for all event sources.
 */
class CEventSourceFB : public CFunctionBlock{
private:

/* \brief the event chain executor used by this ES.
 */
  CEventChainExecutionThread *mEventChainExecutor;
  TEventEntry mEventSourceEventEntry; //! the event entry to start the event chain

public:
  CEventSourceFB(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
                 CStringDictionary::TStringId paInstanceNameId) :
          CFunctionBlock(paSrcRes, paInterfaceSpec, paInstanceNameId),
          mEventChainExecutor(nullptr),
          mEventSourceEventEntry(this, cgExternalEventID) {
  }

  ~CEventSourceFB() override = default;
  void setEventChainExecutor(CEventChainExecutionThread *paEventChainExecutor) { mEventChainExecutor = paEventChainExecutor; };
  CEventChainExecutionThread * getEventChainExecutor() { return mEventChainExecutor; };

  TEventEntry *getEventSourceEventEntry() { return &mEventSourceEventEntry; };
};

#define EVENT_SOURCE_FUNCTION_BLOCK_CTOR(fbclass) \
 fbclass(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) : \
 CEventSourceFB( paSrcRes, &scmFBInterfaceSpec, paInstanceNameId)

#endif /*_ESFB_H_*/
