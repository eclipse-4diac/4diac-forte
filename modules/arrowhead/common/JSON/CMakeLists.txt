# *******************************************************************************
# Copyright (c) 2018, fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Jose Cabral - initial implementation
# *******************************************************************************

# ############################################################################
# Common Arrowhead specific functional blocks
# ############################################################################

target_sources(forte-arrowhead PRIVATE
            ANYToJSON_fbt.h ANYToJSON_fbt.cpp ArrowheadJSONHelper.h
            ArrowheadJSONHelper.cpp GetArrayResponseFromJSON_fbt.h
            GetArrayResponseFromJSON_fbt.cpp
)
