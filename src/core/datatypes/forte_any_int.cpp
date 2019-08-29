/*******************************************************************************
 * Copyright (c) 2013, 2014 ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *     Monika Wenger, Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte_any_int.h"

DEFINE_FIRMWARE_DATATYPE(ANY_INT, g_nStringIdANY_INT)

CIEC_ANY_INT::CIEC_ANY_INT(TLargestIntValueType paVal){
  setSignedValue(paVal);
}
