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
# assembling
# ############################################################################

add_library(forte-modules-utils-splitting OBJECT
            SPLIT_BYTE_INTO_BOOLS_fct.h
            SPLIT_BYTE_INTO_BOOLS_fct.cpp
            SPLIT_BYTE_INTO_QUARTERS_fct.h
            SPLIT_BYTE_INTO_QUARTERS_fct.cpp
            SPLIT_DWORD_INTO_BOOLS_fct.h
            SPLIT_DWORD_INTO_BOOLS_fct.cpp
            SPLIT_DWORD_INTO_QUARTERS_fct.h
            SPLIT_DWORD_INTO_QUARTERS_fct.cpp
            SPLIT_DWORD_INTO_WORDS_fct.h
            SPLIT_DWORD_INTO_WORDS_fct.cpp
            SPLIT_LWORD_INTO_BOOLS_fct.h
            SPLIT_LWORD_INTO_BOOLS_fct.cpp
            SPLIT_LWORD_INTO_QUARTERS_fct.h
            SPLIT_LWORD_INTO_QUARTERS_fct.cpp
            SPLIT_WORD_INTO_BOOLS_fct.h
            SPLIT_WORD_INTO_BOOLS_fct.cpp
            SPLIT_WORD_INTO_BYTES_fct.h
            SPLIT_WORD_INTO_BYTES_fct.cpp
            SPLIT_WORD_INTO_QUARTERS_fct.h
            SPLIT_WORD_INTO_QUARTERS_fct.cpp
)
target_link_libraries(forte PRIVATE forte-modules-utils-splitting)
