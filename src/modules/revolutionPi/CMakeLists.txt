#*******************************************************************************
# * Copyright (c) 2017 fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Johannes Messmer
# *   - initial API and implementation and/or initial documentation
# *******************************************************************************/

forte_add_io(REVOLUTIONPI "Support for Revolution Pi")

if(FORTE_IO_REVOLUTIONPI)
set(FORTE_IO ON CACHE BOOL "Enable IO Modules" FORCE)
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_sourcefile_hcpp( lib/piControlIf )

forte_add_sourcefile_hcpp( RevPiController )
forte_add_sourcefile_hcpp( RevPiHandle )

forte_add_sourcefile_hcpp( fb/IORevPiCore )
forte_add_sourcefile_hcpp( fb/IORevPiBusAdapter )
forte_add_sourcefile_hcpp( fb/IORevPiDIO )
forte_add_sourcefile_hcpp( fb/IORevPiAIO )
endif(FORTE_IO_REVOLUTIONPI)