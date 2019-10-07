/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_ARCH_RCX_RCXUTILITIES_H_
#define SRC_ARCH_RCX_RCXUTILITIES_H_

#include <stdlib.h>
#include "../datatype.h"

/*! \brief Retrieves a random string containing [a-zA-Z0-9]
 *
 *  Gets a random string containing [a-zA-Z0-9]
 *
 *  @param pa_destString String where to store the result random string. It has to have space for pa_length plus 1 for the ending char.
 *  @param pa_length Length of the string without the ending char.
 *
 */
void getRandomString(char* pa_destString, const TForteUInt32 pa_length);

#endif /* SRC_ARCH_RCX_RCXUTILITIES_H_ */
