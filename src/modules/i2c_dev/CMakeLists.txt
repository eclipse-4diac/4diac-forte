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

forte_register_module(I2C_Dev
                      OFF
                      "Interacting with GPIOs of Linux-based devices via I2C using i2c-dev"
)

# ############################################################################
# i2c-dev
# ############################################################################
forte_set_process_interface("I2C-Dev" IX QX IW QW)
add_library(forte-modules-i2c_dev OBJECT
            processinterface.h i2cprocessinterface.h i2cprocessinterface.cpp
)
target_link_libraries(forte PRIVATE forte-modules-i2c_dev)
