/*******************************************************************************
 * Copyright (c) 2018 Johannes Kepler University
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _EXTEVHANDLERHELPER_H_
#define _EXTEVHANDLERHELPER_H_

#include "../funcbloc.h"
#include "../device.h"
#include "../resource.h"
#include "../devexec.h"
#include "../extevhan.h"


/*!\brief Get an external event handler by finding the device a FB is contained in
 *
 */
template<typename T>
T& getExtEvHandler(CFunctionBlock& paFB){
  return paFB.getResource().getDevice().getDeviceExecution().getExtEvHandler<T>();
}



#endif /* SRC_CORE_UTILS_EXTEVHANDLERHELPER_H_ */
