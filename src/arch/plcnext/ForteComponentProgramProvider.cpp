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
