/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Rene Smodic, Gerhard Ebenhofer, Ingo Hegny,
 *   Martin Melik Merkumians
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef GEN_PUBL_H_
#define GEN_PUBL_H_

#include "GEN_PUBLISH.h"

/*! \brief Implementation of the PUBL SIFB.
 *
 */

class GEN_PUBL : public GEN_PUBLISH{
  DECLARE_GENERIC_FIRMWARE_FB(GEN_PUBL)
  public:
    GEN_PUBL(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);

    bool configureFB(const char *paConfigString) override;

  protected:
    static const char * const scmLocalIDPrefix;
    static const char * const scmLocalIDSuffix;

  private:
    char * getDefaultIDString(const char *paID) override;
};

#endif /*GEN_PUBL_H_*/
