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
#ifndef CMDSETPROCESSVARVALUE_H_
#define CMDSETPROCESSVARVALUE_H_

#include "ICmd.h"

#include <Windows.h>
#include "Variant.h"

class COpcProcessVar;
class COpcConnectionImpl;

class CCmd_SetProcessVarValue : public ICmd{
  public:
    explicit CCmd_SetProcessVarValue(COpcProcessVar * pa_pOpcVar);
    virtual ~CCmd_SetProcessVarValue(void);

    virtual void runCommand();
    virtual const char* getCommandName() const;

  private:
    COpcProcessVar* m_pOpcVar;
};

#endif //CMDSETPROCESSVARVALUE_H_
