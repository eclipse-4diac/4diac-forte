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
# IO Device
#############################################################################

SET(SOURCE_GROUP ${SOURCE_GROUP}\\device)

forte_add_sourcefile_hcpp(io_controller)
forte_add_sourcefile_hcpp(io_controller_multi)
forte_add_sourcefile_hcpp(io_controller_poll)
