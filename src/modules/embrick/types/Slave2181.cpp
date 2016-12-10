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
    g_nStringIdDigitalOutput7, g_nStringIdDigitalOutput8 };

const CStringDictionary::TStringId Slave2181::scm_anDataInputTypeIds[] = {
    g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
    g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
    g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
    g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
    g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING,
    g_nStringIdWSTRING };

const CStringDictionary::TStringId Slave2181::scm_anDataOutputNames[] = {
    g_nStringIdQO, g_nStringIdSTATUS };

const CStringDictionary::TStringId Slave2181::scm_anDataOutputTypeIds[] = {
    g_nStringIdBOOL, g_nStringIdWSTRING };

const TForteInt16 Slave2181::scm_anEIWithIndexes[] = { 0, 2 };
const TDataIOID Slave2181::scm_anEIWith[] = { 0, 255, 1, 2, 5, 3, 4, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 255 };
const CStringDictionary::TStringId Slave2181::scm_anEventInputNames[] = {
    g_nStringIdREQ, g_nStringIdMAP };

const TDataIOID Slave2181::scm_anEOWith[] = { 0, 1, 255 };
const TForteInt16 Slave2181::scm_anEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId Slave2181::scm_anEventOutputNames[] = {
    g_nStringIdIND };

const SAdapterInstanceDef Slave2181::scm_astAdapterInstances[] = { {
    g_nStringIdEBBusAdapter, g_nStringIdBusAdapterOut, true }, {
    g_nStringIdEBBusAdapter, g_nStringIdBusAdapterIn, false } };

const SFBInterfaceSpec Slave2181::scm_stFBInterfaceSpec = { 2,
    scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 1,
    scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 17,
    scm_anDataInputNames, scm_anDataInputTypeIds, 2, scm_anDataOutputNames,
    scm_anDataOutputTypeIds, 2, scm_astAdapterInstances };

void Slave2181::executeEvent(int pa_nEIID) {
  Slave::executeEvent(pa_nEIID);

  switch (pa_nEIID) {
  case scm_nEventREQID:
    // TODO add code for REQ event!
    /*
     do not forget to send output event, calling e.g.
     sendOutputEvent(scm_nEventCNFID);
     */
    break;
  case scm_nEventMAPID:
    // TODO add code for MAP event!
    /*
     do not forget to send output event, calling e.g.
     sendOutputEvent(scm_nEventCNFID);
     */
    break;
  }
}

bool Slave2181::init(int index) {
  if (!Slave::init(index))
    return false;

  // 8 Inputs
  addInputBitHandle(DigitalInput1(), 0, 0);
  addInputBitHandle(DigitalInput2(), 0, 1);
  addInputBitHandle(DigitalInput3(), 0, 2);
  addInputBitHandle(DigitalInput4(), 0, 3);
  addInputBitHandle(DigitalInput5(), 0, 4);
  addInputBitHandle(DigitalInput6(), 0, 5);
  addInputBitHandle(DigitalInput7(), 0, 6);
  addInputBitHandle(DigitalInput8(), 0, 7);

  // 8 Outputs
  addOutputBitHandle(DigitalOutput1(), 0, 0);
  addOutputBitHandle(DigitalOutput2(), 0, 1);
  addOutputBitHandle(DigitalOutput3(), 0, 2);
  addOutputBitHandle(DigitalOutput4(), 0, 3);
  addOutputBitHandle(DigitalOutput5(), 0, 4);
  addOutputBitHandle(DigitalOutput6(), 0, 5);
  addOutputBitHandle(DigitalOutput7(), 0, 6);
  addOutputBitHandle(DigitalOutput8(), 0, 7);

  return true;
}

} /* namespace FunctionsBlocks */
} /* namespace EmBrick */

