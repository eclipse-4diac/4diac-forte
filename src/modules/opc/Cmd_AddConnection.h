/*******************************************************************************
 * Copyright (c) 2012 AIT
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef CMDADDCONNECTION_H_
#define CMDADDCONNECTION_H_
#include "ICmd.h"

class COpcConnectionImpl;

class CCmd_AddConnection : public ICmd{
  public:
    CCmd_AddConnection(COpcConnectionImpl *pa_pConnection);
    virtual ~CCmd_AddConnection();

    virtual void runCommand();
    virtual const char* getCommandName() const;
  private:
    COpcConnectionImpl* m_pConnection;
};

#endif // CMDADDCONNECTION_H_
