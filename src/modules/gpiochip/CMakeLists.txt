#*******************************************************************************
# Copyright (c) 2015,2023 fortiss GmbH, OFFIS e.V.
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Waldemar Eisenmenger - initial API and implementation and/or initial documentation
#     Jörg Walter - adaption of sysfs
# *******************************************************************************/ 

if ("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
forte_add_io(GPIOCHIP "Support for Linux GPIOs via /dev/gpiochipX")

if (FORTE_IO_GPIOCHIP)
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  forte_add_sourcefile_hcpp(gpiochip_controller gpiochip_config_fb)
endif(FORTE_IO_GPIOCHIP)
endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
