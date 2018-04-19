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

#############################################################################
# IO Process Interface
#############################################################################

SET(SOURCE_GROUP ${SOURCE_GROUP}\\pi)

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
forte_add_sourcefile_hcpp(processinterface)

forte_add_sourcefile_hcpp(../../../stdfblib/io/IX)
forte_add_sourcefile_hcpp( ../../../stdfblib/io/QX)
forte_add_sourcefile_hcpp( ../../../stdfblib/io/IW)
forte_add_sourcefile_hcpp( ../../../stdfblib/io/QW)
forte_add_sourcefile_hcpp( ../../../stdfblib/io/ID)
forte_add_sourcefile_hcpp( ../../../stdfblib/io/QD)