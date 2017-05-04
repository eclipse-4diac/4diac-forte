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

#ifndef SRC_MODULES_EMBRICK_HANDLER_PIN_H_
#define SRC_MODULES_EMBRICK_HANDLER_PIN_H_

#include <fstream>
#include <sstream>
#include <string>
#include <forte_wstring.h>

namespace EmBrick {
namespace Handlers {

class Pin {
  friend class Bus;

protected:
  Pin(unsigned int pin);
  virtual ~Pin();

  bool set(bool state);
  bool enable() {
    return set(true);
  }
  bool disable() {
    return set(false);
  }

  void init();
  void deInit();

  unsigned int pin;
  std::fstream stream;

  bool hasError() {
    return error != 0;
  }
  const char* error;

private:
  void fail(const char* reason);

  static const char * const scmFailedToOpenFile;
  static const char * const scmFailedToWriteFile;
  static const char * const scmNotInitialised;
};

} /* namespace Handlers */
} /* namespace EmBrick */

#endif /* SRC_MODULES_EMBRICK_HANDLER_PIN_H_ */
