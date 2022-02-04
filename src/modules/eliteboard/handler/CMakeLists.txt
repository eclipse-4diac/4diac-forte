#*******************************************************************************
# Copyright (c) 2021, 2022 Jonathan Lainer
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#   Jonathan Lainer - Initial implementation.
# *******************************************************************************/

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
forte_add_sourcefile_hcpp(EliteBoardDeviceController)
forte_add_handler(EliteBoardDeviceController EliteBoardDeviceController)
