# *******************************************************************************
# Copyright (c) 2015, 2016 fortiss GmbH This program and the accompanying
# materials are made available under the terms of the Eclipse Public License
# 2.0 which is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Alois Zoitl - initial API and implementation and/or initial
# documentation *    Jose Cabral - added fileWriter_fbt
# *******************************************************************************/

if (NOT FORTE_ARCHITECTURE STREQUAL "Posix")
    return()
endif ()

forte_register_module(LMS_EV3
                      OFF
                      "I/O support for Lego Mindstorms EV3 using the ev3_dev operating system"
)

# ############################################################################
# LMS_EV3
# ############################################################################
forte_set_process_interface("Lego Mindstorms EV3"
                            IX
                            QX
                            IW
                            QW
                            ID
                            QD
)
add_library(forte-modules-lms_ev3 OBJECT
            processinterface.h processinterface.cpp fileWriter_fbt.h
            fileWriter_fbt.cpp fileReader_fbt.h fileReader_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-lms_ev3)
