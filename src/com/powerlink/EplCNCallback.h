/*******************************************************************************
 * Copyright (c) 2012 AIT, ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _EPLCNCALLBACK_H_
#define _EPLCNCALLBACK_H_

class IEplCNCallback{
  public:

    virtual void cnSynchCallback() = 0;

  protected:
    virtual ~IEplCNCallback(){
    }
    ;

};

#endif //_EPLCNCALLBACK_H_
