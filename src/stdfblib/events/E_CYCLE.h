/*******************************************************************************
 * Copyright (c) 2005 - 2014 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _E_CYCLE_H_
#define _E_CYCLE_H_

#include "../timedfb.h"

/*! \brief Implementation of the E_CYCLE FB.
 */
class E_CYCLE : public CTimedFB{
  DECLARE_FIRMWARE_FB(E_CYCLE)
private:
public:
  E_CYCLE(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
      CTimedFB( paInstanceNameId, paSrcRes, e_Periodic){
  }

  virtual ~E_CYCLE() {}

};

#endif /*E_CYCLE_H_*/
