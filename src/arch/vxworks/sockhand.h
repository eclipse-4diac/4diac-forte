/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SOCKHAND_H_
#define SOCKHAND_H_

#include <vxWorks.h>
#include <hostLib.h> //for gethostname in OPCUA
#include <unistd.h>
#include <sockLib.h>
#include <selectLib.h>
#include <inetLib.h>

#include <errno.h>
#include <string.h>

//needs a specific defintion here for vxWorks
#undef FD_ZERO
#define FD_ZERO(p)    memset((char *)(p), 0, sizeof(*(p)))


//these include needs to be last
#include "../gensockhand.h"
#include "../fdselecthand.h"
#include "../bsdsocketinterf.h"


typedef CGenericIPComSocketHandler<CFDSelectHandler, CBSDSocketInterface> CIPComSocketHandler;

#endif /* SOCKHAND_H_ */
