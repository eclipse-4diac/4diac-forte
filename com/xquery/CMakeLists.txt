#############################################################################
# Copyright (c) 2017, 2025 fortiss GmbH, Johannes Kepler University Linz 
# 
# This program and the accompanying materials are made available under the 
# terms of the Eclipse Public License 2.0 which is available at 
# https://www.eclipse.org/legal/epl-2.0/ 
# SPDX-License-Identifier: EPL-2.0
#############################################################################

# ############################################################################
# Xquery SIFBs
# ############################################################################
option(FORTE_COM_XQUERYCLIENT "client to send xqueries" OFF)
if (NOT FORTE_COM_XQUERYCLIENT)
    return()
endif ()
set(FORTE_BASEX_SRC_DIR
        ""
        CACHE PATH "Path to BaseX library directory"
)

add_library(forte-com-xquery
        ${FORTE_BASEX_SRC_DIR}/readstring.h
        ${FORTE_BASEX_SRC_DIR}/readstring.c
        ${FORTE_BASEX_SRC_DIR}/md5.h
        ${FORTE_BASEX_SRC_DIR}/md5.c
        ${FORTE_BASEX_SRC_DIR}/basexdbc.h
        ${FORTE_BASEX_SRC_DIR}/basexdbc.c
        xqueryHandler.h
        xqueryHandler.cpp
        xqueryClientLayer.h
        xqueryClientLayer.cpp
)
target_link_libraries(forte-com-xquery PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-com-xquery,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-com-xquery>>)

target_include_directories(forte-com-xquery PRIVATE ${FORTE_BASEX_SRC_DIR})

target_link_libraries(forte-com-xquery PRIVATE ssl crypto)

install(TARGETS forte-com-xquery EXPORT forte-export FILE_SET HEADERS)
