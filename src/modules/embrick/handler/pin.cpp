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
    paError(0), mPinStr(static_cast<std::ostringstream &>((std::ostringstream() << std::dec // Convert pin int to string
      << paPin)).str()) {
  // Disable buffer to avoid latency
  paStream.rdbuf()->pubsetbuf(0, 0);

  // Init pin
  init();
}

EmbrickPinHandler::~EmbrickPinHandler() {
  deInit();
}

void EmbrickPinHandler::init() {
  std::string fileName;
  paStream.clear();

  // Enable pin
  fileName = "/sys/class/gpio/export";
  paStream.open(fileName.c_str(), std::fstream::out);
  if(!paStream.is_open()) {
    return fail(scmFailedToOpenFile);
  }
  paStream << mPinStr;
  if(paStream.fail()) {
    return fail(scmFailedToWriteFile);
  }
  paStream.close();

  // Use pin as output
  fileName = "/sys/class/gpio/gpio" + mPinStr + "/direction";
  paStream.open(fileName.c_str(), std::fstream::out);
  if(!paStream.is_open()) {
    return fail(scmFailedToOpenFile);
  }
  paStream.clear();

  paStream << "out";
  if(paStream.fail()) {
    return fail(scmFailedToWriteFile);
  }
  paStream.close();

  // Prepare pin stream for usage
  fileName = "/sys/class/gpio/gpio" + mPinStr + "/value";
  paStream.open(fileName.c_str(), std::fstream::out);
  if(!paStream.is_open()) {
    return fail(scmFailedToOpenFile);
  }
  DEVLOG_INFO("emBrick[PinHandler]: GPIO %s ready.\n", mPinStr.data());
}

void EmbrickPinHandler::deInit() {
  std::string fileName;

  // Close pin stream
  if(paStream.is_open()) {
    paStream.clear();
    paStream.close();
  }

  // Disable pin
  fileName = "/sys/class/gpio/unexport";
  paStream.open(fileName.c_str(), std::fstream::out);
  if(!paStream.is_open()) {
    return fail(scmFailedToOpenFile);
  }
  paStream << mPinStr;
  if(paStream.fail()) {
    return fail(scmFailedToWriteFile);
  }
  paStream.close();

  DEVLOG_INFO("emBrick[PinHandler]: GPIO %s stopped.\n", mPinStr.data());
}

bool EmbrickPinHandler::set(bool paState) {
  if(!paStream.is_open()) {
    fail(scmNotInitialised);
    return false;
  }

  paStream.clear();
  paStream.seekp(0, std::ios::beg);

  unsigned int val = paState ? 1 : 0;
  paStream << val;
  if(paStream.fail()) {
    fail(scmFailedToWriteFile);
    return false;
  }

  return true;
}

void EmbrickPinHandler::fail(const char* paReason) {
  paError = paReason;
  DEVLOG_ERROR("emBrick[PinHandler]: %s\n", paReason);
}

