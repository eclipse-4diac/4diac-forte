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


void* CMixedStorage::write(const void *paBegin, std::size_t paSize) {
  auto pBegin = reinterpret_cast<const char*>(paBegin);
  auto offset = mData.size();
  mData.insert(mData.end(), pBegin, pBegin + paSize);
  return mData.data() + offset;
}
