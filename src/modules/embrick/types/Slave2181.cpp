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

#include "Slave2181.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "Slave2181_gen.cpp"
#endif

#include <devlog.h>

namespace EmBrick {
namespace FunctionBlocks {
DEFINE_FIRMWARE_FB(Slave2181, g_nStringIdEBSlave2181)

const CStringDictionary::TStringId Slave2181::scm_anDataInputNames[] = {
    g_nStringIdQI, g_nStringIdDigitalInput1, g_nStringIdDigitalInput2,
    g_nStringIdDigitalInput3, g_nStringIdDigitalInput4,
    g_nStringIdDigitalInput5, g_nStringIdDigitalInput6,
    g_nStringIdDigitalInput7, g_nStringIdDigitalInput8,
    g_nStringIdDigitalOutput1, g_nStringIdDigitalOutput2,
    g_nStringIdDigitalOutput3, g_nStringIdDigitalOutput4,
    g_nStringIdDigitalOutput5, g_nStringIdDigitalOutput6,
    g_nStringIdDigitalOutput7, g_nStringIdDigitalOutput8,
    g_nStringIdUpdateInterval };

const CStringDictionary::TStringId Slave2181::scm_anDataInputTypeIds[] = {
    g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
    g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
    g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
    g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
    g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
    g_nStringIdWSTRING, g_nStringIdUINT };

const CStringDictionary::TStringId Slave2181::scm_anDataOutputNames[] = {
    g_nStringIdQO, g_nStringIdSTATUS };

const CStringDictionary::TStringId Slave2181::scm_anDataOutputTypeIds[] = {
    g_nStringIdBOOL, g_nStringIdWSTRING };

const TForteInt16 Slave2181::scm_anEIWithIndexes[] = { 0 };
const TDataIOID Slave2181::scm_anEIWith[] = { 1, 2, 5, 3, 4, 6, 7, 8, 9, 10, 11,
    12, 13, 14, 15, 16, 0, 255 };
const CStringDictionary::TStringId Slave2181::scm_anEventInputNames[] = {
    g_nStringIdMAP };

const TDataIOID Slave2181::scm_anEOWith[] = { 0, 255, 0, 1, 255 };
const TForteInt16 Slave2181::scm_anEOWithIndexes[] = { 0, 2, -1 };
const CStringDictionary::TStringId Slave2181::scm_anEventOutputNames[] = {
    g_nStringIdMAPO, g_nStringIdIND };

const SAdapterInstanceDef Slave2181::scm_astAdapterInstances[] = { {
    g_nStringIdEBBusAdapter, g_nStringIdBusAdapterOut, true }, {
    g_nStringIdEBBusAdapter, g_nStringIdBusAdapterIn, false } };

const SFBInterfaceSpec Slave2181::scm_stFBInterfaceSpec = { 1,
    scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 2,
    scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 18,
    scm_anDataInputNames, scm_anDataInputTypeIds, 2, scm_anDataOutputNames,
    scm_anDataOutputTypeIds, 2, scm_astAdapterInstances };

void Slave2181::initHandles() {
  // 8 Inputs
  addBitHandle(IOMapper::In, DigitalInput1(), 0, 0);
  addBitHandle(IOMapper::In, DigitalInput2(), 0, 1);
  addBitHandle(IOMapper::In, DigitalInput3(), 0, 2);
  addBitHandle(IOMapper::In, DigitalInput4(), 0, 3);
  addBitHandle(IOMapper::In, DigitalInput5(), 0, 4);
  addBitHandle(IOMapper::In, DigitalInput6(), 0, 5);
  addBitHandle(IOMapper::In, DigitalInput7(), 0, 6);
  addBitHandle(IOMapper::In, DigitalInput8(), 0, 7);

  // 8 Outputs
  addBitHandle(IOMapper::Out, DigitalOutput1(), 0, 0);
  addBitHandle(IOMapper::Out, DigitalOutput2(), 0, 1);
  addBitHandle(IOMapper::Out, DigitalOutput3(), 0, 2);
  addBitHandle(IOMapper::Out, DigitalOutput4(), 0, 3);
  addBitHandle(IOMapper::Out, DigitalOutput5(), 0, 4);
  addBitHandle(IOMapper::Out, DigitalOutput6(), 0, 5);
  addBitHandle(IOMapper::Out, DigitalOutput7(), 0, 6);
  addBitHandle(IOMapper::Out, DigitalOutput8(), 0, 7);
}

} /* namespace FunctionsBlocks */
} /* namespace EmBrick */

