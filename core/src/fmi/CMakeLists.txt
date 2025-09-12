# *******************************************************************************
# Copyright (c) 2016,2017 fortiss GmbH This program and the accompanying
# materials are made available under the terms of the Eclipse Public License
# 2.0 which is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Jose Cabral *    - initial API and implementation and/or
# initial documentation
# *******************************************************************************/
# ############################################################################
# core/fmi
# ############################################################################

option(FORTE_ENABLE_FMU
       "Enable FORTE FMU Export. It generates a shared library" OFF
)

if (NOT FORTE_ENABLE_FMU)
    return()
endif ()
target_compile_definitions(forte-core PUBLIC FORTE_FMU)

set(FORTE_FMU_INCLUDE
    ""
    CACHE STRING
          "Folder to include in the compilation. Specify here the location of the fmi headers"
)

add_subdirectory(comm)

message(STATUS "Process Interface: FORTE FMU")

target_sources(forte-core PRIVATE
            fmiInterface.cpp
            fmuValueContainer.h
            fmuValueContainer.cpp
            fmuInstance.h
            fmuInstance.cpp
            processinterface.h
            processinterface.cpp
            fmuEcet.h
            fmuEcet.cpp
            fmiTimerHandler.h
            fmiTimerHandler.cpp
            ../../arch/utils/timespec_utils.h
            ../../arch/utils/timespec_utils.cpp
            ../../stdfblib/io/IX_fbt.h
            ../../stdfblib/io/IX_fbt.cpp
            ../../stdfblib/io/QX_fbt.h
            ../../stdfblib/io/QX_fbt.cpp
            ../../stdfblib/io/IW_fbt.h
            ../../stdfblib/io/IW_fbt.cpp
            ../../stdfblib/io/QW_fbt.h
            ../../stdfblib/io/QW_fbt.cpp
)
target_include_directories(forte-core PUBLIC ${FORTE_FMU_INCLUDE})
