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

target_sources(forte-net PUBLIC
        FILE_SET HEADERS
        FILES
        GEN_CLIENT_fbt.h
        GEN_PUBLISH_fbt.h
        GEN_SEND_RECV_fbt.h
        GEN_SERVER_fbt.h
        GEN_SUBSCRIBE_fbt.h
        $<$<BOOL:${FORTE_COM_LOCAL}>:${CMAKE_CURRENT_SOURCE_DIR}/GEN_PUBL_fbt.h>
        $<$<BOOL:${FORTE_COM_LOCAL}>:${CMAKE_CURRENT_SOURCE_DIR}/GEN_SUBL_fbt.h>
)
