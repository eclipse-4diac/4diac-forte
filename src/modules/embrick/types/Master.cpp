/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "Master.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "Master_gen.cpp"
#endif

namespace EmBrick {
namespace FunctionBlocks {

DEFINE_FIRMWARE_FB(Master, g_nStringIdEBMaster)

const CStringDictionary::TStringId Master::scm_anDataInputNames[] = {
    g_nStringIdQI, g_nStringIdPARAMS };

const CStringDictionary::TStringId Master::scm_anDataInputTypeIds[] = {
    g_nStringIdBOOL, g_nStringIdWSTRING };

const CStringDictionary::TStringId Master::scm_anDataOutputNames[] = {
    g_nStringIdQO, g_nStringIdSTATUS };

const CStringDictionary::TStringId Master::scm_anDataOutputTypeIds[] = {
    g_nStringIdBOOL, g_nStringIdWSTRING };

const TForteInt16 Master::scm_anEIWithIndexes[] = { 0 };
const TDataIOID Master::scm_anEIWith[] = { 0, 1, 255 };
const CStringDictionary::TStringId Master::scm_anEventInputNames[] = {
    g_nStringIdINIT };

const TDataIOID Master::scm_anEOWith[] = { 0, 1, 255, 0, 1, 255 };
const TForteInt16 Master::scm_anEOWithIndexes[] = { 0, 3, -1 };
const CStringDictionary::TStringId Master::scm_anEventOutputNames[] = {
    g_nStringIdINITO, g_nStringIdIND };

const SAdapterInstanceDef Master::scm_astAdapterInstances[] = { {
    g_nStringIdEBBusAdapter, g_nStringIdBusAdapterOut, true } };

const SFBInterfaceSpec Master::scm_stFBInterfaceSpec = { 1,
    scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 2,
    scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 2,
    scm_anDataInputNames, scm_anDataInputTypeIds, 2, scm_anDataOutputNames,
    scm_anDataOutputTypeIds, 1, scm_astAdapterInstances };

void Master::executeEvent(int pa_nEIID) {
  switch (pa_nEIID) {
  case scm_nEventINITID:
    // Init and wait for BusHandler
    bus = &BusHandler::getInstance();
    bus->waitForInit();

    // Init configuration
    BusAdapterOut().INDEX() = 0;
    sendAdapterEvent(scm_nBusAdapterAdpNum, BusAdapter::scm_nEventINITID);

    // TODO Wait for end of configuration
    sendOutputEvent(scm_nEventINITOID);
    break;
  }
}

} /* namespace FunctionsBlocks */
} /* namespace EmBrick */
