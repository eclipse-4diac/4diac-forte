/*
 * fmuValueContainer.h
 *
 *  Created on: 16.08.2017
 *      Author: cabral
 */

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
        TForteUInt32* m_eventCounter;
        CIEC_ANY* m_actualValue;
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
      return m_value->getDataTypeID();
    }


    static fmuValueContainer::valueType getValueFromType(CIEC_ANY::EDataTypeID pa_type);

  private:
    CIEC_ANY* allocateValuePointer(fmuValueContainer::valueType pa_valueType);


  protected:
    CIEC_ANY* m_value;
    bool m_errorOcurred;
    newValueArrived m_callback;
    void* m_callBackArgument;
    UPointerToValue* m_pPointerToValue;
    bool m_isEvent;

};

#endif /* SRC_CORE_FMI_FMUVALUECONTAINER_H_ */
