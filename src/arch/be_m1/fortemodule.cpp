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
#include <vxWorks.h>
#include <sysLib.h>
#include <inetLib.h>
#include <BeTypes.h>
#include <mtypes.h>

#include "fortemodule.h"

#include <fortenew.h>
#include "BE_RMT_DEV.h"
#include <forteinit.h>

#define MIO_VERS_LEN                       24
#define FORTE_DELAY         25            // Access cycle time in ms
#define FORTE_C_PARMS       "OwnParms"        // Group in mconfig.ini
#define FORTE_C_DELAY       "DelayTime"       // Keyword in mconfig.ini

MLOCAL CHAR FORTE_Version[MIO_VERS_LEN] =
#include "forte.ver"
;

extern UINT32 FORTE_MemPart;

//--- Function definitions
MLOCAL VOID ExcHandler(SINT32 Signal);
MLOCAL VOID PanicHandler(UINT32 PanicMode);

//--- Forward declaration
class CFORTEModule;

//--- globals
CFORTEModule* g_poMainModule;

//--- SW-Module memory partition, needed for new operator
UINT32 FORTE_MemPart = 0;

/*--------------------------------------------------------------------------
 * Desc:         This function is the entry point to the module.
 *               After module loading this function will be called as part
 *               of the module handler.
 *               This function has to initialize the interfaces to the
 *               environment (SMI, SVI) so that they are ready to operate.
 *
 *
 * InP:  pConf   Parameter for configuration (data from MCONFIG.INI).
 * InP:  pLoad   Parameter from module loading.
 * OutP:
 *
 * Ret:  >0      Task-Id, module initialized correctly.
 *       <0      Error during initialization.
 *-------------------------------------------------------------------------*/
extern "C" SINT32 forte_Init(MOD_CONF *pConf, MOD_LOAD *pLoad){
  //--- store memory partition, needed for new operator
  FORTE_MemPart = pConf->MemPart;

  //--- Create new application class.
  g_poMainModule = new CFORTEModule();

  //--- Close application if an error is occurred.
  if(g_poMainModule == NULL
    )
    return ERROR;

  //--- Initialize software module.
  SINT32 TaskId = g_poMainModule->Init(pConf, pLoad, CFORTEModule::MINVERS, CFORTEModule::MAXVERS, 10000, FORTE_Version, "FORTE", (VOIDFUNCPTR) PanicHandler, (VOIDFUNCPTR) ExcHandler);

  //--- Close application if an error is occurred.
  if(TaskId == ERROR)
    SAFE_DELETE(g_poMainModule);

  return TaskId;
}

/*--------------------------------------------------------------------------
 * Desc: Handler for CPU exception signal if not in debug mode.
 *
 * InP:  Signal  Signal number (M_SIGEXCEPT)
 * OutP:
 *
 * Ret:
 *-------------------------------------------------------------------------*/
MLOCAL VOID ExcHandler(SINT32 Signal){
  DEVLOG_INFO("%s: Signal %d received by task %x.", g_poMainModule->GetAppName(), Signal, taskIdSelf());

  //--- Deinit application task and resources.
  g_poMainModule->AppExcHandler(Signal);
}

/*--------------------------------------------------------------------------
 * Desc: Handler for panic situation.
 *
 * InP:  PanicMode   Type of panic situation (SYS_APPPANIC, ...)
 * OutP:
 *
 * Ret:
 *-------------------------------------------------------------------------*/
MLOCAL VOID PanicHandler(UINT32 PanicMode){
  // Bring critical parts to a predefined state. For example save
  // data to NV-RAM or close open files.
  //
  // ATTENTION:   Leave this function as fast as possible in order to
  //              give all other modules also the chance to work off
  //              there panic handler!

  g_poMainModule->AppPanicHandler(PanicMode);
}

/*--------------------------------------------------------------------------
 * Desc: Constructor of application class.
 *
 * InP:
 * OutP:
 *
 * Ret:
 *-------------------------------------------------------------------------*/
CFORTEModule::CFORTEModule() :
    m_poDev(NULL){
  forteInit::initForte();
}

/*--------------------------------------------------------------------------
 * Desc: Destructor of application class.
 *
 * InP:
 * OutP:
 *
 * Ret:
 *-------------------------------------------------------------------------*/
CFORTEModule::~CFORTEModule(){
  if(GetDebugLevel() & APP_DBG_INFO1)
    DEVLOG_INFO("FORTE_mod: Destruct done.");
}

/*--------------------------------------------------------------------------
 * Desc:     This virtual function will be called as a part of the
 forte_Init function after loading of the software module.
 *
 * InP:
 * OutP:
 *
 * Ret:  0   Everything is Ok
 *      <0   Error occurred
 *-------------------------------------------------------------------------*/
SINT32 CFORTEModule::AppEarlyInit(VOID){
  return (0);
}

/*--------------------------------------------------------------------------
 * Desc:     This virtual function will be called after loading of the
 *           software module.
 *
 * InP:
 * OutP:
 *
 * Ret:  0   Everything is Ok
 *      <0   Error occurred
 *-------------------------------------------------------------------------*/
SINT32 CFORTEModule::AppInit(VOID){
  m_unFORTEstartOK = 1;
  //--- Add global variables for values that have to be exported via SVI.
  if(GetSVIHandler().AddGlobVar("FORTEstart", SVI_F_INOUT | SVI_F_UINT32, 4, &(m_unFORTEstartOK), 0, 0) != OK){
    DEVLOG_ERROR("FORTE_mod::APPINIT: Can't add SVI variable 'FORTEstart'!");
  }

  DEVLOG_INFO("appInit of forte\n");
  if(0 == m_poDev){
    m_poDev = new BE_RMT_DEV(*this);
    DEVLOG_INFO("appInit after new BE_RMT_DEV");
  }

  //--- Read configuration file
  CfgRead();
  return (0);
}

/*--------------------------------------------------------------------------
 * Desc:     This virtual function will be called bevor the application will
 *           be removed.
 *
 * InP:
 * OutP:
 *
 * Ret:  0   Everything ok
 *      <0   Error occurred
 *-------------------------------------------------------------------------*/
SINT32 CFORTEModule::AppDeInit(VOID){
  //--- Delete walking light class
  SAFE_DELETE(m_poDev);
  return (0);
}

/*--------------------------------------------------------------------------
 * Desc:     This virtual function will be called bevor the application will
 *           be removed.
 *
 * InP:
 * OutP:
 *
 * Ret:  0   Everything ok
 *      <0   Error occurred
 *-------------------------------------------------------------------------*/
SINT32 CFORTEModule::AppLateDeInit(VOID){
  return (0);
}

/*--------------------------------------------------------------------------
 * Desc:     This virtual function will be called when the 'End of Init'
 *           signal will be sent by the system.
 *
 * InP:
 * OutP:
 *
 * Ret:  0   Everything ok
 *      <0   Error occurred
 *-------------------------------------------------------------------------*/
SINT32 CFORTEModule::AppEOI(VOID){
  if(0 != m_poDev){
    m_poDev->startDevice();
    DEVLOG_INFO("m_poDev started\n");
  }
  else{
    DEVLOG_ERROR("no m_poDev preset\n");
  }

  return (0);
}

/*--------------------------------------------------------------------------
 * Desc:     This is a dummy method which will be called when the signal
 *           'stop' is received from the system.
 *
 * InP:
 * OutP:
 * Throws:
 * Ret:      OK      Everything ok
 *           ERROR   An error occurred
 *-------------------------------------------------------------------------*/
SINT32 CFORTEModule::AppStop(VOID){
  if(0 != m_poDev){
    m_poDev->changeFBExecutionState(cg_nMGM_CMD_Stop);
  }

  return OK;
}

/*--------------------------------------------------------------------------
 * Desc:     This is a dummy method which will be called when the signal
 *           'run' is received from the system.
 *
 * InP:
 * OutP:
 * Throws:
 * Ret:      OK      Everything ok
 *           ERROR   An error occurred
 *-------------------------------------------------------------------------*/
SINT32 CFORTEModule::AppRun(VOID){

  return OK;
}

/*--------------------------------------------------------------------------
 * Desc:     This virtual function will be called if a panic situation
 *           appears in the system.
 *
 * InP:
 * OutP:
 *
 * Ret:  0   Everything ok
 *      <0   Error occurred
 *-------------------------------------------------------------------------*/
VOID CFORTEModule::AppPanicHandler(UINT32 PanicMode){
  sys_Printf("%s AppPanicHandler", GetAppName());
}

/*--------------------------------------------------------------------------
 * Desc:     This method handles the RPC call 'RpcNewCfg'.
 *
 * InP:      pMsg    Message data from RPC call.
 * OutP:
 * Throws:
 * Ret:
 *-------------------------------------------------------------------------*/
VOID CFORTEModule::RpcNewCfg(SMI_MSG *pMsg){
  SMI_NEWCFG_R Reply;

  if(GetModState() == RES_S_STOP || GetModState() == RES_S_RUN){
    //--- Read configuration file
    CfgRead();

    Reply.RetCode = SMI_E_OK;
  }
  else
    Reply.RetCode = SMI_E_FAILED;

  smi_FreeData(pMsg);
  smi_SendCReply(GetSmiID(), pMsg, SMI_E_OK, &Reply, sizeof(Reply));
}

/*---------------------------------------------------------------------------
 * Desc: This function reads the configuration from the given
 *       configuration file.
 *
 * InP:
 * OutP:
 * Ret:
 *--------------------------------------------------------------------------*/
VOID CFORTEModule::CfgRead(VOID){
  //--- Get the delay time for my application.
  pf_GetInt(GetAppName(), FORTE_C_PARMS, FORTE_C_DELAY, FORTE_DELAY, (SINT32 *) &m_unFORTEstartOK, GetCfgLine(), GetProfileName());
}

