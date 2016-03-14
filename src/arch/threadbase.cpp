/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "threadbase.h"

namespace forte {
  namespace arch {

    CThreadBase::CThreadBase() :
        mAlive(false){

    }

    CThreadBase::~CThreadBase(){
    }

    void CThreadBase::end(void){
      setAlive(false);
      join();
    }

  } /* namespace arch */
} /* namespace forte */
