/*******************************************************************************
 * Copyright (c) 2019 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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
      const CIEC_TIME::TValueType cMillisecondsPerSecond = 1000 /* 1E3 */;
      const CIEC_TIME::TValueType cMicrosecondsPerSecond = 1000000 /* 1E6 */;
      const CIEC_TIME::TValueType cNanosecondsPerSecond = 1000000000 /* 1E9 */;
    } /* namespace constants */
  } /* namespace core */
} /* namespace forte */

#endif /* SRC_CORE_UTILS_FORTE_CONSTANTS_H_ */
