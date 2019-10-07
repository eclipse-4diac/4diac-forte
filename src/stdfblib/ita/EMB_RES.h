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

#include <resource.h>


class EMB_RES : public CResource{
  DECLARE_FIRMWARE_FB(EMB_RES);

  public:
    EMB_RES(CStringDictionary::TStringId pa_nInstanceNameId,
        CResource* pa_poDevice);
    virtual ~EMB_RES();

  private:
    static const SFBInterfaceSpec scm_stFBInterfaceSpec;
};

#endif //close the ifdef sequence from the beginning of the file

