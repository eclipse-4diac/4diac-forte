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
        criticalregion.h
        devlog.h
        extevhandlerhelper.h
        factory.h
        fixed_string.h
        forte_constants.h
        hook.h
        inplace_vector.h
        mainparam_utils.h
        parameterParser.h
        registry.h
        ringbuf.h
        singlet.h
        string_utils.h
)
