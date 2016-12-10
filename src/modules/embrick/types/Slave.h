/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_MODULES_EMBRICK_TYPES_SLAVE_H_
#define SRC_MODULES_EMBRICK_TYPES_SLAVE_H_

#include <funcbloc.h>
#include "BusAdapter.h"

#include <handler/bus.h>
#include <slave/slave.h>

namespace EmBrick {
namespace FunctionBlocks {

class Slave: public CFunctionBlock {
public:
  Slave(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec,
      const CStringDictionary::TStringId pa_nInstanceNameId,
      TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData);
  virtual ~Slave() {
  }

protected:
  BusAdapter& BusAdapterOut() {
    return (*static_cast<BusAdapter*>(m_apoAdapters[0]));
  }

  static const int scm_nBusAdapterOutAdpNum = 0;
  BusAdapter& BusAdapterIn() {
    return (*static_cast<BusAdapter*>(m_apoAdapters[1]));
  }

  static const int scm_nBusAdapterInAdpNum = 1;

  virtual void executeEvent(int pa_nEIID);

  SlaveType type;
  EmBrick::Slave *slave;

  virtual bool init(int index);

  void addInputBitHandle(CIEC_WSTRING id, uint8_t offset, uint8_t pos);
  void addOutputBitHandle(CIEC_WSTRING id, uint8_t offset, uint8_t pos);

};

} /* namespace FunctionsBlocks */
} /* namespace EmBrick */

#endif /* SRC_MODULES_EMBRICK_TYPES_SLAVE_H_ */
