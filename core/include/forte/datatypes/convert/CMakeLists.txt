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
        convert_functions.h
        convert_function_utils.h
        SintToConvertFunctions.h
        DintToConvertFunctions.h
        DwordToConvertFunctions.h
        overloadedConvertFunctions.h
        ByteToConvertFunctions.h
        UsintToConvertFunctions.h
        BoolToConvertFunctions.h
        TimeInResolutionConvertFunctions.h
        IntToConvertFunctions.h
        UlintToConvertFunctions.h
        TimeToConvertFunctions.h
        DateAndTimeToConvertFunctions.h
        LDateAndTimeToConvertFunctions.h
        UintToConvertFunctions.h
        WcharToConvertFunctions.h
        TimeOfDayToConvertFunctions.h
        LintToConvertFunctions.h
        UdintToConvertFunctions.h
        WstringToConvertFunctions.h
        LwordToConvertFunctions.h
        CharToConvertFunctions.h
        StringToConvertFunctions.h
        LrealToConvertFunctions.h
        WordToConvertFunctions.h
        BcdConvertFunctions.h
        RealToConvertFunctions.h
)
