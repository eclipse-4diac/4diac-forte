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

#ifndef SRC_MODULES_EMBRICK_HANDLER_PIN_H_
#define SRC_MODULES_EMBRICK_HANDLER_PIN_H_

#include <fstream>
#include <sstream>
#include <string>
#include <forte_wstring.h>

class EmbrickPinHandler {
    friend class EmbrickBusHandler;

  protected:
    explicit EmbrickPinHandler(unsigned int paPin);
    virtual ~EmbrickPinHandler();

    bool set(bool paState);
    bool enable() {
      return set(true);
    }
    bool disable() {
      return set(false);
    }

    void init();
    void deInit();

    bool hasError() {
      return paError != 0;
    }
    const char* paError;

  private:
    std::string mPinStr;
    std::fstream paStream;

    void fail(const char* paReason);

    static const char * const scmFailedToOpenFile;
    static const char * const scmFailedToWriteFile;
    static const char * const scmNotInitialised;
};

#endif /* SRC_MODULES_EMBRICK_HANDLER_PIN_H_ */
