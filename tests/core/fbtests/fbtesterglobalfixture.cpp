/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "fbtesterglobalfixture.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "fbtesterglobalfixture_gen.cpp"
#endif
#include <boost/test/unit_test.hpp>

const static SFBInterfaceSpec gscTestDevSpec = {
  0, nullptr, nullptr, nullptr,
  0, nullptr, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

CDevice *CFBTestDataGlobalFixture::smTestDev;
CResource *CFBTestDataGlobalFixture::smTestRes;

CFBTestDataGlobalFixture::CFBTestDataGlobalFixture(){
  //setup is done in the setup so that boost_test can throw exceptions
  smTestDev = new CDevice(&gscTestDevSpec, CStringDictionary::scmInvalidStringId);
  //mimick the behavior provided by typelib
  smTestDev->changeFBExecutionState(EMGMCommandType::Reset);

  smTestRes = (CResource *)CTypeLib::createFB(g_nStringIdEMB_RES, g_nStringIdEMB_RES, smTestDev);

  if(smTestRes != nullptr){
    smTestDev->addFB(smTestRes);
    smTestDev->startDevice();
  }
}


CFBTestDataGlobalFixture::~CFBTestDataGlobalFixture(){
  smTestDev->changeFBExecutionState(EMGMCommandType::Stop);
  delete smTestDev;
  //we don't need to delete the res here as the res is deletes in the destructor of the device
}
