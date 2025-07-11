# *******************************************************************************
# Copyright (c) 2019 TU Wien/ACIN This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Martin Melik-Merkumians - adds package convert in
# core/datatypes

# *******************************************************************************/
# ############################################################################
# Core/datatypes/convert
# ############################################################################
add_library(forte-core-datatypes-convert OBJECT convert_functions.h)
set_property(TARGET forte-core-datatypes-convert PROPERTY LINKER_LANGUAGE CXX)
target_link_libraries(forte PRIVATE forte-core-datatypes-convert)
