
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
#include "core/fmi/fmuInstance.h"
#include "forte/device.h"

using namespace forte::literals;

#include "forte/device.h"
#include "core/fmi/comm/fmuHandler.h"
#include "forte/basicfb.h"
#include "forte/cfb.h"
#include <stdlib.h>
#include "forte/util/criticalregion.h"

fmuInstance *fmuInstance::sFmuInstance = 0;

CSyncObject fmuInstance::sFmuInstanceMutex;

#ifdef FORTE_LOGDEBUG
#include "forte/util/criticalregion.h"
#include <ctime>
#include <sstream>
void fmuInstance::printToFile(const char *message) {
  fmuInstance::debugFile << message;
  std::flush(fmuInstance::debugFile);
}
#endif

fmuInstance::fmuInstance(fmi2String instanceName,
                         fmi2String GUID,
                         fmi2String bootFileLocation,
                         const fmi2CallbackFunctions *callbackFunctions) :
    CDevice(0, {}, 0, 0),
    mState(STATE_START_END),
    mStopTime(-1),
    mNumberOfEcets(0),
    mAllowEcetToRun(false) {

  CCriticalRegion criticalRegion(sFmuInstanceMutex);
  sFmuInstance = this;

  mResource = new EMB_RES({}, this);

  getDeviceExecution().getExtEvHandler<fmiTimerHandler>().removeExecutionThread(mResource->getResourceEventExecution());
  mNumberOfEcets--;

  for (unsigned int i = 0; i < NUMBER_OF_LOG_CATEGORIES; i++) {
    mLoggingCategories[i] = fmi2False;
  }

  this->mInstanceName = instanceName;
  this->mGUID = GUID;
  this->mBootFileLocation = bootFileLocation;
  this->mCallbackFunctions = callbackFunctions;
  this->mState = STATE_ERROR;

#ifdef FORTE_LOGDEBUG
  std::stringstream fileName;
  fileName << "fmu4diacDebug1" << ((long) time(0)) << GUID << "_" << this << ".txt";
  fmuInstance::debugFile.open(fileName.str().c_str(), std::fstream::out);
#endif

  if (loadFBs()) {
    this->mState = STATE_INSTANTIATED;
  }
}

fmuInstance::~fmuInstance() {
  std::map<CFunctionBlock *, std::vector<fmuValueContainer *> *> *deletingMap[2] = {
      getDeviceExecution().getExtEvHandler<fmuHandler>().getInputMap(),
      getDeviceExecution().getExtEvHandler<fmuHandler>().getOutputMap()};

  for (std::vector<CFunctionBlock *>::iterator it = getCommFBs().begin(); it != getCommFBs().end(); ++it) {
    for (unsigned int i = 0; i < 2; i++) {
      if (deletingMap[i]->end() != deletingMap[i]->find(*it)) {
        for (std::vector<fmuValueContainer *>::iterator itContainer = deletingMap[i]->at(*it)->begin();
             itContainer != deletingMap[i]->at(*it)->end(); ++itContainer) {
          delete (*itContainer);
        }
        delete (deletingMap[i]->at(*it));
        deletingMap[i]->erase(*it);
      }
    }
  }
  delete mResource;
}

bool fmuInstance::loadFBs() {

  CFunctionBlock *devMgr = CTypeLib::createFB("MGR"_STRID, "DEV_MGR"_STRID, mResource);
  devMgr->getDataInput("QI"_STRID)->fromString("1");
  devMgr->changeExecutionState(EMGMCommandType::Reset);
  devMgr->changeExecutionState(EMGMCommandType::Start);
  FMU_DEBUG_LOG(this, MODEL_GUID << " About to load FBs from file " << getBootFileLocation().getValue()
                                 << "\n--------------\n");
  CIEC_STRING val = "FORTE_BOOT_FILE=";
  val.append(mBootFileLocation.getValue());
  if (!putenv(val.getValue())) {
    FMU_DEBUG_LOG(this, MODEL_GUID << " Set env WORKED \n--------------\n");
  } else {
    FMU_DEBUG_LOG(this, MODEL_GUID << " Set env FAILED \n--------------\n");
  }
  devMgr->receiveInputEvent(0, *mResource->getResourceEventExecution()); // the first 0 is the eventID.
  delete devMgr;
  FMU_DEBUG_LOG(this, MODEL_GUID << " Already loaded \n--------------\n");

  populateInputsOutputs(this);

  FMU_DEBUG_LOG(this, "VARIABLES: m_outputsAndInputs has " << mOutputsAndInputs.size() << " elements\n");

  return true;
}

void fmuInstance::populateInputsOutputs(forte::CFBContainer *resource) {
  for (forte::CFBContainer::TFunctionBlockList::Iterator itRunner = resource->getFBList().begin();
       itRunner != resource->getFBList().end(); ++itRunner) {
    populateInputsAndOutputsCore(*itRunner);
  }
}

void fmuInstance::populateInputsAndOutputsCore(CFunctionBlock *paFB) {

  forte::StringId functionBlockType = paFB->getFBTypeId();
  if ("EMB_RES"_STRID == functionBlockType) {
    populateInputsOutputs(static_cast<CResource *>(paFB));
    return;
  } else if ("IX"_STRID == functionBlockType || "QX"_STRID == functionBlockType) {
    FMU_DEBUG_LOG(this, "VARIABLES: IO: " << paFB->getInstanceName() << " ADDED SUCCESSFULLY\n");
    fmuValueContainer *newValue = new fmuValueContainer(fmuValueContainer::BOOL, false);
    CFMUProcessInterface *ioFB = static_cast<CFMUProcessInterface *>(paFB);
    ioFB->setValueContainer(newValue);
    mOutputsAndInputs.push_back(newValue);
  } else if ("IW"_STRID == functionBlockType || "QW"_STRID == functionBlockType) {
    FMU_DEBUG_LOG(this, "VARIABLES: IO: " << paFB->getInstanceName() << " ADDED SUCCESSFULLY\n");
    fmuValueContainer *newValue = new fmuValueContainer(fmuValueContainer::INTEGER, false);
    CFMUProcessInterface *ioFB = static_cast<CFMUProcessInterface *>(paFB);
    ioFB->setValueContainer(newValue);
    mOutputsAndInputs.push_back(newValue);
  } else {
    // check Communication Blocks
    const char *functionBlockName = functionBlockType.data();
    if (strncmp(functionBlockName, "PUBLISH_", 8) == 0 || strncmp(functionBlockName, "SUBSCRIBE_", 10) == 0 ||
        strncmp(functionBlockName, "CLIENT_", 7) == 0 || strncmp(functionBlockName, "SERVER_", 7) == 0) {

      if ("fmu[]" == *static_cast<CIEC_STRING *>(paFB->getDIFromPortId(1))) {
        CIEC_ANY::EDataTypeID type;
        std::vector<fmuValueContainer *> *outputs = new std::vector<fmuValueContainer *>;
        std::vector<fmuValueContainer *> *inputs = new std::vector<fmuValueContainer *>;

        for (unsigned int i = 2; i < paFB->getFBInterfaceSpec().getNumDIs(); i++) {
          type = getConnectedDataType(i, true, paFB);
          fmuValueContainer *newValue = new fmuValueContainer(fmuValueContainer::getValueFromType(type), false);
          inputs->push_back(newValue); // if an error occur, the fmuValueContainer will have the flag error to true.
                                       // This must be checked by the FMI interface to kill the simulation
          mOutputsAndInputs.push_back(newValue);
          FMU_DEBUG_LOG(this, "VARIABLES: COMM: " << paFB->getInstanceName() << " INPUT PORT " << i
                                                  << " ADDED SUCCESSFULLY\n")
        }

        for (unsigned int i = 2; i < paFB->getFBInterfaceSpec().getNumDOs(); i++) {
          type = getConnectedDataType(i, false, paFB);
          fmuValueContainer *newValue = new fmuValueContainer(fmuValueContainer::getValueFromType(type), false);
          newValue->setCallbackArgument(newValue);
          newValue->setCallback(fmuHandler::fmuMessageArrived);
          outputs->push_back(newValue); // if an error occur, the fmuValueContainer will have the flag error to true.
                                        // This must be checked by the FMI interface to kill the simulation
          mOutputsAndInputs.push_back(newValue);
          FMU_DEBUG_LOG(this, "VARIABLES: COMM: " << paFB->getInstanceName() << " OUTPUT PORT " << i
                                                  << " ADDED SUCCESSFULLY\n")
        }

        getDeviceExecution().getExtEvHandler<fmuHandler>().getOutputMap()->insert(std::make_pair(paFB, outputs));
        getDeviceExecution().getExtEvHandler<fmuHandler>().getInputMap()->insert(std::make_pair(paFB, inputs));
        getCommFBs().push_back(paFB);
      }
    }
  }

  if ("E_RESTART"_STRID != functionBlockType) {
    // Add interface of the FB as local variables to the FMU
    fillInterfaceElementsArray(paFB, true, false); // data inputs
    fillInterfaceElementsArray(paFB, false, false); // data outputs
    fillInterfaceElementsArray(paFB, true, true); // event inputs
    fillInterfaceElementsArray(paFB, false, true); // event outputs

    CBasicFB *testBasic = dynamic_cast<CBasicFB *>(paFB);
    CCompositeFB *testComposite = dynamic_cast<CCompositeFB *>(paFB);
    if (0 != testBasic) { // basic function Block
      // store internal variables
      if (0 != testBasic->cmVarInternals) {
        for (TPortId i = 0; i < testBasic->cmVarInternals->mNumIntVars; i++) {
          forte::StringId varId = testBasic->cmVarInternals->mIntVarsNames[i];
          CIEC_ANY *var = testBasic->getVar(&varId, 1);
          if (0 != var) {
            fmuValueContainer *newValue =
                new fmuValueContainer(fmuValueContainer::getValueFromType(var->getDataTypeID()), true);
            newValue->setValuePointer(testBasic->getVar(&varId, 1));
            mOutputsAndInputs.push_back(newValue);
            FMU_DEBUG_LOG(this, "VARIABLES: INTERNAL: " << testBasic->getInstanceName() << "." << varId
                                                        << " ADDED SUCCESSFULLY\n")
          } else {
            FMU_DEBUG_LOG(this, "--------ERROR: Unexpected behavior when getting the internal variable "
                                    << varId << " of Function Block: " << testBasic->getInstanceName() << ".\n");
          }
        }
      }
      // store state of ECC
      fmuValueContainer *newValue = new fmuValueContainer(fmuValueContainer::INTEGER, true);
      forte::StringId eccId = "$ECC"_STRID;
      newValue->setValuePointer(testBasic->getVar(&eccId, 1));
      mOutputsAndInputs.push_back(newValue);
      FMU_DEBUG_LOG(this, "VARIABLES: INTERNAL: " << testBasic->getInstanceName() << ".ECC ADDED SUCCESSFULLY\n")
    } else if (0 != testComposite) {
      // populateInputsOutputs for internal FBs
      for (unsigned int i = 0; i < testComposite->cmFBNData->mNumFBs; i++) {
        populateInputsAndOutputsCore(testComposite->mInternalFBs[i]);
      }
    }
  }
}

CIEC_ANY::EDataTypeID fmuInstance::getConnectedDataType(unsigned int portIndex, bool paInput, CFunctionBlock *paFB) {
  CIEC_ANY::EDataTypeID retVal = CIEC_ANY::e_Max;
  const CIEC_ANY *portValue = paInput ? paFB->getDI(portIndex) : paFB->getDO(portIndex);

  if (portValue != nullptr) {
    retVal = portValue->unwrap().getDataTypeID();
  }

  return retVal;
}

void fmuInstance::fillInterfaceElementsArray(CFunctionBlock *paFB, bool isInput, bool isEvent) {

  if (isEvent) {
    TEventID noOfElements = isInput ? paFB->getFBInterfaceSpec().getNumEIs() : paFB->getFBInterfaceSpec().getNumEOs();
    for (TEventID i = 0; i < noOfElements; i++) {
      fmuValueContainer *newValue = new fmuValueContainer(fmuValueContainer::valueType::INTEGER, true);
      newValue->setEventCounterPointer(isInput ? &(paFB->getEIMonitorData(i)) : &(paFB->getEOMonitorData(i)));
      mOutputsAndInputs.push_back(newValue);
      FMU_DEBUG_LOG(this, "VARIABLES: INTERFACE: " << paFB->getInstanceName() << "." << isInput
                              ? paFB->getFBInterfaceSpec().mEINames[i]
                              : paFB->getFBInterfaceSpec().mEONames[i] << " ADDED SUCCESSFULLY\n")
    }
  } else {
    unsigned int noOfElements =
        isInput ? paFB->getFBInterfaceSpec().getNumDIs() : paFB->getFBInterfaceSpec().getNumDOs();
    for (unsigned int i = 0; i < noOfElements; i++) {
      FMU_DEBUG_LOG(this, "VARIABLES: INTERFACE: " << paFB->getInstanceName() << "." << isInput
                              ? paFB->getFBInterfaceSpec().mDINames[i]
                              : paFB->getFBInterfaceSpec().mDONames[i] << ": ");
      fmuValueContainer::valueType valueType = fmuValueContainer::getValueFromType(
          isInput ? paFB->getDIFromPortId(static_cast<TPortId>(i))->getDataTypeID()
                  : paFB->getDOFromPortId(static_cast<TPortId>(i))->getDataTypeID());
      if (fmuValueContainer::valueType::WRONG == valueType) {
        valueType = fmuValueContainer::getValueFromType(getConnectedDataType(i, isInput, paFB));
      }

      if (fmuValueContainer::valueType::WRONG == valueType) {
        FMU_DEBUG_LOG(this, " --------ERROR: WRONG TYPE. NOT ADDED SUCCESSFULLY\n")
        continue;
      }
      FMU_DEBUG_LOG(this, " ADDED SUCCESSFULLY\n")
      fmuValueContainer *newValue = new fmuValueContainer(valueType, true);
      newValue->setValuePointer(isInput ? paFB->getDIFromPortId(static_cast<TPortId>(i))
                                        : paFB->getDOFromPortId(static_cast<TPortId>(i)));
      mOutputsAndInputs.push_back(newValue);
    }
  }
}

void fmuInstance::startInstance() {
  resetInstance();
  changeExecutionState(EMGMCommandType::Start);
}

void fmuInstance::resetInstance() {
  changeExecutionState(EMGMCommandType::Reset);
}

void fmuInstance::stopInstance() {
  // this is needed since RESTART needs the Ecet to stop
  mAllowEcetToRun = true;
  for (unsigned int i = 0; i < mNumberOfEcets; i++) {
    mEcetSemaphore.inc();
  }
  changeExecutionState(EMGMCommandType::Stop);
}
