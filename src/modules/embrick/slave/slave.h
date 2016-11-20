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

#ifndef SRC_MODULES_EMBRICK_SLAVE_SLAVE_H_
#define SRC_MODULES_EMBRICK_SLAVE_SLAVE_H_

#include <stdint.h>
#include <cstring>

namespace EmBrick {

class Slave {
public:
	static Slave* sendInit(int address);
	virtual ~Slave();

	const unsigned int address;

protected:
	Slave(int address);
};

} /* namespace EmBrick */

#endif /* SRC_MODULES_EMBRICK_SLAVE_SLAVE_H_ */
