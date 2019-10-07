/************************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Milan Vathoopan - initial API and implementation and/or initial documentation
 ************************************************************************************/

#ifndef SRC_ARCH_FREERTOS_FORTE_INIT_H_
#define SRC_ARCH_FREERTOS_FORTE_INIT_H_

/* When moving this file to the outside of the architecture,
 * the corresponding defines for exporting in windows or other platform
 * must be added here
 *
 */
#ifndef FORTE_SHARED_PREFIX
# define FORTE_SHARED_PREFIX
#endif

#ifndef FORTE_SHARED_CALL
# define FORTE_SHARED_CALL
#endif

#ifdef __cplusplus
extern "C" {
#endif

  enum FORTE_STATUS {
    FORTE_OK,
    FORTE_DEVICE_ALREADY_STARTED,
    FORTE_WRONG_ENDIANESS,
    FORTE_WRONG_PARAMETERS,
    FORTE_ARCHITECTURE_NOT_READY,
  };

  typedef void* TForteInstance;

  /**
   * \brief Start forte instance
   * @param paPort The port on which to forte will listen. Use 0 for default (normally 61499)
   * @param paResultInstance Address of an instance of forte where the new instance is stored
   * @return FORTE_OK if no error occurred, other values otherwise
   */
  FORTE_SHARED_PREFIX int FORTE_SHARED_CALL forteStartInstance(unsigned int paPort, TForteInstance* paResultInstance);

  /**
   * \brief Start forte instance with possibilities of more arguments
   * @param paArgc Number of arguments in arg
   * @param paArgv Arguments
   * @param paResultInstance Address of an instance of forte where the new instance is stored
   * @return FORTE_OK if no error occurred, other values otherwise
   */
  FORTE_SHARED_PREFIX int FORTE_SHARED_CALL forteStartInstanceGeneric(int paArgc, char *paArgv[], TForteInstance* paResultInstance);

  /**
   * \brief Terminates a Forte instance
   * @param paSignal  Signal value to terminate instance
   * @param paInstance Instance to terminate
   */
  FORTE_SHARED_PREFIX void FORTE_SHARED_CALL forteStopInstance(int paSignal, TForteInstance paInstance);

  /**
   * \brief Terminates a Forte instance
   * @param paInstance Instance to terminate
   */
  FORTE_SHARED_PREFIX void FORTE_SHARED_CALL forteJoinInstance(TForteInstance paInstance);

  /**
   * \brief Initializes the architecture. Prepare all resources needed by the Forte's instances. Must be called once before the first Forte instance is started
   */
  FORTE_SHARED_PREFIX void FORTE_SHARED_CALL forteGlobalInitialize(void);

  /**
   * \brief Deinitializes the architecture. Frees all resources used by Forte's instances. Must be called after the last instance is ended
   */
  FORTE_SHARED_PREFIX void FORTE_SHARED_CALL forteGlobalDeinitialize(void);

#ifdef __cplusplus
}
#endif

#endif /* SRC_ARCH_FREERTOS_FORTE_INIT_H_ */
