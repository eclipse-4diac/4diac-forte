/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *  Alois Zoitl - moved archictecture class to common arch folder and adapted
 *                it for generic use
 *******************************************************************************/

#ifndef SRC_ARCH_FORTE_ARCHITECTURE_H_
#define SRC_ARCH_FORTE_ARCHITECTURE_H_

class CForteArchitecture{

  public:
    static bool initialize(void);

    static void deinitialize(void);

    static bool isInitialized(void){
      return mInitialized;
    }
  private:

    static bool mInitialized; //in some architectures this might be used to avoid double initialization if this would produce an error

};

#endif /* SRC_ARCH_FORTE_ARCHITECTURE_H_ */
