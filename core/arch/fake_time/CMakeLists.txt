#*******************************************************************************
# Copyright (c) 2022 Primetals Technologies Austria GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Christoph Binder - initial implementation
#******************************************************************************

option(FORTE_FAKE_TIME "Forte configuration for using simulated progression of time" FALSE)
mark_as_advanced(FORTE_FAKE_TIME)
if(NOT FORTE_FAKE_TIME)
  return()
endif(NOT FORTE_FAKE_TIME)

target_sources(forte-core PRIVATE
    forte_architecture_time.cpp
    faketimerha.cpp
)

target_compile_definitions(forte-core PUBLIC FORTE_FAKE_TIME)
