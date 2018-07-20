#*******************************************************************************
# * Copyright (c) 2017 fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *    Jose Cabral - initial API and implementation and/or initial documentation
# *******************************************************************************/ 
 
forte_add_module(uMIC "Interacting with GPIOs using the umic library for MicroControl")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#######################################################
# uMIC
#######################################################

forte_add_sourcefile_hcpp(../../stdfblib/io/IX)
forte_add_sourcefile_hcpp( ../../stdfblib/io/QX)
forte_add_sourcefile_hcpp(processinterface)
forte_add_link_library(umic.so)

