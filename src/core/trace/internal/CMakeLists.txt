#############################################################################
# Copyright (c) 2025 Jose Cabral
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Martin Jobst
#      - initial API and implementation and/or initial documentation
#############################################################################


if(FORTE_TRACE_CTF_TEST OR FORTE_REPLAY_DEVICE)
  forte_add_sourcefile_hcpp(EventMessage)
  forte_add_sourcefile_hcpp(internalTracer)
  forte_add_sourcefile_hcpp(flexibleTracer)

  set(FORTE_TRACER_INCLUDE internal/flexibleTracer.h PARENT_SCOPE)
  set(FORTE_TRACER CFlexibleTracer PARENT_SCOPE)
endif()
