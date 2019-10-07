/*******************************************************************************
 * Copyright (c) 2017 - 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "io_adapter_multi.h"

using namespace forte::core::io;

IOConfigFBMultiAdapter::IOConfigFBMultiAdapter(const TForteUInt8* const paSlaveConfigurationIO, const TForteUInt8 paSlaveConfigurationIONum,
    CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpecSocket, const CStringDictionary::TStringId paInstanceNameId,
    const SFBInterfaceSpec *paInterfaceSpecPlug, bool paIsPlug, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CAdapter(paSrcRes, paInterfaceSpecSocket, paInstanceNameId, paInterfaceSpecPlug, paIsPlug, paFBConnData, paFBVarsData),
        cmSlaveConfigurationIO(paSlaveConfigurationIO), mSlaveConfigurationIONum(paSlaveConfigurationIONum) {
}

IOConfigFBMultiAdapter::~IOConfigFBMultiAdapter() {
}

