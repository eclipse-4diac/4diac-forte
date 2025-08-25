# *******************************************************************************
# * Copyright (c) 2017, 2024 fortiss GmbH, Monika Wenger, Johannes Kepler
#   University Linz *
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v2.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v20.html *
# * Contributors:
# * Johannes Messmer
# * - initial API and implementation and/or initial documentation
# * Monika Wenger, Alois Zoitl
# * - migration to current runtime
# *******************************************************************************/

forte_register_io(REVOLUTIONPI OFF "Support for Revolution Pi")

add_library(forte-modules-revolutionPi OBJECT
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
target_link_libraries(forte-modules-revolutionPi PUBLIC forte-core)
target_link_libraries(forte PUBLIC forte-modules-revolutionPi)
