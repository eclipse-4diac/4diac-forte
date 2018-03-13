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
#ifndef _FMUINSTANCE_H_
#define _FMUINSTANCE_H_

#include "fmuConfig.h"
#include "fmuValueContainer.h"
#include "fmiTimerHandler.h"
#include "../device.h"
#include "../../stdfblib/ita/EMB_RES.h"
#include <vector>
#include <fstream>
#include "../device.h"

class fmuInstance : public CDevice{

#ifdef FMU_DEBUG
  public:
    static unsigned int intanceNo;
#endif

    //TODO: make these privates and implement getters and setters?
  public:
    EMB_RES m_resource;
    CIEC_STRING m_instanceName;
    CIEC_STRING m_GUID;
    CIEC_STRING m_bootFileLocation;
    const fmi2CallbackFunctions *m_callbackFunctions;
    fmi2Boolean m_loggingCategories[NUMBER_OF_LOG_CATEGORIES];
    coSimulationState m_state;
    fmi2Real m_stopTime; //if the environment tries to compute past stopTime the FMU has to return fmi2Status = fmi2Error

    fmiTimerHandler& getTimer(){
      return static_cast<fmiTimerHandler&>(CDevice::getTimer());
    }

    void startInstance(void);
    void resetInstance(void);
    void stopInstance(void);
    bool loadFBs();

  public:
#ifdef FMU_DEBUG
    static std::fstream staticFile;
    static CPCSyncObject mutex;
    static void printToFile(const char* message);
#endif

    std::vector<fmuValueContainer*> m_outputsAndInputs;
    std::vector<fmuValueContainer*> m_internalVariables;
    std::vector<CFunctionBlock*> m_parametersFBs;
    std::vector<CFunctionBlock*> m_commFBs;

    fmuInstance(fmi2String instanceName, fmi2String GUID, fmi2String bootFileLocation, const fmi2CallbackFunctions *callbackFunctions);

    static CIEC_ANY::EDataTypeID getConnectedDataType(unsigned int portIndex, bool pa_isInput, CFunctionBlock* pa_poFB);
    void fillInterfaceElementsArray(CFunctionBlock* pa_poFB, bool isInput, bool isEvent);

    virtual ~fmuInstance();
  private:
    void populateInputsOutputs(forte::core::CFBContainer* resource, std::vector<fmuValueContainer*>* pa_parameters);
    void populateInputsAndOutputsCore(CFunctionBlock* pa_poFB, std::vector<fmuValueContainer*>* pa_parameters);


};
#endif /*_FMUINSTANCE_H_ */

