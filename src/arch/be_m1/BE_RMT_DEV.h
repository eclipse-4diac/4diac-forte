/*******************************************************************************
 * Copyright (c) 2011 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef BE_RMT_DEV_H_
#define BE_RMT_DEV_H_

#include "../../../stdfblib/ita/RMT_DEV.h"
#include <arch/be_m1/fortemodule.h>

class BE_RMT_DEV : public RMT_DEV{
  public:
    explicit BE_RMT_DEV(CFORTEModule &pa_roModule);

    virtual ~BE_RMT_DEV();

    CFORTEModule & getModule() const{
      return m_roModule;
    }
  protected:
  private:
    CFORTEModule & m_roModule;
};

#endif /* BE_RMT_DEV_H_ */
