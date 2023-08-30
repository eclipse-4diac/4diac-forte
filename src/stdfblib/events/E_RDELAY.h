/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _E_RDELAY_H_
#define _E_RDELAY_H_

#include "../timedfb.h"

/*! \brief Implementation of the E_RDELAY FB.
 */
class E_RDELAY : public CTimedFB{
  DECLARE_FIRMWARE_FB(E_RDELAY)
private:

  void executeEvent(TEventID paEIID, CEventChainExecutionThread * const paECET) override;
public:
  E_RDELAY(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);
  ~E_RDELAY() override = default;

};

#endif /*E_RDELAY_H_*/
