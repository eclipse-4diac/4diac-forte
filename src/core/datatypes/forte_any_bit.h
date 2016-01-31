/*******************************************************************************
  * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Monika Wenger,
  *    Martin Melik Merkumians
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _ANY_BIT_H_
#define _ANY_BIT_H_

#include "forte_any_elementary.h"

/*!\ingroup COREDTS IIEC_ANY_BIT represents any bit data types according to IEC 61131.
 */
class CIEC_ANY_BIT : public CIEC_ANY_ELEMENTARY{
  DECLARE_FIRMWARE_DATATYPE(ANY_BIT)

  public:
    virtual ~CIEC_ANY_BIT(){
    }

  protected:
    CIEC_ANY_BIT() :
        CIEC_ANY_ELEMENTARY(){
    }
};

#endif /*_MANY_BIT_H_*/
