/*******************************************************************************
 * Copyright (c) 2016 - 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "fmuConfig.h"
#include "fmuInstance.h"

fmi2String loggingNames[] = { "logAll", "logCalls", "logError" };

#define CHECK_REF_AND_VALUE() \
  if(nvr > 0 && (!vr || !value)) { \
    LOG_FMU(componentInstance, fmi2Error, LOG_ERROR, "One or both of the arrays vr[] and value[] were invalid.") \
    componentInstance->setState(STATE_ERROR); \
    return fmi2Error; \
  }

#define CHECK_REF_IN_IO() \
  if(vr[i] >= componentInstance->getOutputsAndInputs().size()) { \
    LOG_FMU(componentInstance, fmi2Error, LOG_ERROR, "The reference %d is out of range. The model supports up to %d.", vr[i], componentInstance->getOutputsAndInputs().size() - 1) \
    componentInstance->setState(STATE_ERROR); \
    return fmi2Error; \
  }

extern "C" {

  fmi2Status functionNotSupported(fmi2Component c, const char *functionName);
  void freeInstanceInternal(fmi2Component c);

  const char* fmi2GetTypesPlatform() {
    return fmi2TypesPlatform;
  }

  const char* fmi2GetVersion() {
    return fmi2Version;
  }

  fmi2Status fmi2SetDebugLogging(fmi2Component c, fmi2Boolean loggingOn, size_t nCategories, const fmi2String categories[]) {
    ENTRY_FUNCTION(FMI2_SET_DEBUG_LOGGING)

    for(unsigned int i = 0; i < NUMBER_OF_LOG_CATEGORIES; i++) {
      componentInstance->getLoggingCategories()[i] = fmi2False;
    }

    if(fmi2True == loggingOn) {
      if(nCategories > 0) {
        for(unsigned int i = 0; i < nCategories; i++) {
          fmi2Boolean categoryFound = fmi2False;
          for(unsigned int j = 0; j < NUMBER_OF_LOG_CATEGORIES; j++) {
            if(strcmp(loggingNames[j], categories[i]) == 0) {
              componentInstance->getLoggingCategories()[j] = fmi2True;
              categoryFound = fmi2True;
              break;
            }
          }
          if(!categoryFound) {
            componentInstance->getCallbackFunctions()->logger(componentInstance->getCallbackFunctions()->componentEnvironment,
              componentInstance->getInstanceName().getValue(), fmi2Warning, loggingNames[LOG_ERROR], "The category %s is not supported.", categories[i]);
          }
        }
      } else {
        componentInstance->getLoggingCategories()[LOG_ALL] = fmi2True;
      }
    }

    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "fmi2SetDebugLogging - GUID = %s", componentInstance->getGuid().getValue())
    return fmi2OK;
  }

  fmi2Component fmi2Instantiate(fmi2String instanceName, fmi2Type fmuType, fmi2String fmuGUID, fmi2String fmuResourceLocation,
      const fmi2CallbackFunctions* functions, fmi2Boolean visible, fmi2Boolean loggingOn) {
    NOT_USED(visible)
    fmuInstance *componentInstance = 0;

    if(!functions || !functions->logger) { //logger must be called if instantiation failed, so logger should exists
      return 0;
    }

    if(!instanceName || 0 == strlen(instanceName) || strcmp(instanceName, " ") == 0) {
      functions->logger(functions->componentEnvironment, "-", fmi2Error, loggingNames[LOG_ERROR], "The instanceName is not valid.");
      return 0;
    }

    if(fmi2CoSimulation != fmuType) { //FORTE only supports co-simulation
      functions->logger(functions->componentEnvironment, instanceName, fmi2Error, loggingNames[LOG_ERROR], "FORTE only supports co-simulation.");
      return 0;
    }

    if(!fmuGUID || 0 == strlen(fmuGUID)) {
      functions->logger(functions->componentEnvironment, instanceName, fmi2Error, loggingNames[LOG_ERROR], "The GUID is not valid.");
      return 0;
    }

    if(strcmp(fmuGUID, MODEL_GUID)) {
      functions->logger(functions->componentEnvironment, instanceName, fmi2Warning, loggingNames[LOG_ERROR],
        "The given GUID: %s is not the same as the model's GUID: %s.", fmuGUID, MODEL_GUID);
      //return 0; //TODO: MODEL_GUID is the only not fully supported characteristic by booting the instance from a file
    }

    if(!fmuResourceLocation || 0 == strlen(fmuResourceLocation)) {
      functions->logger(functions->componentEnvironment, instanceName, fmi2Error, loggingNames[LOG_ERROR], "The fmuResourceLocation string is not valid.");
      return 0;
    }

    functions->logger(functions->componentEnvironment, instanceName, fmi2OK, loggingNames[LOG_ALL], "Resource location is %s", fmuResourceLocation);

    std::string file(fmuResourceLocation);
    if(0 == file.compare(0, 6, "file:/")) {
      if(0 == file.compare(0, 8, "file:///")) {
        file = file.substr(8, std::string::npos);
      } else {
        file = file.substr(6, std::string::npos);
      }
      functions->logger(functions->componentEnvironment, instanceName, fmi2OK, loggingNames[LOG_ALL], "Resource location is now %s", file.c_str());
    } else {
      functions->logger(functions->componentEnvironment, instanceName, fmi2Error, loggingNames[LOG_ERROR],
        "The fmuResourceLocation is not valid. It should start with file:/");
    }
    if('/' != *file.rbegin()) {
      file += "/";
    }
    file += "forte.fboot";

    if(!functions->allocateMemory || !functions->freeMemory) { //stepFinished is optional, that's why is not checked.
      functions->logger(functions->componentEnvironment, instanceName, fmi2Error, loggingNames[LOG_ERROR], "Some of the callbackFunctions are not valid.");
      return 0;
    }

    componentInstance = new fmuInstance(instanceName, fmuGUID, file.c_str(), functions);
    if(!componentInstance) {
      functions->logger(functions->componentEnvironment, instanceName, fmi2Error, loggingNames[LOG_ERROR], "Error creating the instance object.");
      return 0;
    }

    if(STATE_INSTANTIATED != componentInstance->getState()) {
      freeInstanceInternal(componentInstance);
      functions->logger(functions->componentEnvironment, instanceName, fmi2Error, loggingNames[LOG_ERROR], "Loading the FB was not correct");
      return 0;
    }

    for(unsigned int i = 0; i < NUMBER_OF_LOG_CATEGORIES; i++) {
      componentInstance->getLoggingCategories()[i] = loggingOn;
    }

    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "fmi2Instantiate - GUID = %s", componentInstance->getGuid().getValue())
    return componentInstance;
  }

  void fmi2FreeInstance(fmi2Component c) {
    ENTRY_FUNCTION_NO_RETURN(FMI2_FREE_INSTANCE)
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), componentInstance->getGuid().getValue() << " - fmi2FreeInstance\n" << "--------------\n")
    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "fmi2FreeInstance - GUID = %s", componentInstance->getGuid().getValue())
    freeInstanceInternal(componentInstance);
  }

  fmi2Status fmi2SetupExperiment(fmi2Component c, fmi2Boolean toleranceDefined, fmi2Real tolerance, fmi2Real startTime, fmi2Boolean stopTimeDefined,
      fmi2Real stopTime) {
    NOT_USED(toleranceDefined)
    NOT_USED(tolerance)
    ENTRY_FUNCTION(FMI2_SETUP_EXPERIMENT)

    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), componentInstance->getGuid().getValue() << " - fmi2SetupExperiment \n" << "--------------\n")
    //toleranceDefined might be ignored in co-simulation according to FMI.
    //TODO: Check if doing this with startTime is OK. The time in the timerHandler cannot be changed because is private in the base class
    CIEC_LREAL advanceTime = startTime;
    componentInstance->advanceInstanceTime(advanceTime);

    if(stopTimeDefined) {
      componentInstance->setStopTime(stopTime);
    }
    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "fmi2SetupExperiment - GUID = %s", componentInstance->getGuid().getValue())
    return fmi2OK;
  }

  fmi2Status fmi2EnterInitializationMode(fmi2Component c) {
    ENTRY_FUNCTION(FMI2_ENTER_INITIALIZATION_MODE)
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), componentInstance->getGuid().getValue() << "fmi2EnterInitializationMode \n" << "--------------\n")
    componentInstance->setState(STATE_INITIALIZATION_MODE);
    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "fmi2EnterInitializationMode - GUID = %s", componentInstance->getGuid().getValue())
    return fmi2OK;
  }

  fmi2Status fmi2ExitInitializationMode(fmi2Component c) {
    ENTRY_FUNCTION(FMI2_EXIT_INITIALIZATION_MODE)
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), componentInstance->getGuid().getValue() << " - fmi2ExitInitializationMode \n" << "--------------\n")
    componentInstance->startInstance(); //TODO: Check where exactly this function must be called.
    componentInstance->setState(STATE_STEP_COMPLETE);
    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "fmi2ExitInitializationMode - GUID = %s", componentInstance->getGuid().getValue())
    return fmi2OK;
  }

  fmi2Status fmi2Terminate(fmi2Component c) {
    ENTRY_FUNCTION(FMI2_TERMINATE)
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), componentInstance->getGuid().getValue() << " - fmi2Terminate \n" << "--------------\n")
    componentInstance->stopInstance();
    componentInstance->setState(STATE_TERMINATED);
    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "fmi2Terminate - GUID = %s", componentInstance->getGuid().getValue())
    return fmi2OK;
  }

  fmi2Status fmi2Reset(fmi2Component c) {
    ENTRY_FUNCTION(FMI2_RESET)
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), componentInstance->getGuid().getValue() << " - fmi2Reset \n" << "--------------\n")
    componentInstance->resetInstance();
    componentInstance->setState(STATE_INSTANTIATED);
    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "fmi2Reset - GUID = %s", componentInstance->getGuid().getValue())
    return fmi2OK;
  }

  fmi2Status fmi2GetReal(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Real value[]) {
    ENTRY_FUNCTION(FMI2_GET_REAL)
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), componentInstance->getGuid().getValue() << " - fmi2GetReal \n")
    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "fmi2GetReal - GUID = %s", componentInstance->getGuid().getValue())
    CHECK_REF_AND_VALUE()

    for(unsigned int i = 0; i < nvr; i++) {
      CHECK_REF_IN_IO()
      FMU_DEBUG_LOG(static_cast<fmuInstance*>(c),
        componentInstance->getGuid().getValue() << " GetReal - vr[" << i << "] = " << vr[i] << "; and value[" << i << "] returned = ")
      value[i] = static_cast<fmi2Real>(*componentInstance->getOutputsAndInputs().at(vr[i])->getValueAsReal());
      FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), value[i] << "\n")

    }
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), "--------------\n")
    return fmi2OK;
  }

  fmi2Status fmi2GetInteger(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Integer value[]) {
    ENTRY_FUNCTION(FMI2_GET_INTEGER)
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), componentInstance->getGuid().getValue() << " - fmi2GetInteger \n")
    CHECK_REF_AND_VALUE()

    for(unsigned int i = 0; i < nvr; i++) {
      CHECK_REF_IN_IO()
      FMU_DEBUG_LOG(static_cast<fmuInstance*>(c),
        componentInstance->getGuid().getValue() << " GetInt - vr[" << i << "] = " << vr[i] << "; and the value returned is = ")
      value[i] = static_cast<fmi2Integer>(*componentInstance->getOutputsAndInputs().at(vr[i])->getValueAsInt());
      FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), value[i] << "\n")
    }
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), "--------------\n")

    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "fmi2GetInteger - GUID = %s", componentInstance->getGuid().getValue())
    return fmi2OK;
  }

  fmi2Status fmi2GetBoolean(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Boolean value[]) {
    ENTRY_FUNCTION(FMI2_GET_BOOLEAN)
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), componentInstance->getGuid().getValue() << " - fmi2GetBoolean \n")
    CHECK_REF_AND_VALUE()

    for(unsigned int i = 0; i < nvr; i++) {
      CHECK_REF_IN_IO()
      FMU_DEBUG_LOG(static_cast<fmuInstance*>(c),
        componentInstance->getGuid().getValue() << " GetBoolean - vr[" << i << "] = " << vr[i] << "; and value[" << i << "] returned is = ")
      value[i] = static_cast<fmi2Boolean>(*componentInstance->getOutputsAndInputs().at(vr[i])->getValueAsBool());
      FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), value[i] << "\n")
    }
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), "--------------\n")

    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "fmi2GetBoolean - GUID = %s", componentInstance->getGuid().getValue())
    return fmi2OK;
  }

  fmi2Status fmi2GetString(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2String value[]) {
    ENTRY_FUNCTION(FMI2_GET_STRING)
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), componentInstance->getGuid().getValue() << " - fmi2GetString \n")
    CHECK_REF_AND_VALUE()

    for(unsigned int i = 0; i < nvr; i++) {
      CHECK_REF_IN_IO()
      FMU_DEBUG_LOG(static_cast<fmuInstance*>(c),
        componentInstance->getGuid().getValue() << " GetString - vr[" << i << "] = " << vr[i] << "; and value[" << i << "] returned is = ")
      value[i] = static_cast<fmi2String>(componentInstance->getOutputsAndInputs().at(vr[i])->getValueAsString()->getValue());
      FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), value[i] << "\n")
    }
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), "--------------\n")

    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "fmi2GetString - GUID = %s", componentInstance->getGuid().getValue())
    return fmi2OK;
  }

  fmi2Status fmi2SetReal(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Real value[]) {
    ENTRY_FUNCTION(FMI2_SET_REAL)
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), componentInstance->getGuid().getValue() << " - fmi2SetReal \n")
    CHECK_REF_AND_VALUE()

    for(unsigned int i = 0; i < nvr; i++) {
      CHECK_REF_IN_IO()
      FMU_DEBUG_LOG(static_cast<fmuInstance*>(c),
        componentInstance->getGuid().getValue() << " SetReal - vr[" << i << "] = " << vr[i] << "; and value[" << i << "] to set is = " << value[i] << "\n")
      CIEC_LREAL valToWrite(static_cast<TForteDFloat>(value[i]));
      componentInstance->getOutputsAndInputs()[vr[i]]->setValue(valToWrite);
    }
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), "-----------\n")
    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "fmi2SetReal - GUID = %s", componentInstance->getGuid().getValue())
    return fmi2OK;
  }

  fmi2Status fmi2SetInteger(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Integer value[]) {
    ENTRY_FUNCTION(FMI2_SET_INTEGER)
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), componentInstance->getGuid().getValue() << " - fmi2SetInteger \n")
    CHECK_REF_AND_VALUE()
    for(unsigned int i = 0; i < nvr; i++) {
      CHECK_REF_IN_IO()
      FMU_DEBUG_LOG(static_cast<fmuInstance*>(c),
        componentInstance->getGuid().getValue() << " SetInt - vr[" << i << "] = " << vr[i] << "; and value[" << i << "] to set is = " << value[i] << "\n")
      CIEC_LINT valToWrite(static_cast<TForteInt64>(value[i]));
      componentInstance->getOutputsAndInputs()[vr[i]]->setValue(valToWrite);
    }
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), "-----------\n")

    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "fmi2SetInteger - GUID = %s", componentInstance->getGuid().getValue())
    return fmi2OK;
  }

  fmi2Status fmi2SetBoolean(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Boolean value[]) {
    ENTRY_FUNCTION(FMI2_SET_BOOLEAN)
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), componentInstance->getGuid().getValue() << " - fmi2SetBoolean \n")
    CHECK_REF_AND_VALUE()

    for(unsigned int i = 0; i < nvr; i++) {
      CHECK_REF_IN_IO()
      FMU_DEBUG_LOG(static_cast<fmuInstance*>(c),
        componentInstance->getGuid().getValue() << " SetBoolean - vr[" << i << "] = " << vr[i] << "; and value[" << i << "] to set is = " << value[i] << "\n")
      CIEC_BOOL valToWrite(value[i] != fmi2False);
      componentInstance->getOutputsAndInputs()[vr[i]]->setValue(valToWrite);
    }
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), "-----------\n")

    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "fmi2SetBoolean - GUID = %s", componentInstance->getGuid().getValue())
    return fmi2OK;
  }

  fmi2Status fmi2SetString(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2String value[]) {
    ENTRY_FUNCTION(FMI2_SET_STRING)
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), componentInstance->getGuid().getValue() << " - fmi2SetString \n")
    CHECK_REF_AND_VALUE()

    for(unsigned int i = 0; i < nvr; i++) {
      CHECK_REF_IN_IO()
      FMU_DEBUG_LOG(static_cast<fmuInstance*>(c),
        componentInstance->getGuid().getValue() << " SetString - vr[" << i << "] = " << vr[i] << "; and value[" << i << "] to set is = " << value[i] << "\n")
      CIEC_STRING valToWrite(value[i]); //copies the string into the CIEC_STRING variable
      componentInstance->getOutputsAndInputs()[vr[i]]->setValue(valToWrite);
    }
    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), "-----------\n")

    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "fmi2SetString - GUID = %s", componentInstance->getGuid().getValue())
    return fmi2OK;
  }

  fmi2Status fmi2GetFMUstate(fmi2Component c, fmi2FMUstate* FMUstate) {
    NOT_USED(FMUstate)
    return functionNotSupported(c, "fmi2GetFMUstate");
  }

  fmi2Status fmi2SetFMUstate(fmi2Component c, fmi2FMUstate FMUstate) {
    NOT_USED(FMUstate)
    return functionNotSupported(c, "fmi2SetFMUstate");
  }

  fmi2Status fmi2FreeFMUstate(fmi2Component c, fmi2FMUstate* FMUstate) {
    NOT_USED(FMUstate)
    return functionNotSupported(c, "fmi2FreeFMUstate");
  }

  fmi2Status fmi2SerializedFMUstateSize(fmi2Component c, fmi2FMUstate FMUstate, size_t *size) {
    NOT_USED(FMUstate)
    NOT_USED(size)
    return functionNotSupported(c, "fmi2SerializedFMUstateSize");
  }

  fmi2Status fmi2SerializeFMUstate(fmi2Component c, fmi2FMUstate FMUstate, fmi2Byte serializedState[], size_t size) {
    NOT_USED(FMUstate)
    NOT_USED(serializedState)
    NOT_USED(size)
    return functionNotSupported(c, "fmi2SerializeFMUstate");
  }

  fmi2Status fmi2DeSerializeFMUstate(fmi2Component c, const fmi2Byte serializedState[], size_t size, fmi2FMUstate* FMUstate) {
    NOT_USED(FMUstate)
    NOT_USED(serializedState)
    NOT_USED(size)
    return functionNotSupported(c, "fmi2DeSerializeFMUstate");
  }

  fmi2Status fmi2GetDirectionalDerivative(fmi2Component c, const fmi2ValueReference vUnknown_ref[], size_t nUnknown, const fmi2ValueReference vKnown_ref[],
      size_t nKnown, const fmi2Real dvKnown[], fmi2Real dvUnknown[]) {
    NOT_USED(vUnknown_ref)
    NOT_USED(nUnknown)
    NOT_USED(vKnown_ref)
    NOT_USED(nKnown)
    NOT_USED(dvKnown)
    NOT_USED(dvUnknown)
    return functionNotSupported(c, "fmi2GetDirectionalDerivative");
  }

  fmi2Status fmi2SetRealInputDerivatives(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Integer order[], const fmi2Real value[]) {
    NOT_USED(vr)
    NOT_USED(nvr)
    NOT_USED(order)
    NOT_USED(value)
    return functionNotSupported(c, "fmi2SetRealInputDerivatives");
  }

  fmi2Status fmi2GetRealOutputDerivatives(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Integer order[], fmi2Real value[]) {
    NOT_USED(vr)
    NOT_USED(nvr)
    NOT_USED(order)
    NOT_USED(value)
    return functionNotSupported(c, "fmi2GetRealOutputDerivatives");
  }

  fmi2Status fmi2DoStep(fmi2Component c, fmi2Real currentCommunicationPoint, fmi2Real communicationStepSize, fmi2Boolean noSetFMUStatePriorToCurrentPoint) {
    NOT_USED(noSetFMUStatePriorToCurrentPoint)
    ENTRY_FUNCTION(FMI2_DO_STEP)
    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "fmi2DoStep - GUID = %s", componentInstance->getGuid().getValue())
    CIEC_LREAL advanceTime = communicationStepSize;

    FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), componentInstance->getGuid().getValue() << " - fmi2DoStep\n")
    if(-1 != componentInstance->getStopTime() && (currentCommunicationPoint + communicationStepSize) > componentInstance->getStopTime()) {
      LOG_FMU(componentInstance, fmi2Error, LOG_ERROR, "Computation time past stopTime: %f", componentInstance->getStopTime())
      FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), componentInstance->getGuid().getValue() << " - fmi2DoStep Error1 \n")
      return fmi2Error;
    }

    if(!componentInstance->advanceInstanceTime(advanceTime)) {
      FMU_DEBUG_LOG(static_cast<fmuInstance*>(c), componentInstance->getGuid().getValue() << " - fmi2DoStep Error2 \n")
      return fmi2Error;
    }
    return fmi2OK;
  }

  fmi2Status fmi2CancelStep(fmi2Component c) {
    ENTRY_FUNCTION(FMI2_CANCEL_STEP)
    LOG_FMU(componentInstance, fmi2Error, LOG_ERROR, "fmi2CancelStep is not yet implemented")
    return fmi2Error;
  }

  fmi2Status fmi2GetStatus(fmi2Component c, const fmi2StatusKind s, fmi2Status *value) {
    NOT_USED(s)
    NOT_USED(value)
    ENTRY_FUNCTION(FMI2_GET_STATUS)
    LOG_FMU(componentInstance, fmi2Discard, LOG_ERROR, "fmi2GetStatus is not yet implemented")
    return fmi2Discard;
  }

  fmi2Status fmi2GetRealStatus(fmi2Component c, const fmi2StatusKind s, fmi2Real *value) {
    NOT_USED(s)
    NOT_USED(value)
    ENTRY_FUNCTION(FMI2_GET_REAL_STATUS)
    LOG_FMU(componentInstance, fmi2Discard, LOG_ERROR, "fmi2GetRealStatus is not yet implemented")
    return fmi2Discard;
  }

  fmi2Status fmi2GetIntegerStatus(fmi2Component c, const fmi2StatusKind s, fmi2Integer *value) {
    NOT_USED(s)
    NOT_USED(value)
    ENTRY_FUNCTION(FMI2_GET_INTEGER_STATUS)
    LOG_FMU(componentInstance, fmi2Discard, LOG_ERROR, "fmi2GetIntegerStatus is not yet implemented")
    return fmi2Discard;
  }

  fmi2Status fmi2GetBooleanStatus(fmi2Component c, const fmi2StatusKind s, fmi2Boolean *value) {
    NOT_USED(s)
    NOT_USED(value)
    ENTRY_FUNCTION(FMI2_GET_BOOLEAN_STATUS)
    LOG_FMU(componentInstance, fmi2Discard, LOG_ERROR, "fmi2GetBooleanStatus is not yet implemented")
    return fmi2Discard;
  }

  fmi2Status fmi2GetStringStatus(fmi2Component c, const fmi2StatusKind s, fmi2String *value) {
    NOT_USED(s)
    NOT_USED(value)
    ENTRY_FUNCTION(FMI2_GET_STRING_STATUS)
    LOG_FMU(componentInstance, fmi2Discard, LOG_ERROR, "fmi2GetStringStatus is not yet implemented")
    return fmi2Discard;
  }

  fmi2Status functionNotSupported(fmi2Component c, const char* functionName) {
    if(0 == c) {
      return fmi2Error;
    }
    fmuInstance* componentInstance = static_cast<fmuInstance*>(c);
    LOG_FMU(componentInstance, fmi2OK, LOG_CALL, "%s - GUID = %s", functionName, componentInstance->getGuid().getValue())
    LOG_FMU(componentInstance, fmi2OK, LOG_ERROR, "Function %s is not supported.", functionName)
    return fmi2Error;
  }

  void freeInstanceInternal(fmi2Component c) {
    fmuInstance* componentInstance = static_cast<fmuInstance*>(c);
    delete componentInstance;
  }
}

