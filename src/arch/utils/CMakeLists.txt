#*******************************************************************************
# Copyright (c) 2018 ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#   Martin Melik Merkumians
# *      - initial API and implementation and/or initial documentation
# *******************************************************************************/

add_library(forte-arch-utils OBJECT
	mainparam_utils.cpp
)

target_link_libraries(forte PRIVATE forte-arch-utils)
