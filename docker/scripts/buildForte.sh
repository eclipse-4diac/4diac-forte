#!/bin/bash
#*******************************************************************************************
# Copyright (c) 2020 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Jose Cabral
#       -  initial implementation
# ******************************************************************************************

# exit when any command fails
set -e

# store the the last executed command
trap 'lastCommand=$currentCommand; currentCommand=$BASH_COMMAND' DEBUG
# echo an error message before exiting
trap 'if [ $? -eq 0 ]; then echo \"The script exited without errors\"; else echo "\"${lastCommand}\" command failed with exit code $?."; fi' EXIT


BASH_SCRIPT_PATH="$(cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

source "${BASH_SCRIPT_PATH}"/prepareOtherRepos.sh "$@"

####################################################################
#####################  Variables ###################################
####################################################################
# - FORTE_REPO_PATH: The forte repository is set to the parent folder of this script. If the script is executed to another repository, 
# set the FORTE_REPO_PATH variable to the absolut path of the desired forte repository before calling this script
# - FORTE_OUTPUT_DIR: The output directory where all devices folders with their binaries will be placed. Default /usr/forte_output/

if [ -z "$FORTE_REPO_PATH" ]; then
  FORTE_REPO_PATH="$(cd ${BASH_SCRIPT_PATH} && cd ../../ && pwd )/"
fi

if [ -z "$FORTE_OUTPUT_DIR" ]; then
  FORTE_OUTPUT_DIR=/usr/forte_output/
fi

COMMON_FLAGS="-DCMAKE_BUILD_TYPE=Debug \
              -DFORTE_LOGLEVEL=Debug \
              -DFORTE_BUILD_EXECUTABLE=ON \
              -DFORTE_COM_ETH=ON \
              -DFORTE_MODULE_UTILS=ON \
              -DFORTE_MODULE_CONVERT=ON \
              -DFORTE_MODULE_IEC61131=ON \
              -DFORTE_MODULE_Arrowhead=ON \
              -DFORTE_COM_HTTP=ON \
              -DFORTE_COM_HTTP_LISTENING_PORT=8080 \
              ${USER_FLAGS} \
              -DFORTE_EXTERNAL_MODULES_DIRECTORY=${FORTE_EXTERNAL_MODULES}"

ARM_COMPILER="-DCMAKE_C_COMPILER=${ARM_BIN}/arm-linux-gnueabihf-gcc \
              -DCMAKE_CXX_COMPILER=${ARM_BIN}/arm-linux-gnueabihf-g++ \
              -DCMAKE_FIND_ROOT_PATH=${ARM_ROOT} "

####################################################################
#####################  FUNCTIONS ###################################
####################################################################

# The function takes 3 parameters: 
#   - the architecture (linux32, arm, ...) used for the needed libraries
#   - the device (linux32, rpi, BBB, ...) used for the exported folder under ${FORTE_OUTPUT_DIR}
#   - extra parameteters to be added to CMake
function compile(){

  CURRENT_ARCH=$1
  CURRENT_DEVICE=$2
  EXTRA_FLAGS=$3

  CURRENT_BIN="${FORTE_REPO_PATH}bin/${CURRENT_DEVICE}"
  CURRENT_OUTPUT="${FORTE_OUTPUT_DIR}${CURRENT_DEVICE}"
  CURRENT_LUA_LIBS="${REPOS_DIR}${LUA_FOLDER_NAME}_bin/${CURRENT_ARCH}/lib/"

  if [ "${OPCUA_ENABLED}" -eq "1" ]; then
    OPCUA_FLAGS="-DFORTE_COM_OPC_UA=ON \
               -DFORTE_COM_OPC_UA_INCLUDE_DIR=${REPOS_DIR}${OPCUA_FOLDER_NAME}/bin/${CURRENT_ARCH}/ \
               -DFORTE_COM_OPC_UA_LIB_DIR=${REPOS_DIR}${OPCUA_FOLDER_NAME}/bin/${CURRENT_ARCH}/bin \
               -DFORTE_COM_OPC_UA_LIB=libopen62541.a \
               -DFORTE_COM_OPC_UA_MASTER_BRANCH=ON \
               -DFORTE_COM_OPC_UA_ENCRYPTION=ON \
               -DFORTE_COM_OPC_UA_ENCRYPTION_INCLUDE_DIR=${REPOS_DIR}${MBEDTLS_FOLDER_NAME}/include \
               -DFORTE_COM_OPC_UA_ENCRYPTION_LIB_DIR=${REPOS_DIR}${MBEDTLS_FOLDER_NAME}/bin/${CURRENT_ARCH}/library/"
  else
    OPCUA_FLAGS=""
  fi
  
  if [ "${MQTT_ENABLED}" -eq "1" ]; then
    MQTT_FLAGS=" -DFORTE_COM_PAHOMQTT=ON \
                 -DFORTE_COM_PAHOMQTT_INCLUDE_DIR=${REPOS_DIR}${MQTT_FOLDER_NAME}/src \
                 -DFORTE_COM_PAHOMQTT_LIB=libpaho-mqtt3a-static.a \
                 -DFORTE_COM_PAHOMQTT_LIB_DIR=${REPOS_DIR}${MQTT_FOLDER_NAME}/bin/${CURRENT_ARCH}/src"
  else
    MQTT_FLAGS="" 
  fi

  if [ "${LUA_ENABLED}" -eq "1" ]; then
    LUA_FLAGS="-DFORTE_USE_LUATYPES=LuaJIT \
               -DLUAJIT_INCLUDE_DIR=${REPOS_DIR}${LUA_FOLDER_NAME}/src \
               -DLUAJIT_LIBRARY=${CURRENT_LUA_LIBS}/libluajit-5.1.so" 
  else
    LUA_FLAGS="" 
  fi

  ALL_FLAGS="${COMMON_FLAGS} ${LUA_FLAGS} ${OPCUA_FLAGS} ${MQTT_FLAGS} ${EXTRA_FLAGS}"

  printMessage "Compiling forte for arch ${CURRENT_ARCH} and device ${CURRENT_DEVICE}"
  echo -e "\nThe following flags will be used: \n${ALL_FLAGS}\n" 

  cleanFolderAndGoThere "${CURRENT_BIN}"

  cmake ${ALL_FLAGS} ${FORTE_REPO_PATH}
  make -j8

  mkdir -p "${CURRENT_OUTPUT}"
  cp src/forte "${CURRENT_OUTPUT}"

  if [ "${LUA_ENABLED}" -eq "1" ]; then
    cp "${CURRENT_LUA_LIBS}"*.so* "${CURRENT_OUTPUT}"
  fi
}



cd "${FORTE_REPO_PATH}"

###################################################################
############# COMPILE FOR LINUX32 #################################
###################################################################
if [ "${LINUX32_ENABLED}" -eq "1" ]; then
  compile "${LINUX32_ARCH}" "${LINUX32_ARCH}" "-DFORTE_ARCHITECTURE=Posix"
fi

###################################################################
################# COMPILE FOR RPI #################################
###################################################################
if [ "${RPI_ENABLED}" -eq "1" ]; then
  RPI_FLAGS="-DFORTE_ARCHITECTURE=Posix \
             ${ARM_COMPILER} \
            -DCMAKE_CXX_FLAGS=-static-libstdc++ \
            -DFORTE_MODULE_SysFs=ON"

  compile "${ARM_ARCH}" "rpi" "${RPI_FLAGS}"
fi

###################################################################
################# COMPILE FOR EMBRICK #############################
###################################################################
if [ "${EMBRICK_ENABLED}" -eq "1" ]; then
  EMBRICK_FLAGS="-DFORTE_ARCHITECTURE=Posix \
                ${ARM_COMPILER} \
                -DFORTE_IO=ON \
                -DFORTE_IO_EMBRICK=ON"
 
  compile "${ARM_ARCH}" "embrick" "${EMBRICK_FLAGS}"
fi
