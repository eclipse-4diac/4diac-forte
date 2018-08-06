
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(pikeos /opt/pikeos-4.2)

# tools root
set(tools ${pikeos}/cdk/arm/v7hf)

# search path for includes, libs etc.
#set(CMAKE_FIND_ROOT_PATH  /opt/pikeos-4.2/target/arm/v7hf/posix)
#SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


#work around https://cmake.org/Bug/view.php?id=9985
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-nostdinc++ -std=c++98")



set(CMAKE_C_COMPILER ${tools}/bin/arm_v7hf-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/arm_v7hf-g++)
set(CMAKE_CXX_LINKER ${tools}/bin/arm_v7hf-ld)
set(CMAKE_NM ${tools}/bin/arm_v7hf-nm)
set(CMAKE_OBJCOPY ${tools}/bin/arm_v7hf-objcopy)
set(CMAKE_CXX_OBJDUMP ${tools}/bin/arm_v7hf-objdump)
set(CMAKE_CXX_RANLIB ${tools}/bin/arm_v7hf-ranlib)

set(CMAKE_CXX_LINK_FLAGS "-T ${pikeos}/target/arm/v7hf/ldscript/app.ld")


# include paths
set_property(GLOBAL APPEND PROPERTY FORTE_INCLUDE_DIRECTORIES
    "${pikeos}/target/arm/v7hf/posix/include"
    "${pikeos}/target/arm/v7hf/include"
    "${pikeos}/cdk/arm/v7hf/lib/gcc/arm-unknown-eabihf/5.4.0/include"
    "${pikeos}/target/arm/v7hf/posix/include/c++98"
    "${pikeos}/target/arm/v7hf/posix/lwip/include"
    "${pikeos}/target/arm/v7hf/posix/lwip/include/opts"
)


#library paths
link_directories(
    "${pikeos}/target/arm/v7hf/posix/lwip/lib"
    "${pikeos}/target/arm/v7hf/posix/lib"
    "${pikeos}/target/arm/v7hf/posix/os/smp"
    "${pikeos}/target/arm/v7hf/lib"
    "${pikeos}/cdk/arm/v7hf/lib/gcc/arm-unknown-eabihf/5.4.0"
)
  
  forte_add_link_library("${pikeos}/target/arm/v7hf/posix/lib/cxx_init-c++98.o")
  forte_add_link_library("${pikeos}/target/arm/v7hf/posix/os/smp/crt0.o")

  forte_add_link_library("-lstdcxx-c++98")
  forte_add_link_library("-llwip")
  forte_add_link_library("-lsbuf")
  forte_add_link_library("-lm")
  forte_add_link_library("-lc")
  forte_add_link_library("-lpse51")
  forte_add_link_library("-lvm")
  forte_add_link_library("-lstand")
  forte_add_link_library("-lp4")
  forte_add_link_library("-lgcc")
