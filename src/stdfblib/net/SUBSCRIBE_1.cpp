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

#include "SUBSCRIBE_1.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "SUBSCRIBE_1_gen.cpp"
#endif

DEFINE_GENERIC_FIRMWARE_FB(FORTE_SUBSCRIBE_1, g_nStringIdSUBSCRIBE_1)

FORTE_SUBSCRIBE_1::FORTE_SUBSCRIBE_1(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
        GEN_SUBSCRIBE(paInstanceNameId, paSrcRes) {
  configureFB("SUBSCRIBE_1");
}

