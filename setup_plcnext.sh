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

export forte_bin_dir="../build/plcnext"

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
  
  cmake -G "Eclipse CDT4 - Unix Makefiles" -D TOOLCHAIN_ROOT=/opt/pxc/2.2.1 \
    -D CMAKE_TOOLCHAIN_FILE=$1/axcf2152.cmake \
    -DFORTE_ARCHITECTURE=PLCnext -DFORTE_COM_ETH=ON -DFORTE_COM_FBDK=ON -DFORTE_COM_LOCAL=ON -DFORTE_TESTS=OFF \
    -DFORTE_MODULE_CONVERT=ON -DFORTE_MODULE_IEC61131=ON -DFORTE_MODULE_UTILS=ON ../../org.eclipse.4diac.forte
else
  echo "unable to create ${forte_bin_dir}"
  exit 1
fi
