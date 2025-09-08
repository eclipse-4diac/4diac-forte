#############################################################################
# Copyright (c) 2025 Martin Erich Jobst
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Martin Erich Jobst
#      - initial API and implementation and/or initial documentation
#############################################################################

target_sources(forte-utils PRIVATE
        ASSEMBLE_BYTE_FROM_BOOLS_fct.cpp
        ASSEMBLE_BYTE_FROM_QUARTERS_fct.cpp
        ASSEMBLE_DWORD_FROM_BOOLS_fct.cpp
        ASSEMBLE_DWORD_FROM_BYTES_fct.cpp
        ASSEMBLE_DWORD_FROM_QUARTERS_fct.cpp
        ASSEMBLE_DWORD_FROM_WORDS_fct.cpp
        ASSEMBLE_LWORD_FROM_BOOLS_fct.cpp
        ASSEMBLE_LWORD_FROM_QUARTERS_fct.cpp
        ASSEMBLE_WORD_FROM_BOOLS_fct.cpp
        ASSEMBLE_WORD_FROM_BYTES_fct.cpp
        ASSEMBLE_WORD_FROM_QUARTERS_fct.cpp
)
