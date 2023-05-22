/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH
 *                      2018 Johannes Kepler University
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#ifndef _GEN_AND_H_
#define _GEN_AND_H_

#include "genbitbase.h"

class GEN_AND : public CGenBitBase {
  DECLARE_GENERIC_FIRMWARE_FB(GEN_AND)

  private:
    void executeEvent(TEventID paEIID) override;

    GEN_AND(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);
    ~GEN_AND() override;
};

#endif //_GEN_AND_H_

