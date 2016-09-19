/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Rene Smodic, Gerhard Ebenhofer, Martin Melik Merkumians
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _GEN_PUBLISHOPCUA_H_
#define _GEN_PUBLISHOPCUA_H_

#include "../../core/cominfra/commfb.h"

/*! \brief Implementation of the PUBLISH SIFB.
 */

class GEN_PUBLISHOPCUA : public forte::com_infra::CCommFB {
DECLARE_GENERIC_FIRMWARE_FB(GEN_PUBLISHOPCUA)

public:
  GEN_PUBLISHOPCUA(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);
  virtual ~GEN_PUBLISHOPCUA(){};
};

#endif //_GEN_PUBLISHOPCUA_H_



