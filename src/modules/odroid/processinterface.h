/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _PROCESSINTERFACE_H_
#define _PROCESSINTERFACE_H_

#include "../sysfs/sysfsprocint.h"

class COdroidProcessInterface : public CSysFsProcessInterface{
  public:
    COdroidProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData);
    virtual ~COdroidProcessInterface();

  protected:
    bool initialise(bool paIsInput);
    bool deinitialise();
    bool readWord();

  private:

};

//tell the IX and QX FB that this is the process interface to be used
typedef COdroidProcessInterface CProcessInterface;

#endif
