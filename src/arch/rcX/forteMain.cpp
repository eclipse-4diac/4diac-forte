/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <forteMain.h>
#include <fortenew.h>

#include "../../stdfblib/ita/RMT_DEV.h"

/*!\brief Check if the correct endianess has been configured.
 *
 * If the right endianess is not set this function will end FORTE.
 */

//this keeps away a lot of rtti and exception handling stuff
//extern "C" void __cxa_pure_virtual(void){
  //TODO maybe add some reporting here
  //Although we should never get here
  //if we are here something very very bad has happened e.g., stack overflow or other memory corruption

//}
RMT_DEV *poDev = 0;

void createDev(const char *pa_acMGRID);

int startForte(){
  createDev("localhost:61499");
  return 0;
}

void endForte(int pa_nSig){
  (void) pa_nSig;
  if(0 != poDev){
    poDev->changeFBExecutionState(cg_nMGM_CMD_Kill);
  }
  DEVLOG_INFO("FORTE finished\n");
  delete poDev;
}

/*!\brief Creates the Device-Object
 * \param pa_acMGRID A string containing IP and Port like [IP]:[Port]
 */
void createDev(const char *pa_acMGRID){
  poDev = new RMT_DEV;

  poDev->setMGR_ID(pa_acMGRID);
  poDev->startDevice();
  DEVLOG_INFO("FORTE is up and running\n");
}









