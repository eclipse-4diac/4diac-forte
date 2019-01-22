/*******************************************************************************
 * Copyright (c) 2005 - 2011 ACIN, Profactor GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Thomas Strasser,
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _RMT_RES_H_
#define _RMT_RES_H_

#include <resource.h>

class RMT_RES : public CResource{
  DECLARE_FIRMWARE_FB(RMT_RES)

  public:
    RMT_RES(CStringDictionary::TStringId pa_nInstanceNameId, CResource* pa_roDevice);
    virtual ~RMT_RES();

    void joinResourceThread();

  private:
    static const SFBInterfaceSpec scm_stFBInterfaceSpec;

    FORTE_FB_DATA_ARRAY(0,1,0, 0);

    static const CStringDictionary::TStringId scm_aunVarInputNameIds[];
    static const CStringDictionary::TStringId scm_aunDIDataTypeIds[];

};

#endif //close the ifdef sequence from the beginning of the file
