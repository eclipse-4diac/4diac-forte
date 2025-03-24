#*******************************************************************************
# Copyright (c) 2025 Jose Cabral
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Jose Cabral
# *    - initial API and implementation and/or initial documentation
# *******************************************************************************/

if(FORTE_TRACE_CTF)
  forte_add_sourcefile_cpp(bt2MessageFactory.cpp)
  forte_add_sourcefile_cpp(TraceReaderSink.cpp)
  forte_add_link_library(babeltrace2)
  forte_add_sourcefile_hcpp(deviceReplayer resourceReplayer utils)
endif()