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
#pragma once
#include <Arp/System/Core/Arp.h>
#include <Arp/System/Acf/ComponentBase.hpp>
#include <Arp/System/Acf/IApplication.hpp>
#include <Arp/Plc/Esm/IProgramComponent.hpp>

#include "ForteComponentProgramProvider.h"


using namespace Arp;
using namespace Arp::System::Acf;
using namespace Arp::Plc::Esm;

namespace ForteLibrary{

  class ForteComponent : public ComponentBase, public IProgramComponent{
    public:
      static IComponent::Ptr Create(IApplication& paApplication, const String& paComponentName);

      ForteComponent(IApplication& paApplication, const String& paName);
      virtual ~ForteComponent() = default;

      void Initialize() override;
      void LoadSettings(const String& paSettingsPath) override;
      void SetupSettings() override;
      void SubscribeServices() override;
      void LoadConfig() override;
      void SetupConfig() override;
      void ResetConfig() override;
      void PublishServices() override;
      void Dispose() override;
      void PowerDown() override;

      IProgramProvider* GetProgramProvider() override;

      ForteComponent(const ForteComponent& paArg) = delete;
      ForteComponent& operator= (const ForteComponent& paArg) = delete;

    private:
        ForteComponentProgramProvider mProgramProvider;

        C4diacFORTEInstance m4diacForteInstance;
  };

  ///////////////////////////////////////////////////////////////////////////////
  // inline methods of class ForteComponent
  inline ForteComponent::ForteComponent(IApplication& paApplication, const String& paName)
                        : ComponentBase(paApplication, paName, ComponentCategory::Custom, Version(0)), mDev(0){
  }

  inline IComponent::Ptr ForteComponent::Create(IApplication& paApplication, const String& paComponentName){
      return IComponent::Ptr(new ForteComponent(paApplication, paComponentName));
  }

  inline IProgramProvider* ForteComponent::GetProgramProvider(){
      return &mProgramProvider;
  }

} // end of namespace ForteLibrary
