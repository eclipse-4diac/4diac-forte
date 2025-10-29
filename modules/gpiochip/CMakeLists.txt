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

if (NOT LINUX)
    return()
endif ()

option(FORTE_MODULE_GPIOCHIP "Support for Linux GPIOs via /dev/gpiochipX" OFF)

if (NOT FORTE_MODULE_GPIOCHIP)
    return()
endif ()

add_library(forte-gpiochip
            gpiochip_controller.h gpiochip_controller.cpp
            gpiochip_config_fb.h gpiochip_config_fb.cpp
)
target_link_libraries(forte-gpiochip PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-gpiochip,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-gpiochip>>)
install(TARGETS forte-gpiochip EXPORT forte-export FILE_SET HEADERS)
