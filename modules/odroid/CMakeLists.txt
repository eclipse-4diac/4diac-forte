# *******************************************************************************
# Copyright (c) 2016 fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Alois Zoitl - initial API and implementation and/or initial
# documentation
# *******************************************************************************/

if (NOT FORTE_ARCHITECTURE STREQUAL "Posix")
    return()
endif ()

option(FORTE_MODULE_Odroid "Interacting with GPIOs and analogue inputs of the Odroid device" OFF)

if (NOT FORTE_MODULE_Odroid)
    return()
endif ()

# ############################################################################
# SysFs
# ############################################################################
forte_set_process_interface("Odroid" IX QX IW)
add_library(forte-odroid
            ../sysfs/sysfsprocint.h ../sysfs/sysfsprocint.cpp
            processinterface.h processinterface.cpp
)
target_link_libraries(forte-odroid PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-odroid,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-odroid>>)
install(TARGETS forte-odroid EXPORT forte-export FILE_SET HEADERS)
