/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#ifndef _FORTESTITERATOR_H_
#define _FORTESTITERATOR_H_

#include "forte_any.h"
#include "iec61131_cast_helper.h"
#include "./utils/staticassert.h"
#include "iec61131_functions.h"

#include <math.h>

/** @brief This iterator class is a generator/input iterator, producing a new value on each iteration, until the preset goal value is met
 *
 */
template<typename E, typename B = E>
class ST_FOR_ITER {
  public:
    ST_FOR_ITER(E &paCounter, const E paStart, const E paTo, const B paBy = B(1)) :
        mCounter(paCounter), mInternalCounter(paStart), mTo(paTo), mBy(paBy) {
      mCounter = mInternalCounter;
      const typename B::TValueType byValue = static_cast<typename B::TValueType>(mBy);
      const typename E::TValueType toValue = static_cast<typename E::TValueType>(mTo);
      if (byValue > 0) {
        const typename E::TValueType limit = E::scmMaxVal - static_cast<typename E::TValueType>(byValue);
        if (toValue > limit) {
          DEVLOG_ERROR("The given TO value would produce an overflow - TO changed to the highest achievable value\n");
          mTo = E(limit);
        }
      } else if (byValue < 0) {
        const typename E::TValueType limit = E::scmMinVal - static_cast<typename E::TValueType>(byValue); // byValue is negative, so its in fact an addition
        if (toValue < limit) {
          DEVLOG_ERROR("The given TO value would produce an underflow - TO changed to the lowest achievable value\n");
          mTo = E(limit);
        }
      } else {
        DEVLOG_ERROR("Potential infinite loop - BY value is 0\n");
      }
    }

    virtual ~ST_FOR_ITER() = default;

    ST_FOR_ITER begin() {
      return isExpired() ? ST_FOR_ITER() : *this;
    }

    ST_FOR_ITER end() {
      return ST_FOR_ITER();
    }

    ST_FOR_ITER operator++() {
      const typename E::TValueType internalCounterValue = static_cast<typename E::TValueType>(mInternalCounter);
      const typename B::TValueType byValue = static_cast<typename B::TValueType>(mBy);

      mInternalCounter = E(internalCounterValue + static_cast<typename E::TValueType>(byValue));
      mCounter = mInternalCounter;
      return *this;
    }

    E& operator*() {
      return mInternalCounter;
    }

    bool operator==(const ST_FOR_ITER &other) {
      return (*this).isExpired() == other.isExpired();
    }

    bool operator!=(const ST_FOR_ITER &other) {
      return !((*this).isExpired() == other.isExpired());
    }

  protected:
  private:
    ST_FOR_ITER() : mCounter(mInternalCounter), mInternalCounter(E(1)), mTo(E(0)), mBy(B(1)) {
    }

    bool isExpired() const {
      const typename B::TValueType byValue = static_cast<typename B::TValueType>(mBy);
      if(byValue > 0) {
        return static_cast<typename E::TValueType>(mInternalCounter) > static_cast<typename E::TValueType>(mTo) ? true : false;
      }
      return static_cast<typename E::TValueType>(mInternalCounter) < static_cast<typename E::TValueType>(mTo) ? true : false;
    }

    E &mCounter;
    E mInternalCounter;
    E mTo;
    B mBy;

    /*********************************************/
    /* Type checking section, not for actual use */
    /*********************************************/
    void testMethod() {
      static_assert((std::is_base_of<CIEC_ANY_INT, E>::value), "E is not of type ANY_INT");
      static_assert((std::is_base_of<CIEC_ANY_INT, B>::value), "B is not of type ANY_INT");
    }
};

#endif /* _FORTESTITERATOR_H_ */
