#############################################################################
# Copyright (c) 2026 Monika Wenger
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#############################################################################

target_sources(forte-opcua PUBLIC
        FILE_SET HEADERS
        FILES
        F_OpcUaCreateMethodId_fct.h
        F_OpcUaCreateObjectId_fct.h
        F_OpcUaCreatePaths_fct.h
        F_OpcUaCreateSubscribeId_fct.h
        F_OpcUaCreateWriteId_fct.h
)