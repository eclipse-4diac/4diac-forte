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

#ifndef SRC_MODULES_EMBRICK_SLAVE_SLAVE2301_H_
#define SRC_MODULES_EMBRICK_SLAVE_SLAVE2301_H_

#include "slave.h"

namespace EmBrick {

class Slave2301: public Slave {
	struct SendImage {
		uint8_t Relays;
	};

public:
	Slave2301(int address, Packages::SlaveInit init);
	virtual ~Slave2301();

protected:
	SendImage* sendImage;

	virtual void prepareUpdate();
};

} /* namespace EmBrick */

#endif /* SRC_MODULES_EMBRICK_SLAVE_SLAVE2301_H_ */
