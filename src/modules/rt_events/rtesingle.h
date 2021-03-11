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
#ifndef RTESINGLE_H_
#define RTESINGLE_H_

#include <funcbloc.h>
#include <ecet.h>


/*!\brief Base class for all single output real-time event function blocks.
 *
 */
class CRTEventSingle: public CFunctionBlock{
private:
  CEventChainExecutionThread m_oECEO;
  CSyncObject m_oSyncObj;              //!<Lock for protected the RT_E_FB from mutual events
  //CIEC_TIME *m_poDeadline;
  bool m_bInitialized;

  virtual CIEC_TIME &Deadline() = 0;
  virtual CIEC_BOOL &QI() = 0;
  virtual CIEC_BOOL &QO() = 0;
  /*! \brief check if the succeeding EC part needs to be activated.
  *
  * This function makes the assumption that the input event with
  * the id 0 is the INIT event input.
  * \param pa_nEIID the event id of the event that triggered the FB
  * \return if true the succeeding EC part will be activated.
  */
  virtual bool checkActivation(int pa_nEIID) = 0;
    virtual void executeEvent(int pa_nEIID) {
      if(0 != pa_nEIID) { //it is not the init event
        if(m_bInitialized && checkActivation(pa_nEIID)) {
          CEventConnection *eoCon = getEOConUnchecked(1);
          if(eoCon->isConnected()) {
            eoCon->triggerEvent(&m_oECEO);
            m_oECEO.resumeSelfSuspend();
          }
        }
      } else { // we got init
        if(QI() == true) {
          if(!m_bInitialized) {
            m_oECEO.changeExecutionState(cg_nMGM_CMD_Start);
            m_bInitialized = true;
          }
          m_oECEO.setDeadline(Deadline());
        } else {
          m_oECEO.changeExecutionState(cg_nMGM_CMD_Stop);
          m_bInitialized = false;
        }
        QO() = QI();
        sendOutputEvent(0);
      }
    }

public:


  CRTEventSingle(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec,
        const CStringDictionary::TStringId pa_nInstanceNameId, TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
   CFunctionBlock( pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId, pa_acFBConnData, pa_acFBVarsData)
  {
      m_bInitialized = false;
  };
  virtual ~CRTEventSingle(){};
};

#endif /*RTESINGLE_H_*/
