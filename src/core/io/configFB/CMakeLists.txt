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
# IO Configuration FBs
#############################################################################

SET(SOURCE_GROUP ${SOURCE_GROUP}\\configFB)

forte_add_sourcefile_hcpp(io_base io_configFB_controller 
                           io_adapter_multi io_master_multi io_slave_multi
                           io_adapter_split io_controller_split io_controller_part
                           )
