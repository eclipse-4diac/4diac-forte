/*******************************************************************************
 * Copyright (c) 2012, 2013 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "POWERLINK_MN.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "POWERLINK_MN_gen.cpp"
#endif

#include "EplWrapper.h"
#include <cstdio>

DEFINE_FIRMWARE_FB(FORTE_POWERLINK_MN, g_nStringIdPOWERLINK_MN)

const CStringDictionary::TStringId FORTE_POWERLINK_MN::scmDataInputNames[] = { g_nStringIdQI, g_nStringIdCDC_CFG, g_nStringIdAPP_CFG, g_nStringIdDEV_NAME };

const CStringDictionary::TStringId FORTE_POWERLINK_MN::scmDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING };

const CStringDictionary::TStringId FORTE_POWERLINK_MN::scmDataOutputNames[] = { g_nStringIdQO, g_nStringIdSTATUS };

const CStringDictionary::TStringId FORTE_POWERLINK_MN::scmDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdSTRING };

const TForteInt16 FORTE_POWERLINK_MN::scmEIWithIndexes[] = { 0 };
const TDataIOID FORTE_POWERLINK_MN::scmEIWith[] = { 0, 1, 2, 3, scmWithListDelimiter };
const CStringDictionary::TStringId FORTE_POWERLINK_MN::scmEventInputNames[] = { g_nStringIdINIT };

const TDataIOID FORTE_POWERLINK_MN::scmEOWith[] = { 0, 1, scmWithListDelimiter };
const TForteInt16 FORTE_POWERLINK_MN::scmEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId FORTE_POWERLINK_MN::scmEventOutputNames[] = { g_nStringIdINITO };

const SFBInterfaceSpec FORTE_POWERLINK_MN::scmFBInterfaceSpec = { 1, scmEventInputNames, scmEIWith, scmEIWithIndexes, 1, scmEventOutputNames, scmEOWith, scmEOWithIndexes, 4, scmDataInputNames, scmDataInputTypeIds, 2, scmDataOutputNames, scmDataOutputTypeIds, 0, 0 };

FORTE_POWERLINK_MN::~FORTE_POWERLINK_MN(){
  shutdownStack();
}

void FORTE_POWERLINK_MN::executeEvent(TEventID paEIID){
  switch (paEIID){
    case scmEventINITID:

      if(QI() == true){
        QO() = QI();

        CEplStackWrapper* eplStack = &CEplStackWrapper::getInstance();

        eplStack->waitUntilOperational(false);
        eplStack->eplStackInit(APP_CFG().getValue(), CDC_CFG().getValue(), DEV_NAME().getValue());

        //TODO check if error occured during initialization

      }
      else if(QI() == false){
        QO() = QI();
        shutdownStack();
      }

      sendOutputEvent(scmEventINITOID);
      break;
  }
}

void FORTE_POWERLINK_MN::shutdownStack(){
  CEplStackWrapper::getInstance().eplStackShutdown();
}

