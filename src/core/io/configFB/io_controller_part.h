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

#ifndef SRC_CORE_IO_CONFIGFB_IO_CONTROLLER_PART_H_
#define SRC_CORE_IO_CONFIGFB_IO_CONTROLLER_PART_H_

#include "io_adapter_split.h"
#include "io_base.h"
#include "io_controller_split.h"

namespace forte {
  namespace core {
    namespace IO {

class IOConfigFBPartController: public IOConfigFBBase {
public:
  IOConfigFBPartController(CResource *pa_poSrcRes,
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

  IOConfigFBSplitAdapter& IOConfigFBMultiAdapter() {
    return (*static_cast<IOConfigFBSplitAdapter*>(m_apoAdapters[0]));
  }

  static const int scm_nSplitAdapterAdpNum = 0;

  virtual void executeEvent(int pa_nEIID);

  IOConfigFBSplitController* master;

  virtual void initHandles() = 0;

  void initHandle(IODeviceController::HandleDescriptor *handleDescriptor);

};

    } //namespace IO
  } //namepsace core
} //namespace forte

#endif /* SRC_CORE_IO_CONFIGFB_IO_CONTROLLER_PART_H_ */
