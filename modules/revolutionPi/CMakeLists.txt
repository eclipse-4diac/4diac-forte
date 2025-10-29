#******************************************************************************
# Copyright (c) 2017, 2025 fortiss GmbH, Monika Wenger, 
#                          Johannes Kepler University Linz
# 
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v2.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v20.html *
#
#  Contributors:
#    Johannes Messmer
#      - initial API and implementation and/or initial documentation
#    Monika Wenger, Alois Zoitl
#      - migration to current runtime
# *******************************************************************************/

if (NOT LINUX)
    return()
endif()

option(FORTE_MODULE_REVOLUTIONPI "Support for Revolution Pi" OFF)

if (NOT FORTE_MODULE_REVOLUTIONPI)
    return()
endif ()

add_library(forte-revolutionPi
            lib/piControlIf.h
            lib/piControlIf.c
            RevPiController.h
            RevPiController.cpp
            RevPiHandle.h
            RevPiHandle.cpp
            fb/IORevPiCore.h
            fb/IORevPiCore.cpp
            fb/IORevPiBusAdapter.h
            fb/IORevPiBusAdapter.cpp
            fb/IORevPiDIO.h
            fb/IORevPiDIO.cpp
            fb/IORevPiAIO.h
            fb/IORevPiAIO.cpp
)
target_link_libraries(forte-revolutionPi PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-revolutionPi,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-revolutionPi>>)
install(TARGETS forte-revolutionPi EXPORT forte-export FILE_SET HEADERS)
