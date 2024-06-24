#*******************************************************************************
# Copyright (c) 2012 - 2014 AIT, fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
# *******************************************************************************/

forte_add_module(POWERLINK OFF "openPOWERLINK Service Interface Function Blocks")

#############################################################################
# COM Service Interface Function Blocks
#############################################################################

if (NOT "${FORTE_ARCHITECTURE}" STREQUAL "Win32" AND NOT "${FORTE_ARCHITECTURE}" STREQUAL "Posix")
    MESSAGE(FATAL_ERROR "openPOWERLINK is only supported for FORTE on Windows and Linux platforms.")
endif ()

SET(FORTE_MODULE_POWERLINK_LIB_DIR "" CACHE PATH "Path to openPOWERLINK-V1.08 directory")
SET(FORTE_MODULE_POWERLINK_TINYXML_DIR "" CACHE PATH "Path to TinyXML directory")

# Download tinyXML
if (NOT FORTE_MODULE_POWERLINK_TINYXML_DIR)
    include(FetchContent)
    FetchContent_Declare(
            tinyxml
            URL https://downloads.sourceforge.net/project/tinyxml/tinyxml/2.6.2/tinyxml_2_6_2.zip
            URL_HASH   SHA256=ac6bb9501c6f50cc922d22f26b02fab168db47521be5e845b83d3451a3e1d512
    )

    FetchContent_Populate(tinyxml)

    message("Downloaded tinyXML in ${tinyxml_SOURCE_DIR}")

    set(FORTE_MODULE_POWERLINK_TINYXML_DIR ${tinyxml_SOURCE_DIR})
endif ()

if ("${FORTE_ARCHITECTURE}" STREQUAL "Win32")
    forte_add_sourcefile_cpp(EplWrapper.cpp)
    forte_add_definition(-D_CONSOLE -DWPCAP)

    forte_add_include_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/Target/X86/Windows/WpdPack/Include)
    forte_add_link_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/Target/X86/Windows/WpdPack/Lib)
    forte_add_sourcefile_with_path_cpp(${FORTE_MODULE_POWERLINK_LIB_DIR}/EplStack/trace.c)

    forte_add_link_library(openPOWERLINK.lib)
    forte_add_link_library(wpcap.lib)
    forte_add_link_library(iphlpapi.lib)
elseif ("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
    # Download and build openPOWERLINK only for posix as there are too many problems with the Windows build
    if (NOT FORTE_MODULE_POWERLINK_LIB_DIR)
        FetchContent_Declare(
                openpowerlink
                URL https://downloads.sourceforge.net/project/openpowerlink/openPOWERLINK/V1.8.0/openPOWERLINK-V1.08.zip
                URL_HASH MD5=7ae68359cbc9092d125d43c75b4794ad
        )
        FetchContent_Populate(openpowerlink)

        message("Downloaded openPOWERLINK in ${openpowerlink_SOURCE_DIR}")

        # TODO: Pass target architecture to openPOWERLINK build, important for Windows as wpcap is only available for x86
        # TODO: Ninja generator does not work with openPOWERLINK build as in does not build the .lib file
        execute_process(
                COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" CMakeLists.txt # -D CFG_X86_WINDOWS_DLL=ON (for Windows)
                WORKING_DIRECTORY "${openpowerlink_SOURCE_DIR}"
        )

        execute_process(
                COMMAND ${CMAKE_COMMAND} --build . --config Release
                WORKING_DIRECTORY "${openpowerlink_SOURCE_DIR}"
        )

        # Move the openPOWERLINK library to the FORTE_MODULE_POWERLINK_LIB_DIR as expected
        file(RENAME
                "${openpowerlink_SOURCE_DIR}/Examples/X86/Generic/powerlink_user_lib/libpowerlink.a"
                "${openpowerlink_SOURCE_DIR}/libpowerlink.a")

        set(FORTE_MODULE_POWERLINK_LIB_DIR "${openpowerlink_SOURCE_DIR}")
    endif ()

    forte_add_link_library(-lpowerlink -lpcap -lrt)
endif ()

forte_add_sourcefile_cpp(EplWrapper.cpp)
forte_add_sourcefile_h(EplWrapper.h)

forte_add_sourcefile_hcpp(
        fb/PowerlinkFunctionBlockDI
        fb/PowerlinkFunctionBlockDO
        fb/PowerlinkFunctionBlockAI
        fb/PowerlinkFunctionBlockAO
        fb/PowerlinkFunctionBlockAT
        fb/POWERLINK_MN_fbt
        fb/X20AI2622_fbt
        fb/X20AI4622_fbt
        fb/X20AO4622_fbt
        fb/X20AT2402_fbt
        fb/X20AT4222_fbt
        fb/X20DI4653_fbt
        fb/X20DI9371_fbt
        fb/X20DI9372_fbt
        fb/X20DO4623_fbt
        fb/X20DO4649_fbt
        fb/X20DO9321_fbt
        fb/X20DO9322_fbt
        EplXmlReader
        ModuleList
        ProcessImageMatrix)

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_startup_hook(powerlinkStartupHook)
forte_add_sourcefile_cpp(startup.cpp)

forte_add_link_directories(${FORTE_MODULE_POWERLINK_LIB_DIR})
forte_add_include_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/Include)
forte_add_include_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/SharedBuff)
forte_add_include_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/ObjDicts/CiA302-4_MN)
forte_add_include_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/Examples/X86/Generic/powerlink_user_lib)
forte_add_sourcefile_with_path_cpp(${FORTE_MODULE_POWERLINK_LIB_DIR}/ObjDicts/CiA302-4_MN/EplApiProcessImageSetup.c)
forte_add_sourcefile_with_path_cpp(${FORTE_MODULE_POWERLINK_LIB_DIR}/EplStack/EplTgtConio.c)

forte_add_include_directories(${FORTE_MODULE_POWERLINK_TINYXML_DIR})
forte_add_sourcefile_with_path_hcpp(
        ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinystr
        ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinyxml
)

forte_add_sourcefile_with_path_cpp(
        ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinyxmlerror.cpp
        ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinyxmlparser.cpp
)

forte_add_definition(-DCONFIG_POWERLINK_USERSTACK)
