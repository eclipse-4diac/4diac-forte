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
add_library(forte-modules-utils-assembling OBJECT
            ASSEMBLE_BYTE_FROM_BOOLS_fct.h
            ASSEMBLE_BYTE_FROM_BOOLS_fct.cpp
            ASSEMBLE_BYTE_FROM_QUARTERS_fct.h
            ASSEMBLE_BYTE_FROM_QUARTERS_fct.cpp
            ASSEMBLE_DWORD_FROM_BOOLS_fct.h
            ASSEMBLE_DWORD_FROM_BOOLS_fct.cpp
            ASSEMBLE_DWORD_FROM_BYTES_fct.h
            ASSEMBLE_DWORD_FROM_BYTES_fct.cpp
            ASSEMBLE_DWORD_FROM_QUARTERS_fct.h
            ASSEMBLE_DWORD_FROM_QUARTERS_fct.cpp
            ASSEMBLE_DWORD_FROM_WORDS_fct.h
            ASSEMBLE_DWORD_FROM_WORDS_fct.cpp
            ASSEMBLE_LWORD_FROM_BOOLS_fct.h
            ASSEMBLE_LWORD_FROM_BOOLS_fct.cpp
            ASSEMBLE_LWORD_FROM_QUARTERS_fct.h
            ASSEMBLE_LWORD_FROM_QUARTERS_fct.cpp
            ASSEMBLE_WORD_FROM_BOOLS_fct.h
            ASSEMBLE_WORD_FROM_BOOLS_fct.cpp
            ASSEMBLE_WORD_FROM_BYTES_fct.h
            ASSEMBLE_WORD_FROM_BYTES_fct.cpp
            ASSEMBLE_WORD_FROM_QUARTERS_fct.h
            ASSEMBLE_WORD_FROM_QUARTERS_fct.cpp
)
target_link_libraries(forte PRIVATE forte-modules-utils-assembling)
