# *******************************************************************************
# Copyright (c) 2017 fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Johannes Messmer *   - initial API and implementation and/or
# initial documentation
# *******************************************************************************/

# ############################################################################
# IO Configuration FBs
# ############################################################################

target_sources(forte-core PRIVATE
            io_base.h
            io_base.cpp
            io_configFB_controller.h
            io_configFB_controller.cpp
            io_adapter_multi.h
            io_adapter_multi.cpp
            io_master_multi.h
            io_master_multi.cpp
            io_slave_multi.h
            io_slave_multi.cpp
)
