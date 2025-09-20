#pragma once

#ifdef FORTE_TRACER_INTERNAL

#include "internal/flexibleTracer.h"
namespace forte::trace {
  using CForteTracer = CFlexibleTracer;
}

#else

#include "barectf_platform_forte.h"
namespace forte::trace {
  using CForteTracer = BarectfPlatformFORTE;
}

#endif
