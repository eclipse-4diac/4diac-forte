# *******************************************************************************
# Copyright (c) 2017 fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Johannes Messmer *   - initial API and implementation and/or
# initial documentation
# *******************************************************************************/

# ############################################################################
# IO Device
# ############################################################################

add_library(forte-core-io-device OBJECT
            io_controller.h io_controller.cpp io_controller_multi.h
            io_controller_multi.cpp io_controller_poll.h
            io_controller_poll.cpp
)
target_link_libraries(forte PRIVATE forte-core-io-device)
