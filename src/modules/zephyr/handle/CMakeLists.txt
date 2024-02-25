#############################################################################
# Copyright (c) 2024 KT Elektronik GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#  Dirk Kaar - initial API and implementation and/or initial documentation
#############################################################################

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
forte_add_sourcefile_hcpp(IOHandleGPIO)
forte_add_sourcefile_hcpp(IOHandleADC)
forte_add_sourcefile_hcpp(IOHandlePWM)

