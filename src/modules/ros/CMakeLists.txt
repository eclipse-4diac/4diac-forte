# Copyright (c) 2016 - 2017 fortiss GmbH
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v10.html
#
# Contributors:
#    Ben Schneider, Mathias Lüdtke
#      - initial implementation and documentation
 
#############################################################################
# Communication Layer for ROS (publish, subscribe)
#############################################################################

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#MACRO(forte_add_network_layer NAME ONOFF CONFIGNAME CLASSNAME FILENAME DISCRIPTION) ==> from buildsupport/forte.cmake
forte_add_network_layer(ROS OFF "ros" CROSLayer ROSLayer "Layer for providing and receiving data to/from ROS via publish/subscribe")

forte_add_definition("-std=c++11") # execute action server needs c++11

#############################################################################
# ROS Publish and Subscribe FBs
#############################################################################

forte_add_module(ROS "Function Blocks to communicate with ROS (topics, services, actions)")
forte_add_sourcefile_hcpp(ROSManager)
forte_add_sourcefile_hcpp(EXECUTE_ACTION_CLIENT)
forte_add_sourcefile_hcpp(EXECUTE_ACTION_SERVER)
forte_add_sourcefile_hcpp(TRIGGER_SERVICE_CLIENT)
forte_add_sourcefile_hcpp(TRIGGER_SERVICE_SERVER)

#forte_add_sourcefile_hcpp(TopLevelCoordinatorBasicFB)

find_package(catkin COMPONENTS actionlib reapp_msgs roscpp std_msgs std_srvs topic_tools)
if (catkin_FOUND)
    forte_add_include_directories(${catkin_INCLUDE_DIRS})
else()
    forte_add_link_directories(/opt/ros/indigo/lib/)
    forte_add_include_directories(opt/ros/indigo/lib/include)
    set(${catkin_LIBRARIES}
        -lroscpp
        -lrosconsole
        -lroscpp_serialization
        -ltopic_tools
        -lactionlib
        -lreapp_msgs
        -lstd_msgs
        -lstd_srvs

        -lrostime
        -lboost_system
        -lboost_thread
    )
endif(catkin_FOUND)
forte_add_link_library(${catkin_LIBRARIES})
