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

#include "SUBSCRIBE_1_fbt.h"

USE_STRING_ID(SUBSCRIBE_1);

DEFINE_GENERIC_FIRMWARE_FB(FORTE_SUBSCRIBE_1, STRID(SUBSCRIBE_1))

FORTE_SUBSCRIBE_1::FORTE_SUBSCRIBE_1(const CStringDictionary::TStringId paInstanceNameId,
                                     forte::core::CFBContainer &paContainer) :
    GEN_SUBSCRIBE(paInstanceNameId, paContainer) {
  configureFB("SUBSCRIBE_1");
}
