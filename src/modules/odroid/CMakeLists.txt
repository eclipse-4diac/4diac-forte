#*******************************************************************************
# Copyright (c) 2016 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Alois Zoitl - initial API and implementation and/or initial documentation
# *******************************************************************************/ 
 
forte_add_module(Odroid OFF "Interacting with GPIOs and analogue inputs of the Odroid device")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#######################################################
# SysFs
#######################################################


forte_add_sourcefile_hcpp(../../stdfblib/io/IX)
forte_add_sourcefile_hcpp( ../../stdfblib/io/QX)
forte_add_sourcefile_hcpp(../../stdfblib/io/IW)
forte_add_sourcefile_hcpp(../sysfs/sysfsprocint)
forte_add_sourcefile_hcpp(processinterface)

