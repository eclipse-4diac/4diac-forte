#*******************************************************************************
# * Copyright (c) 2016 Johannes Messmer (messmer@fortiss.org)
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

forte_add_sourcefile_hcpp(configFB/base configFB/controller)
# forte_add_sourcefile_hcpp(configFB/multi/master)
forte_add_sourcefile_hcpp(device/controller)
