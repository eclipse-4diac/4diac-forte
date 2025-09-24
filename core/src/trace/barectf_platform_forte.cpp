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

#include "barectf_platform_forte.h"

#include <iomanip>
#include <chrono>

#include "forte/arch/forte_architecture_time.h"
#include "forte/util/devlog.h"
#include "forte/util/mainparam_utils.h"

namespace forte::trace {
  namespace {
    class OutputDirectoryOption final
        : public util::CommandLineParser::
              OptionImpl<"t", "trace", "<directory>", "Set the output directory for CTF traces"> {
      public:
        bool parseOption(const std::string_view paArgument) override {
          BarectfPlatformFORTE::setup(paArgument);
          return true;
        }
    };

    [[maybe_unused]] OutputDirectoryOption gOutputDirectory;

    constinit std::string traceDirectory;
    constinit bool enabled = false;

    std::string dateCapture() {
      const auto now = std::chrono::system_clock::now();
      const std::time_t time = std::chrono::system_clock::to_time_t(now);
      const auto millisecondsPart =
          std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
      struct tm ptm;
      arch::forte_localtime(&time, &ptm);
      std::ostringstream stream;
      stream << std::put_time(&ptm, "%Y%m%d_%H%M%S");
      stream << std::setfill('0') << std::setw(3) << millisecondsPart;
      return stream.str();
    }
  } // namespace

  void BarectfPlatformFORTE::setup(std::string_view paDirectory) {
    if (paDirectory.empty()) {
      DEVLOG_INFO("[TRACE_CTF]: no output directory given, disabling TRACE_CTF\n");
      enabled = false;
      return;
    }
    std::filesystem::path directory = std::filesystem::path(paDirectory).make_preferred();
    if (directory.is_relative()) {
      directory = std::filesystem::absolute(directory);
    }

    if (std::filesystem::is_directory(directory)) {
      DEVLOG_INFO("[TRACE_CTF]: enabling TRACE_CTF, output in \"%s\"\n", directory.c_str());
      enabled = true;
      traceDirectory = directory.string();
    } else {
      DEVLOG_INFO("[TRACE_CTF]: non-existent output directory given \"%s\", disabling TRACE_CTF\n", directory.c_str());
      enabled = false;
    }
  }

  uint64_t BarectfPlatformFORTE::getClock(void *const) {
    return arch::getNanoSecondsMonotonic();
  }

  int BarectfPlatformFORTE::isBackendFull(void *data) {
    BarectfPlatformFORTE *platform = static_cast<BarectfPlatformFORTE *>(data);
    return platform->output.fail();
  }

  void BarectfPlatformFORTE::openPacket(void *data) {
    if (enabled) {
      BarectfPlatformFORTE *platform = static_cast<BarectfPlatformFORTE *>(data);
      barectf_default_open_packet(&platform->context);
    }
  }

  void BarectfPlatformFORTE::closePacket(void *data) {
    if (enabled) {
      BarectfPlatformFORTE *platform = static_cast<BarectfPlatformFORTE *>(data);
      barectf_default_close_packet(&platform->context);
      platform->output.write(reinterpret_cast<const char *>(barectf_packet_buf(&platform->context)),
                             barectf_packet_buf_size(&platform->context));
    }
  }

  constinit barectf_platform_callbacks BarectfPlatformFORTE::barectfCallbacks = {.default_clock_get_value = getClock,
                                                                                 .is_backend_full = isBackendFull,
                                                                                 .open_packet = openPacket,
                                                                                 .close_packet = closePacket};

  BarectfPlatformFORTE::BarectfPlatformFORTE(std::filesystem::path filename, size_t bufferSize) :
      buffer(enabled ? new uint8_t[bufferSize] : nullptr) {
    if (enabled) {
      output = std::ofstream(filename, std::ios::binary);
      barectf_init(&context, buffer.get(), static_cast<uint32_t>(bufferSize), barectfCallbacks, this);
      barectf_enable_tracing(&context, enabled);
      openPacket(this);
    } else {
      barectf_init(&context, buffer.get(), static_cast<uint32_t>(0), barectfCallbacks, this);
      barectf_enable_tracing(&context, enabled);
    }
  }

  BarectfPlatformFORTE::BarectfPlatformFORTE(StringId instanceName, size_t bufferSize) :
      BarectfPlatformFORTE(std::filesystem::path{traceDirectory} /
                               (std::string("trace_") + (instanceName.data() ? instanceName.data() : "null") + "_" +
                                dateCapture() + ".ctf"),
                           bufferSize) {
  }

  BarectfPlatformFORTE::~BarectfPlatformFORTE() {
    if (enabled) {
      if (barectf_packet_is_open(&context)) {
        closePacket(this);
      }
      output.flush();
    }
  }
} // namespace forte::trace
