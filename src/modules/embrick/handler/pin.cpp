/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "pin.h"
#include <devlog.h>

const char * const EmbrickPinHandler::scmFailedToOpenFile =
    "Failed to open sysfs file.";
const char * const EmbrickPinHandler::scmFailedToWriteFile =
    "Failed to write sysfs file.";
const char * const EmbrickPinHandler::scmNotInitialised =
    "Failed to write to not initialised sysfs stream.";

EmbrickPinHandler::EmbrickPinHandler(unsigned int pin) : pinStr(static_cast<std::ostringstream &>((std::ostringstream() << std::dec  // Convert pin int to string
    << pin)).str()),
    error(0) {
  // Disable buffer to avoid latency
  stream.rdbuf()->pubsetbuf(0, 0);

  // Init pin
  init();
}

EmbrickPinHandler::~EmbrickPinHandler() {
  deInit();
}

void EmbrickPinHandler::init() {
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

void EmbrickPinHandler::deInit() {
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

bool EmbrickPinHandler::set(bool state) {
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

void EmbrickPinHandler::fail(const char* reason) {
  error = reason;
  DEVLOG_ERROR("emBrick[PinHandler]: %s\n", reason);
}


