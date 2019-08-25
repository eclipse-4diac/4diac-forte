/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
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
#ifndef _ANY_REA_H_
#define _ANY_REA_H_

#include "forte_any_num.h"

/*!\ingroup COREDTS CIEC_ANY_REAL represents any real data types according to IEC 61131.

 */
class CIEC_ANY_REAL : public CIEC_ANY_NUM{
  DECLARE_FIRMWARE_DATATYPE(ANY_REAL)

  public:
    virtual ~CIEC_ANY_REAL(){
    }

  protected:
    CIEC_ANY_REAL() {
    }
};

#endif /*_MANY_REA_H_*/
