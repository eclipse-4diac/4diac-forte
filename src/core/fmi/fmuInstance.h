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
#ifndef _FMUINSTANCE_H_
#define _FMUINSTANCE_H_

#include "fmuConfig.h"
#include "fmuValueContainer.h"
#include "fmiTimerHandler.h"
#include "../device.h"
#include <vector>
#include <map>
#include <fstream>
#include "../device.h"
#include "../../stdfblib/ita/EMB_RES.h"

class fmuInstance : public CDevice{

  public:
    fmuInstance(fmi2String instanceName, fmi2String GUID, fmi2String bootFileLocation, const fmi2CallbackFunctions *callbackFunctions);
    virtual ~fmuInstance();

#ifdef FMU_DEBUG
    std::fstream debugFile;
    void printToFile(const char* message);
#endif

    void startInstance(void);
    void resetInstance(void);
    void stopInstance(void);
    bool loadFBs();

    static fmuInstance* sFmuInstance;

    void registerEcet(CFMUEventChainExecutionThread* paExecutionThread){
      mNumberOfEcets++;
      paExecutionThread->setAllowedToRun(&mAllowEcetToRun);
      paExecutionThread->setStepSemaphore(&mEcetSemaphore);
      static_cast<fmiTimerHandler*>(&getTimer())->addExecutionThread(paExecutionThread);
    }

    bool advanceInstanceTime(CIEC_LREAL& mTime){
      return static_cast<fmiTimerHandler&>(getTimer()).advanceTicks(mTime, &mAllowEcetToRun, &mEcetSemaphore);
    }

    const CIEC_STRING& getBootFileLocation() const{
      return mBootFileLocation;
    }

    const fmi2CallbackFunctions* getCallbackFunctions() const{
      return mCallbackFunctions;
    }

    const CIEC_STRING& getGuid() const{
      return mGUID;
    }

    const CIEC_STRING& getInstanceName() const{
      return mInstanceName;
    }

    fmi2Boolean* getLoggingCategories(){
      return mLoggingCategories;
    }

    const EMB_RES& getResource() const{
      return *mResource;
    }

    coSimulationState getState() const{
      return mState;
    }

    fmi2Real getStopTime() const{
      return mStopTime;
    }

    void setState(coSimulationState state){
      mState = state;
    }

    std::vector<CFunctionBlock*>& getCommFBs(){
      return mCommFBs;
    }

    const std::vector<fmuValueContainer*>& getOutputsAndInputs() const{
      return mOutputsAndInputs;
    }

    void setStopTime(fmi2Real stopTime){
      mStopTime = stopTime;
    }

  private:
    EMB_RES* mResource;
    CIEC_STRING mInstanceName;
    CIEC_STRING mGUID;
    CIEC_STRING mBootFileLocation;
    const fmi2CallbackFunctions *mCallbackFunctions;
    fmi2Boolean mLoggingCategories[NUMBER_OF_LOG_CATEGORIES];
    coSimulationState mState;
    fmi2Real mStopTime; //if the environment tries to compute past stopTime the FMU has to return fmi2Status = fmi2Error

    std::vector<fmuValueContainer*> mOutputsAndInputs;
    std::vector<CFunctionBlock*> mCommFBs;
    std::map<CDataConnection*, CIEC_ANY*> mParameters;

    void populateInputsOutputs(forte::core::CFBContainer* paResource);
    void populateInputsAndOutputsCore(CFunctionBlock* paFB);

    CIEC_ANY::EDataTypeID getConnectedDataType(unsigned int paPortIndex, bool paIsInput, CFunctionBlock* paFB);

    void fillInterfaceElementsArray(CFunctionBlock* paFB, bool paIsInput, bool paIsEvent);

    unsigned int mNumberOfEcets;

    bool mAllowEcetToRun;

    forte::arch::CSemaphore mEcetSemaphore;

    static CSyncObject sFmuInstanceMutex;

};
#endif /*_FMUINSTANCE_H_ */

