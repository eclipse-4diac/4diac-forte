/*******************************************************************************
 * Copyright (c) 2013, 2015 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _LMS_DEV_H_
#define _LMS_DEV_H_

#include <device.h>
#include "EMB_RMT_RES.h"

class EMB_RMT_DEV : public CDevice{


public:
  EMB_RMT_RES MGR;

  EMB_RMT_DEV(const char * pa_acId);
  virtual ~EMB_RMT_DEV();

/*! \brief Adds additional functionality to the originals execute func of the device.
 *
 * This is that it waits till the thread of the MGR resource has anded
 */
  virtual int startDevice(void);

  virtual EMGMResponse changeFBExecutionState(EMGMCommandType pa_unCommand);

private:

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

};

#endif /*EMB_RMT_DEV_H_*/
