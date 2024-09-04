/*******************************************************************************
 * Copyright (c) 2005, 2024 ACIN, Profactor GmbH, fortiss GmbH,
 *                                Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Gerhard Ebenhofer, Rene Smodic, Ingo Hegny
 *               - initial API and implementation and/or initial documentation
 *   Alois Zoitl - Copied and modified from RMT_DEV.h
 *******************************************************************************/
#pragma once

#include "device.h"
#include "if2indco.h"
#include "RMT_RES.h"

class FakeTimeDev : public CDevice{
  public:

    FakeTimeDev(const std::string &paMGR_ID = "localhost:61499");
    ~FakeTimeDev() override;

    bool initialize() override;

  /*! \brief Adds additional functionality to the originals execute func of the device.
  *
  * This is that it waits till the thread of the MGR resource has anded
  */
    int startDevice() override;

    void awaitShutdown() override;

    EMGMResponse changeExecutionState(EMGMCommandType paCommand) override;

    EMGMResponse writeValue(forte::core::TNameIdentifier &paNameList, const std::string & paValue, bool paForce = false) override;

  private:
    CInterface2InternalDataConnection mDConnMGR_ID;

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    static const CStringDictionary::TStringId scmDINameIds[];
    static const CStringDictionary::TStringId scmDIDataTypeIds[];

    CIEC_WSTRING var_MGR_ID;
    CIEC_TIME var_FakeTime;
    CDataConnection *conn_MGR_ID;
    CDataConnection *conn_FakeTime;

    CIEC_ANY* getDI(size_t) override;
    CDataConnection** getDIConUnchecked(TPortId) override;

    RMT_RES MGR;
};
