/*******************************************************************************
 * Copyright (c) 2022 Martin Erich Jobst
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

#include "barectf_platform_forte.h"

#include "forte_architecture_time.h"

uint64_t BarectfPlatformFORTE::getClock(void *const data) {
  return getNanoSecondsMonotonic();
}

int BarectfPlatformFORTE::isBackendFull(void *data) {
  BarectfPlatformFORTE *platform = static_cast<BarectfPlatformFORTE *>(data);
  return platform->output.fail();
}

void BarectfPlatformFORTE::openPacket(void *data) {
  BarectfPlatformFORTE *platform = static_cast<BarectfPlatformFORTE *>(data);
  barectf_default_open_packet(&platform->context);
}

void BarectfPlatformFORTE::closePacket(void *data) {
  BarectfPlatformFORTE *platform = static_cast<BarectfPlatformFORTE *>(data);
  barectf_default_close_packet(&platform->context);
  platform->output.write(reinterpret_cast<const char *>(barectf_packet_buf(&platform->context)),
                         barectf_packet_buf_size(&platform->context));
}

const struct barectf_platform_callbacks BarectfPlatformFORTE::barectfCallbacks = {
        .default_clock_get_value = getClock,
        .is_backend_full = isBackendFull,
        .open_packet = openPacket,
        .close_packet = closePacket
};

BarectfPlatformFORTE::BarectfPlatformFORTE(std::string filename, size_t bufferSize) : output(filename,
                                                                                             std::ios::binary),
                                                                                      buffer(new uint8_t[bufferSize]) {
  barectf_init(&context, buffer.get(), static_cast<uint32_t>(bufferSize), barectfCallbacks, this);
  openPacket(this);
}

BarectfPlatformFORTE::BarectfPlatformFORTE(CStringDictionary::TStringId instanceName, size_t bufferSize)
        : BarectfPlatformFORTE(
        std::string("trace_") + (CStringDictionary::getInstance().get(instanceName) ?: "null") + ".ctf",
        bufferSize) {
}

BarectfPlatformFORTE::~BarectfPlatformFORTE() {
  if (barectf_packet_is_open(&context) && !barectf_packet_is_empty(&context)) {
    closePacket(this);
  }
  output.flush();
}
