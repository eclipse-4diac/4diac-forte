/*******************************************************************************
 * Copyright (c) 2006 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _EMB_RES_H_
#define _EMB_RES_H_

#include "resource.h"
#include "E_RESTART_fbt.h"

class EMB_RES : public CResource{
  DECLARE_FIRMWARE_FB(EMB_RES);

  public:
    EMB_RES(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paDevice);
    ~EMB_RES() override;

    CIEC_ANY *getDI(size_t) override;
    CDataConnection **getDIConUnchecked(TPortId) override;

  private:
    forte::core::CInternalFB<FORTE_E_RESTART> fb_START;
    static const SFBInterfaceSpec scmFBInterfaceSpec;
};

#endif //close the ifdef sequence from the beginning of the file

