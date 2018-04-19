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
# IO Configuration FBs
#############################################################################

SET(SOURCE_GROUP ${SOURCE_GROUP}\\configFB)

forte_add_sourcefile_hcpp(io_base io_configFB_controller)

forte_add_sourcefile_hcpp(multi/io_adapter)
forte_add_sourcefile_hcpp(multi/io_master)
forte_add_sourcefile_hcpp(multi/io_slave)

forte_add_sourcefile_hcpp(split/io_adapter_split)
forte_add_sourcefile_hcpp(split/io_controller_split)
forte_add_sourcefile_hcpp(split/io_controller_part)
