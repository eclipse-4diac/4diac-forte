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
#include <Arp/Plc/Esm/ProgramBase.hpp>
#include <Arp/System/Commons/Logging.h>

namespace ForteLibrary {
  namespace ForteComponent {

    using namespace Arp;
    using namespace Arp::Plc::Esm;

    class ForteIOHandler : public ProgramBase, private Loggable<ForteIOHandler>{
      public:
        explicit ForteIOHandler(const String& paName);
        ForteIOHandler(const ForteIOHandler& paArg) = delete;
        virtual ~ForteIOHandler(void) = default;

        ForteIOHandler& operator=(const ForteIOHandler& paArg) = delete;

        void Execute(void)override;

    };
  } // end of namespace ForteComponent
} // end of namespace ForteLibrary
