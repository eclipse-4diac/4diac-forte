/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "pin.h"
#include <devlog.h>

const char * const EmbrickPinHandler::scmFailedToOpenFile = "Failed to open sysfs file.";
const char * const EmbrickPinHandler::scmFailedToWriteFile = "Failed to write sysfs file.";
const char * const EmbrickPinHandler::scmNotInitialised = "Failed to write to not initialised sysfs stream.";

EmbrickPinHandler::EmbrickPinHandler(unsigned int paPin) :
    mError(0),
    mPinStr((std::ostringstream() << std::dec << paPin).str()) { // Convert pin int to string

  // Disable buffer to avoid latency
  mStream.rdbuf()->pubsetbuf(0, 0);

  // Init pin
  init();
}

EmbrickPinHandler::~EmbrickPinHandler() {
  deInit();
}

void EmbrickPinHandler::init() {
  std::string fileName;
  mStream.clear();

  // Enable pin
  fileName = "/sys/class/gpio/export";
  mStream.open(fileName.c_str(), std::fstream::out);
  if(!mStream.is_open()) {
    return fail(scmFailedToOpenFile);
  }
  mStream << mPinStr;
  if(mStream.fail()) {
    return fail(scmFailedToWriteFile);
  }
  mStream.close();

  // Use pin as output
  fileName = "/sys/class/gpio/gpio" + mPinStr + "/direction";
  mStream.open(fileName.c_str(), std::fstream::out);
  if(!mStream.is_open()) {
    return fail(scmFailedToOpenFile);
  }
  mStream.clear();

  mStream << "out";
  if(mStream.fail()) {
    return fail(scmFailedToWriteFile);
  }
  mStream.close();

  // Prepare pin stream for usage
  fileName = "/sys/class/gpio/gpio" + mPinStr + "/value";
  mStream.open(fileName.c_str(), std::fstream::out);
  if(!mStream.is_open()) {
    return fail(scmFailedToOpenFile);
  }
  DEVLOG_INFO("emBrick[PinHandler]: GPIO %s ready.\n", mPinStr.data());
}

void EmbrickPinHandler::deInit() {
  std::string fileName;

  // Close pin stream
  if(mStream.is_open()) {
    mStream.clear();
    mStream.close();
  }

  // Disable pin
  fileName = "/sys/class/gpio/unexport";
  mStream.open(fileName.c_str(), std::fstream::out);
  if(!mStream.is_open()) {
    return fail(scmFailedToOpenFile);
  }
  mStream << mPinStr;
  if(mStream.fail()) {
    return fail(scmFailedToWriteFile);
  }
  mStream.close();

  DEVLOG_INFO("emBrick[PinHandler]: GPIO %s stopped.\n", mPinStr.data());
}

bool EmbrickPinHandler::set(bool paState) {
  if(!mStream.is_open()) {
    fail(scmNotInitialised);
    return false;
  }

  mStream.clear();
  mStream.seekp(0, std::ios::beg);

  unsigned int val = paState ? 1 : 0;
  mStream << val;
  if(mStream.fail()) {
    fail(scmFailedToWriteFile);
    return false;
  }

  return true;
}

void EmbrickPinHandler::fail(const char* paReason) {
  mError = paReason;
  DEVLOG_ERROR("emBrick[PinHandler]: %s\n", paReason);
}

