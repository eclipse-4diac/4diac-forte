/*******************************************************************************
 * Copyright (c) 2017 - 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#ifndef SRC_CORE_IO_CONFIGFB_BASE_H_
#define SRC_CORE_IO_CONFIGFB_BASE_H_

#include "esfb.h"
#include "genfb.h"

namespace forte {
  namespace core {
    namespace io {

      class IOConfigFBBase : public CGenFunctionBlock<CEventSourceFB> {
        public:
          IOConfigFBBase(forte::core::CFBContainer &paContainer, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId);
          ~IOConfigFBBase() override;
          bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;
      };

    } //namespace IO
  } //namepsace core
} //namespace forte

#endif /* SRC_CORE_IO_CONFIGFB_BASE_H_ */
