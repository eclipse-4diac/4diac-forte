#############################################################################
# Copyright (c) 2025 Martin Erich Jobst
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Martin Erich Jobst
#      - initial API and implementation and/or initial documentation
#############################################################################

target_sources(forte-core PRIVATE
        basefb.h
        basefb.cpp
        buffer.h
        channel.h
        common.h
        common.cpp
        factory.h
        fb.h
        fb.cpp
        observer.h
)

add_subdirectory(impl)
