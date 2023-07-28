/*******************************************************************************
 * Copyright (c) 2022 Martin Erich Jobst
 *               2023 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Jobst
 *      - initial implementation
 *******************************************************************************/
#ifndef BARECTF_PLATFORM_FORTE_H
#define BARECTF_PLATFORM_FORTE_H

#include <string>
#include <fstream>
#include <memory>
#include <filesystem>

#include "stringdict.h"

#include "barectf.h"

class BarectfPlatformFORTE final {
private:
    std::ofstream output;
    std::unique_ptr<uint8_t []> buffer;
    barectf_default_ctx context;

    static bool enabled;
    static std::filesystem::path traceDirectory;

    static uint64_t getClock(void *const data);
    static int isBackendFull(void *data);
    static void openPacket(void *data);
    static void closePacket(void * data);
    static const struct barectf_platform_callbacks barectfCallbacks;
    static std::string dateCapture(void);
public:
    barectf_default_ctx *getContext() {
      return &context;
    }

    BarectfPlatformFORTE(std::filesystem::path filename, size_t bufferSize);
    BarectfPlatformFORTE(CStringDictionary::TStringId instanceName, size_t bufferSize);
    ~BarectfPlatformFORTE();

    BarectfPlatformFORTE(const BarectfPlatformFORTE&) = delete;
    BarectfPlatformFORTE& operator=(const BarectfPlatformFORTE&) = delete;

    static void setup(std::string directory);
};

#endif // BARECTF_PLATFORM_FORTE_H
