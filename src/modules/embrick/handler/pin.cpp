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

#include "pin.h"
#include <devlog.h>

namespace EmBrick {
namespace Handlers {

const char * const Pin::scmFailedToOpenFile =
    "Failed to open sysfs file.";
const char * const Pin::scmFailedToWriteFile =
    "Failed to write sysfs file.";
const char * const Pin::scmNotInitialised =
    "Failed to write to not initialised sysfs stream.";

Pin::Pin(unsigned int pin) :
    error(0) {
  // Convert pin int to string
  pinStr = static_cast<std::ostringstream &>((std::ostringstream() << std::dec
      << pin)).str();
  // Disable buffer to avoid latency
  stream.rdbuf()->pubsetbuf(0, 0);

  // Init pin
  init();
}

Pin::~Pin() {
  deInit();
}

void Pin::init() {
  std::string fileName;
  stream.clear();

  // Enable pin
  fileName = "/sys/class/gpio/export";
  stream.open(fileName.c_str(), std::fstream::out);
  if (!stream.is_open())
    return fail(scmFailedToOpenFile);

  stream << pinStr;
  if (stream.fail())
    return fail(scmFailedToWriteFile);
  stream.close();

  // Use pin as output
  fileName = "/sys/class/gpio/gpio" + pinStr + "/direction";
  stream.open(fileName.c_str(), std::fstream::out);
  if (!stream.is_open())
    return fail(scmFailedToOpenFile);
  stream.clear();

  stream << "out";
  if (stream.fail())
    return fail(scmFailedToWriteFile);
  stream.close();

  // Prepare pin stream for usage
  fileName = "/sys/class/gpio/gpio" + pinStr + "/value";
  stream.open(fileName.c_str(), std::fstream::out);
  if (!stream.is_open())
    return fail(scmFailedToOpenFile);

  DEVLOG_INFO("emBrick[PinHandler]: GPIO %s ready.\n", pinStr.data());
}

void Pin::deInit() {
  std::string fileName;

  // Close pin stream
  if (stream.is_open()) {
    stream.clear();
    stream.close();
  }

  // Disable pin
  fileName = "/sys/class/gpio/unexport";
  stream.open(fileName.c_str(), std::fstream::out);
  if (!stream.is_open())
    return fail(scmFailedToOpenFile);

  stream << pinStr;
  if (stream.fail())
    return fail(scmFailedToWriteFile);
  stream.close();

  DEVLOG_INFO("emBrick[PinHandler]: GPIO %s stopped.\n", pinStr.data());
}

bool Pin::set(bool state) {
  if (!stream.is_open()) {
    fail(scmNotInitialised);
    return false;
  }

  stream.clear();
  stream.seekp(0, std::ios::beg);

  unsigned int val = state ? 1 : 0;
  stream << val;
  if (stream.fail()) {
    fail(scmFailedToWriteFile);
    return false;
  }

  return true;
}

void Pin::fail(const char* reason) {
  error = reason;
  DEVLOG_ERROR("emBrick[PinHandler]: %s\n", reason);
}

} /* namespace Handlers */
} /* namespace EmBrick */
