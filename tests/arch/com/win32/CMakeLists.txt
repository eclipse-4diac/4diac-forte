#############################################################################
# Copyright (c) 2026 Martin Erich Jobst
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Martin Erich Jobst - initial implementation
#############################################################################

if (FORTE_COM_ETH)
    forte_test_add_inc_directories(${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/core/arch/win32/src)
    forte_test_add_sourcefile_cpp(socket_test_helpers.cpp)
    forte_test_add_sourcefile_cpp(net_tests.cpp)
endif ()
