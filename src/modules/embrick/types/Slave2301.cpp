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

#include "Slave2301.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "Slave2301_gen.cpp"
#endif

namespace EmBrick {
namespace FunctionBlocks {

DEFINE_FIRMWARE_FB(Slave2301, g_nStringIdEBSlave2301)

const CStringDictionary::TStringId Slave2301::scm_anDataInputNames[] = {
    g_nStringIdQI, g_nStringIdRelay_1, g_nStringIdRelay_2, g_nStringIdRelay_3,
    g_nStringIdRelay_4, g_nStringIdRelay_5, g_nStringIdRelay_6,
    g_nStringIdUpdateInterval };

const CStringDictionary::TStringId Slave2301::scm_anDataInputTypeIds[] =
    { g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdWSTRING,
        g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
        g_nStringIdWSTRING, g_nStringIdUINT };

const CStringDictionary::TStringId Slave2301::scm_anDataOutputNames[] = {
    g_nStringIdQO, g_nStringIdSTATUS };

const CStringDictionary::TStringId Slave2301::scm_anDataOutputTypeIds[] = {
    g_nStringIdBOOL, g_nStringIdWSTRING };

const TForteInt16 Slave2301::scm_anEIWithIndexes[] = { 0 };
const TDataIOID Slave2301::scm_anEIWith[] = { 1, 2, 3, 4, 5, 6, 0, 255 };
const CStringDictionary::TStringId Slave2301::scm_anEventInputNames[] = {
    g_nStringIdMAP };

const TDataIOID Slave2301::scm_anEOWith[] = { 0, 255, 0, 1, 255 };
const TForteInt16 Slave2301::scm_anEOWithIndexes[] = { 0, 2, -1 };
const CStringDictionary::TStringId Slave2301::scm_anEventOutputNames[] = {
    g_nStringIdMAPO, g_nStringIdIND };

const SAdapterInstanceDef Slave2301::scm_astAdapterInstances[] = { {
    g_nStringIdEBBusAdapter, g_nStringIdBusAdapterOut, true }, {
    g_nStringIdEBBusAdapter, g_nStringIdBusAdapterIn, false } };

const SFBInterfaceSpec Slave2301::scm_stFBInterfaceSpec = { 1,
    scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 2,
    scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 8,
    scm_anDataInputNames, scm_anDataInputTypeIds, 2, scm_anDataOutputNames,
    scm_anDataOutputTypeIds, 2, scm_astAdapterInstances };

void Slave2301::initHandles() {
  // 6 Relays
  addBitHandle(IOMapper::Out, Relay_1(), 0, 0);
  addBitHandle(IOMapper::Out, Relay_2(), 0, 1);
  addBitHandle(IOMapper::Out, Relay_3(), 0, 2);
  addBitHandle(IOMapper::Out, Relay_4(), 0, 3);
  addBitHandle(IOMapper::Out, Relay_5(), 0, 4);
  addBitHandle(IOMapper::Out, Relay_6(), 0, 5);
}

} /* namespace FunctionsBlocks */
} /* namespace EmBrick */

