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

#ifndef SRC_CORE_IO_CONFIGFB_IO_CONTROLLER_SPLIT_H_
#define SRC_CORE_IO_CONFIGFB_IO_CONTROLLER_SPLIT_H_

#include "../io_controller.h"
#include "io_adapter_split.h"

#define FUNCTION_BLOCK_CTOR_FOR_IO_SPLIT_CONTROLLER(fbclass) \
 fbclass(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : \
 IO::ConfigurationFB::SplitController( (const TForteUInt8* const) &scm_splitAdapter, scm_splitAdapter_num, pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData)

namespace IO {
namespace ConfigurationFB {

class SplitController;

typedef CSinglyLinkedList<SplitController*> TControllerList;

class SplitController: public Controller {
public:
  SplitController(const TForteUInt8* const scm_splitAdapter,
      const TForteUInt8 scm_splitAdapter_num, CResource *pa_poSrcRes,
      const SFBInterfaceSpec *pa_pstInterfaceSpec,
      const CStringDictionary::TStringId pa_nInstanceNameId,
      TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData);

  static SplitController* getControllerById(TForteUInt16 id);

  using Controller::initHandle;

protected:

  virtual void onStartup();

  virtual void onStop();

  virtual void executeEvent(int pa_nEIID);

private:
  const TForteUInt8* scm_splitAdapter;
  const TForteUInt8 scm_splitAdapter_num;

  TForteUInt8 splitIterator;

  static TControllerList *instances;

  static TForteUInt16 instancesIncrement;

  TForteUInt16 id;

  static const char * const scmFailedToInitParts;
};

} /* namespace ConfigurationFB */
} /* namespace IO */

#endif /* SRC_CORE_IO_CONFIGFB_IO_CONTROLLER_SPLIT_H_ */
