#*******************************************************************************
# Copyright (c) 2017 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Jose Cabral - initial API and implementation and/or initial documentation
# *******************************************************************************/ 
 
if ("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
forte_add_module(uMIC OFF "Interacting with GPIOs using the umic library for MicroControl")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#######################################################
# uMIC
#######################################################

forte_add_sourcefile_hcpp(processinterface)
forte_set_process_interface("uMIC" IX QX)
forte_add_link_library(umic.so)

endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
