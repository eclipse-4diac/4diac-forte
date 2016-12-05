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

#ifndef SRC_MODULES_EMBRICK_SLAVE_SLAVE2181_H_
#define SRC_MODULES_EMBRICK_SLAVE_SLAVE2181_H_

#include "slave.h"

namespace EmBrick {

class Slave2181: public Slave {
  struct SendImage {
    uint8_t DigitalOutputs;
  };

public:
  Slave2181(int address, Packages::SlaveInit init);

  void set(uint8_t pin, bool state);
protected:
  SendImage* sendImage;

  virtual void prepareUpdate();
};

} /* namespace EmBrick */

#endif /* SRC_MODULES_EMBRICK_SLAVE_SLAVE2181_H_ */
