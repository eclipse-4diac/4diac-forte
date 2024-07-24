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

std::unique_ptr<CTesterDevice> CFBTestDataGlobalFixture::smTestDev;
CResource *CFBTestDataGlobalFixture::smTestRes;

CFBTestDataGlobalFixture::CFBTestDataGlobalFixture(){
  //setup is done in the setup so that boost_test can throw exceptions
  smTestDev = std::make_unique<CTesterDevice>();
  //mimick the behavior provided by typelib
  smTestDev->changeExecutionState(EMGMCommandType::Reset);

  smTestRes = (CResource *)CTypeLib::createFB(g_nStringIdEMB_RES, g_nStringIdEMB_RES, *smTestDev);

  if(smTestRes != nullptr){
    smTestDev->addFB(smTestRes);
    smTestDev->startDevice();
  }
}


CFBTestDataGlobalFixture::~CFBTestDataGlobalFixture(){
  smTestDev->changeExecutionState(EMGMCommandType::Stop);
  smTestDev.reset();
  //we don't need to delete the res here as the res is deletes in the destructor of the device
}
