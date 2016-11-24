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

#include "slave2181.h"

namespace EmBrick {

Slave2181::Slave2181(int address, Packages::SlaveInit init) :
		Slave(address, init) {
	sendImage = reinterpret_cast<SendImage*>(updateSendImage);
	sendImage->DigitalOutputs = 0;
}

void Slave2181::prepareUpdate() {
	set(5, true);
}

void Slave2181::set(uint8_t pin, bool state) {
	// TODO Make function thread safe

	// Set digital output
	if (state)
		sendImage->DigitalOutputs = (uint8_t) (sendImage->DigitalOutputs | 1 << pin);
	else
		sendImage->DigitalOutputs = (uint8_t) (sendImage->DigitalOutputs & ~(1 << pin));

	// TODO Inform bus about change -> maybe wake up to force loop

	// TODO Confirm the operation with a confirm event
	// DISCUSS The QX blocks automatically send an confirm event -> is that intended?
}

} /* namespace EmBrick */
