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
# Arrowhead specific functional blocks for service registry
# ############################################################################

add_subdirectory(HTTP)
add_subdirectory(OpcUa)

target_sources(forte-arrowhead PRIVATE
            # datatype
            forte_serviceregistryentry.h
            forte_serviceregistryentry.cpp
            forte_servicequeryform.h
            forte_servicequeryform.cpp
            # Service Registry
            FieldsToServiceQueryForm.h
            FieldsToServiceQueryForm.cpp
            FieldsToServiceRegistryEntry.h
            FieldsToServiceRegistryEntry.cpp
            QueryService.h
            QueryService.cpp
            QueryServicesAdp_adp.h
            QueryServicesAdp_adp.cpp
            RegisterService.h
            RegisterService.cpp
            RegisterServiceAdp_adp.h
            RegisterServiceAdp_adp.cpp
            ServiceRegistryEntry2ServiceRegistryEntry_fbt.h
            ServiceRegistryEntry2ServiceRegistryEntry_fbt.cpp
)
