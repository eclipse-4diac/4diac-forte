# *******************************************************************************
# Copyright (c) 2010 - 2012 ACIN This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Alois Zoitl *    - initial API and implementation and/or
# initial documentation
# *******************************************************************************/
# ############################################################################
# Net FB
# ############################################################################

add_library(forte-stdfblib-net OBJECT
            GEN_CLIENT_fbt.h
            GEN_CLIENT_fbt.cpp
            GEN_PUBLISH_fbt.h
            GEN_PUBLISH_fbt.cpp
            GEN_SEND_RECV_fbt.h
            GEN_SEND_RECV_fbt.cpp
            GEN_SERVER_fbt.h
            GEN_SERVER_fbt.cpp
            GEN_SUBSCRIBE_fbt.h
            GEN_SUBSCRIBE_fbt.cpp
            PUBLISH_1_fbt.h
            PUBLISH_1_fbt.cpp
            SUBSCRIBE_1_fbt.h
            SUBSCRIBE_1_fbt.cpp
            $<$<BOOL:${FORTE_COM_LOCAL}>:GEN_PUBL_fbt.h>
            $<$<BOOL:${FORTE_COM_LOCAL}>:GEN_PUBL_fbt.cpp>
            $<$<BOOL:${FORTE_COM_LOCAL}>:GEN_SUBL_fbt.h>
            $<$<BOOL:${FORTE_COM_LOCAL}>:GEN_SUBL_fbt.cpp>
)
target_link_libraries(forte PRIVATE forte-stdfblib-net)
