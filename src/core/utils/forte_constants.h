/*******************************************************************************
 * Copyright (c) 2019 TU Wien/ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Martin Melik Merkumians
 *    - common file for constants used in FORTE
 *******************************************************************************/

#ifndef SRC_CORE_UTILS_FORTE_CONSTANTS_H_
#define SRC_CORE_UTILS_FORTE_CONSTANTS_H_

#include "forte_time.h"

namespace forte {
  namespace core {
    namespace constants {
      const CIEC_TIME::TValueType cMillisecondsPerSecond = 1E3;
      const CIEC_TIME::TValueType cMicrosecondsPerSecond = 1E6;
      const CIEC_TIME::TValueType cNanosecondsPerSecond = 1E9;
    } /* namespace constants */
  } /* namespace core */
} /* namespace forte */

#endif /* SRC_CORE_UTILS_FORTE_CONSTANTS_H_ */
