/*******************************************************************************
 * Copyright (c) 2023 Primetals Technolgies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - migrated from posix main
 *******************************************************************************/
#include <ros/ros.h>
#include <develog.h>

void rosStartupHook(int argc, char *arg[]){
  if(argc <= 1){ //! Default Value (localhost:61499)
    std::string rosdistro = "indigo";
    if (rosdistro == (std::string)std::getenv("ROS_DISTRO")){
      DEVLOG_INFO("path to forte.exe: %s \n", arg[0]);
      ros::init(argc, arg, "ros_Functionblocks_in_FORTE");
    }
  }
}
