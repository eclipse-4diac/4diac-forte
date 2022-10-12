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
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _ANY_DURATION_H_
#define _ANY_DURATION_H_

#include "forte_any_magnitude.h"

/*!\ingroup COREDTS CIEC_ANY_DURATION represents the user defined data types according to
 *  IEC 61131.
 */
class CIEC_ANY_DURATION : public CIEC_ANY_MAGNITUDE {
  DECLARE_FIRMWARE_DATATYPE(ANY_DURATION)
public:

  /*! \brief Defines the time base in units per second that will be used in the TIME data type
   *
   *  The default value will be 1 000 000 000, which means that the time bas is 1 ns.
   *  For Smaller devices which do not use 64 bit datatypes a time base of 1000 (i.e., 1ms)
   *  or 1000000 (i.e., 1micro s) may be suitable.
   */
  static const uint_fast64_t csmForteTimeBaseUnitsPerSecond = 1000000000; /* 1E9 */

  ~CIEC_ANY_DURATION() override = default;

  EDataTypeID getDataTypeID() const override {
    return CIEC_ANY::e_ANY;
  }

protected:
  CIEC_ANY_DURATION() = default;
};

#endif /*_ANY_DURATION_H_*/