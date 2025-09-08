#pragma once

#ifdef FORTE_TRACER_INTERNAL

#include "internal/flexibleTracer.h"
using CForteTracer = CFlexibleTracer;

#else

#include "barectf_platform_forte.h"
using CForteTracer = BarectfPlatformFORTE;

#endif
