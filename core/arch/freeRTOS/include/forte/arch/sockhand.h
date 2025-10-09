/************************************************************************************
 * Copyright (c) 2016, 2023 fortiss GmbH, OFFIS e.V.
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Milan Vathoopan, Guru Chandrasekhara - initial API and implementation and/or initial documentation
 * Jörg Walter - make lwIP exchangeable with FreeRTOS-plus-TCP
 ************************************************************************************/

#pragma once

#include <errno.h>
#include <string.h>
#include "forte/util/devlog.h"

#ifdef FORTE_FREERTOS_PLUS_TCP
#include "sockhand_plustcp.h"
#else
#include "sockhand_lwip.h"
#endif

// these include needs to be last
#include "forte/arch/gensockhand.h"
#include "forte/arch/fdselecthand.h"
#include "forte/arch/bsdsocketinterf.h"

namespace forte::arch {
  typedef CGenericIPComSocketHandler<CFDSelectHandler, CBSDSocketInterface> CIPComSocketHandler;
}
