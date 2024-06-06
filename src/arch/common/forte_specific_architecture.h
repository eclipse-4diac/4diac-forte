/*******************************************************************************
 * Copyright (c) 2024 
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_ARCH_COMMON_FORTE_SPECIFIC_ARCHITECTURE_H_
#define SRC_ARCH_COMMON_FORTE_SPECIFIC_ARCHITECTURE_H_

#include "forte_specific_architecture.h"

class CForteSpecificArchitecture{
  public:

    static int initialize(int argc , char* argv[]);

    static int deinitialize();
};

#endif /* SRC_ARCH_COMMON_FORTE_SPECIFIC_ARCHITECTURE_H_ */
