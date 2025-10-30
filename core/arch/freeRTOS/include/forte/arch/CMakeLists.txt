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

target_sources(forte-core PUBLIC
        FILE_SET HEADERS
        FILES
        forte_sem.h
        forte_sync.h
        forte_thread.h
        sockhand.h
        $<$<BOOL:${FORTE_FREERTOS_PLUS_TCP}>:${CMAKE_CURRENT_SOURCE_DIR}/sockhand_plustcp.h>
        $<$<NOT:$<BOOL:${FORTE_FREERTOS_PLUS_TCP}>>:${CMAKE_CURRENT_SOURCE_DIR}/sockhand_lwip.h>
)
