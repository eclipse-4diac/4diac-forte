# *******************************************************************************
# Copyright (c) 2017 fortiss GmbH, Sichuan Qunyuan Technology Co., Ltd. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Johannes Messmer - initial API and implementation and/or
# initial documentation
#    Zijun Tang - add createSlaveHandler() hook and generic FB constructor to io_slave_multi
# *******************************************************************************/

# ############################################################################
# IO Configuration FBs
# ############################################################################

target_sources(forte-core PRIVATE
        io_adapter_multi.cpp
        io_base.cpp
        io_configFB_controller.cpp
        io_master_multi.cpp
        io_slave_multi.cpp
)
