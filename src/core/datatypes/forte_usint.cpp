/*******************************************************************************
  * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Ingo Hegny,  Monika Wenger
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include "forte_usint.h"

DEFINE_FIRMWARE_DATATYPE(USINT, g_nStringIdUSINT)

const TForteUInt16 CIEC_USINT::scm_unMaxStringBufSize = 100;

const CIEC_USINT::TValueType CIEC_USINT::scm_nMaxVal = std::numeric_limits<CIEC_USINT::TValueType>::max();
