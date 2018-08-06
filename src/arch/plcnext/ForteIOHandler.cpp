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
