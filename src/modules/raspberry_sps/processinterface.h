/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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
