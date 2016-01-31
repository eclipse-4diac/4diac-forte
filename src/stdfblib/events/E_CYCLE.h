/*******************************************************************************
 * Copyright (c) 2005 - 2014 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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
	E_CYCLE(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : CTimedFB( pa_nInstanceNameId, pa_poSrcRes){
    m_stTimeListEntry.m_eType = e_Periodic;
  };
	virtual ~E_CYCLE() {};
};

#endif /*E_CYCLE_H_*/
