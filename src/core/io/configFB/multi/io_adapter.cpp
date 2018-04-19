/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "io_adapter.h"

IOConfigFBMultiAdapter::IOConfigFBMultiAdapter(const TForteUInt8* const scm_slaveConfigurationIO,
    const TForteUInt8 scm_slaveConfigurationIO_num, CResource *pa_poSrcRes,
    const SFBInterfaceSpec *pa_pstInterfaceSpecSocket,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    const SFBInterfaceSpec *pa_pstInterfaceSpecPlug, bool pa_bIsPlug,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    CAdapter(pa_poSrcRes, pa_pstInterfaceSpecSocket, pa_nInstanceNameId,
        pa_pstInterfaceSpecPlug, pa_bIsPlug, pa_acFBConnData, pa_acFBVarsData), scm_slaveConfigurationIO(
        scm_slaveConfigurationIO), scm_slaveConfigurationIO_num(
        scm_slaveConfigurationIO_num) {
}

IOConfigFBMultiAdapter::~IOConfigFBMultiAdapter() {
  // TODO Auto-generated destructor stub
}
