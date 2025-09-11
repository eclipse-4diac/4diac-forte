# ############################################################################
# Copyright (c) 2018 fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Ben Schneider - initial API and implementation and/or initial
# documentation
# ############################################################################

# ############################################################################
# ROS Example Publish and Subscribe FBs
# ############################################################################

if (NOT FORTE_ARCHITECTURE STREQUAL "Posix")
    return()
endif ()

option(FORTE_MODULE_ROS "Function Blocks to communicate with ROS (publish/subscribe)" OFF)

if (NOT FORTE_MODULE_ROS)
    return()
endif ()

target_compile_definitions(forte-global INTERFACE FORTE_ROS)

# ############################################################################
# Communication Layer for ROS (publish, subscribe)
# ############################################################################

option(FORTE_COM_ROS "Layer for providing and receiving data to/from ROS via publish/subscribe" ON)

find_package(catkin QUIET COMPONENTS actionlib reapp_msgs roscpp std_msgs
                                     std_srvs topic_tools
)
if (catkin_FOUND)
    target_link_libraries(forte PUBLIC ${catkin_LIBRARIES})
else ()
    target_link_libraries(forte
                          PUBLIC roscpp
                                 rosconsole
                                 roscpp_serialization
                                 topic_tools
                                 actionlib
                                 rostime
                                 boost_system
                                 boost_thread
    )
endif (catkin_FOUND)

add_library(forte-ros
            $<$<BOOL:${FORTE_COM_ROS}>:ROSLayer.cpp>
            $<$<BOOL:${FORTE_COM_ROS}>:ROSLayer.h>
            ROSManager.h
            ROSManager.cpp
            ServiceCallManager.h
            ServiceCallManager.cpp
            TRIGGER_SERVICE_CLIENT.h
            TRIGGER_SERVICE_CLIENT.cpp
            TRIGGER_SERVICE_SERVER.h
            TRIGGER_SERVICE_SERVER.cpp
            EXECUTE_ACTION_CLIENT.h
            EXECUTE_ACTION_CLIENT.cpp
            EXECUTE_ACTION_SERVER.h
            EXECUTE_ACTION_SERVER.cpp
            startup.cpp
)
target_link_libraries(forte-ros PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<LINK_LIBRARY:WHOLE_ARCHIVE,forte-ros>)
target_include_directories(forte-ros PUBLIC ${catkin_INCLUDE_DIRS})
target_include_directories(forte-ros PUBLIC /opt/ros/indigo/include)
target_link_directories(forte-ros PUBLIC /opt/ros/indigo/lib)
