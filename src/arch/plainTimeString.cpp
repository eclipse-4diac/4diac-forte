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

#include "forte_architecture_time.h"
#include <sstream>

std::string getRealtimeString() {
  std::ostringstream stream;
  stream << "T#" << getNanoSecondsRealtime();
  return stream.str();
}
