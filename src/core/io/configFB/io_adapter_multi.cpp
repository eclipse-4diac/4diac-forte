/*******************************************************************************
 * Copyright (c) 2017 - 2018 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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

