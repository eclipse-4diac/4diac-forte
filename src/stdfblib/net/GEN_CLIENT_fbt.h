/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Rene Smodic, Gerhard Ebenhofer, Martin Melik Merkumians
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _GEN_CLIENT_H_
#define _GEN_CLIENT_H_

#include "../../core/cominfra/commfb.h"

/*! \brief Implementation of the CLIENT_* SIFB.
 *
 */

class GEN_CLIENT : public forte::com_infra::CCommFB {
  DECLARE_GENERIC_FIRMWARE_FB(GEN_CLIENT)

public:
  GEN_CLIENT(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);
  ~GEN_CLIENT() override = default;

};

#endif //_GEN_CLIENT_H_
