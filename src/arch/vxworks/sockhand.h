/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SOCKHAND_H_
#define SOCKHAND_H_

#include <vxWorks.h>
#include <unistd.h>
#include <sockLib.h>
#include <selectLib.h>
#include <inetLib.h>

#include <errno.h>
#include <string.h>


//these include needs to be last
#include "../fdselecthand.h"
#include "../bsdsocketinterf.h"
#include "../gensockhand.h"

typedef CGenericIPComSocketHandler<CFDSelectHandler, CBSDSocketInterface> CIPComSocketHandler;

#endif /* SOCKHAND_H_ */
