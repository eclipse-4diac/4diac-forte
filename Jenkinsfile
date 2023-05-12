/*******************************************************************************
 * Copyright (c) 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/

pipeline {
  agent {
    kubernetes {
      label 'build'
      yaml """
apiVersion: v1
kind: Pod
spec:
  containers:
  - name: build
    image: quay.io/repository/eclipse-4diac/4diac-fortebuildcontainer:latest
    command:
    - cat
    tty: true
"""
    }
  }
  parameters {
    string defaultValue: 'refs/heads/develop', name: 'GERRIT_REFSPEC'
  }
  stages {
    stage('Build noNamespace') {
      steps {
        container('build') {
          cmakeBuild
            generator: 'Unix Makefiles',
            buildDir: 'noNamespace',
            sourceDir: 'source',
            cmakeArgs: '-DCMAKE_BUILD_TYPE=Debug -DFORTE_ARCHITECTURE=Posix -DFORTE_COM_ETH=ON -DFORTE_LOGLEVEL=LOGDEBUG -DFORTE_MODULE_UTILS=ON -DFORTE_COM_OPC_UA=ON -DFORTE_COM_OPC_UA_INCLUDE_DIR=${WORKDIR}/open62541/binShared -DFORTE_COM_OPC_UA_LIB_DIR=${WORKDIR}/open62541/binShared/bin -DFORTE_COM_OPC_UA_LIB=libopen62541.so'
        }
      }
    }
    stage('Build multicast') {
      steps {
        container('build') {
          cmakeBuild
            generator: 'Unix Makefiles',
            buildDir: 'multicast',
            sourceDir: 'source',
            cmakeArgs: '-DCMAKE_BUILD_TYPE=Debug -DFORTE_ARCHITECTURE=Posix -DFORTE_COM_ETH=ON -DFORTE_MODULE_UTILS=ON -DFORTE_LOGLEVEL=LOGDEBUG -DFORTE_COM_OPC_UA=ON -DFORTE_COM_OPC_UA_INCLUDE_DIR=${WORKDIR}/open62541/binStatic -DFORTE_COM_OPC_UA_LIB_DIR=${WORKDIR}/open62541/binStatic/bin -DFORTE_COM_OPC_UA_LIB=libopen62541.a -DFORTE_COM_OPC_UA_MULTICAST=ON'
        }
      }
    }
    stage('Build static') {
      steps {
        container('build') {
          cmakeBuild
            generator: 'Unix Makefiles',
            buildDir: 'static',
            sourceDir: 'source',
            cmakeArgs: '-DCMAKE_BUILD_TYPE=Debug -DFORTE_EventChainExternalEventListSize=20 -DFORTE_ARCHITECTURE=Posix -DFORTE_COM_ETH=ON -DFORTE_COM_FBDK=ON -DFORTE_COM_LOCAL=ON -DFORTE_COM_RAW=ON -DFORTE_SYSTEM_TESTS=ON -DFORTE_MODULE_CONVERT=ON -DFORTE_MODULE_IEC61131=ON -DFORTE_MODULE_UTILS=ON -DFORTE_LOGLEVEL=LOGDEBUG -DFORTE_COM_OPC_UA=ON -DFORTE_COM_OPC_UA_INCLUDE_DIR=${WORKDIR}/open62541/binStatic -DFORTE_COM_OPC_UA_LIB_DIR=${WORKDIR}/open62541/binStatic/bin -DFORTE_COM_OPC_UA_LIB=libopen62541.a -DFORTE_USE_TEST_CONFIG_IN_FORTE=ON'
        }
      }
    }
    stage('Build staticEncrypted') {
      steps {
        container('build') {
          cmakeBuild
            generator: 'Unix Makefiles',
            buildDir: 'staticEncrypted',
            sourceDir: 'source',
            cmakeArgs: '-DCMAKE_BUILD_TYPE=Debug -DFORTE_USE_STD_11=ON -DFORTE_RTTI_AND_EXCEPTIONS=ON -DFORTE_EventChainExternalEventListSize=20 -DFORTE_ARCHITECTURE=Posix -DFORTE_COM_ETH=ON -DFORTE_COM_FBDK=ON -DFORTE_COM_LOCAL=ON -DFORTE_COM_RAW=ON -DFORTE_TESTS=ON -DFORTE_SYSTEM_TESTS=ON -DFORTE_MODULE_CONVERT=ON -DFORTE_MODULE_IEC61131=ON -DFORTE_MODULE_UTILS=ON -DFORTE_LOGLEVEL=LOGDEBUG -DFORTE_COM_OPC_UA=ON -DFORTE_COM_OPC_UA_INCLUDE_DIR=${WORKDIR}/open62541/binStaticEncrypted -DFORTE_COM_OPC_UA_LIB_DIR=${WORKDIR}/open62541/binStaticEncrypted/bin -DFORTE_COM_OPC_UA_LIB=libopen62541.a -DFORTE_COM_OPC_UA_ENCRYPTION=ON -DFORTE_USE_TEST_CONFIG_IN_FORTE=OFF -DFORTE_COM_HTTP=ON -DFORTE_IO=ON -DFORTE_MODULE_RT_Events=ON -DFORTE_MODULE_RECONFIGURATION=ON'
        }
      }
    }
    stage('Test static') {
      steps {
        container('build') {
          ctest
            workingDir: 'static',
            arguments: '--verbose'
        }
      }
    }
    stage('Test staticEncrypted') {
      steps {
        container('build') {
          ctest
            workingDir: 'staticEncrypted',
            arguments: '--verbose'
        }
      }
    }
  }
}
