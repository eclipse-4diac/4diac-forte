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
#pragma once
#include <Arp/System/Core/Arp.h>
#include <Arp/Plc/Esm/IProgramProvider.hpp>

using namespace Arp;
using namespace Arp::Plc::Esm;

namespace ForteLibrary{

  class ForteComponentProgramProvider : public IProgramProvider {
    public:
      ForteComponentProgramProvider(void) = default;
      ForteComponentProgramProvider(const ForteComponentProgramProvider& paArg) = delete;
      ~ForteComponentProgramProvider(void) = default;

      ForteComponentProgramProvider& operator= (const ForteComponentProgramProvider& paArg) = delete;

      IProgram* CreateProgram(const String& paProgramName, const String& paProgramType) override;
   };

} // end of namespace ForteLibrary
