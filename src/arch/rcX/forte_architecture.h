/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_ARCH_RCX_FORTE_ARCHITECTURE_H_
#define SRC_ARCH_RCX_FORTE_ARCHITECTURE_H_

class CrcXArchitecture{

  public:
    static bool initialize(void);

    static void deinitialize(void);

    static bool isInitialized(void);
  private:

    static bool initialized; //in some architectures this might be used to avoid double initialization if this would produce an error
    static void initialize_constructors(void);

    static void initialize_destructors(void);

};

typedef CrcXArchitecture CForteArchitecture;

#endif /* SRC_ARCH_RCX_FORTE_ARCHITECTURE_H_ */
