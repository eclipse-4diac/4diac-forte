/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Rene Smodic, Gerhard Ebenhofer, Ingo Hegny,
 *   Martin Melik Merkumians
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef GEN_PUBL_H_
#define GEN_PUBL_H_

#include "GEN_PUBLISH.h"
#include <localcomlayer.h>

/*! \brief Implementation of the PUBL SIFB.
 *
 */

class GEN_PUBL : public GEN_PUBLISH{
  DECLARE_GENERIC_FIRMWARE_FB(GEN_PUBL)
  public:
    GEN_PUBL(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

    virtual bool configureFB(const char *pa_acConfigString);

  private:

    virtual char * getDefaultIDString(){
      return forte::com_infra::CLocalComLayer::getDefaultLocalCommIdString(ID().getValue());
    };
};

#endif /*GEN_PUBL_H_*/
