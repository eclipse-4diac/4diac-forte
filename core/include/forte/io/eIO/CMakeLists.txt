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
        CeBuffer.h
        CeConfig.h
        CeSpec.h
        CMakeLists.txt
        eConfigFB.h
        eGenAdapter_adp.h
        einputfb.h
        eIW_fbt.h
        eIWconfig_fbt.h
        eIX_fbt.h
        eIXconfig_fbt.h
)
