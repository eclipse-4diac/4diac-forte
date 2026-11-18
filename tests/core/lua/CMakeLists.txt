# *******************************************************************************
# Copyright (c) 2026 Aimirim STI
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# AI Disclosure: This file was largely AI-generated. The AI-generated
# portions are made available under CC0-1.0 and not subject to the
# project's licence. The human contributor has reviewed and verified
# that the code is correct.
#
# SPDX-License-Identifier: EPL-2.0 AND CC0-1.0
# Assisted-by: Anthropic Claude Opus 5.5 (claude-opus-5-5)
#
# Contributors:
#   Pedro Ricardo C. Souza - initial implementation
# *******************************************************************************/

if (NOT FORTE_DYNAMIC_TYPE_LOAD)
    return()
endif ()

forte_test_add_sourcefile_cpp(luaadaptertests.cpp)
forte_test_add_sourcefile_cpp(luabfbtests.cpp)
forte_test_add_sourcefile_cpp(luabittests.cpp)
forte_test_add_sourcefile_cpp(luacfbtests.cpp)
forte_test_add_sourcefile_cpp(luaenginetests.cpp)
forte_test_add_sourcefile_cpp(stfunctests.cpp)

set_property(GLOBAL APPEND PROPERTY FORTE_TEST_LIBRARIES forte-lua-library)
