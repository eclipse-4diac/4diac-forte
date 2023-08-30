/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
 *               2020 Johannes Kepler University Linz
 *               2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Monika Wenger,
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *    Ernst Blecha - add multibit partial access
 *    Martin Melik Merkumians - changes multibit access index from template
 *      parameter to call argument
  *******************************************************************************/
#ifndef _ANY_BIT_H_
#define _ANY_BIT_H_

#include "forte_any_elementary.h"
#include "../utils/staticassert.h"
#include <limits>

/*!\ingroup COREDTS IIEC_ANY_BIT represents any bit data types according to IEC 61131.
 */
class CIEC_ANY_BIT : public CIEC_ANY_ELEMENTARY{
  public:
    ~CIEC_ANY_BIT() override = default;

  protected:
    CIEC_ANY_BIT() = default;
};

#endif /*_MANY_BIT_H_*/
