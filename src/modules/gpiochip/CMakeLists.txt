# *******************************************************************************
# Copyright (c) 2015,2023 fortiss GmbH, OFFIS e.V. This program and the
# accompanying materials are made available under the terms of the Eclipse
# Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Waldemar Eisenmenger - initial API and implementation and/or
# initial documentation Jörg Walter - adaption of sysfs
# *******************************************************************************/

if (NOT FORTE_ARCHITECTURE STREQUAL "Posix")
    return()
endif ()

forte_register_io(GPIOCHIP OFF "Support for Linux GPIOs via /dev/gpiochipX")

add_library(forte-modules-gpiochip OBJECT
            gpiochip_controller.h gpiochip_controller.cpp
            gpiochip_config_fb.h gpiochip_config_fb.cpp
)
target_link_libraries(forte PRIVATE forte-modules-gpiochip)
