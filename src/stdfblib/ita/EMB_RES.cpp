/*******************************************************************************
 * Copyright (c) 2006 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Gerhard Ebenhofer,
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <stringdict.h>
#include "EMB_RES.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "EMB_RES_gen.cpp"
#endif


DEFINE_FIRMWARE_FB(EMB_RES, g_nStringIdEMB_RES)
;

EMB_RES::EMB_RES(CStringDictionary::TStringId pa_nInstanceNameId,
    CResource* pa_poDevice) :
  CResource(pa_poDevice, 0, pa_nInstanceNameId, 0, 0){

  addFB(CTypeLib::createFB(g_nStringIdSTART, g_nStringIdE_RESTART, this));
}

EMB_RES::~EMB_RES(){
}

