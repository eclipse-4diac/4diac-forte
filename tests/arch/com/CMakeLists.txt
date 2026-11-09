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


if (FORTE_ARCHITECTURE STREQUAL "Posix" OR FORTE_ARCHITECTURE STREQUAL "Linux" OR FORTE_ARCHITECTURE STREQUAL "Darwin")
    forte_test_add_subdirectory(posix)
elseif (FORTE_ARCHITECTURE STREQUAL "Win32" OR FORTE_ARCHITECTURE STREQUAL "Windows")
    forte_test_add_subdirectory(win32)
else ()
    return()
endif ()

if (FORTE_COM_ETH)
    forte_test_add_sourcefile_cpp(socket_test_helpers_tests.cpp)
    forte_test_add_sourcefile_cpp(socket_channel_tests.cpp)
    forte_test_add_sourcefile_cpp(tcp_tests.cpp)
    forte_test_add_sourcefile_cpp(udp_tests.cpp)
    forte_test_add_sourcefile_cpp(tcp_listen_tests.cpp)
    forte_test_add_sourcefile_cpp(udp_listen_tests.cpp)
    forte_test_add_sourcefile_cpp(tcp_parallel_tests.cpp)
    forte_test_add_sourcefile_cpp(net_parallel_tests.cpp)
endif ()
