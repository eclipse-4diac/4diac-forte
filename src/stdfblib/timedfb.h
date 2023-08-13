/*******************************************************************************
 * Copyright (c) 2005, 2023 ACIN, Profactor GmbH, fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Gunnar Grabmair, Martin Melik Merkumians,
 *   Ingo Hegny, Gerhard Ebenhofer
 *    - initial API and implementation and/or initial documentation
 *   Martin Jobst - add generic readInputData and writeOutputData
 *******************************************************************************/
#ifndef _TIMEDFB_H_
#define _TIMEDFB_H_

#include "../core/esfb.h"
#include "../arch/timerha.h"
#include "../core/resource.h"

/*!\brief Base class for timed function block like E_CYCLE or E_DELAY providing this interface
 */
class CTimedFB : public CEventSourceFB{
private:
protected:
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;
  static const CStringDictionary::TStringId scm_aunEINameIds[];
  static const CStringDictionary::TStringId scm_aunEONameIds[];
  static const CStringDictionary::TStringId scm_aunDINameIds[];
  static const CStringDictionary::TStringId scm_aunDIDataTypeNameIds[];

  static const TEventID csm_nEventSTARTID = 0;
  static const TEventID csm_nEventSTOPID = 1;

  static const TEventID csm_nEOID = 0;


  bool mActive; //!> flag to indicate that the timed fb is currently active
  STimedFBListEntry mTimeListEntry; //!> The Timer list entry of this timed FB
/*!\brief execute the input events of timed FBs as far it is possible
 *
 * Derived Timed FBs only normally need only the start event as this is different for each timed FB type (e.g. periodic vs. onetimeshot)
 */
  void executeEvent(TEventID paEIID, CEventChainExecutionThread * const paECET) override;

  void readInputData(TEventID paEI) override;
  void writeOutputData(TEventID paEO) override;

  CIEC_TIME& DT() {
     return *static_cast<CIEC_TIME*>(getDI(0));
  }
public:
  CTimedFB(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes, ETimerActivationType paType);
  ~CTimedFB() override = default;

  EMGMResponse changeFBExecutionState(EMGMCommandType pa_unCommand) override;

};

#endif /*TIMEDFB_H_*/
