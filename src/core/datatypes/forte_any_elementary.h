/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN, nxtControl GmbH, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Martin Melik Merkumians, Alois Zoitl,
 *    Monika Wenger, Stansilav Meduna
 *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _ANY_ELE_H_
#define _ANY_ELE_H_

#include "forte_any.h"
#include <ctype.h>
#include <string_utils.h>

/*!\ingroup COREDTS IIEC_ANY_ELEMENTARY represents the elementary data types according to
 *  IEC 61131.
 */
class CIEC_ANY_ELEMENTARY : public CIEC_ANY{
  DECLARE_FIRMWARE_DATATYPE(ANY_ELEMENTARY)

  public:

    CIEC_ANY_ELEMENTARY(const CIEC_ANY_ELEMENTARY &pa_roVal) :
        CIEC_ANY(){
      setLargestUInt(pa_roVal.getLargestUInt());
    }

    virtual ~CIEC_ANY_ELEMENTARY(){
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_ANY;
    }

    virtual int toString(char* paValue, size_t paBufferSize) const;
    virtual int fromString(const char *pa_rsValue);

  protected:
    CIEC_ANY_ELEMENTARY() {
    }

  private:

    int checkTypeSpec(const char* pa_pacValue, const char* pa_pacHashPos) const;
    bool isCastable(CStringDictionary::TStringId paTypeNameId) const;

    static EDataTypeID getElementaryDataTypeId(CStringDictionary::TStringId paTypeNameId);

    const static CStringDictionary::TStringId scm_anTypeNameStringIds[];
    const static int scm_nMaxTypeNameLength = 13;

};

#endif /*_MANY_ELE_H_*/
