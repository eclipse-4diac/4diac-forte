/*******************************************************************************
 * Copyright (c) 2024, 2025 Samator Indo Gas, Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Ketut Kumajaya - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte/iec61131_functions.h"

namespace forte::arch {
  std::string getRealtimeString() {
    std::string str;
    func_NOW().toString(str);
    return str;
  }
} // namespace forte::arch
