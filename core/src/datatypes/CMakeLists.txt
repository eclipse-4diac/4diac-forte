# ############################################################################
# Copyright (c) 2010 - 2014 Profactor GmbH, ACIN, fortiss GmbH
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Michael Hofmann, Alois Zoitl, Monika Wenger, Gerhard Ebenhofer
# - initial implementation and rework communication infrastructure
# ############################################################################
# ############################################################################
# Core Datatypes
# ############################################################################

target_sources(forte-core PRIVATE
        forte_any.cpp
        forte_any_bit_variant.cpp
        forte_any_char_variant.cpp
        forte_any_chars.cpp
        forte_any_chars_variant.cpp
        forte_any_date.cpp
        forte_any_date_variant.cpp
        forte_any_duration.cpp
        forte_any_duration_variant.cpp
        forte_any_elementary.cpp
        forte_any_elementary_variant.cpp
        forte_any_int_variant.cpp
        forte_any_magnitude_variant.cpp
        forte_any_num_variant.cpp
        forte_any_real.cpp
        forte_any_real_variant.cpp
        forte_any_signed_variant.cpp
        forte_any_string.cpp
        forte_any_string_variant.cpp
        forte_any_unsigned_variant.cpp
        forte_any_variant.cpp
        forte_array.cpp
        forte_array_dynamic.cpp
        forte_bool.cpp
        forte_byte.cpp
        forte_char.cpp
        forte_date.cpp
        forte_date_and_time.cpp
        forte_dint.cpp
        forte_dword.cpp
        forte_int.cpp
        forte_ldate.cpp
        forte_ldate_and_time.cpp
        forte_lint.cpp
        forte_lreal.cpp
        forte_ltime.cpp
        forte_ltime_of_day.cpp
        forte_lword.cpp
        forte_real.cpp
        forte_sint.cpp
        forte_state.cpp
        forte_string.cpp
        forte_struct.cpp
        forte_time.cpp
        forte_time_of_day.cpp
        forte_udint.cpp
        forte_uint.cpp
        forte_ulint.cpp
        forte_usint.cpp
        forte_wchar.cpp
        forte_word.cpp
        forte_wstring.cpp
        unicode_utils.cpp
        unicode_utils.h
)

# std::codecvt<char16_t, char, std::mbstate_t> in forte_wstring.h is
# deprecated, and clang libc++ loudly complains about this fact. There is no
# standardised replacement, not even planned, so we can't upgrade to a clean
# API. Just silence the useless warnings for now.
target_compile_definitions(forte-core PUBLIC _LIBCPP_DISABLE_DEPRECATION_WARNINGS)

add_subdirectory(convert)
