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
        forte_architecture.h
        forte_architecture_time.h
        forte_fileio.h
        forte_printer.h
        forte_specific_architecture.h
        threadbase.h
)

set_source_files_properties(
        bsdsocketinterf.h
        fdselecthand.h
        TARGET_DIRECTORY forte-core
        PROPERTIES
        SKIP_LINTING ON
)
