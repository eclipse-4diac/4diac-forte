/*******************************************************************************
 * Copyright (c) 2018 Johannes Kepler University
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "ForteComponent.h"

namespace ForteLibrary {

  void ForteComponent::Initialize(){
  }

  void ForteComponent::LoadSettings(const String& /*paSettingsPath*/){
  }

  void ForteComponent::SetupSettings(){
  }

  void ForteComponent::SubscribeServices(){
  }

  void ForteComponent::LoadConfig(){
  }

  void ForteComponent::SetupConfig(){
    CDevice::startupNewDevice(pIpPort);
  }

  void ForteComponent::ResetConfig(){
  }

  void ForteComponent::PublishServices(){
  }

  void ForteComponent::Dispose(){
  }

  void ForteComponent::PowerDown(){
    CDevice::triggerDeviceShutdown();
  }

} // end of namespace ForteLibrary
