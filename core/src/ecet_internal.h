/*******************************************************************************
 * Copyright (c) 2026 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/

#pragma once

#include "forte/stringid.h"
#include "forte/config/forte_config.h"

#ifdef FORTE_TRACE_CTF
#include "trace/tracerConfig.h"
#endif

namespace forte {
  class ECETInternal {
    public:
      explicit ECETInternal(StringId paName)
#ifdef FORTE_TRACE_CTF
          :
          mTracer(paName, FORTE_TRACE_CTF_BUFFER_SIZE)
#endif
      {
      }

#ifdef FORTE_TRACE_CTF
      trace::CForteTracer &getTracer() {
        return mTracer;
      }
#endif

    private:
#ifdef FORTE_TRACE_CTF
      trace::CForteTracer mTracer;
#endif // FORTE_TRACE_CTF
  };
} // namespace forte
