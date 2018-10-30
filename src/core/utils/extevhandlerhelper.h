/*******************************************************************************
  * Copyright (c) 2018 Johannes Kepler University
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
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
