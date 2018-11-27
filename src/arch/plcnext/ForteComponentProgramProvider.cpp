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

#include "ForteComponentProgramProvider.h"
#include "ForteIOHandler.h"

namespace ForteLibrary {

  IProgram* ForteComponentProgramProvider::CreateProgram(const String& paProgramName, const String& paProgramType) {
      if (paProgramType == "ForteIOHandler"){
          return new ForteComponent::ForteIOHandler(paProgramName);
      }
      return nullptr;
  }

} // end of namespace ForteLibrary
