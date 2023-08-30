/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Martin Melik Merkumians, Alois Zoitl,
 *    Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _ANY_DER_H_
#define _ANY_DER_H_

#include "forte_any.h"

/*!\ingroup COREDTS CIEC_ANY_DERIVED represents the user defined data types according to
 *  IEC 61131.
 */
class CIEC_ANY_DERIVED : public CIEC_ANY {
  public:
    ~CIEC_ANY_DERIVED() override = default;

  protected:
    CIEC_ANY_DERIVED() = default;
};

#endif /*_MANY_DER_H_*/
