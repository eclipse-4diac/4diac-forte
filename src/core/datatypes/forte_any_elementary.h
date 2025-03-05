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
#include "core/util/string_utils.h"
#include <map>

/*!\ingroup COREDTS IIEC_ANY_ELEMENTARY represents the elementary data types according to
 *  IEC 61131.
 */
class CIEC_ANY_ELEMENTARY : public CIEC_ANY {
  public:

    CIEC_ANY_ELEMENTARY(const CIEC_ANY_ELEMENTARY &paVal) :
        CIEC_ANY() {
      setLargestUInt(paVal.getLargestUInt());
    }

    ~CIEC_ANY_ELEMENTARY() override = default;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_ANY;
    }

    int toString(char* paValue, size_t paBufferSize) const override;
    int fromString(const char *paValue) override;

    static EDataTypeID getElementaryDataTypeId(CStringDictionary::TStringId paTypeNameId);

  protected:
    CIEC_ANY_ELEMENTARY() = default;

  private:

    bool isTypeSpecifier(const char* paValue, const char* paHashPosition) const;
    bool isCastable(CStringDictionary::TStringId paTypeNameId) const;

    const static std::map<CStringDictionary::TStringId, CIEC_ANY::EDataTypeID> scm_StringToTypeId;

    const static int scmMaxTypeNameLength = 13;

};

#endif /*_MANY_ELE_H_*/
