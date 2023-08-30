/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
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

#include <device.h>
#include <if2indco.h>
#include "RMT_RES.h"

/*! \brief Implementation of the RMT_DEV.
 */


class RMT_DEV : public CDevice{
  public:
    RMT_RES MGR;

    RMT_DEV();
    ~RMT_DEV() override;

    bool initialize() override;

  /*! \brief Adds additional functionality to the originals execute func of the device.
  *
  * This is that it waits till the thread of the MGR resource has anded
  */
    int startDevice() override;

    EMGMResponse changeFBExecutionState(EMGMCommandType paCommand) override;

    void setMGR_ID(const char * const paConn);

  private:
    CInterface2InternalDataConnection mDConnMGR_ID;

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    static const CStringDictionary::TStringId scmDINameIds[];
    static const CStringDictionary::TStringId scmDIDataTypeIds[];

    CIEC_WSTRING& MGR_ID() {
      return *static_cast<CIEC_WSTRING*>(getDI(0));
    }
};

#endif /*RMT_DEV_H_*/
