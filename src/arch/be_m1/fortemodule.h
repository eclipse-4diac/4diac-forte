/*******************************************************************************
 * Copyright (c) 2012 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef __FORTE_MODULE_H__
#define __FORTE_MODULE_H__

#include <BETypes.h>
#include <mtypes.h>
#include <BEModule.hpp>


class BE_RMT_DEV;

class CFORTEModule : public BEModule{
  public:
    CFORTEModule();
    virtual ~CFORTEModule();

    static const UINT32 MINVERS = 2;
    static const UINT32 MAXVERS = 2;

    virtual VOID AppPanicHandler(UINT32 PanicMode);
    VOID CfgRead(VOID);

  private:
    virtual SINT32 AppEarlyInit(VOID);
    virtual SINT32 AppInit(VOID);
    virtual SINT32 AppDeInit(VOID);
    virtual SINT32 AppLateDeInit(VOID);
    virtual SINT32 AppEOI(VOID);
    virtual SINT32 AppStop(VOID);
    virtual SINT32 AppRun(VOID);
    virtual VOID RpcNewCfg(SMI_MSG *pMsg);

    BE_RMT_DEV *m_poDev;

    UINT32 m_unFORTEstartOK;
};


#endif  // __FORTE_MODULE_H__
