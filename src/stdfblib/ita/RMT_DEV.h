/*******************************************************************************
 * Copyright (c) 2005, 2024 ACIN, Profactor GmbH, fortiss GmbH,
 *                          Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Gerhard Ebenhofer, Rene Smodic, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _RMT_DEV_H_
#define _RMT_DEV_H_

#include "../../core/device.h"
#include "RMT_RES.h"

/*! \brief Implementation of the RMT_DEV.
 */


 class RMT_DEV : public CDevice{
  public:
    RMT_DEV(const std::string &paMGR_ID = "localhost:61499");
    ~RMT_DEV() override;

    bool initialize() override;

  /*! \brief Adds additional functionality to the originals execute func of the device.
  *
  * This is that it waits till the thread of the MGR resource has anded
  */
    int startDevice() override;

    void awaitShutdown() override;

    EMGMResponse changeExecutionState(EMGMCommandType paCommand) override;

    void setMGR_ID(const std::string& paVal);

  private:
    static const SFBInterfaceSpec scmFBInterfaceSpec;

    static const CStringDictionary::TStringId scmDINameIds[];
    static const CStringDictionary::TStringId scmDIDataTypeIds[];

    CIEC_ANY *getDI(size_t) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CConnection *getResIf2InConnectionUnchecked(TPortId ) override;

    COutDataConnection<CIEC_WSTRING> conn_MGR_ID_int;
    CDataConnection *conn_MGR_ID;

    RMT_RES MGR;
};

#endif /*RMT_DEV_H_*/
