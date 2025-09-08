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

#include "forte/iec61499/net/SUBSCRIBE_1_fbt.h"

using namespace forte::core::literals;

using namespace forte::iec61499::net;

DEFINE_GENERIC_FIRMWARE_FB(FORTE_SUBSCRIBE_1, "SUBSCRIBE_1"_STRID)

FORTE_SUBSCRIBE_1::FORTE_SUBSCRIBE_1(const forte::core::StringId paInstanceNameId,
                                     forte::core::CFBContainer &paContainer) :
    GEN_SUBSCRIBE(paInstanceNameId, paContainer) {
  configureFB("SUBSCRIBE_1");
}
