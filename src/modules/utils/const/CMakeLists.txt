# *******************************************************************************
# Copyright (c) 2023 HR Agrartechnik GmbH This program and the accompanying
# materials are made available under the terms of the Eclipse Public License
# 2.0 which is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Franz Hoepfinger - initial API and implementation and/or
# initial documentation
# *******************************************************************************/

# ############################################################################
# const
# ############################################################################

add_library(forte-modules-utils-const OBJECT quarterconst_gcf.h
                                             quarterconst_gcf.cpp
)
target_link_libraries(forte PRIVATE forte-modules-utils-const)
