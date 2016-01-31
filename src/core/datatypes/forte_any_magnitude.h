/*******************************************************************************
  * Copyright (c) 2005 - 2011 Profactor GmbH, ACIN
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Alois Zoitl, Monika Wenger,
  *    Martin Melik Markumians
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _ANY_MAG_H_
#define _ANY_MAG_H_

#include "forte_any_elementary.h"

/*!\ingroup COREDTS CIEC_ANY_MAGNITUDE represents the magnitude data types according to
 *  IEC 61131.

 */
class CIEC_ANY_MAGNITUDE : public CIEC_ANY_ELEMENTARY{
  DECLARE_FIRMWARE_DATATYPE(ANY_MAGNITUDE)

  public:
    virtual ~CIEC_ANY_MAGNITUDE(){
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_ANY;
    }

  protected:
    CIEC_ANY_MAGNITUDE(){
    }
};

#endif /*_MANY_MAG_H_*/
