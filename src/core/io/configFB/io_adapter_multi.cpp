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

using namespace forte::core::IO;

IOConfigFBMultiAdapter::IOConfigFBMultiAdapter(const TForteUInt8* const paSlaveConfigurationIO,
    const TForteUInt8 paSlaveConfigurationIO_num, CResource *pa_poSrcRes,
    const SFBInterfaceSpec *pa_pstInterfaceSpecSocket,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    const SFBInterfaceSpec *pa_pstInterfaceSpecPlug, bool pa_bIsPlug,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    CAdapter(pa_poSrcRes, pa_pstInterfaceSpecSocket, pa_nInstanceNameId,
        pa_pstInterfaceSpecPlug, pa_bIsPlug, pa_acFBConnData, pa_acFBVarsData), mSlaveConfigurationIO(
        paSlaveConfigurationIO), mSlaveConfigurationIO_num(
        paSlaveConfigurationIO_num) {
}

IOConfigFBMultiAdapter::~IOConfigFBMultiAdapter() {
  // TODO Auto-generated destructor stub
}

