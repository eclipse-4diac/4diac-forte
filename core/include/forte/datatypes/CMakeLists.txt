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

target_sources(forte-core PUBLIC
        FILE_SET HEADERS
        FILES
        forte_any.h
        forte_any_bit.h
        forte_any_bit_partial.h
        forte_any_bit_variant.h
        forte_any_char.h
        forte_any_char_variant.h
        forte_any_chars.h
        forte_any_chars_variant.h
        forte_any_date.h
        forte_any_date_variant.h
        forte_any_derived.h
        forte_any_duration.h
        forte_any_duration_variant.h
        forte_any_elementary.h
        forte_any_elementary_variant.h
        forte_any_int.h
        forte_any_int_variant.h
        forte_any_magnitude.h
        forte_any_magnitude_variant.h
        forte_any_num.h
        forte_any_num_variant.h
        forte_any_real.h
        forte_any_real_variant.h
        forte_any_signed.h
        forte_any_signed_variant.h
        forte_any_string.h
        forte_any_string_variant.h
        forte_any_unique_ptr.h
        forte_any_unsigned.h
        forte_any_unsigned_variant.h
        forte_any_variant.h
        forte_array.h
        forte_array_common.h
        forte_array_dynamic.h
        forte_array_fixed.h
        forte_array_variable.h
        forte_bool.h
        forte_byte.h
        forte_char.h
        forte_date.h
        forte_date_and_time.h
        forte_dint.h
        forte_dword.h
        forte_int.h
        forte_ldate.h
        forte_ldate_and_time.h
        forte_lint.h
        forte_lreal.h
        forte_ltime.h
        forte_ltime_of_day.h
        forte_lword.h
        forte_real.h
        forte_sint.h
        forte_state.h
        forte_string.h
        forte_string_fixed.h
        forte_struct.h
        forte_time.h
        forte_time_of_day.h
        forte_udint.h
        forte_uint.h
        forte_ulint.h
        forte_usint.h
        forte_wchar.h
        forte_word.h
        forte_wstring.h
)

add_subdirectory(convert)
