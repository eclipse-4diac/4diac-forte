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
#include "ForteIOHandler.h"

#include "Arp/System/Commons/Logging.h"
#include "Arp/System/Core/ByteConverter.hpp"

namespace ForteLibrary {
  namespace ForteComponent {

    ForteIOHandler::ForteIOHandler(const String& paName) : ProgramBase(paName){
       //TODO AddPortInfo
    }

    void ForteIOHandler::Execute(){
       //TODO implement program
    }

  } // end of namespace ForteComponent
} // end of namespace ForteLibrary
