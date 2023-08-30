/*******************************************************************************
 * Copyright (c) 2005 - 2011 Profactor GmbH, ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl, Monika Wenger,
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _ANY_NUM_H_
#define _ANY_NUM_H_

#include "forte_any_magnitude.h"

/*!\ingroup COREDTS CIEC_ANY_NUM represents any number data types according to IEC 61131.

 */
class CIEC_ANY_NUM : public CIEC_ANY_MAGNITUDE {
  public:
    ~CIEC_ANY_NUM() override = default;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_ANY;
    }
    
  protected:
    CIEC_ANY_NUM() = default;

};

#endif /*_MANY_NUM_H_*/
