/*******************************************************************************
  * Copyright (c) 2009 - 2013 ACIN
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Monika Wenger, Alois Zoitl,
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include "forte_lint.h"

#ifdef FORTE_USE_64BIT_DATATYPES

DEFINE_FIRMWARE_DATATYPE(LINT, g_nStringIdLINT);

const CIEC_LINT::TValueType CIEC_LINT::scm_nMinVal = std::numeric_limits<CIEC_LINT::TValueType>::min();
const CIEC_LINT::TValueType CIEC_LINT::scm_nMaxVal = std::numeric_limits<CIEC_LINT::TValueType>::max();

#endif
