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

#include "BusAdapter.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "BusAdapter_gen.cpp"
#endif

namespace EmBrick {
namespace FunctionBlocks {

DEFINE_ADAPTER_TYPE(BusAdapter, g_nStringIdEBBusAdapter)

const CStringDictionary::TStringId BusAdapter::scm_anDataInputNames[] = {
    g_nStringIdQO };

const CStringDictionary::TStringId BusAdapter::scm_anDataInputTypeIds[] = {
    g_nStringIdBOOL };

const CStringDictionary::TStringId BusAdapter::scm_anDataOutputNames[] = {
    g_nStringIdINDEX, g_nStringIdUpdateInterval };

const CStringDictionary::TStringId BusAdapter::scm_anDataOutputTypeIds[] = {
    g_nStringIdINT, g_nStringIdUINT };

const TDataIOID BusAdapter::scm_anEIWith[] = { 0, 255 };
const TForteInt16 BusAdapter::scm_anEIWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId BusAdapter::scm_anEventInputNames[] = {
    g_nStringIdINITO };

const TDataIOID BusAdapter::scm_anEOWith[] = { 0, 1, 255 };
const TForteInt16 BusAdapter::scm_anEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId BusAdapter::scm_anEventOutputNames[] = {
    g_nStringIdINIT };

const SFBInterfaceSpec BusAdapter::scm_stFBInterfaceSpecSocket = { 1,
    scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 1,
    scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 1,
    scm_anDataInputNames, scm_anDataInputTypeIds, 2, scm_anDataOutputNames,
    scm_anDataOutputTypeIds, 0, 0 };

const SFBInterfaceSpec BusAdapter::scm_stFBInterfaceSpecPlug = { 1,
    scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 1,
    scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 2,
    scm_anDataOutputNames, scm_anDataOutputTypeIds, 1, scm_anDataInputNames,
    scm_anDataInputTypeIds, 0, 0 };

} /* namespace FunctionsBlocks */
} /* namespace EmBrick */
