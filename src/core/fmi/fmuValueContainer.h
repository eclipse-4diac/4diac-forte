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

#ifndef SRC_CORE_FMI_FMUVALUECONTAINER_H_
#define SRC_CORE_FMI_FMUVALUECONTAINER_H_

#include "../datatypes/forte_any.h"
#include "../datatypes/forte_bool.h"
#include "../datatypes/forte_lint.h"
#include "../datatypes/forte_lreal.h"
#include "../datatypes/forte_string.h"

class fmuValueContainer{

  public:
    enum valueType{
      BOOL, INTEGER, REAL, STRING, WRONG,
    };

    union UPointerToValue{
        TForteUInt32* mEventCounter;
        CIEC_ANY* mActualValue;
    };

    typedef void (*newValueArrived)(void*);

  public:
    explicit fmuValueContainer(fmuValueContainer::valueType pa_valueType, bool pa_needPointerToValue);
    fmuValueContainer(fmuValueContainer& pa_Source);
    virtual ~fmuValueContainer();

    void setCallback(newValueArrived pa_callback);
    void setCallbackArgument(void* pa_callbackArguments);
    void setEventCounterPointer(TForteUInt32* pa_eventCounter);
    void setValuePointer(CIEC_ANY* pa_valuePointer);

    void setValue(const CIEC_ANY& pa_value);

    CIEC_BOOL* getValueAsBool() const;
    CIEC_LINT* getValueAsInt() const;
    CIEC_LREAL* getValueAsReal() const;
    CIEC_STRING* getValueAsString() const;

    CIEC_ANY::EDataTypeID getDataTypeID() const{
      return mValue->getDataTypeID();
    }


    static fmuValueContainer::valueType getValueFromType(CIEC_ANY::EDataTypeID pa_type);

  private:
    CIEC_ANY* allocateValuePointer(fmuValueContainer::valueType pa_valueType);


  protected:
    CIEC_ANY* mValue;
    bool mErrorOcurred;
    newValueArrived mCallback;
    void* mCallBackArgument;
    UPointerToValue* mPointerToValue;
    bool mIsEvent;

};

#endif /* SRC_CORE_FMI_FMUVALUECONTAINER_H_ */
