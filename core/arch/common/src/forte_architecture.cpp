/*******************************************************************************
 * Copyright (c) 2017, 2024 fortiss GmbH, Jose Cabral
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *  Alois Zoitl - moved archictecture class to common arch folder and adapted
 *                it for generic use
 *******************************************************************************/

#include "forte/arch/forte_architecture.h"

#include "forte/arch/forte_specific_architecture.h"

#include "forte/startuphook.h"

namespace forte::arch {
  bool CForteGeneralArchitecture::mInitialized = false;

  int CForteGeneralArchitecture::initialize(int argc, char *argv[]) {
    if (mInitialized) {
      return 0;
    }
    if (auto result = CForteSpecificArchitecture::initialize(argc, argv); result != 0) {
      return result;
    }
    StartupHookRegistry::invoke(argc, argv);
    mInitialized = true;
    return 0;
  }

  int CForteGeneralArchitecture::deinitialize() {
    if (!mInitialized) {
      return 0;
    }

    if (auto result = CForteSpecificArchitecture::deinitialize(); result != 0) {
      return result;
    }
    mInitialized = false;
    return 0;
  }

  bool CForteGeneralArchitecture::isInitialized() {
    return mInitialized;
  }
} // namespace forte::arch
