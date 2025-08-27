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

#include "forte/esfb.h"

namespace forte::core::io {

  class IOConfigFBBase : public CEventSourceFB {
    public:
      IOConfigFBBase(forte::core::CFBContainer &paContainer,
                     const SFBInterfaceSpec &paInterfaceSpec,
                     const forte::core::StringId paInstanceNameId);
      ~IOConfigFBBase() override;
  };

} // namespace forte::core::io

#endif /* SRC_CORE_IO_CONFIGFB_BASE_H_ */
