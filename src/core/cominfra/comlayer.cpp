/*******************************************************************************
  * Copyright (c) 2010-2014 fortiss, TU Wien ACIN and Profactor GmbH.
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Alois Zoitl - initial implementation and rework communication infrastructure
  *    Michael Hofmann - rework communication infrastructure
  *******************************************************************************/
#include "comlayer.h"
#include "commfb.h"

using namespace forte::com_infra;

CComLayer::CComLayer(CComLayer* pa_poUpperLayer, CBaseCommFB * pa_poComFB) :
  m_eConnectionState(forte::com_infra::e_Disconnected), m_poTopLayer(pa_poUpperLayer), m_poBottomLayer(0), m_poFb(pa_poComFB){
  if(0 != m_poTopLayer){
    m_poTopLayer->setBottomLayer(this);
  }
}

CComLayer::~CComLayer(){
  if(m_poBottomLayer != 0){
    m_poBottomLayer->closeConnection();
    delete m_poBottomLayer;
  }
}

EComResponse CComLayer::processInterrupt(){
  return e_Nothing;
}
