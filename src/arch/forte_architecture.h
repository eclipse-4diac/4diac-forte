/*******************************************************************************
 * Copyright (c) 2017, 2024 fortiss GmbH, Jose Cabral
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

class CForteGeneralArchitecture{

  public:
    
    /**
     * @brief Initalize everything needed for forte once and calls the specific initialization of each architecture.
     * This is called only once before any forte instances is started. 
     * 
     * @param argc Number of arguments passed, same as in the main function
     * @param argv Arguments passed, same as in the main function
     * @return 0 if no error happenned, another value specificied by the specific architecture otherwise
     */
    static int initialize(int argc, char *argv[]);

    /**
     * @brief Uninitialize everything that was done in `initialize` and calls the specific uninitialization of each architecture
     * This is called only once after all forte instances were used.
     *  
     * @return 0 if no error happenned, another value specificied by the specific architecture otherwise
    */
    static int deinitialize();

    /**
     * @brief Checks if the lower layers were already initialized
     * 
     * @return true if the lower layers were already initialized, false otherwise.
    */
    static bool isInitialized();

  private:
    static bool mInitialized; 
};

using CForteArchitecture = CForteGeneralArchitecture;

#endif /* SRC_ARCH_FORTE_ARCHITECTURE_H_ */
