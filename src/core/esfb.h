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
  CEventChainExecutionThread *m_poEventChainExecutor;
  SEventEntry mEventSourceEventEntry; //! the event entry to start the event chain

public:
  CEventSourceFB(CResource *pa_poSrcRes,
           const SFBInterfaceSpec *pa_pstInterfaceSpec,
           const CStringDictionary::TStringId pa_nInstanceNameId, TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    CFunctionBlock(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId, pa_acFBConnData, pa_acFBVarsData),
    m_poEventChainExecutor(0),
    mEventSourceEventEntry(this, cg_nExternalEventID){
  }
  virtual ~CEventSourceFB() {};
  void setEventChainExecutor(CEventChainExecutionThread *pa_poEventChainExecutor) { m_poEventChainExecutor = pa_poEventChainExecutor; };
  CEventChainExecutionThread * getEventChainExecutor(void) { return m_poEventChainExecutor; };

  SEventEntry *getEventSourceEventEntry(void) { return &mEventSourceEventEntry; };
};

#define EVENT_SOURCE_FUNCTION_BLOCK_CTOR(fbclass) \
 fbclass(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : \
 CEventSourceFB( pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData)

#endif /*_ESFB_H_*/
