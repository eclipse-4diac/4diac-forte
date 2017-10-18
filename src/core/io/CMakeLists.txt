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
# IO
#############################################################################

set(FORTE_IO OFF CACHE BOOL "Enable IO Modules")
  if(NOT FORTE_IO)
    return()
  endif(NOT FORTE_IO)

SET(SOURCE_GROUP ${SOURCE_GROUP}\\io)

# forte_add_definition(-std=c++11)

add_subdirectory(configFB)
add_subdirectory(device)
add_subdirectory(handles)
add_subdirectory(mapper)
add_subdirectory(pi)
