# *******************************************************************************
# Copyright (c) 2016 fortiss GmbH and Herwig Eichler, www.conmeleon.org This
# program and the accompanying materials are made available under the terms of
# the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Alois Zoitl and Herwig Eichler
# *******************************************************************************/

if (NOT FORTE_ARCHITECTURE STREQUAL "Posix")
    return()
endif ()

forte_register_module(CONMELEON_C1 OFF
                      "Interacting with the CONMELEON C1 IO board"
)

message(CHECK_START
        "Module Conmeleon C1 is deprecated and may be removed in future releases"
)

# ############################################################################
# CONMELEON C1 IO board driver
# ############################################################################

forte_set_process_interface("Conmeleon C1" IX QX IW)

add_library(forte-modules-conmeleon_c1 OBJECT
            gpio/gpiopin.h
            gpio/gpiopin.cpp
            spi/spidevice.h
            spi/spidevice.cpp
            spi/ads1018.h
            spi/ads1018.cpp
            util/fileres.h
            util/fileres.cpp
            util/uncopyable.h
            processinterface.h
            processinterface.cpp
)
target_link_libraries(forte-modules-conmeleon_c1 PUBLIC forte-core)
target_link_libraries(forte PUBLIC forte-modules-conmeleon_c1)
