# Indicate that this is a cross compiled unix-like system
#
set(UNIX 1)
set(CMAKE_CROSSCOMPILING TRUE)
#
# Library prefixes, suffixes, extra libs.
#
set(CMAKE_LINK_LIBRARY_SUFFIX "")
set(CMAKE_STATIC_LIBRARY_PREFIX "lib")     # lib
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")      # .a

set(CMAKE_SHARED_LIBRARY_PREFIX "lib")     # lib
set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")     # .so
set(CMAKE_EXECUTABLE_SUFFIX ".vxe")        # .vxe

#set(CMAKE_DL_LIBS "dl")

#set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
set(CMAKE_FIND_LIBRARY_PREFIXES "lib")


#ignor build host paths
set(CMAKE_SYSTEM_IGNORE_PATH
  /lib             /lib64             /include
  /usr/lib         /usr/lib64         /usr/include
  /usr/local/lib   /usr/local/lib64   /usr/local/include
  /usr/X11/lib     /usr/X11/lib64     /usr/X11/include
  /usr/lib/X11     /usr/lib64/X11     /usr/include/X11
  /usr/X11R6/lib   /usr/X11R6/lib64   /usr/X11R6/include
  /usr/X11R7/lib   /usr/X11R7/lib64   /usr/X11R7/include
)

set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>" )
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_CXX_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>" )