#*******************************************************************************
# Copyright (c) 2016 fortiss GmbH and Herwig Eichler, www.conmeleon.org
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Alois Zoitl and Herwig Eichler
# *******************************************************************************/ 
 
forte_add_module(CONMELEON_C1 OFF "Interacting with the CONMELEON C1 IO board")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#######################################################
# CONMELEON C1 IO board driver
#######################################################

forte_add_definition(-std=c++11)

forte_add_sourcefile_hcpp(../../stdfblib/io/IX)
forte_add_sourcefile_hcpp( ../../stdfblib/io/QX)
forte_add_sourcefile_hcpp( ../../stdfblib/io/IW)
forte_add_sourcefile_hcpp(gpio/gpiopin)
forte_add_sourcefile_hcpp(spi/spidevice)
forte_add_sourcefile_hcpp(spi/ads1018)
forte_add_sourcefile_hcpp(util/fileres)
forte_add_sourcefile_h(util/uncopyable.h)
forte_add_sourcefile_hcpp(processinterface)


