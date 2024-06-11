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

#ifndef _GEN_OR_H_
#define _GEN_OR_H_

#include "genbitbase_fbt.h"

class GEN_OR : public CGenBitBase {
  DECLARE_GENERIC_FIRMWARE_FB(GEN_OR)

  private:

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    GEN_OR(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);
    ~GEN_OR() override;
};

#endif //_GEN_OR_H_

