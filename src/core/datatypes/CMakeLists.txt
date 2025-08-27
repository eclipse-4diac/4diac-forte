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
add_subdirectory(convert)

target_sources(forte-core PRIVATE
            forte_any.h
            forte_any.cpp
            forte_any_elementary.h
            forte_any_elementary.cpp
            forte_any_magnitude.h
            forte_any_num.h
            forte_any_real.h
            forte_any_real.cpp
            forte_real.h
            forte_real.cpp
            forte_lreal.h
            forte_lreal.cpp
            forte_any_int.h
            forte_any_unsigned.h
            forte_usint.h
            forte_usint.cpp
            forte_uint.h
            forte_uint.cpp
            forte_udint.h
            forte_udint.cpp
            forte_ulint.h
            forte_ulint.cpp
            forte_any_signed.h
            forte_sint.h
            forte_sint.cpp
            forte_int.h
            forte_int.cpp
            forte_dint.h
            forte_dint.cpp
            forte_lint.h
            forte_lint.cpp
            forte_any_duration.h
            forte_any_duration.cpp
            forte_time.h
            forte_time.cpp
            forte_ltime.h
            forte_ltime.cpp
            forte_any_bit.h
            forte_bool.h
            forte_bool.cpp
            forte_byte.h
            forte_byte.cpp
            forte_word.h
            forte_word.cpp
            forte_dword.h
            forte_dword.cpp
            forte_lword.h
            forte_lword.cpp
            forte_any_chars.h
            forte_any_chars.cpp
            forte_any_string.h
            forte_any_string.cpp
            forte_string.h
            forte_string.cpp
            forte_string_fixed.h
            forte_wstring.h
            forte_wstring.cpp
            forte_any_char.h
            forte_char.h
            forte_char.cpp
            forte_wchar.h
            forte_wchar.cpp
            forte_any_date.h
            forte_any_date.cpp
            forte_date_and_time.h
            forte_date_and_time.cpp
            forte_date.h
            forte_date.cpp
            forte_time_of_day.h
            forte_time_of_day.cpp
            forte_ldate_and_time.h
            forte_ldate_and_time.cpp
            forte_ltime_of_day.h
            forte_ltime_of_day.cpp
            forte_ldate.h
            forte_ldate.cpp
            forte_state.h
            forte_state.cpp
            forte_any_variant.h
            forte_any_variant.cpp
            forte_any_elementary_variant.h
            forte_any_elementary_variant.cpp
            forte_any_magnitude_variant.h
            forte_any_magnitude_variant.cpp
            forte_any_num_variant.h
            forte_any_num_variant.cpp
            forte_any_real_variant.h
            forte_any_real_variant.cpp
            forte_any_int_variant.h
            forte_any_int_variant.cpp
            forte_any_signed_variant.h
            forte_any_signed_variant.cpp
            forte_any_unsigned_variant.h
            forte_any_unsigned_variant.cpp
            forte_any_duration_variant.h
            forte_any_duration_variant.cpp
            forte_any_bit_variant.h
            forte_any_bit_variant.cpp
            forte_any_chars_variant.h
            forte_any_chars_variant.cpp
            forte_any_string_variant.h
            forte_any_string_variant.cpp
            forte_any_char_variant.h
            forte_any_char_variant.cpp
            forte_any_date_variant.h
            forte_any_date_variant.cpp
            forte_any_unique_ptr.h
            forte_any_bit_partial.h
            # include STRUCT Datatype-Support
            forte_struct.h
            forte_struct.cpp
            forte_array.h
            forte_array.cpp
            forte_array_common.h
            forte_array_variable.h
            forte_array_fixed.h
            forte_array_dynamic.h
            forte_array_dynamic.cpp
            unicode_utils.h
            unicode_utils.cpp
)
