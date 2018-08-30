/*******************************************************************************
 * Copyright (c) 2017 - 2018 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#ifndef SRC_CORE_IO_CONFIGFB_BASE_H_
#define SRC_CORE_IO_CONFIGFB_BASE_H_

#include <esfb.h>

namespace forte {
  namespace core {
    namespace io {

      class IOConfigFBBase : public CEventSourceFB {
        public:
          IOConfigFBBase(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId,
              TForteByte *paFBConnData, TForteByte *paFBVarsData);
          virtual ~IOConfigFBBase();
      };

    } //namespace IO
  } //namepsace core
} //namespace forte

#endif /* SRC_CORE_IO_CONFIGFB_BASE_H_ */
