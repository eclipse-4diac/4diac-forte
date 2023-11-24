# Hints for building 4diac FORTE for Zephyr OS

In general, the same instructions as for the other target platforms apply. You can build via cmake-gui or use the setup_zephyr.sh script.

The easiest integration into a Zephyr application can be achieved by adding 4diac FORTE to the app's West / CMake build process. This has the
important benefit that you don't have to discover and set all the compiler paths, includes, flags, by yourself.

4diac FORTE requires Zephyr OS version 3.5 or later.

## Integrating 4diac FORTE into a Zephyr app

In your app's CMakeLists.txt, add the following snippet:

```
zephyr_get_compile_definitions_for_lang_as_string(CXX cxx_definitions)
zephyr_get_compile_options_for_lang_as_string(CXX cxx_options)
zephyr_get_include_directories_for_lang_as_string(CXX cxx_includes)
zephyr_get_system_include_directories_for_lang_as_string(CXX cxx_system_includes)

set(external_project_cxxflags
	"${cxx_definitions} ${cxx_options} ${cxx_includes} ${cxx_system_includes} -Wno-error"
)

include(ExternalProject)

set(forte_src_dir ${CMAKE_CURRENT_SOURCE_DIR}/lib/org.eclipse.4diac.forte)
set(FORTE_LIB_DIR ${forte_src_dir}/bin/zephyr/src)

ExternalProject_Add(forte
	SOURCE_DIR ${forte_src_dir}
	BINARY_DIR ${forte_src_dir}/bin/zephyr
	CONFIGURE_COMMAND
		cmake -G "Ninja"
			-DCMAKE_C_COMPILER_WORKS=ON
			-DCMAKE_CXX_COMPILER_WORKS=ON
			-DNULLPTR_SUPPORTED=ON
			-DCMAKE_SYSTEM_NAME=Generic
			-DFORTE_ARCHITECTURE=Zephyr
			-DFORTE_TESTS=OFF
			-DFORTE_SUPPORT_BOOT_FILE=ON
			-DFORTE_MODULE_CONVERT=ON
			-DFORTE_BUILD_EXECUTABLE=OFF
			-DFORTE_BUILD_STATIC_LIBRARY=ON
			-DFORTE_MODULE_IEC61131=ON
			-DCMAKE_C_FLAGS_DEBUG="-g"
			-DCMAKE_BUILD_TYPE=MINSIZEREL
			-DFORTE_LOGLEVEL=NOLOG
			-DFORTE_COM_ETH=ON
			-DFORTE_COM_FBDK=ON
			-DFORTE_COM_LOCAL=ON
			-DFORTE_MODULE_UTILS=ON
			-DFORTE_SOURCE_DIR=${forte_src_dir}
			-DFORTE_EXTERNAL_MODULES_DIRECTORY=${forte_src_dir}/ExportedFBs
			-DFORTE_MODULE_EXTERNAL_PIDS=ON
			-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
			-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
			-DCMAKE_CXX_FLAGS=${external_project_cxxflags}
			../../
	BUILD_COMMAND
		ninja
	BUILD_BYPRODUCTS ${FORTE_LIB_DIR}/libforte-static.a
	INSTALL_COMMAND ""
)

add_dependencies(forte zephyr_interface)

add_library(forte_lib STATIC IMPORTED GLOBAL)
add_dependencies(forte_lib forte)
set_target_properties(forte_lib PROPERTIES IMPORTED_LOCATION ${FORTE_LIB_DIR}/libforte-static.a)
target_link_libraries(app PUBLIC forte_lib)
```

Adapt the value of `forte_src_dir` to your actual FORTE source tree location.

In your app's `prj.conf`, turn on the required C++ support:

```
# For C++ standard library support
CONFIG_COMPILER_OPT="-Werror"
CONFIG_POSIX_CLOCK=y
CONFIG_CPP=y
CONFIG_GLIBCXX_LIBCPP=y
CONFIG_STD_CPP20=y

# Dynamic thread creation for 4diac FORTE
CONFIG_DYNAMIC_THREAD=y
CONFIG_DYNAMIC_THREAD_ALLOC=y

CONFIG_INIT_STACKS=y
CONFIG_THREAD_STACK_INFO=y
CONFIG_KERNEL_MEM_POOL=y
CONFIG_HEAP_MEM_POOL_SIZE=64000

# TCP/IP networking for 4diac FORTE
CONFIG_NETWORKING=y
CONFIG_NET_CONFIG_SETTINGS=y
CONFIG_NET_L2_ETHERNET=y
CONFIG_NET_IPV6=y
CONFIG_NET_IPV4=y

CONFIG_POSIX_MAX_FDS=12
CONFIG_NET_MAX_CONTEXTS=16
CONFIG_NET_SOCKETS=y
CONFIG_NET_SOCKETS_POSIX_NAMES=y

CONFIG_NET_ARP=y
CONFIG_NET_UDP=y
CONFIG_NET_TCP=y

# LittleFS support
CONFIG_FLASH=y
CONFIG_FLASH_MAP=y
CONFIG_FLASH_PAGE_LAYOUT=y

CONFIG_FILE_SYSTEM=y
CONFIG_FILE_SYSTEM_LITTLEFS=y
```

