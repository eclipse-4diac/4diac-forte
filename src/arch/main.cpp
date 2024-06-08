/*******************************************************************************
 * Copyright (c) 2006, 2024 ACIN, Profactor GmbH, AIT, fortiss GmbH,
 *                          Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer, Thomas Strasser, Rene Smodic,
 *  Martin Melik Merkumians, Ingo Hegny, Filip Andren - initial API and implementation and/or initial documentation
 *  Tarik Terzimehic - make OPC UA server port setable from the command line
 *******************************************************************************/

#include "forte.h"
#include "devlog.h"

TForteInstance g4diacForteInstance;
/**
 * @brief This method is implemented by each architecture to call to stop forte, usually using signals.
 * It's not part of CForteArchitecture because it's only related to the main execution and not to forte itself
 * 
 * @param paEndForte Callback that will end forte
 */
void hookEndForte(void (*paEndForte)());

void endForte(){
  forteRequestStopInstance(g4diacForteInstance);
}

int main(int argc, char *argv[]){

  if(auto result = forteGlobalInitialize(argc, argv); result != FORTE_OK){
    return result;
  }

  if(auto result = forteStartInstanceGeneric(argc, argv, &g4diacForteInstance); result != FORTE_OK){
    forteGlobalDeinitialize();
    return result;
  }

  hookEndForte(endForte);  
  
  DEVLOG_INFO("FORTE is up and running\n");
  forteWaitForInstanceToStop(g4diacForteInstance);
  DEVLOG_INFO("FORTE finished\n");

  return forteGlobalDeinitialize();
}




