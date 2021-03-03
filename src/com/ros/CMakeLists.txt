#############################################################################
#  Copyright (c) 2018 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
#  Contributors:
#    Ben Schneider - initial API and implementation and/or initial documentation
#############################################################################

#############################################################################
# ROS Example Publish and Subscribe FBs
#############################################################################

forte_add_module(ROS OFF "Function Blocks to communicate with ROS (publish/subscribe)")

if (FORTE_MODULE_ROS)
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

  forte_add_definition("-DFORTE_ROS")

  #############################################################################
  # Communication Layer for ROS (publish, subscribe)
  #############################################################################

  forte_add_handler(CROSManager ROSManager)
  forte_add_handler(CServiceCallManager ServiceCallManager)


  forte_add_network_layer(ROS ON "ros" CROSLayer ROSLayer "Layer for providing and receiving data to/from ROS via publish/subscribe")

  forte_add_sourcefile_hcpp(ROSManager)
  forte_add_sourcefile_hcpp(ServiceCallManager)
  forte_add_sourcefile_hcpp(TRIGGER_SERVICE_CLIENT)
  forte_add_sourcefile_hcpp(TRIGGER_SERVICE_SERVER)
  forte_add_sourcefile_hcpp(EXECUTE_ACTION_CLIENT)
  forte_add_sourcefile_hcpp(EXECUTE_ACTION_SERVER)


  find_package(catkin QUIET COMPONENTS actionlib reapp_msgs roscpp std_msgs std_srvs topic_tools)
  if (catkin_FOUND)
      forte_add_include_directories(${catkin_INCLUDE_DIRS})
  else()
      forte_add_link_directories(/opt/ros/indigo/lib)
      forte_add_include_system_directories(/opt/ros/indigo/include)

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

endif(FORTE_MODULE_ROS)
