/*******************************************************************************
 * Copyright (c) 2012 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef CMDADDPROCESSVAR_H_
#define CMDADDPROCESSVAR_H_
#include "ICmd.h"

class COpcConnectionImpl;
class COpcProcessVar;

class CCmd_AddOPCProcessVar : public ICmd{
  public:
    CCmd_AddOPCProcessVar(COpcConnectionImpl *pa_pConnection, COpcProcessVar* pa_pNewItem);
    virtual ~CCmd_AddOPCProcessVar(void);

    virtual void runCommand();
    virtual const char* getCommandName() const;

  private:
    COpcConnectionImpl *m_pOpcConn;
    COpcProcessVar* m_pNewProcessVar;
};

#endif //CMDADDPROCESSVAR_H_
