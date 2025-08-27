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

if (NOT FORTE_IO)
    return()
endif ()

target_sources(forte-core PUBLIC
        FILE_SET HEADERS
        FILES
        inputfb.h
        outputfb.h
        processinterfacefb.h
        IB_fbt.h
        ID_fbt.h
        IE_fbt.h
        IL_fbt.h
        IW_fbt.h
        IX_fbt.h
        QB_fbt.h
        QD_fbt.h
        QL_fbt.h
        QW_fbt.h
        QX_fbt.h
)
