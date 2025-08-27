#pragma once

#ifdef FORTE_TRACER_INTERNAL

#include "core/trace/internal/flexibleTracer.h"
using CForteTracer = CFlexibleTracer;

#else

#include "core/trace/barectf_platform_forte.h"
using CForteTracer = BarectfPlatformFORTE;

#endif
