/*******************************************************************************
 * Copyright (c) 2016 -2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef _FMU_CONFIG_H_
#define _FMU_CONFIG_H_

#include <fmi2Functions.h>
#include <fmi2FunctionTypes.h>
#include <fmi2TypesPlatform.h>

#define MODEL_GUID "FORTE_FMU" //Should be defined in the boot file

#define LOG_ALL             0
#define LOG_CALL            1
#define LOG_ERROR           2

#define NUMBER_OF_LOG_CATEGORIES 3

extern fmi2String loggingNames[NUMBER_OF_LOG_CATEGORIES];

typedef enum {
  STATE_START_END             = 1,
  STATE_INSTANTIATED          = 2,
  STATE_INITIALIZATION_MODE   = 4,
  STATE_STEP_COMPLETE         = 8,
  STATE_STEP_IN_PROGRESS      = 16,
  STATE_STEP_FAILED           = 32,
  STATE_STEP_CANCELED         = 64,
  STATE_TERMINATED            = 128,
  STATE_ERROR                 = 256,
  STATE_FATAL                 = 512,
} coSimulationState;

typedef enum {
  FMI2_GET_TYPES_PLATFORM = 0,
  FMI2_GET_VERSION,
  FMI2_SET_DEBUG_LOGGING,
  FMI2_INSTANTIATE,
  FMI2_FREE_INSTANCE,
  FMI2_SETUP_EXPERIMENT,
  FMI2_ENTER_INITIALIZATION_MODE,
  FMI2_EXIT_INITIALIZATION_MODE,
  FMI2_TERMINATE,
  FMI2_RESET,
  FMI2_GET_REAL,
  FMI2_GET_INTEGER,
  FMI2_GET_BOOLEAN,
  FMI2_GET_STRING,
  FMI2_SET_REAL,
  FMI2_SET_INTEGER,
  FMI2_SET_BOOLEAN,
  FMI2_SET_STRING,
  FMI2_GET_FMUSTATE,
  FMI2_SET_FMUSTATE,
  FMI2_FREE_FMUSTATE,
  FMI2_SERIALIZED_FMUSTATE_SIZE,
  FMI2_SERIALIZE_FMUSTATE,
  FMI2_DE_SERIALIZE_FMUSTATE,
  FMI2_GET_DIRECTIONAL_DERIVATIVE,
  FMI2_SET_REAL_INPUT_DERIVATIVES,
  FMI2_GET_REAL_OUTPUT_DERIVATIVES,
  FMI2_DO_STEP,
  FMI2_CANCEL_STEP,
  FMI2_GET_STATUS,
  FMI2_GET_REAL_STATUS,
  FMI2_GET_INTEGER_STATUS,
  FMI2_GET_BOOLEAN_STATUS,
  FMI2_GET_STRING_STATUS
} coSimulationFunctions;

//Allowed function calls for each state
const unsigned int allowedStatesInFunction[] = {
    STATE_START_END | STATE_INSTANTIATED | STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE |
      STATE_STEP_IN_PROGRESS | STATE_STEP_FAILED | STATE_STEP_CANCELED | STATE_TERMINATED | STATE_ERROR,   //FMI2_GET_TYPES_PLATFORM
    STATE_START_END | STATE_INSTANTIATED | STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE |
      STATE_STEP_IN_PROGRESS | STATE_STEP_FAILED | STATE_STEP_CANCELED | STATE_TERMINATED | STATE_ERROR,   //FMI2_GET_VERSION
    STATE_INSTANTIATED | STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE | STATE_STEP_IN_PROGRESS |
      STATE_STEP_FAILED | STATE_STEP_CANCELED | STATE_TERMINATED | STATE_ERROR,                            //FMI2_SET_DEBUG_LOGGING
    STATE_START_END,                                                                                       //FMI2_INSTANTIATE
    STATE_INSTANTIATED | STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE | STATE_STEP_FAILED |
      STATE_STEP_CANCELED | STATE_TERMINATED | STATE_ERROR,                                                //FMI2_FREE_INSTANCE
    STATE_INSTANTIATED,                                                                                    //FMI2_SETUP_EXPERIMENT
    STATE_INSTANTIATED,                                                                                    //FMI2_ENTER_INITIALIZATION_MODE
    STATE_INITIALIZATION_MODE,                                                                             //FMI2_EXIT_INITIALIZATION_MODE
    STATE_STEP_COMPLETE | STATE_STEP_FAILED,                                                               //FMI2_TERMINATE
    STATE_INSTANTIATED | STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE | STATE_STEP_FAILED |
      STATE_STEP_CANCELED | STATE_TERMINATED | STATE_ERROR,                                                //FMI2_RESET
    STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE | STATE_STEP_FAILED |
      STATE_STEP_CANCELED | STATE_TERMINATED | STATE_ERROR,                                                //FMI2_GET_REAL
    STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE | STATE_STEP_FAILED |
      STATE_STEP_CANCELED | STATE_TERMINATED | STATE_ERROR,                                                //FMI2_GET_INTEGER
    STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE | STATE_STEP_FAILED |
      STATE_STEP_CANCELED | STATE_TERMINATED | STATE_ERROR,                                                //FMI2_GET_BOOLEAN
    STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE | STATE_STEP_FAILED |
      STATE_STEP_CANCELED | STATE_TERMINATED | STATE_ERROR,                                                //FMI2_GET_STRING
    STATE_INSTANTIATED | STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE,                                  //FMI2_SET_REAL
    STATE_INSTANTIATED | STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE,                                  //FMI2_SET_INTEGER
    STATE_INSTANTIATED | STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE,                                  //FMI2_SET_BOOLEAN
    STATE_INSTANTIATED | STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE,                                  //FMI2_SET_STRING
    STATE_INSTANTIATED | STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE | STATE_STEP_FAILED |
      STATE_STEP_CANCELED | STATE_TERMINATED | STATE_ERROR,                                                //FMI2_GET_FMUSTATE
    STATE_INSTANTIATED | STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE | STATE_STEP_FAILED |
      STATE_STEP_CANCELED | STATE_TERMINATED | STATE_ERROR,                                                //FMI2_SET_FMUSTATE
    STATE_INSTANTIATED | STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE | STATE_STEP_FAILED |
      STATE_STEP_CANCELED | STATE_TERMINATED | STATE_ERROR,                                                //FMI2_FREE_FMUSTATE
    STATE_INSTANTIATED | STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE | STATE_STEP_FAILED |
      STATE_STEP_CANCELED | STATE_TERMINATED | STATE_ERROR,                                                //FMI2_SERIALIZED_FMUSTATE_SIZE
    STATE_INSTANTIATED | STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE | STATE_STEP_FAILED |
      STATE_STEP_CANCELED | STATE_TERMINATED | STATE_ERROR,                                                //FMI2_SERIALIZE_FMUSTATE
    STATE_INSTANTIATED | STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE | STATE_STEP_FAILED |
      STATE_STEP_CANCELED | STATE_TERMINATED | STATE_ERROR,                                                //FMI2_DE_SERIALIZE_FMUSTATE
    STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE | STATE_STEP_FAILED | STATE_STEP_CANCELED |
      STATE_TERMINATED | STATE_ERROR,                                                                      //FMI2_GET_DIRECTIONAL_DERIVATIVE
    STATE_INSTANTIATED | STATE_INITIALIZATION_MODE | STATE_STEP_COMPLETE,                                  //FMI2_SET_REAL_INPUT_DERIVATIVES
    STATE_STEP_COMPLETE | STATE_STEP_FAILED | STATE_STEP_CANCELED | STATE_TERMINATED | STATE_ERROR,        //FMI2_GET_REAL_OUTPUT_DERIVATIVES
    STATE_STEP_COMPLETE,                                                                                   //FMI2_DO_STEP
    STATE_STEP_IN_PROGRESS,                                                                                //FMI2_CANCEL_STEP
    STATE_STEP_COMPLETE | STATE_STEP_IN_PROGRESS | STATE_STEP_FAILED | STATE_TERMINATED,                   //FMI2_GET_STATUS
    STATE_STEP_COMPLETE | STATE_STEP_IN_PROGRESS | STATE_STEP_FAILED | STATE_TERMINATED,                   //FMI2_GET_REAL_STATUS
    STATE_STEP_COMPLETE | STATE_STEP_IN_PROGRESS | STATE_STEP_FAILED | STATE_TERMINATED,                   //FMI2_GET_INTEGER_STATUS
    STATE_STEP_COMPLETE | STATE_STEP_IN_PROGRESS | STATE_STEP_FAILED | STATE_TERMINATED,                   //FMI2_GET_BOOLEAN_STATUS
    STATE_STEP_COMPLETE | STATE_STEP_IN_PROGRESS | STATE_STEP_FAILED | STATE_TERMINATED,                   //FMI2_GET_STRING_STATUS
};

#define LOG_FMU(instance, status, categoryIndex, message, ...)                                                  \
  if (categoryIndex < NUMBER_OF_LOG_CATEGORIES                                                                  \
        && (instance->getLoggingCategories()[categoryIndex] || instance->getLoggingCategories()[LOG_ALL])) {    \
      instance->getCallbackFunctions()->logger(instance->getCallbackFunctions()->componentEnvironment,          \
           instance->getInstanceName().getValue(), status, loggingNames[categoryIndex], message, ##__VA_ARGS__);\
  }


#define ASSERT_STATE(instance, function)                                                                        \
  if(!(instance->getState() & allowedStatesInFunction[function])) {                                             \
    instance->setState(STATE_ERROR);                                                                            \
    LOG_FMU(instance, fmi2Error, LOG_ERROR, "The function cannot be called in the given state")                 \
    return fmi2Error;                                                                                           \
  }

#define ASSERT_STATE_NO_RETURN(instance, function)                                                              \
  if(!(instance->getState() & allowedStatesInFunction[function])) {                                             \
    instance->setState(STATE_ERROR);                                                                            \
    LOG_FMU(instance, fmi2Error, LOG_ERROR, "The function cannot be called in the given state")                 \
    return;                                                                                                     \
  }


#define ENTRY_FUNCTION(function)                                                                                \
    fmuInstance* componentInstance = static_cast<fmuInstance*>(c);                                              \
    if(NULL == c) {return fmi2Error;}                                                                           \
    ASSERT_STATE(componentInstance, function)                                                                   \

#define ENTRY_FUNCTION_NO_RETURN(function)                                                                      \
    fmuInstance* componentInstance = static_cast<fmuInstance*>(c);                                              \
    if(NULL == c) {return;}                                                                                     \
    ASSERT_STATE_NO_RETURN(componentInstance, function)                                                         \

#define NOT_USED(var)                                                                                           \
  (void)var;

#ifdef FMU_DEBUG
#  include <forte_sync.h>
#  include <sstream>
#  include "fmuInstance.h"
#  define FMU_DEBUG_LOG(instance, message)                                                                      \
  {                                                                                                             \
    std::stringstream ss;                                                                                       \
    ss << message;                                                                                              \
    (instance)->printToFile(ss.str().c_str());                                                                    \
  }
#else
# define FMU_DEBUG_LOG(instance, message)
#endif /*FMU_DEBUG*/

#define GET_FMU_INSTANCE_FROM_COMM_LAYER()   \
    GET_FMU_INSTANCE_FROM_FB(this->getCommFB())

#define GET_FMU_INSTANCE_FROM_FB(fb) \
  ((fmuInstance*)&((fb)->getResource().getDevice()))

#endif /* _FMU_CONFIG_H_ */
