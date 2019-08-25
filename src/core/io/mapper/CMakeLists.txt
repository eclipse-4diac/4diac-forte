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
# IO Mapper
#############################################################################

SET(SOURCE_GROUP ${SOURCE_GROUP}\\mapper)

forte_add_sourcefile_hcpp(io_observer)
forte_add_sourcefile_hcpp(io_handle)
forte_add_sourcefile_hcpp(io_mapper)
forte_add_sourcefile_hcpp(io_handle_bit)
