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

#include "slave2301.h"

namespace EmBrick {

Slave2301::Slave2301(int address, Packages::SlaveInit init) : Slave(address, init) {
	sendImage = reinterpret_cast<SendImage*>(updateSendImage);
	sendImage->Relays = 0;
}

Slave2301::~Slave2301() {
	// TODO Auto-generated destructor stub
}

void Slave2301::prepareUpdate() {
	sendImage->Relays = 0x01;
}

} /* namespace EmBrick */
