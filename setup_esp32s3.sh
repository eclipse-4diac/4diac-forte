#!/bin/bash
echo "----------------------------------------------------------------------------"
echo " Automatically set up development environment for FreeRTOS-platform on ESP32-S3"
echo "----------------------------------------------------------------------------"
echo ""
echo " Includes 64bit-datatypes, float-datatypes, Ethernet-Interface,"
echo " ASN1-encoding, ..."
echo ""
echo " To include tests set directories for boost-test-framework and "
echo " set FORTE_TESTS-option to 'ON'"
echo ""
echo "----------------------------------------------------------------------------"

export forte_bin_dir="bin/esp32s3"
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
    -DCMAKE_TOOLCHAIN_FILE="${FORTE_BUILDSUPPORT_DIRECTORY}/toolchains/toolchain-esp32s3.cmake" \
    -DFORTE_ARCHITECTURE=FreeRTOSLwIP \
    -DCMAKE_C_FLAGS_DEBUG="-g ${CMAKE_C_FLAGS}" \
    -DCMAKE_C_FLAGS_MINSIZEREL="-Os -DNDEBUG ${CMAKE_C_FLAGS}" \
    -DCMAKE_C_FLAGS_RELEASE="-O3 -DNDEBUG ${CMAKE_C_FLAGS}" \
    -DCMAKE_C_FLAGS_RELWITHDEBINFO="-O2 -g -DNDEBUG ${CMAKE_C_FLAGS}" \
    -DCMAKE_CXX_FLAGS_DEBUG="-g ${CMAKE_CXX_FLAGS}" \
    -DCMAKE_CXX_FLAGS_MINSIZEREL="-Os -DNDEBUG ${CMAKE_CXX_FLAGS}" \
    -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG ${CMAKE_CXX_FLAGS}" \
    -DCMAKE_CXX_FLAGS_RELWITHDEBINFO="-O2 -g -DNDEBUG ${CMAKE_CXX_FLAGS}" \
    -DCMAKE_INSTALL_PREFIX=../../../Application/lib/esp32s3 \
    -DCMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES="${PWD}/../../../Application/build_ESP32S3_dual_can/config/" \
    -DFORTE_BUILD_EXECUTABLE=OFF \
    -DFORTE_EventChainExternalEventListSize=256 \
    -DFORTE_EventChainEventListSize=512 \
    -DFORTE_MODULE_ISOBUS_UT=ON \
    -DFORTE_MODULE_ISOBUS_PGN=ON \
    -DFORTE_MODULE_ISOBUS_TECU=ON \
    -DFORTE_IO=ON \
    -DFORTE_MODULE_LOGIBUS_IO=ON \
    -DFORTE_MODULE_ISOBUS_UT_IO=ON \
    -DFORTE_MODULE_ISOBUS_TC_IO=ON \
    -DFORTE_MODULE_DATAPANEL_IO=ON \
    -DFORTE_MODULE_BLINKMARINE_IO=ON \
    -DFORTE_MODULE_SECOND_CAN_IO=ON \
    -DFORTE_MODULE_FUNK_IO=ON \
    -DFORTE_MODULE_STORAGE_ESP32=ON \
    -DFORTE_MODULE_STORAGE=ON \
    -DFORTE_MODULE_SIGNALPROCESSING=ON \
    -DFORTE_MODULE_ESP_ADDONS=ON \
    -DFORTE_MODULE_LOGIBUS_IO_ESP32_DIGITAL_IN=ON \
    -DFORTE_MODULE_LOGIBUS_IO_ESP32_ANALOG_IN=ON \
    -DFORTE_MODULE_LOGIBUS_IO_ESP32_DIGITAL_OUT=ON \
    -DCMAKE_BUILD_TYPE=MINSIZEREL \
    -DFORTE_LOGLEVEL=LOGINFO \
    -DFORTE_COM_ETH=ON \
    -DFORTE_COM_FBDK=ON \
    -DFORTE_COM_LOCAL=ON \
    -DFORTE_SUPPORT_BOOT_FILE=ON \
    -DFORTE_BOOT_FILE_LOCATION="/data/test_FORTE_PC.fboot" \
    -DFORTE_TESTS=OFF \
    -DFORTE_MODULE_IEC61131=ON \
    -DFORTE_MODULE_CONVERT=ON \
    -DFORTE_MODULE_UTILS=ON \
    -DFORTE_MODULE_LOGIBUS_UTILS=ON \
    -DFORTE_MODULE_ADAPTER=ON \
    -DFORTE_MODULE_LOGIBUS_SIGNALPROCESSING=ON \
    ../../
else
  echo "unable to create ${forte_bin_dir}"
  exit 1
fi
