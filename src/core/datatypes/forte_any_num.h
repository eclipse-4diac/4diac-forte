/*******************************************************************************
  * Copyright (c) 2005 - 2011 Profactor GmbH, ACIN
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
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
class CIEC_ANY_NUM : public CIEC_ANY_MAGNITUDE{
  DECLARE_FIRMWARE_DATATYPE(ANY_NUM)

  public:
    virtual ~CIEC_ANY_NUM(){
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_ANY;
    }
  protected:
    CIEC_ANY_NUM() {

    }

};

#endif /*_MANY_NUM_H_*/
