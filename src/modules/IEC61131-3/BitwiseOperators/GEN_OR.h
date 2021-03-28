/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH
 *                      2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *******************************************************************************/

#ifndef _GEN_OR_H_
#define _GEN_OR_H_

#include "genbitbase.h"

class GEN_OR: public CGenBitBase {
DECLARE_GENERIC_FIRMWARE_FB(GEN_OR)

private:

  virtual void executeEvent(int paEIID);

  GEN_OR(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);
  virtual ~GEN_OR();

public:

  template<typename T> void calculateValue() {
    T oIn;
    T oOut;

    for (unsigned int nInputIndex = 0; nInputIndex < getFBInterfaceSpec()->m_nNumDIs; nInputIndex++) {

      oIn.saveAssign(*static_cast<T*>(getDI(nInputIndex)));

      if (0 == nInputIndex) {
        st_OUT().saveAssign(oIn);
      } else {
        oOut.saveAssign(st_OUT());
        st_OUT().saveAssign(OR(oOut, oIn));
      }
    }
  }
};

#endif //_GEN_OR_H_

