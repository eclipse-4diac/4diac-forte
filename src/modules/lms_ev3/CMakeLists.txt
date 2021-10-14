#*******************************************************************************
# Copyright (c) 2015, 2016 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Alois Zoitl - initial API and implementation and/or initial documentation
# *    Jose Cabral - added fileWriter
# *******************************************************************************/ 
 
forte_add_module(LMS_EV3 OFF "I/O support for Lego Mindstorms EV3 using the ev3_dev operating system")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#######################################################
# LMS_EV3
#######################################################


forte_add_sourcefile_hcpp(../../stdfblib/io/IX)
forte_add_sourcefile_hcpp( ../../stdfblib/io/QX)
forte_add_sourcefile_hcpp(../../stdfblib/io/IW)
forte_add_sourcefile_hcpp( ../../stdfblib/io/QW)
forte_add_sourcefile_hcpp( ../../stdfblib/io/ID)
forte_add_sourcefile_hcpp( ../../stdfblib/io/QD)
forte_add_sourcefile_hcpp(processinterface)

forte_add_sourcefile_hcpp(fileWriter)
forte_add_sourcefile_hcpp(fileReader)
