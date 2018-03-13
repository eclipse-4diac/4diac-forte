
/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "fmuInstance.h"
#include "device.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "fmuInstance_gen.cpp"
#endif
#include "device.h"
#include "./comm/fmuHandler.h"
#include <vector>
#include "../basicfb.h"
#include "../cfb.h"
#include <stdlib.h>

#ifdef FMU_DEBUG
#include "criticalregion.h"
#include <ctime>
#include <sstream>
unsigned int fmuInstance::intanceNo = 0;
std::fstream fmuInstance::staticFile;
CPCSyncObject fmuInstance::mutex;
void fmuInstance::printToFile(const char* message){
  CCriticalRegion criticalRegion(fmuInstance::mutex);
  fmuInstance::staticFile << message;
  std::flush(fmuInstance::staticFile);
}
#endif

fmuInstance::fmuInstance(fmi2String instanceName, fmi2String GUID, fmi2String bootFileLocation, const fmi2CallbackFunctions *callbackFunctions) :
    CDevice(0, CStringDictionary::scm_nInvalidStringId, 0, 0),
    m_resource(CStringDictionary::scm_nInvalidStringId, this), m_state(STATE_START_END), m_stopTime(-1) {

  for (unsigned int i = 0; i < NUMBER_OF_LOG_CATEGORIES; i++){
    m_loggingCategories[i] = fmi2False;
  }

  this->m_instanceName = instanceName;
  this->m_GUID = GUID;
  this->m_bootFileLocation = bootFileLocation;
  this->m_callbackFunctions = callbackFunctions;

#ifdef FMU_DEBUG
  if (!fmuInstance::staticFile.is_open()){
    std::stringstream fileName;
    fileName << "fmu4diacDebug" << ((long) time(0)) << GUID << "_" << fmuInstance::intanceNo++ << ".txt";
    fmuInstance::staticFile.open(fileName.str().c_str(), std::fstream::out);
  }
 #endif
}

fmuInstance::~fmuInstance(){
  std::map<CFunctionBlock*, std::vector<fmuValueContainer*>*>* deletingMap[2] =
  { static_cast<fmuHandler*>(static_cast<fmuHandler*>(getDeviceExecution().getHandler(fmuHandler::handlerIdentifier)))->getInputMap(), static_cast<fmuHandler*>(getDeviceExecution().getHandler(fmuHandler::handlerIdentifier))->getOutputMap() };

  for(std::vector<CFunctionBlock*>::iterator it = m_commFBs.begin(); it != m_commFBs.end(); ++it){
    for(unsigned int i = 0; i < 2; i++){
      if(deletingMap[i]->end() != deletingMap[i]->find(*it)){
        for(std::vector<fmuValueContainer*>::iterator itContainer = deletingMap[i]->at(*it)->begin(); itContainer != deletingMap[i]->at(*it)->end(); ++itContainer){
          delete (*itContainer);
        }
        delete (deletingMap[i]->at(*it));
        deletingMap[i]->erase(*it);
      }
    }
  }
}

bool fmuInstance::loadFBs(){

  CFunctionBlock* devMgr = CTypeLib::createFB(g_nStringIdMGR, g_nStringIdDEV_MGR, &m_resource);
  devMgr->getDataInput(g_nStringIdQI)->fromString("1");
  devMgr->changeFBExecutionState(cg_nMGM_CMD_Reset);
  devMgr->changeFBExecutionState(cg_nMGM_CMD_Start);
#ifdef FMU_DEBUG
      FMU_DEBUG_LOG(MODEL_GUID << " About to load FBs from file " << m_bootFileLocation.getValue() << "\n--------------\n")
#endif
  CIEC_STRING val = "FORTE_BOOT_FILE=";
      val.append(m_bootFileLocation.getValue());
  if(!putenv(val.getValue())){
#ifdef FMU_DEBUG
      FMU_DEBUG_LOG(MODEL_GUID << " Set env WORKED \n--------------\n")
#endif
  }else{
#ifdef FMU_DEBUG
    FMU_DEBUG_LOG(MODEL_GUID << " Set env FAILED \n--------------\n")
#endif
  }
  devMgr->receiveInputEvent(0, *m_resource.getResourceEventExecution()); //the first 0 is the eventID.
  delete devMgr;
#ifdef FMU_DEBUG
      FMU_DEBUG_LOG(MODEL_GUID << " Already loaded \n--------------\n")
#endif

  //parameters are always after all variables
  std::vector<fmuValueContainer*> parameters;
  populateInputsOutputs(this, &parameters);

  for(std::vector<fmuValueContainer*>::iterator it = parameters.begin(); it != parameters.end(); ++it){
#ifdef FMU_DEBUG
    FMU_DEBUG_LOG("VARIABLES: PARAMETER ADDED SUCCESSFULLY\n")
#endif
    m_outputsAndInputs.push_back((*it));
  }
#ifdef FMU_DEBUG
  FMU_DEBUG_LOG("VARIABLES: m_outputsAndInputs has " << m_outputsAndInputs.size() << " elements\n")
#endif

  return true;
}

void fmuInstance::populateInputsOutputs(forte::core::CFBContainer* resource, std::vector<fmuValueContainer*>* pa_parameters){
  for(forte::core::CFBContainer::TFunctionBlockList::Iterator itRunner = resource->getFBList().begin();
      itRunner != resource->getFBList().end(); ++itRunner){
    populateInputsAndOutputsCore(*itRunner, pa_parameters);
  }
}

void fmuInstance::populateInputsAndOutputsCore(CFunctionBlock* pa_poFB, std::vector<fmuValueContainer*>* pa_parameters){

  CStringDictionary::TStringId functionBlockType = pa_poFB->getFBTypeId();
  bool isParameter = false;
  if(g_nStringIdEMB_RES == functionBlockType){
    populateInputsOutputs(static_cast<CResource*>(pa_poFB), pa_parameters);
    return;
  }else if(g_nStringIdFMU_PARAM_BOOL == functionBlockType || g_nStringIdFMU_PARAM_INT == functionBlockType || g_nStringIdFMU_PARAM_REAL == functionBlockType || g_nStringIdFMU_PARAM_STRING == functionBlockType){
    m_parametersFBs.push_back(pa_poFB);
    CIEC_ANY::EDataTypeID type = pa_poFB->getDOFromPortId(0)->getDataTypeID();
    fmuValueContainer* newValue = new fmuValueContainer(fmuValueContainer::getValueFromType(type), true);
    newValue->setValuePointer(pa_poFB->getDOFromPortId(0)); //PARAM
    pa_parameters->push_back(newValue);
    isParameter = true;
  }
  else if(g_nStringIdIX == functionBlockType || g_nStringIdQX == functionBlockType){
#ifdef FMU_DEBUG
    FMU_DEBUG_LOG("VARIABLES: IO: " << pa_poFB->getInstanceName() <<  " ADDED SUCCESSFULLY\n")
#endif
    fmuValueContainer* newValue = new fmuValueContainer(fmuValueContainer::BOOL, false);
    CFMUProcessInterface* ioFB = static_cast<CFMUProcessInterface*>(pa_poFB);
    ioFB->setValueContainer(newValue);
    m_outputsAndInputs.push_back(newValue);
  }else if(g_nStringIdIW == functionBlockType || g_nStringIdQW == functionBlockType){
#ifdef FMU_DEBUG
    FMU_DEBUG_LOG("VARIABLES: IO: " << pa_poFB->getInstanceName() <<  " ADDED SUCCESSFULLY\n")
#endif
    fmuValueContainer* newValue = new fmuValueContainer(fmuValueContainer::INTEGER, false);
    CFMUProcessInterface* ioFB = static_cast<CFMUProcessInterface*>(pa_poFB);
    ioFB->setValueContainer(newValue);
    m_outputsAndInputs.push_back(newValue);
  }
  else{
    //check Communication Blocks
    const char* functionBlockName = CStringDictionary::getInstance().get(functionBlockType);
    if(strncmp(functionBlockName, "PUBLISH_", 8) == 0
        || strncmp(functionBlockName, "SUBSCRIBE_", 10) == 0
        || strncmp(functionBlockName, "CLIENT_", 7) == 0
        || strncmp(functionBlockName, "SERVER_", 7) == 0){

      if("fmu[]" == *static_cast<CIEC_STRING*>(pa_poFB->getDIFromPortId(1))){
        CIEC_ANY::EDataTypeID type;
        std::vector<fmuValueContainer*>* outputs = new std::vector<fmuValueContainer*>;
        std::vector<fmuValueContainer*>* inputs = new std::vector<fmuValueContainer*>;

        for(unsigned int i = 2; i < pa_poFB->getFBInterfaceSpec()->m_nNumDIs; i++){
          type = getConnectedDataType(i, true, pa_poFB);
          fmuValueContainer* newValue = new fmuValueContainer(fmuValueContainer::getValueFromType(type), false);
          inputs->push_back(newValue); //if an error occur, the fmuValueContainer will have the flag error to true. This must be checked by the FMI interface to kill the simulation
          m_outputsAndInputs.push_back(newValue);
#ifdef FMU_DEBUG
  FMU_DEBUG_LOG("VARIABLES: COMM: " <<   pa_poFB->getInstanceName() <<  " INPUT PORT " << i << " ADDED SUCCESSFULLY\n")
#endif
        }

        for(unsigned int i = 2; i < pa_poFB->getFBInterfaceSpec()->m_nNumDOs; i++){
          type = getConnectedDataType(i, false, pa_poFB);
          fmuValueContainer* newValue = new fmuValueContainer(fmuValueContainer::getValueFromType(type), false);
          newValue->setCallbackArgument(newValue);
          newValue->setCallback(fmuHandler::fmuMessageArrived);
          outputs->push_back(newValue); //if an error occur, the fmuValueContainer will have the flag error to true. This must be checked by the FMI interface to kill the simulation
          m_outputsAndInputs.push_back(newValue);
#ifdef FMU_DEBUG
  FMU_DEBUG_LOG("VARIABLES: COMM: " <<   pa_poFB->getInstanceName() <<  " OUTPUT PORT " << i << " ADDED SUCCESSFULLY\n")
#endif
        }

        static_cast<fmuHandler*>(getDeviceExecution().getHandler(fmuHandler::handlerIdentifier))->getOutputMap()->insert(std::make_pair(pa_poFB, outputs));
        static_cast<fmuHandler*>(getDeviceExecution().getHandler(fmuHandler::handlerIdentifier))->getInputMap()->insert(std::make_pair(pa_poFB, inputs));
        m_commFBs.push_back(pa_poFB);
      }
    }
  }

  if(!isParameter && g_nStringIdE_RESTART != functionBlockType){
    //Add interface of the FB as local variables to the FMU
    fillInterfaceElementsArray(pa_poFB, true, false); //data inputs
    fillInterfaceElementsArray(pa_poFB, false, false); //data outputs
    fillInterfaceElementsArray(pa_poFB, true, true); //event inputs
    fillInterfaceElementsArray(pa_poFB, false, true); //event outputs

    CBasicFB* testBasic = dynamic_cast<CBasicFB*>(pa_poFB);
    CCompositeFB* testComposite = dynamic_cast<CCompositeFB*>(pa_poFB);
    if(0 != testBasic){ //basic function Block
      //store internal variables
      if(0 != testBasic->cm_pstVarInternals){
        for(unsigned int i = 0; i < testBasic->cm_pstVarInternals->m_nNumIntVars; i++){
          CStringDictionary::TStringId varId = testBasic->cm_pstVarInternals->m_aunIntVarsNames[i];
          CIEC_ANY* var = testBasic->getVar(&varId, 1);
          if(0 != var){
            fmuValueContainer* newValue = new fmuValueContainer(fmuValueContainer::getValueFromType(var->getDataTypeID()), true);
            newValue->setValuePointer(testBasic->getVar(&varId, 1));
            m_outputsAndInputs.push_back(newValue);
#ifdef FMU_DEBUG
            FMU_DEBUG_LOG("VARIABLES: INTERNAL: " << testBasic->getInstanceName() << "." << CStringDictionary::getInstance().get(varId) << " ADDED SUCCESSFULLY\n")
#endif
          }else{
#ifdef FMU_DEBUG
            FMU_DEBUG_LOG("--------ERROR: Unexpected behavior when getting the internal variable " <<  CStringDictionary::getInstance().get(varId) << " of Function Block: " << testBasic->getInstanceName() << ".\n");
#endif
          }

        }
      }
      //store state of ECC
      fmuValueContainer* newValue = new fmuValueContainer(fmuValueContainer::INTEGER, true);
      CStringDictionary::TStringId eccId = CStringDictionary::getInstance().getId("$ECC");
      if(CStringDictionary::scm_nInvalidStringId == eccId){
        CStringDictionary::getInstance().insert("$ECC");
        eccId = CStringDictionary::getInstance().getId("$ECC");
      }
      newValue->setValuePointer(testBasic->getVar(&eccId, 1));
      m_outputsAndInputs.push_back(newValue);
#ifdef FMU_DEBUG
      FMU_DEBUG_LOG("VARIABLES: INTERNAL: " << testBasic->getInstanceName() << ".ECC ADDED SUCCESSFULLY\n")
#endif
    }
    else if(0 != testComposite){
      //populateInputsOutputs for internal FBs
      for(unsigned int i = 0; i < testComposite->cm_cpoFBNData->m_nNumFBs; i++){
        populateInputsAndOutputsCore(testComposite->m_apoInternalFBs[i], pa_parameters);
      }
    }
  }
}

CIEC_ANY::EDataTypeID fmuInstance::getConnectedDataType(unsigned int portIndex, bool pa_isInput, CFunctionBlock* pa_poFB) {

  CIEC_ANY::EDataTypeID retVal = CIEC_ANY::e_Max;
  /* Retrieve Publisher, Connection and Signals Source Function Block Information */
  const CStringDictionary::TStringId portNameId = pa_isInput ? pa_poFB->getFBInterfaceSpec()->m_aunDINames[portIndex] : pa_poFB->getFBInterfaceSpec()->m_aunDONames[portIndex];
  const CDataConnection *portConnection = pa_isInput ? pa_poFB->getDIConnection(portNameId) : pa_poFB->getDOConnection(portNameId);
  if(portConnection != NULL){
    //TODO for now we assume that the subscriber connection only has one destination. Needs fix!
    if(!pa_isInput && portConnection->getDestinationList().isEmpty()){
#ifdef FMU_DEBUG
      FMU_DEBUG_LOG("--------ERROR: Subscriber does not have any connection.\n");
#endif
    }
    else{
      CSinglyLinkedList<SConnectionPoint>::Iterator it = portConnection->getDestinationList().begin();
      const SConnectionPoint remoteConnectionPoint = pa_isInput ? portConnection->getSourceId() : *it;

      const CIEC_ANY *remotePort = pa_isInput ? remoteConnectionPoint.mFB->getDOFromPortId(remoteConnectionPoint.mPortId) : remoteConnectionPoint.mFB->getDIFromPortId(remoteConnectionPoint.mPortId);

      retVal = remotePort->getDataTypeID();
    }
  }
  else{
#ifdef FMU_DEBUG
    FMU_DEBUG_LOG("--------ERROR: Got invalid port connection on FB " << pa_poFB->getInstanceName() << " at port " << CStringDictionary::getInstance().get(portNameId) << ". It must be connected to another FB.\n");
#endif
  }

  return retVal;
}

void fmuInstance::fillInterfaceElementsArray(CFunctionBlock* pa_poFB, bool isInput, bool isEvent){

  if(isEvent){
    unsigned int noOfElements = isInput ? pa_poFB->getFBInterfaceSpec()->m_nNumEIs : pa_poFB->getFBInterfaceSpec()->m_nNumEOs;
    for(unsigned int i = 0; i < noOfElements; i++){
      fmuValueContainer* newValue = new fmuValueContainer(fmuValueContainer::valueType::INTEGER, true);
      newValue->setEventCounterPointer(isInput ? &(pa_poFB->getEIMonitorData(static_cast<TEventID>(i)).mMonitorEventData[0].mEventCount) : &(pa_poFB->getEOMonitorData(static_cast<TEventID>(i)).mMonitorEventData[0].mEventCount));
      m_outputsAndInputs.push_back(newValue);
#ifdef FMU_DEBUG
      FMU_DEBUG_LOG("VARIABLES: INTERFACE: " << pa_poFB->getInstanceName() << "." << CStringDictionary::getInstance().get(isInput ? pa_poFB->getFBInterfaceSpec()->m_aunEINames[i] : pa_poFB->getFBInterfaceSpec()->m_aunEONames[i]) << " ADDED SUCCESSFULLY\n")
#endif
    }
  }
  else{
    unsigned int noOfElements = isInput ? pa_poFB->getFBInterfaceSpec()->m_nNumDIs : pa_poFB->getFBInterfaceSpec()->m_nNumDOs;
    for(unsigned int i = 0; i < noOfElements; i++){
#ifdef FMU_DEBUG
      FMU_DEBUG_LOG("VARIABLES: INTERFACE: " << pa_poFB->getInstanceName() << "." << CStringDictionary::getInstance().get(isInput ? pa_poFB->getFBInterfaceSpec()->m_aunDINames[i] : pa_poFB->getFBInterfaceSpec()->m_aunDONames[i]) << ": ");
#endif
      fmuValueContainer::valueType valueType = fmuValueContainer::getValueFromType(isInput ? pa_poFB->getDIFromPortId(static_cast<TPortId>(i))->getDataTypeID() : pa_poFB->getDOFromPortId(static_cast<TPortId>(i))->getDataTypeID());
      if(fmuValueContainer::valueType::WRONG == valueType){
        valueType = fmuValueContainer::getValueFromType(getConnectedDataType(i, isInput, pa_poFB));
      }

      if(fmuValueContainer::valueType::WRONG == valueType){
#ifdef FMU_DEBUG
        FMU_DEBUG_LOG(" --------ERROR: WRONG TYPE. NOT ADDED SUCCESSFULLY\n")
#endif
        continue;
      }
#ifdef FMU_DEBUG
      FMU_DEBUG_LOG(" ADDED SUCCESSFULLY\n")
#endif
      fmuValueContainer* newValue = new fmuValueContainer(valueType, true);
      newValue->setValuePointer(isInput ? pa_poFB->getDIFromPortId(static_cast<TPortId>(i)) : pa_poFB->getDOFromPortId(static_cast<TPortId>(i)));
      m_outputsAndInputs.push_back(newValue);
    }
  }
}

void fmuInstance::startInstance(void){
  resetInstance();
  (static_cast<CDevice*>(m_resource.getResourcePtr()))->changeFBExecutionState(cg_nMGM_CMD_Start);
}

void fmuInstance::resetInstance(void){
  (static_cast<CDevice*>(m_resource.getResourcePtr()))->changeFBExecutionState(cg_nMGM_CMD_Reset);
}

void fmuInstance::stopInstance(){
  (static_cast<CDevice*>(m_resource.getResourcePtr()))->changeFBExecutionState(cg_nMGM_CMD_Stop);
}
