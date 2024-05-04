/*******************************************************************************
 * Copyright (c) 2024 Samator Indo Gas
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Ketut Kumajaya - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "../core/iec61131_functions.h"

std::string getRealtimeString() {
  CIEC_DATE_AND_TIME dt = func_NOW();
  std::string str(dt.getToStringBufferSize() - 1, '\0'); // -1 for exclude the last \0
  dt.toString(str.data(), str.size());
  return str;
}
