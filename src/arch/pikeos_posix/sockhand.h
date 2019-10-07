/*******************************************************************************
 * Copyright (c) 2010 - 2015, 2018 ACIN, fortiss GmbH, SYSGO AG
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *  Agostino Mascitti - PikeOS adaption
 *******************************************************************************/
#ifndef PIKEOS_SOCKHAND_H_
#define PIKEOS_SOCKHAND_H_

#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <lwip/inet.h>

#include <errno.h>
#include <string.h>

//these include needs to be last
#include "../gensockhand.h"
#include "../fdselecthand.h"
#include "../bsdsocketinterf.h"


typedef CGenericIPComSocketHandler<CFDSelectHandler, CBSDSocketInterface> CIPComSocketHandler;

#endif /* PIKEOS_SOCKHAND_H_ */
