# *******************************************************************************
# Copyright (c) 2015 fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Gerd Kainz, Alois Zoitl - initial API and implementation
# and/or initial documentation
# *******************************************************************************/

if (NOT FORTE_ARCHITECTURE STREQUAL "Posix")
    return()
endif ()

option(FORTE_MODULE_I2C_Dev "Interacting with GPIOs of Linux-based devices via I2C using i2c-dev" OFF)

if (NOT FORTE_MODULE_I2C_Dev)
    return()
endif ()

message(FATAL_ERROR "This module has not yet been ported to 4diac FORTE 3.0")

# ############################################################################
# i2c-dev
# ############################################################################
forte_set_process_interface("I2C-Dev" IX QX IW QW)
add_library(forte-i2c_dev
            processinterface.h i2cprocessinterface.h i2cprocessinterface.cpp
)
target_link_libraries(forte-i2c_dev PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-i2c_dev,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-i2c_dev>>)
install(TARGETS forte-i2c_dev EXPORT forte-export FILE_SET HEADERS)
