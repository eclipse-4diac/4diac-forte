/*******************************************************************************
 * Copyright (c) 2010 - 2015 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef SOCKHAND_H_
#define SOCKHAND_H_

#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

//these include needs to be last
#include "../gensockhand.h"
#include "../fdselecthand.h"
#include "../bsdsocketinterf.h"

typedef CGenericIPComSocketHandler<CFDSelectHandler, CBSDSocketInterface> CIPComSocketHandler;

#endif /* SOCKHAND_H_ */
