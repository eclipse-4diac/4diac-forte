/*********************************************************************
 * Copyright (c) 2022 Davor Cihlar
 *
 * This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License 2.0
 * which is available at https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Davor Cihlar
 *     - initial implementation
 **********************************************************************/

#include "mixedStorage.h"

using namespace forte::core::util;


void* CMixedStorage::write(const void *pa_pBegin, std::size_t pa_unSize) {
  auto pBegin = reinterpret_cast<const char*>(pa_pBegin);
  m_vData.insert(m_vData.end(), pBegin, pBegin + pa_unSize);
  return reinterpret_cast<void*>(&*m_vData.end() - pa_unSize);
}
