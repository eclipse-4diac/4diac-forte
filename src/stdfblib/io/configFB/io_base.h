/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_STDFBLIB_IO_CONFIGFB_BASE_H_
#define SRC_STDFBLIB_IO_CONFIGFB_BASE_H_

#include <esfb.h>

namespace IO {
namespace ConfigurationFB {

class Base: public CEventSourceFB {
public:
  Base(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec,
      const CStringDictionary::TStringId pa_nInstanceNameId,
      TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData);
  virtual ~Base();
};

} /* namespace ConfigurationFB */
} /* namespace IO */

#endif /* SRC_STDFBLIB_IO_CONFIGFB_BASE_H_ */
