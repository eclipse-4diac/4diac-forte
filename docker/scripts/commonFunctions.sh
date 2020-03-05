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

function printHelp(){
  echo "Usage: buildAll.sh [-A] [-M] [-r] [-e] [-x] [-o] [-m] [-l] [-h] [-U ARGS]"
  echo -e "\t-A Enable all devices"
  echo -e "\t-M Enable all modules"
  echo -e "\t-r Enable raspberry pi device"
  echo -e "\t-e Enable embrick device"
  echo -e "\t-x Enable linux32 device"
  echo -e "\t-o Enable opcua module"
  echo -e "\t-m Enable mqtt module"
  echo -e "\t-l Enable lua module"
  echo -e "\t-U ARG Pass user arguments to the compilation"
  echo -e "\t-h Print this message"
}


OPCUA_ENABLED=0
MQTT_ENABLED=0
LUA_ENABLED=0
LINUX32_ENABLED=0
RPI_ENABLED=0
EMBRICK_ENABLED=0
ARM_ENABLED=0 # If rpi or embrick is enabled, this is also enabled. It helps with clarity when getting the tools and compiling the needed modules
USER_FLAGS=""

while getopts ":AMrexomlhU:" opt; do
  case ${opt} in
    A ) 
       LINUX32_ENABLED=1
       RPI_ENABLED=1
       EMBRICK_ENABLED=1
       ARM_ENABLED=1
       ;;
    M ) 
       OPCUA_ENABLED=1
       MQTT_ENABLED=1
       LUA_ENABLED=1
       ;;
    r )
       RPI_ENABLED=1
       ARM_ENABLED=1
       ;;  
    e )
       EMBRICK_ENABLED=1
       ARM_ENABLED=1
       ;;     
    x )
       LINUX32_ENABLED=1
       ;;
    o ) 
       OPCUA_ENABLED=1
       ;;
    m ) 
       MQTT_ENABLED=1
       ;;
    l ) 
       LUA_ENABLED=1
       ;;
    h )  printHelp
         exit 1
       ;;
    U ) 
        USER_FLAGS="${OPTARG}"
       ;;
    \? )
         printHelp
         exit 1
       ;;
  esac
done

##################################
# Print message in a nice format #
#   $1: message to be printed    #    
##################################
function printMessage(){
  SIZE_OF_MESSAGE=$((${#1} + 4))
  WRAPPER=$(printf '#%.0s' $(eval "echo {1.."$(($SIZE_OF_MESSAGE))"}"))
  echo -e "\n$WRAPPER"
  printf '# %s #\n' "$1"
  echo -e "$WRAPPER\n"
}

#################################
# Clean folder and go inside it #
#   $1: Folder to clean         # 
#################################
function cleanFolderAndGoThere(){
  if [ -d "${1}" ];
  then
     rm -r "${1}"
  fi
  
  mkdir -p "${1}"
  cd "${1}"
}
