/*******************************************************************************
 * Copyright (c) 2024 
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_ARCH_COMMON_FORTE_SPECIFIC_ARCHITECTURE_H_
#define SRC_ARCH_COMMON_FORTE_SPECIFIC_ARCHITECTURE_H_

class CForteSpecificArchitecture{
  public:

    /**
     * @brief Initalize everything needed in the lower layers that is needed for forte to run
     * This is called only once before any forte instances is started
     * 
     * @param argc Number of arguments passed, same as in the main function
     * @param argv Arguments passed, same as in the main function
     * @return 0 if no error happenned, another value specificied by the specific architecture otherwise
     */
    static int initialize(int argc , char* argv[]);

    /**
     * @brief Uninitialize everything needed in the lower layers. This is called only once after all forte instances were used
     *  
     * @return 0 if no error happenned, another value specificied by the specific architecture otherwise
    */
    static int deinitialize();
};

#endif /* SRC_ARCH_COMMON_FORTE_SPECIFIC_ARCHITECTURE_H_ */
