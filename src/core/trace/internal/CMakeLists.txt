# ############################################################################
# Copyright (c) 2025 Jose Cabral
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Martin Jobst - initial API and implementation and/or initial
# documentation
# ############################################################################
add_library(forte-core-trace-internal OBJECT
            EventMessage.h EventMessage.cpp internalTracer.h
            internalTracer.cpp flexibleTracer.h flexibleTracer.cpp
)
target_link_libraries(forte PRIVATE forte-core-trace-internal)
