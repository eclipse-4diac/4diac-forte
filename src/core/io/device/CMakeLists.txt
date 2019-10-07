#*******************************************************************************
# Copyright (c) 2017 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Johannes Messmer
# *   - initial API and implementation and/or initial documentation
# *******************************************************************************/

#############################################################################
# IO Device
#############################################################################

SET(SOURCE_GROUP ${SOURCE_GROUP}\\device)

forte_add_sourcefile_hcpp(io_controller)
forte_add_sourcefile_hcpp(io_controller_multi)
forte_add_sourcefile_hcpp(io_controller_poll)
