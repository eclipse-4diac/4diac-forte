/*******************************************************************************
 * Copyright (c) 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Jobst - initial implementation
 *******************************************************************************/

#include "PUBLISH_1.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "PUBLISH_1_gen.cpp"
#endif

DEFINE_GENERIC_FIRMWARE_FB(FORTE_PUBLISH_1, g_nStringIdPUBLISH_1)

FORTE_PUBLISH_1::FORTE_PUBLISH_1(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
        GEN_PUBLISH(paInstanceNameId, paSrcRes) {
  configureFB("PUBLISH_1");
}

