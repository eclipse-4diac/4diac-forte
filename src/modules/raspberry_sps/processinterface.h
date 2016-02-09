/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Gerd Kainz, Alois Zoitl - initial API and implementation and/or initial documentation
 *    Alois Zoitl - made better use of generic i2c process interface implementation
 *                  changed from i2c-dev to simple read writes
 *******************************************************************************/

#ifndef _PROCESSINTERFACE_H_
#define _PROCESSINTERFACE_H_

#include "../i2c_dev/i2cprocessinterface.h"
#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_string.h>

class CProcessInterface : public CI2CProcessInterface{

  public:
    CProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
        const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData,
        TForteByte *paFBVarsData);
    virtual ~CProcessInterface();

  protected:
    bool writePin();
    bool readPin();
    bool readWord();
};

#endif /* _PROCESSINTERFACE_H_ */
