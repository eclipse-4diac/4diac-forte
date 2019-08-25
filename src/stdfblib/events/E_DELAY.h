/*******************************************************************************
 * Copyright (c) 2006 - 2014 ACIN, Profactor GmbH, fortiss GmbH
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
#ifndef _E_DELAY_H_
#define _E_DELAY_H_

#include "../timedfb.h"

/*! \brief Implementation of the E_DELAY FB.
 *
 * The E_DELAY block will delay the event propagation of the ec. In the current implementation it is not
 * a busy wait. Instead it is implemented like an event source. The main difference to an event source
 * is that it will not start in an own new ecexectuionthread (or background trhead of the resource). It
 * will use the EC-Thread of the EC that sent the start command.
 */
class E_DELAY : public CTimedFB{
  DECLARE_FIRMWARE_FB(E_DELAY)
private:

  void executeEvent(int pa_nEIID);
public:
  E_DELAY(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);
  virtual ~E_DELAY(){};

};

#endif /*E_DELAY_H_*/
