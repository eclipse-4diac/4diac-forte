/*******************************************************************************
 * Copyright (c) 2005 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Thomas Strasser,
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _RMT_RES_H_
#define _RMT_RES_H_

#include "resource.h"
#include "DEV_MGR.h"
#include "E_RESTART.h"
#include "E_SR.h"

class RMT_RES : public CResource{
  DECLARE_FIRMWARE_FB(RMT_RES)

  public:
    RMT_RES(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paDevice);
    ~RMT_RES() override;

    bool initialize() override;

    void joinResourceThread() const;

    CIEC_WSTRING var_MGR_ID;
    CDataConnection *conn_MGR_ID;

    CIEC_ANY *getDI(size_t) override;
    CDataConnection **getDIConUnchecked(TPortId) override;

  private:
    forte::core::CInternalFB<FORTE_E_RESTART> fb_START;
    forte::core::CInternalFB<FORTE_E_SR> fb_MGR_FF;
    forte::core::CInternalFB<DEV_MGR> fb_MGR;

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    static const CStringDictionary::TStringId scmVarInputNameIds[];
    static const CStringDictionary::TStringId scmDIDataTypeIds[];

};

#endif //close the ifdef sequence from the beginning of the file
