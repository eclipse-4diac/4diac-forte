/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "fbtesterglobalfixture.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "fbtesterglobalfixture_gen.cpp"
#endif
#include <boost/test/unit_test.hpp>

const static SFBInterfaceSpec gscTestDevSpec = { 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
          0,
          0 };

CDevice *CFBTestDataGlobalFixture::smTestDev;
CResource *CFBTestDataGlobalFixture::smTestRes;

CFBTestDataGlobalFixture::CFBTestDataGlobalFixture(){
  //setup is done in the setup so that boost_test can throw exceptions
  smTestDev = new CDevice(&gscTestDevSpec, CStringDictionary::scm_nInvalidStringId, 0, 0);
  //mimick the behavior provided by typelib
  smTestDev->changeFBExecutionState(cg_nMGM_CMD_Reset);

  smTestRes = (CResource *)CTypeLib::createFB(g_nStringIdEMB_RES, g_nStringIdEMB_RES, smTestDev);

  if(smTestRes != 0){
    smTestDev->addFB(smTestRes);
    smTestDev->startDevice();
  }
}


CFBTestDataGlobalFixture::~CFBTestDataGlobalFixture(){
  smTestDev->changeFBExecutionState(cg_nMGM_CMD_Stop);
  delete smTestDev;
  //we don't need to delete the res here as the res is deletes in the destructor of the device
}
