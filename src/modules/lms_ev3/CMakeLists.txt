#*******************************************************************************
# * Copyright (c) 2015, 2016 fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *    Alois Zoitl - initial API and implementation and/or initial documentation
# *    Jose Cabral - added fileWriter
# *******************************************************************************/ 
 
forte_add_module(LMS_EV3 "I/O support for Lego Mindstorms EV3 using the ev3_dev operating system")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#######################################################
# LMS_EV3
#######################################################


forte_add_sourcefile_hcpp(../../stdfblib/io/IX)
forte_add_sourcefile_hcpp( ../../stdfblib/io/QX)
forte_add_sourcefile_hcpp(../../stdfblib/io/IW)
forte_add_sourcefile_hcpp( ../../stdfblib/io/QW)
forte_add_sourcefile_hcpp(processinterface)
forte_add_sourcefile_hcpp(fileWriter)
