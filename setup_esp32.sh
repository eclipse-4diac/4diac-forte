#!/bin/bash
echo "----------------------------------------------------------------------------"
echo " Automatically set up development environment for POSIX-platform"
echo "----------------------------------------------------------------------------"
echo ""
echo " Includes 64bit-datatypes, float-datatypes, Ethernet-Interface,"
echo " ASN1-encoding, ..."
echo ""
echo " To include tests set directories for boost-test-framework and "
echo " set FORTE_TESTS-option to 'ON'"
echo ""
echo "----------------------------------------------------------------------------"

export forte_bin_dir="bin/esp32"
export FORTE_BUILDSUPPORT_DIRECTORY="${PWD}/buildsupport"

#set to boost-include directory
export forte_boost_test_inc_dirs=""
#set to boost-library directory
export forte_boost_test_lib_dirs=""

if [ ! -d "$forte_bin_dir" ]; then
  mkdir -p "$forte_bin_dir"
fi

if [ -d "$forte_bin_dir" ]; then
  
  echo "For building forte go to $forte_bin_dir and execute \"make\""
  echo "forte can be found at ${forte_bin_dir}/src"
  echo "forte_tests can be found at ${forte_bin_dir}/tests"
  
  
  cd "./$forte_bin_dir"
  
  cmake -G "Unix Makefiles" \
    -DCMAKE_TOOLCHAIN_FILE="${FORTE_BUILDSUPPORT_DIRECTORY}/toolchains/toolchain-esp32-basic.cmake" \
    -DCMAKE_C_FLAGS_DEBUG="-g ${CMAKE_C_FLAGS}" \
    -DCMAKE_C_FLAGS_MINSIZEREL="-Os -DNDEBUG ${CMAKE_C_FLAGS}" \
    -DCMAKE_C_FLAGS_RELEASE="-O3 -DNDEBUG ${CMAKE_C_FLAGS}" \
    -DCMAKE_C_FLAGS_RELWITHDEBINFO="-O2 -g -DNDEBUG ${CMAKE_C_FLAGS}" \
    -DCMAKE_CXX_FLAGS_DEBUG="-g ${CMAKE_CXX_FLAGS}" \
    -DCMAKE_CXX_FLAGS_MINSIZEREL="-Os -DNDEBUG ${CMAKE_CXX_FLAGS}" \
    -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG ${CMAKE_CXX_FLAGS}" \
    -DCMAKE_CXX_FLAGS_RELWITHDEBINFO="-O2 -g -DNDEBUG ${CMAKE_CXX_FLAGS}" \
    -DFORTE_LINKED_STRINGDICT=OFF \
    -DESP32_SDK_CONFIG_DIR="../../../Application/build/config/" \
    -DFORTE_FREERTOS_ALLOC="SPIRAM" \
    -DFORTE_FREERTOS_MINIMAL_STACK_SIZE=15000 \
    -DFORTE_BUILD_EXECUTABLE=OFF \
    -DFORTE_BUILD_STATIC_LIBRARY=ON \
    -DFORTE_IO=ON \
    -DFORTE_IO_COMMON_IO=ON \
    -DFORTE_MODULE_ESP32=ON \
    -DFORTE_MODULE_signalprocessing=ON \
    -DFORTE_MODULE_esp-addons=ON \
    -DFORTE_MODULE_HUTSCHIENENMOPED=ON \
    -DFORTE_MODULE_ESP32_DIGITAL_IN=ON \
    -DFORTE_MODULE_ESP32_DIGITAL_OUT=ON \
    -DCMAKE_BUILD_TYPE=MINSIZEREL \
    -DFORTE_LOGLEVEL=LOGINFO \
    -DFORTE_COM_ETH=ON \
    -DFORTE_COM_FBDK=ON \
    -DFORTE_COM_LOCAL=ON \
    -DFORTE_SUPPORT_BOOT_FILE=ON \
    -DFORTE_BootfileLocation="/data/test_FORTE_PC.fboot" \
    -DFORTE_TESTS=OFF \
    -DFORTE_MODULE_IEC61131=ON \
    -DFORTE_MODULE_CONVERT=ON \
    -DFORTE_MODULE_UTILS=ON \
    -DFORTE_USE_LUATYPES=Lua \
    -DLUA_INCLUDE_DIR="../../Application/components/lua/src" \
    ../../
else
  echo "unable to create ${forte_bin_dir}"
  exit 1
fi
