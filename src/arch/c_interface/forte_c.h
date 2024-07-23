/*******************************************************************************
 * Copyright (c) 2018 - 2024 fortiss GmbH, Jose Cabral
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_ARCH_FORTE_C_H_
#define SRC_ARCH_FORTE_C_H_

#ifndef FORTE_SHARED_PREFIX
  #define FORTE_SHARED_PREFIX
#endif

#ifdef __cplusplus
extern "C" {
#endif

  typedef enum FORTE_STATUS{
    FORTE_OK = 0,
    FORTE_DEVICE_ALREADY_STARTED,
    FORTE_WRONG_ENDIANESS,
    FORTE_WRONG_PARAMETERS,
    FORTE_ARCHITECTURE_NOT_READY,
    FORTE_COULD_NOT_CREATE_DEVICE,
  } FORTE_STATUS;

  typedef void* TForteInstance;

  /**
   * \brief Start forte instance
   * @param paPort The port on which forte will listen. Use 0 for default 61499
   * @param paResultInstance Address  to store the created forte instance
   * @return FORTE_OK if no error occurred, other values otherwise
   */
  FORTE_SHARED_PREFIX FORTE_STATUS forteStartInstance(unsigned int paPort, TForteInstance* paResultInstance);

  /**
   * \brief Start forte instance with posibilities of more arguments
   * @param argc Number of arguments in arg
   * @param argv Arguments
   * @param paResultInstance Address  to store the created forte instance
   * @return FORTE_OK if no error occurred, other values otherwise
   */
  FORTE_SHARED_PREFIX FORTE_STATUS forteStartInstanceGeneric(int argc, char *argv[], TForteInstance* paResultInstance);

  /**
   * \brief Request termination of a Forte instance
   * @param paInstance Instance to request for termination
   */
  FORTE_SHARED_PREFIX void forteRequestStopInstance(TForteInstance paInstance);

  /**
   * \brief Waits indefinitely for the intance to stop and deletes it
   * @param paInstance Instance to terminate
  */
  FORTE_SHARED_PREFIX void forteWaitForInstanceToStop(TForteInstance paInstance);

  /**
   * \brief Initializes the architecture. Prepare all resources needed by the Forte's instances. Must be called once before the first Forte instance is started
   * @param argc Number of arguments in arg
   * @param argv Arguments
   * @return 0 if no error occurred, other values otherwise. Any error value is provided by the architecture and not by forte
   */
  FORTE_SHARED_PREFIX int forteGlobalInitialize(int argc, char *argv[]);

  /**
   * \brief Deinitializes the architecture. Frees all resources used by Forte's instances. Must be called after the last instance is ended
   * @return 0 if no error occurred, other values otherwise.  Any error value is provided by the architecture and not by forte
   */
  FORTE_SHARED_PREFIX int forteGlobalDeinitialize();

#ifdef __cplusplus
}
#endif

#endif /* SRC_ARCH_FORTE_C_H_ */
