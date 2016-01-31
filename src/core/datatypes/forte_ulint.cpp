/*******************************************************************************
  * Copyright (c) 2009 - 2013 ACIN
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Monika Wenger, Alois Zoitl
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include "forte_ulint.h"

#ifdef FORTE_USE_64BIT_DATATYPES

DEFINE_FIRMWARE_DATATYPE(ULINT, g_nStringIdULINT)

const TForteUInt16 CIEC_ULINT::scm_unMaxStringBufSize = 100;

const CIEC_ULINT::TValueType CIEC_ULINT::scm_nMaxVal = std::numeric_limits<CIEC_ULINT::TValueType>::max();

#endif
