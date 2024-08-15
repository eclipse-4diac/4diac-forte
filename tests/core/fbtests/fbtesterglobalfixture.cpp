/*******************************************************************************
 * Copyright (c) 2018, 2024 fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *   Martin Jobst - add smart pointer for internal FBs
 *******************************************************************************/

#include "fbtesterglobalfixture.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "fbtesterglobalfixture_gen.cpp"
#endif
#include <boost/test/unit_test.hpp>

std::unique_ptr<CTesterDevice> CFBTestDataGlobalFixture::smTestDev;

CTesterDevice::CTesterDevice(const CStringDictionary::TStringId paInstanceNameId) :
        CDevice(&scTestDevSpec, paInstanceNameId), mResource(g_nStringIdEMB_RES, *this) {
}

CFBTestDataGlobalFixture::CFBTestDataGlobalFixture(){
  //setup is done in the setup so that boost_test can throw exceptions
  smTestDev = std::make_unique<CTesterDevice>();
  //mimick the behavior provided by typelib
  smTestDev->initialize();
  smTestDev->changeExecutionState(EMGMCommandType::Reset);
  smTestDev->startDevice();
}


CFBTestDataGlobalFixture::~CFBTestDataGlobalFixture(){
  smTestDev->changeExecutionState(EMGMCommandType::Stop);
  smTestDev.reset();
  //we don't need to delete the res here as the res is deletes in the destructor of the device
}
