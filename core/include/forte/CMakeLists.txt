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
        adapter.h
        adapterconn.h
        basefb.h
        basicfb.h
        cfb.h
        conn.h
        dataconn.h
        datatype.h
        devexec.h
        device.h
        devicefactory.h
        ecet.h
        ecetfactory.h
        esfb.h
        event.h
        eventconn.h
        extevhan.h
        fbcontainer.h
        forte_st_iterator.h
        forte_st_util.h
        forteinstance.h
        funcbloc.h
        genfb.h
        iec61131_cast_helper.h
        iec61131_functions.h
        inoutdataconn.h
        interfacespec.h
        mgmcmd.h
        mgmcmdstruct.h
        resource.h
        simplefb.h
        startuphook.h
        stringid.h
        timerha.h
        timerhandlerfactory.h
        typelib.h
)

add_subdirectory(com)
add_subdirectory(cominfra)
add_subdirectory(config)
add_subdirectory(datatypes)
add_subdirectory(util)
