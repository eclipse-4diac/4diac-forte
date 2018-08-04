/*******************************************************************************
 * Copyright (c) 2018 Johannes Kepler University
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "ForteComponent.h"

namespace ForteLibrary {

  void ForteComponent::Initialize(void){
    mDev = new RMT_DEV;
  }

  void ForteComponent::LoadSettings(const String& /*paSettingsPath*/){
  }

  void ForteComponent::SetupSettings(void){
  }

  void ForteComponent::SubscribeServices(void){
  }

  void ForteComponent::LoadConfig(void){
  }

  void ForteComponent::SetupConfig(void){
    mDev->setMGR_ID("localhost:61499");
    mDev->startDevice();
  }

  void ForteComponent::ResetConfig(void){
  }

  void ForteComponent::PublishServices(void){
  }

  void ForteComponent::Dispose(void){
    delete mDev;
    mDev = 0;
  }

  void ForteComponent::PowerDown(void){
    if(0 != mDev){
      mDev->changeFBExecutionState(cg_nMGM_CMD_Kill);
    }
  }

} // end of namespace ForteLibrary
