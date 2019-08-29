#*******************************************************************************
# Copyright (c) 2018 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Jose Cabral - initial API and implementation and/or initial documentation
# *******************************************************************************/ 

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})  

forte_add_sourcefile_hcpp(wagoHandle wagoDeviceController)
forte_add_sourcefile_hcpp(types/WagoMaster types/WagoBusAdapter types/wagoSlaveBase)
forte_add_sourcefile_hcpp(types/Wago1506 types/Wago1405_6 types/Wago1504_5 types/Wago459)

