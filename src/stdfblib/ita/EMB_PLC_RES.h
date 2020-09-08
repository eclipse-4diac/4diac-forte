/*******************************************************************************
 * Copyright (c) 2006 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Peter Gsellmann, Martin Melik Merkumians - initial implementation
 *******************************************************************************/
#ifndef _EMB_PLC_RES_H_
#define _EMB_PLC_RES_H_

#include <resource.h>

class EMB_PLC_RES : public CResource {
  DECLARE_FIRMWARE_FB(EMB_PLC_RES)
    ;

  public:
    EMB_PLC_RES(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poDevice);
    virtual ~EMB_PLC_RES();

    EMGMResponse changeFBExecutionState(EMGMCommandType pa_unCommand);

    CIEC_TIME& CYCLE_TIME() {
      return *static_cast<CIEC_TIME*>(getDI(0));
    }

  protected:
    virtual void postFBcreateAction(CFunctionBlock &paNewFB);

  private:FORTE_FB_DATA_ARRAY(0, 1, 0, 0)
    ;
    static const CStringDictionary::TStringId scm_aunDINameIds[];
    static const CStringDictionary::TStringId scm_aunDIDataTypeIds[];
    static const SFBInterfaceSpec scm_stFBInterfaceSpec;
};

#endif //close the ifdef sequence from the beginning of the file

