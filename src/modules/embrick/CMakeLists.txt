#*******************************************************************************
# * Copyright (c) 2016 fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Johannes Messmer
# *   - initial API and implementation and/or initial documentation
# *******************************************************************************/

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lrt")

forte_add_io(EMBRICK "Support for the modular emBRICK system")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#############################################################################
# emBrick FBs
#############################################################################
forte_add_sourcefile_hcpp(utils/conditionSync)

forte_add_sourcefile_hcpp(handler/spi handler/pin handler/bus)

forte_add_sourcefile_hcpp(slave/slave slave/handle)
forte_add_sourcefile_hcpp(slave/handles/bit slave/handles/analog slave/handles/analog10)

forte_add_sourcefile_hcpp(types/BusAdapter)
forte_add_sourcefile_hcpp(types/Master)
forte_add_sourcefile_hcpp(types/Slave types/Slave2181 types/Slave2301)
