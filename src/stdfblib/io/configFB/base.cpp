/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (messmer@fortiss.org)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "../configFB/base.h"

namespace IO {
namespace ConfigurationFB {

Base::Base(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    CEventSourceFB(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId,
        pa_acFBConnData, pa_acFBVarsData) {
  // TODO Auto-generated constructor stub

}

Base::~Base() {
  // TODO Auto-generated destructor stub
}

} /* namespace ConfigurationFB */
} /* namespace IO */
