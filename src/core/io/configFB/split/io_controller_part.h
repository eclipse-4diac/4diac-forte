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

#ifndef SRC_CORE_IO_CONFIGFB_SPLIT_IO_CONTROLLER_PART_H_
#define SRC_CORE_IO_CONFIGFB_SPLIT_IO_CONTROLLER_PART_H_

#include "io_adapter_split.h"
#include "../io_base.h"
#include "io_controller_split.h"

namespace IO {
namespace ConfigurationFB {

class PartController: public Base {
public:
  PartController(CResource *pa_poSrcRes,
      const SFBInterfaceSpec *pa_pstInterfaceSpec,
      const CStringDictionary::TStringId pa_nInstanceNameId,
      TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData);

protected:
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  }

  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  }

  static const TEventID scm_nEventMAPID = 0;

  static const TEventID scm_nEventMAPOID = 0;

  SplitAdapter& Adapter() {
    return (*static_cast<SplitAdapter*>(m_apoAdapters[0]));
  }

  static const int scm_nSplitAdapterAdpNum = 0;

  virtual void executeEvent(int pa_nEIID);

  SplitController* master;

  virtual void initHandles() = 0;

  void initHandle(Device::Controller::HandleDescriptor *handleDescriptor);

};

} /* namespace ConfigurationFB */
} /* namespace IO */

#endif /* SRC_CORE_IO_CONFIGFB_SPLIT_IO_CONTROLLER_PART_H_ */
