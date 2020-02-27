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

source ${BASH_SCRIPT_PATH}/commonFunctions.sh "$@"

####################################################################
#####################  Variables ###################################
####################################################################

# The following variables are used in the script and can be set before
# calling the script:
#   - TOOLS_DIR: absolut path where the needed tools will be installed. Default /usr/tools/
#   - ARM_TOOLS_NAME: folder name inside the TOOLS_DIR where the arm-bcm2708 folder is installed. Default armTools
#   - REPOS_DIR: absolut path to the folder containing all repostories. Default /usr/repos/
#   - MBEDTLS_FOLDER_NAME: name of the folder containing the mbetdls repository. Default mbedtls
#   - OPCUA_FOLDER_NAME: name of the folder containing the open62541 repository. Default open62541
#   - MQTT_FOLDER_NAME: name of the folder containing the paho mqtt repository. Default paho.mqtt.c
#   - LUA_FOLDER_NAME: name of the folder containing the luajit repository. Default luajit-2.0
#   - LINUX32_ARCH: name for the build folder when compiling for linux32 architecture. Default linux32
#   - ARM_ARCH: name for the build folder when compiling for arm architecture. Default arm
#   - CLEAN_ALL: if set to 1, it will compile even if the libraries are found. Default 0

if [ -z "$TOOLS_DIR" ]; then
  TOOLS_DIR=/usr/tools/
fi

if [ -z "$REPOS_DIR" ]; then
  REPOS_DIR=/usr/repos/
fi

if [ -z "$ARM_TOOLS_NAME" ]; then
  ARM_TOOLS_NAME=armTools
fi

if [ -z "$MBEDTLS_FOLDER_NAME" ]; then
  MBEDTLS_FOLDER_NAME=mbedtls
fi

if [ -z "$OPCUA_FOLDER_NAME" ]; then
  OPCUA_FOLDER_NAME=open62541
fi

if [ -z "$MQTT_FOLDER_NAME" ]; then
  MQTT_FOLDER_NAME=paho.mqtt.c
fi

if [ -z "$LUA_FOLDER_NAME" ]; then
  LUA_FOLDER_NAME=luajit-2.0
fi

if [ -z "$LINUX32_ARCH" ]; then
  LINUX32_ARCH=linux32
fi

if [ -z "$ARM_ARCH" ]; then
  ARM_ARCH=arm
fi

ARM_BIN="${TOOLS_DIR}${ARM_TOOLS_NAME}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin"
ARM_ROOT="${TOOLS_DIR}${ARM_TOOLS_NAME}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf"
CLEAN_ALL=0

LUA_EXTRA_FLAGS=""
CMAKE_EXTRA_FLAGS=""

####################################################################
#####################  FUNCTIONS ###################################
####################################################################

#################################
# Compile mbedtls               #
#   - $1: architecture to build #
#################################
function compileMbetls(){
  if [ ${CLEAN_ALL} -eq "1" ]  || ! [ -f "${REPOS_DIR}${MBEDTLS_FOLDER_NAME}/bin/${1}/library/libmbedcrypto.a" ]; then

    printMessage "Compiling mbedtls for ${1}"
  
    cleanFolderAndGoThere "${REPOS_DIR}${MBEDTLS_FOLDER_NAME}/bin/${1}"
  
    cmake -DENABLE_TESTING=Off \
          ${CMAKE_EXTRA_FLAGS} \
          "${REPOS_DIR}${MBEDTLS_FOLDER_NAME}" && \
      make -j8
  else 
     printMessage "Library for mbedtls for $1 already found and won't be recompiled"
  fi  
}

#################################
# Compile open62541             #
#   - $1: architecture to build #
#################################
function compileOpen62541(){
  if [ ${CLEAN_ALL} -eq "1" ] || ! [ -f "${REPOS_DIR}${OPCUA_FOLDER_NAME}/bin/${1}/bin/libopen62541.a" ]; then
    printMessage "Compiling open62541 for ${1}"
  
    cleanFolderAndGoThere "${REPOS_DIR}${OPCUA_FOLDER_NAME}/bin/${1}"
  
    cmake -DBUILD_SHARED_LIBS=OFF \
          -DCMAKE_BUILD_TYPE=RelWithDebInfo \
          -DUA_ENABLE_AMALGAMATION=ON \
          -DUA_ENABLE_ENCRYPTION=ON \
          -DMBEDCRYPTO_LIBRARY="${REPOS_DIR}${MBEDTLS_FOLDER_NAME}/bin/${1}/library/libmbedcrypto.a" \
          -DMBEDTLS_LIBRARY="${REPOS_DIR}${MBEDTLS_FOLDER_NAME}/bin/${1}/library/libmbedtls.a" \
          -DMBEDX509_LIBRARY="${REPOS_DIR}${MBEDTLS_FOLDER_NAME}/bin/${1}/library/libmbedx509.a" \
          -DMBEDTLS_INCLUDE_DIRS="${REPOS_DIR}${MBEDTLS_FOLDER_NAME}/include" \
          ${CMAKE_EXTRA_FLAGS} \
          ${REPOS_DIR}${OPCUA_FOLDER_NAME} && \
      make -j8
  else 
     printMessage "Library for open62541 for $1 already found and won't be recompiled"
  fi
}

#################################
# Compile luajit                #
#   - $1: architecture to build #
#################################
function compileLuajit(){
   if [ ${CLEAN_ALL} -eq "1" ] || ! [ -f "${REPOS_DIR}${LUA_FOLDER_NAME}_bin/${1}/lib/libluajit-5.1.so" ]; then
    #We create a new folder in the repos folder to copy all files to compile.
    #We don't create inside the luajit repo since it might create problems 
    #copying recursive itself 
    CURRENT_FOLDER="${REPOS_DIR}${LUA_FOLDER_NAME}_bin/${1}"
   
    printMessage "Compiling luajit for ${1}"
  
    cleanFolderAndGoThere "${CURRENT_FOLDER}"
    
    #The later eval LUA_COMMAND helps avoid expansion problems of LUA_EXTRA_FLAGS related to the debugging information which adds single quoutes
    LUA_COMMAND="make ${LUA_EXTRA_FLAGS:+"$LUA_EXTRA_FLAGS"} PREFIX=${CURRENT_FOLDER} -j8"
    
    cp -R ${REPOS_DIR}${LUA_FOLDER_NAME}/* ./ && \
      eval "$LUA_COMMAND" && \
      make install PREFIX="${CURRENT_FOLDER}" -j8
  else 
     printMessage "Library for luajit for $1 already found and won't be recompiled"
  fi
}

#################################
# Compile paho mqtt             #
#   - $1: architecture to build #
#################################
function compileMqtt(){
   if [ ${CLEAN_ALL} -eq "1" ] || ! [ -f "${REPOS_DIR}${MQTT_FOLDER_NAME}/bin/${1}/src/libpaho-mqtt3a-static.a" ]; then
    printMessage "Compiling paho mqtt for ${1}"
  
    cleanFolderAndGoThere "${REPOS_DIR}${MQTT_FOLDER_NAME}/bin/${1}"
  
    cmake -DPAHO_ENABLE_TESTING=OFF \
          -DPAHO_BUILD_STATIC=ON \
          ${CMAKE_EXTRA_FLAGS} \
          ${REPOS_DIR}${MQTT_FOLDER_NAME} && \
    make -j8
  else 
     printMessage "Library for mqtt for $1 already found and won't be recompiled"
  fi
}

#################################
# Compile all                   #
#   - $1: architecture to build #
#################################
function compileAll(){

  if [ "${OPCUA_ENABLED}" -eq "1" ]; then
    compileMbetls "${1}"
    compileOpen62541 "${1}"
  fi

  if [ "${LUA_ENABLED}" -eq "1" ]; then
    compileLuajit "${1}"
  fi
  
  if [ "${MQTT_ENABLED}" -eq "1" ]; then
    compileMqtt "${1}"
  fi  
  
}

################################################################
#####################  TOOLS ###################################
################################################################

if [ "${ARM_ENABLED}" -eq "1" ]; then
  #####################################
  # Get ARM Compiler from repositorty #
  #####################################
  if ! [ -d "${TOOLS_DIR}${ARM_TOOLS_NAME}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin" ]
  then
      printMessage "Getting compiler for ARM"
      
      mkdir -p ${TOOLS_DIR}${ARM_TOOLS_NAME}_temp/ && \
        mkdir -p ${TOOLS_DIR}${ARM_TOOLS_NAME}/arm-bcm2708/ && \
        git clone https://github.com/raspberrypi/tools ${TOOLS_DIR}${ARM_TOOLS_NAME}_temp/ && \
        mv ${TOOLS_DIR}${ARM_TOOLS_NAME}_temp/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/ ${TOOLS_DIR}${ARM_TOOLS_NAME}/arm-bcm2708/ && \
        rm -r ${TOOLS_DIR}${ARM_TOOLS_NAME}_temp/
  fi
fi

####################################################################
#####################  REPOS  ######################################
####################################################################

########################
# Prepare repos folder #
########################

CURRENT_FOLDER="${REPOS_DIR}"

if ! [ -d "${CURRENT_FOLDER}" ]
then  
   printMessage "Creating folder to hold repositories"
   mkdir -p ${CURRENT_FOLDER}
fi 

if [ "${OPCUA_ENABLED}" -eq "1" ]; then
  ###################
  # Prepare mbedtls #
  ###################
  
  CURRENT_FOLDER="${REPOS_DIR}${MBEDTLS_FOLDER_NAME}"
  
  if ! [ -d "${CURRENT_FOLDER}" ]
  then
      printMessage "Creating folder for mbetdls repository"
     
      mkdir -p "${CURRENT_FOLDER}" && \
        git clone https://github.com/ARMmbed/mbedtls "${CURRENT_FOLDER}"
  fi
  
  cd "${CURRENT_FOLDER}" && \
    git checkout mbedtls-2.7.1 
  
  #####################
  # Prepare open62541 #
  #####################
  
  CURRENT_FOLDER="${REPOS_DIR}${OPCUA_FOLDER_NAME}"
  
  if ! [ -d "${CURRENT_FOLDER}" ]
  then
      printMessage "Creating folder for opcua repository"
      
      mkdir -p "${CURRENT_FOLDER}" && \
        git clone https://github.com/open62541/open62541.git "${CURRENT_FOLDER}"
  fi    
  
  cd "${CURRENT_FOLDER}" && \
  git checkout v1.0 && \
  git submodule update --recursive --init

fi

if [ "${LUA_ENABLED}" -eq "1" ]; then
  ##################
  # Prepare luajit #
  ##################
  
  CURRENT_FOLDER="${REPOS_DIR}${LUA_FOLDER_NAME}"
  
  if ! [ -d "${CURRENT_FOLDER}" ]
  then
      printMessage "Creating folder for lua repository"
      
      mkdir -p "${CURRENT_FOLDER}" && \
        git clone http://luajit.org/git/luajit-2.0.git "${CURRENT_FOLDER}"
  fi
      
  cd "${CURRENT_FOLDER}" && \
  git checkout v2.0 

fi

if [ "${MQTT_ENABLED}" -eq "1" ]; then
  #####################
  # Prepare paho mqtt #
  #####################
  
  CURRENT_FOLDER="${REPOS_DIR}${MQTT_FOLDER_NAME}"
  
  if ! [ -d "${CURRENT_FOLDER}" ]
  then
      printMessage "Creating folder for mqtt repository"
      
      mkdir -p "${CURRENT_FOLDER}" && \
        git clone https://github.com/eclipse/paho.mqtt.c.git "${CURRENT_FOLDER}"
  fi
  
  cd "${CURRENT_FOLDER}" && \
    git checkout v1.2.1 
fi


###################################################################
############# COMPILE FOR LINUX32 #################################
###################################################################
if [ "${LINUX32_ENABLED}" -eq "1" ]; then
  LUA_EXTRA_FLAGS=""
  CMAKE_EXTRA_FLAGS=""
  
  compileAll "${LINUX32_ARCH}"
fi

###################################################################
################# COMPILE FOR ARM #################################
###################################################################

if [ "${ARM_ENABLED}" -eq "1" ]; then
  LUA_EXTRA_FLAGS="HOST_CC='gcc -m32' CROSS=${ARM_BIN}/arm-linux-gnueabihf-"
  CMAKE_EXTRA_FLAGS="-DCMAKE_C_COMPILER=${ARM_BIN}/arm-linux-gnueabihf-gcc -DCMAKE_CXX_COMPILER=${ARM_BIN}/arm-linux-gnueabihf-g++ -DCMAKE_FIND_ROOT_PATH=${ARM_ROOT}"
  
  export LDFLAGS=-lrt #This flag is needed for mqtt. If can be added to te regular flags, it will be nicer 
  compileAll "${ARM_ARCH}"
  unset LDFLAGS
fi