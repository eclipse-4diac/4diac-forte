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
