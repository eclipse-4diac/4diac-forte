/*******************************************************************************
 * Copyright (c) 2021 Davor Cihlar
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Davor Cihlar - multiple FBs sharing a single Modbus connection
 *******************************************************************************/
#ifndef _MODBUSENUMS_H_
#define _MODBUSENUMS_H_

enum EModbusFunction {
  eDiscreteInput = 0,
  eCoil,
  eInputRegister,
  eHoldingRegister
};

enum EModbusFlowControl {
  eFlowNone = 0,
  eFlow485,
  eFlowArduino,
  eFlowDelay,
  eFlowLongDelay,
};

#endif
