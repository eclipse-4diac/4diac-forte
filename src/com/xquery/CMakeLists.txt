# *********************************************************************
# Copyright (c) 2017 fortiss GmbH
# *
# * This program and the accompanying materials are made
# * available under the terms of the Eclipse Public License 2.0
# * which is available at https://www.eclipse.org/legal/epl-2.0/
# *
# * SPDX-License-Identifier: EPL-2.0
# **********************************************************************

#############################################################################
# Xquery SIFBs
#############################################################################
forte_add_network_layer(XqueryClient OFF "xquery" CXqueryClientLayer xqueryClientLayer "client to send xqueries")

if(FORTE_COM_XqueryClient)

set(OPENSSL_INCLUDE_DIR "" CACHE PATH "OpenSSL Include Directory")
set(OPENSSL_LIBRARY_DIR "" CACHE PATH "OpenSSL Library Directory")
set(FORTE_BASEX_SRC_DIR "" CACHE PATH "Path to BaseX library directory")

forte_add_include_directories(${OPENSSL_INCLUDE_DIR})
forte_add_link_directories(${OPENSSL_LIBRARY_DIR})
#ifdef WIN32
forte_add_link_library(libssl.a)
forte_add_link_library(libcrypto.a)
#else
forte_add_link_library(libssl.a -lpthread -ldl)
forte_add_link_library(libcrypto.a -lpthread -ldl)
#endif
forte_add_include_directories(${FORTE_BASEX_SRC_DIR})
forte_add_sourcefile_with_path_hc(${FORTE_BASEX_SRC_DIR}/readstring ${FORTE_BASEX_SRC_DIR}/md5 ${FORTE_BASEX_SRC_DIR}/basexdbc)

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
forte_add_sourcefile_hcpp(xqueryHandler xqueryClientLayer)
forte_add_handler(CXqueryHandler xqueryHandler)

endif(FORTE_COM_XqueryClient)

#forte_add_subdirectory(testFBs)
