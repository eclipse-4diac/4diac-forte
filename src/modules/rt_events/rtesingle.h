/*******************************************************************************
 * Copyright (c) 2006 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include <funcbloc.h>
#include <ecet.h>


/*!\brief Base class for all single output real-time event function blocks.
 *
 */
class CRTEventSingle: public CFunctionBlock{
private:
  CEventChainExecutionThread m_oECEO;
  CSyncObject m_oSyncObj;              //!<Lock for protected the RT_E_FB from mutual events
  bool m_bInitialized;

  /*! \brief check if the succeeding EC part needs to be activated.
  *
  * This function makes the assumption that the input event with
  * the id 0 is the INIT event input.
  * \param pa_nEIID the event id of the event that triggered the FB
  * \return if true the succeeding EC part will be activated.
  */
  virtual bool checkActivation(TEventID pa_nEIID) = 0;

  void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override {
    if(paEIID) { //it is not the init event
      if(m_bInitialized && checkActivation(paEIID)) {
        CEventConnection *eoCon = getEOConUnchecked(1);
        if(eoCon->isConnected()) {
          eoCon->triggerEvent(&m_oECEO);
          m_oECEO.resumeSelfSuspend();
        }
      }
    } else { // we got init
      if(var_QI == true) {
        if(!m_bInitialized) {
          m_oECEO.changeExecutionState(EMGMCommandType::Start);
          m_bInitialized = true;
        }
        m_oECEO.setDeadline(var_Deadline);
      } else {
        m_oECEO.changeExecutionState(EMGMCommandType::Stop);
        m_bInitialized = false;
      }
      var_QO = var_QI;
      sendOutputEvent(0, paECET);
    }
  }

public:
  CRTEventSingle(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec,
                 const CStringDictionary::TStringId pa_nInstanceNameId) :
          CFunctionBlock(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId) {
  };

  CIEC_BOOL var_QI;
  CIEC_TIME var_Deadline;
  CIEC_BOOL var_QO;
};

