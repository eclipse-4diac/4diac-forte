#############################################################################
# Communication Layer for ROS (publish, subscribe)
#############################################################################

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#MACRO(forte_add_network_layer NAME ONOFF CONFIGNAME CLASSNAME FILENAME DISCRIPTION) ==> from buildsupport/forte.cmake
forte_add_network_layer(ROS OFF "ros" CROSLayer ROSLayer "Layer for providing and receiving data to/from ROS via publish/subscribe")

#forte_add_definition("-std=c++11")

#############################################################################
# ROS Publish and Subscribe FBs
#############################################################################

message("[FORTE] CMake variable FORTE_ARCHITECTURE: ${FORTE_ARCHITECTURE}")
message("[FORTE] CMake variable FORTE_COM_ROS: ${FORTE_COM_ROS}")
message("[FORTE] CMake variable FORTE_MODULE_ROS: ${FORTE_MODULE_ROS}")
message("[FORTE] CMake variable FORTE_TRACE_EVENTS: ${FORTE_TRACE_EVENTS}")
#message("[FORTE] CMake variable ros com layer: ${FORTE_TRACE_EVENTS}")
#message("[FORTE] CMake variable ros com layer: ${FORTE_COM_ROS}")
#message("[FORTE] CMake variable ros com layer: ${FORTE_COM_ROS}")
#message("[FORTE] CMake variable ros com layer: ${FORTE_COM_ROS}")
#message("[FORTE] CMake variable ros com layer: ${FORTE_COM_ROS}")
#message("[FORTE] CMake variable ros com layer: ${FORTE_COM_ROS}")

forte_add_module(ROS "Function Blocks to communicate with ROS (publish/subscribe)")
forte_add_sourcefile_hcpp(ROSManager)
forte_add_sourcefile_hcpp(ServiceCallManager)
forte_add_sourcefile_hcpp(TRIGGER_SERVICE_CLIENT)
forte_add_sourcefile_hcpp(TRIGGER_SERVICE_SERVER)
forte_add_sourcefile_hcpp(EXECUTE_ACTION_CLIENT)
forte_add_sourcefile_hcpp(EXECUTE_ACTION_SERVER)

#forte_add_sourcefile_hcpp(TopLevelCoordinatorBasicFB)

find_package(catkin QUIET COMPONENTS actionlib reapp_msgs roscpp std_msgs std_srvs topic_tools)
if (catkin_FOUND)
    forte_add_include_directories(${catkin_INCLUDE_DIRS})
else()
    forte_add_link_directories(/opt/ros/indigo/lib)
    forte_add_include_directories(/opt/ros/indigo/include)

    set(catkin_LIBRARIES
        -lroscpp
        -lrosconsole
        -lroscpp_serialization
        -ltopic_tools
        -lactionlib

        -lrostime
        -lboost_system
        -lboost_thread
    )
endif(catkin_FOUND)

forte_add_link_library(${catkin_LIBRARIES})
