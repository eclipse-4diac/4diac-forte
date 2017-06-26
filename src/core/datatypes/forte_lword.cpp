/*******************************************************************************
  * Copyright (c) 2010 - 2013 ACIN
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Martin Melik Merkiumians, Alois Zoitl, Monika Wenger
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include "forte_lword.h"

#ifdef FORTE_USE_64BIT_DATATYPES

DEFINE_FIRMWARE_DATATYPE(LWORD, g_nStringIdLWORD)

const CIEC_LWORD::TValueType CIEC_LWORD::scm_nMaxVal = std::numeric_limits<CIEC_LWORD::TValueType>::max();

#endif
