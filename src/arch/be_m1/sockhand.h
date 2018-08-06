/*******************************************************************************
 * Copyright (c) 2012, 2015 ACIN and fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef SOCKHAND_H_
#define SOCKHAND_H_


#include <sockLib.h>
#include <selectLib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//these include needs to be last
#include "../gensockhand.h"
#include "../fdselecthand.h"
#include "../bsdsocketinterf.h"


typedef CGenericIPComSocketHandler<CFDSelectHandler, CBSDSocketInterface> CIPComSocketHandler;

#endif /* SOCKHAND_H_ */
