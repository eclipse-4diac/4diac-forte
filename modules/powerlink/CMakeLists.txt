# *******************************************************************************
# Copyright (c) 2014 - 2024 AIT, fortiss GmbH This program and the
# accompanying materials are made available under the terms of the Eclipse
# Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Filip Andren, Alois Zoitl - initial API and implementation
# and/or initial documentation Michael Gafert - automatic download and linking
# of openPOWERLINK and tinyXML
# *******************************************************************************/

if (NOT WIN32 AND NOT LINUX)
    return()
endif ()

option(FORTE_MODULE_POWERLINK "openPOWERLINK Service Interface Function Blocks" OFF)

if (NOT FORTE_MODULE_POWERLINK)
    return()
endif ()

# ############################################################################
# COM Service Interface Function Blocks
# ############################################################################

set(FORTE_MODULE_POWERLINK_LIB_DIR
    ""
    CACHE PATH "Path to openPOWERLINK-V1.08 directory"
)
set(FORTE_MODULE_POWERLINK_TINYXML_DIR
    ""
    CACHE PATH "Path to TinyXML directory"
)

if (WIN32)
    target_compile_definitions(forte-powerlink PRIVATE WPCAP)
    target_link_libraries(forte PUBLIC openPOWERLINK wpcap iphlpapi)
elseif (LINUX)
    target_link_libraries(forte PUBLIC powerlink pcap rt)
endif ()

add_library(forte-powerlink
            ${FORTE_MODULE_POWERLINK_LIB_DIR}/EplStack/trace.c
            EplWrapper.cpp
            EplWrapper.h
            fb/PowerlinkFunctionBlockDI.h
            fb/PowerlinkFunctionBlockDI.cpp
            fb/PowerlinkFunctionBlockDO.h
            fb/PowerlinkFunctionBlockDO.cpp
            fb/PowerlinkFunctionBlockAI.h
            fb/PowerlinkFunctionBlockAI.cpp
            fb/PowerlinkFunctionBlockAO.h
            fb/PowerlinkFunctionBlockAO.cpp
            fb/PowerlinkFunctionBlockAT.h
            fb/PowerlinkFunctionBlockAT.cpp
            fb/POWERLINK_MN_fbt.h
            fb/POWERLINK_MN_fbt.cpp
            fb/X20AI2622_fbt.h
            fb/X20AI2622_fbt.cpp
            fb/X20AI4622_fbt.h
            fb/X20AI4622_fbt.cpp
            fb/X20AO4622_fbt.h
            fb/X20AO4622_fbt.cpp
            fb/X20AT2402_fbt.h
            fb/X20AT2402_fbt.cpp
            fb/X20AT4222_fbt.h
            fb/X20AT4222_fbt.cpp
            fb/X20DI4653_fbt.h
            fb/X20DI4653_fbt.cpp
            fb/X20DI9371_fbt.h
            fb/X20DI9371_fbt.cpp
            fb/X20DI9372_fbt.h
            fb/X20DI9372_fbt.cpp
            fb/X20DO4623_fbt.h
            fb/X20DO4623_fbt.cpp
            fb/X20DO4649_fbt.h
            fb/X20DO4649_fbt.cpp
            fb/X20DO9321_fbt.h
            fb/X20DO9321_fbt.cpp
            fb/X20DO9322_fbt.h
            fb/X20DO9322_fbt.cpp
            EplXmlReader.h
            EplXmlReader.cpp
            ModuleList.h
            ModuleList.cpp
            ProcessImageMatrix.h
            ProcessImageMatrix.cpp
            startup.cpp
            ${FORTE_MODULE_POWERLINK_LIB_DIR}/ObjDicts/CiA302-4_MN/EplApiProcessImageSetup.c
            ${FORTE_MODULE_POWERLINK_LIB_DIR}/EplStack/EplTgtConio.c
            ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinystr.h
            ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinystr.cpp
            ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinyxml.h
            ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinyxml.cpp
            ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinyxmlerror.cpp
            ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinyxmlparser.cpp
)
target_link_libraries(forte-powerlink PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-powerlink,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-powerlink>>)
target_include_directories(forte-powerlink
                           PUBLIC ${FORTE_MODULE_POWERLINK_LIB_DIR}/Target/X86/Windows/WpdPack/Include
)
target_include_directories(forte-powerlink
                           PUBLIC ${FORTE_MODULE_POWERLINK_LIB_DIR}/Include
)
target_include_directories(forte-powerlink
                           PUBLIC ${FORTE_MODULE_POWERLINK_LIB_DIR}/SharedBuff
)
target_include_directories(forte-powerlink
                           PUBLIC ${FORTE_MODULE_POWERLINK_LIB_DIR}/ObjDicts/CiA302-4_MN
)
target_include_directories(forte-powerlink
                           PUBLIC ${FORTE_MODULE_POWERLINK_LIB_DIR}/Examples/X86/Generic/powerlink_user_lib
)
target_include_directories(forte-powerlink
                           PUBLIC ${FORTE_MODULE_POWERLINK_TINYXML_DIR}
)
target_link_directories(forte-powerlink
                        PUBLIC
                        ${FORTE_MODULE_POWERLINK_LIB_DIR}/Target/X86/Windows/WpdPack/Lib
)
target_link_directories(forte-powerlink PUBLIC
                        ${FORTE_MODULE_POWERLINK_LIB_DIR}
)

install(TARGETS forte-powerlink EXPORT forte-export FILE_SET HEADERS)
