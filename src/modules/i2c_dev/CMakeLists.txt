#*******************************************************************************
# Copyright (c) 2015 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Gerd Kainz, Alois Zoitl - initial API and implementation and/or initial documentation
# *******************************************************************************/ 
 
if ("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
forte_add_module(I2C-Dev OFF "Interacting with GPIOs of Linux-based devices via I2C using i2c-dev")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#######################################################
# i2c-dev
#######################################################


forte_add_sourcefile_h(processinterface.h)
forte_set_process_interface("I2C-Dev" IX QX IW QW)
forte_add_sourcefile_hcpp(i2cprocessinterface)

endif ("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
