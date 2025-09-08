/*******************************************************************************
 * Copyright (c) 2021, 2022 Jonathan Lainer (kontakt@lainer.co.at)
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jonathan Lainer - Initial implementation.
 *******************************************************************************/

#include "PinHandle.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/util/extevhandlerhelper.h"

IOHandleGPIO::IOHandleGPIO(EliteBoardDeviceController *paDeviceCtrl, EliteBoardDeviceController::GPIODescriptor desc) :
    IOHandle(static_cast<IODeviceController *>(paDeviceCtrl), desc.mDirection, CIEC_ANY::e_BOOL),
    mGPIO_Port(desc.mGPIO_Port),
    mGPIO_Pin(desc.mPin),
    mId(desc.mId) {
}

void IOHandleGPIO::onObserver(IOObserver *paObserver) {
  IOHandle::onObserver(paObserver);
  mDirection = paObserver->getDirection();

  // Since the direction of the GPIO is set at this time, the controller-handle-list has to be updated
  mController->updateHandleList(mId, this);

  GPIO_InitTypeDef initParams{.Pin = mGPIO_Pin,
                              .Mode = (mDirection == IOMapper::In) ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT_PP,
                              .Pull = GPIO_NOPULL,
                              .Speed = GPIO_SPEED_FREQ_LOW};
  HAL_GPIO_Init(mGPIO_Port, &initParams);
}

void IOHandleGPIO::dropObserver() {
  IOHandle::dropObserver();
  mDirection = IOMapper::UnknownDirection;
  HAL_GPIO_DeInit(mGPIO_Port, mGPIO_Pin);
}

void IOHandleGPIO::get(CIEC_ANY &paState) {
  static_cast<CIEC_BOOL &>(paState) = HAL_GPIO_ReadPin(mGPIO_Port, mGPIO_Pin) ? true_BOOL : false_BOOL;
}

void IOHandleGPIO::set(const CIEC_ANY &paState) {
  auto targetState = (true == static_cast<const CIEC_BOOL &>(paState)) ? GPIO_PIN_SET : GPIO_PIN_RESET;
  HAL_GPIO_WritePin(mGPIO_Port, mGPIO_Pin, targetState);
}
