/************************************************************************************
 Copyright (c) 2024 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#ifndef ZEPHYRIO_HANDLEDESCRIPTOR_H
#define ZEPHYRIO_HANDLEDESCRIPTOR_H

#include "core/io/device/io_controller.h"
#include "core/io/mapper/io_handle.h"
#include "extevhan.h"

class IOHandleDescriptor : public forte::core::io::IODeviceController::HandleDescriptor {
public:
  enum HandleType {
    Gpio,
    Adc,
    Pwm,
  };

  HandleType mType;

  IOHandleDescriptor(std::string const &paId,
    forte::core::io::IOMapper::Direction paDirection, HandleType paType)
    : HandleDescriptor(paId, paDirection), mType(paType) {}
};

#endif /* ZEPHYRIO_HANDLEDESCRIPTOR_H */
