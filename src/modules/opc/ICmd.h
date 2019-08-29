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
#ifndef ICMD_H_
#define ICMD_H_

class ICmd{
  public:
    ICmd(){
    }
    ;
    virtual ~ICmd(){
    }
    ;

    virtual void runCommand() = 0;
    virtual const char* getCommandName() const = 0;
};

#endif //ICMD_H_
