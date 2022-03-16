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
        counter(paCounter), internalCounter(paStart), to(paTo), by(paBy), done(false) {
      counter = internalCounter;
      const typename B::TValueType byValue = static_cast<typename B::TValueType>(by);
      const typename E::TValueType toValue = static_cast<typename E::TValueType>(to);
      if (byValue > 0) {
        const typename E::TValueType limit = E::scm_nMaxVal - static_cast<typename E::TValueType>(byValue);
        if (toValue > limit) {
          DEVLOG_ERROR("The given TO value would produce an overflow - TO changed to the highest achievable value");
          to = E(limit);
        }
      } else if (byValue < 0) {
        const typename E::TValueType limit = E::scm_nMinVal - static_cast<typename E::TValueType>(byValue); // byValue is negative, so its in fact an addition
        if (toValue < limit) {
          DEVLOG_ERROR("The given TO value would produce an underflow - TO changed to the lowest achievable value");
          to = E(limit);
        }
      } else {
        DEVLOG_ERROR("Potential infinite loop - BY value is 0");
      }
    }

    virtual ~ST_FOR_ITER() = default;

    ST_FOR_ITER begin() {
      return *this;
    }

    ST_FOR_ITER end() {
      return ST_FOR_ITER(true);
    }

    ST_FOR_ITER operator++() {
      const typename E::TValueType internalCounterValue = static_cast<typename E::TValueType>(internalCounter);
      const typename B::TValueType byValue = static_cast<typename B::TValueType>(by);

      internalCounter = E(internalCounterValue + static_cast<typename E::TValueType>(byValue));
      counter = internalCounter;
      if(byValue > 0) {
        done = internalCounter > to ? true : false;
      } else {
        done = internalCounter < to ? true : false;
      }
      return *this;
    }

    E& operator*() {
      return internalCounter;
    }

    bool operator==(const ST_FOR_ITER &other) {
      return (*this).done == other.done;
    }

    bool operator!=(const ST_FOR_ITER &other) {
      return !((*this).done == other.done);
    }

  protected:
  private:
    ST_FOR_ITER(const bool paDone) :
        counter(internalCounter), done(paDone) {
    }

    E &counter;
    E internalCounter;
    E to;
    E by;
    bool done;

    /*********************************************/
    /* Type checking section, not for actual use */
    /*********************************************/
    void testMethod() {
      FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_INT, E>::value), EIsNotOfTypeAnyInt);
      FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_INT, B>::value), BIsNotOfTypeAnyInt);
    }
};

#endif /* _FORTESTITERATOR_H_ */
