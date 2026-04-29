/*******************************************************************************
 * Copyright (c) 2026 HR Agrartechnik GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Franz Höpfinger
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte/arch/forte_fileio.h"
#include "forte/arch/forte_ramdisk.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <map>
#include <mutex>
#include <string>
#include <vector>

struct RamdiskFileInfo {
    std::string filename;
    char mode;
};

static std::map<std::string, std::vector<char>> g_ramdiskEntries;
static std::map<void *, RamdiskFileInfo> g_ramdiskOpenFiles;
static std::mutex g_ramdiskMutex;

extern "C" {

char *forte_getenv(const char *env_var) {
  return getenv(env_var);
}

size_t forte_strnlen_s(const char *str, size_t strsz) {
  if (nullptr == str) {
    return 0;
  }
  return strnlen(str, strsz);
}

int forte_ramdisk_load(const char *filename) {
  if (filename == nullptr) {
    return -1;
  }

  std::ifstream file(filename, std::ios::binary | std::ios::ate);
  if (!file) {
    std::lock_guard<std::mutex> lock(g_ramdiskMutex);
    g_ramdiskEntries[filename] = std::vector<char>();
    return -1;
  }

  std::streamsize size = file.tellg();
  if (size < 0) {
    std::lock_guard<std::mutex> lock(g_ramdiskMutex);
    g_ramdiskEntries[filename] = std::vector<char>();
    return -1;
  }

  file.seekg(0, std::ios::beg);

  std::vector<char> buffer(static_cast<size_t>(size));
  if (!file.read(buffer.data(), size)) {
    std::lock_guard<std::mutex> lock(g_ramdiskMutex);
    g_ramdiskEntries[filename] = std::vector<char>();
    return -1;
  }

  std::lock_guard<std::mutex> lock(g_ramdiskMutex);
  g_ramdiskEntries[filename] = std::move(buffer);
  return 0;
}

void forte_ramdisk_unload(const char *filename) {
  if (filename == nullptr) {
    return;
  }
  std::lock_guard<std::mutex> lock(g_ramdiskMutex);
  g_ramdiskEntries.erase(filename);
}

void *forte_fopen(const char *filename, const char *mode) {
  if (filename != nullptr && mode != nullptr) {
    std::lock_guard<std::mutex> lock(g_ramdiskMutex);
    auto it = g_ramdiskEntries.find(filename);
    if (it != g_ramdiskEntries.end()) {
      // Only use RAMDISK for read modes; write modes fallback to real file
      if (mode[0] == 'r') {
        if (it->second.empty()) {
          return nullptr;
        }
        FILE *file = fmemopen(it->second.data(), it->second.size(), mode);
        if (file != nullptr) {
          g_ramdiskOpenFiles[file] = {filename, mode[0]};
        }
        return file;
      }
    }
  }
  return fopen(filename, mode);
}

int forte_fclose(void *file) {
  FILE *f = static_cast<FILE *>(file);
  {
    std::lock_guard<std::mutex> lock(g_ramdiskMutex);
    auto it = g_ramdiskOpenFiles.find(f);
    if (it != g_ramdiskOpenFiles.end()) {
      if (it->second.mode == 'r') {
        // Remove from RAMDISK; vector memory freed automatically by RAII
        g_ramdiskEntries.erase(it->second.filename);
      }
      g_ramdiskOpenFiles.erase(it);
    }
  }
  return fclose(f);
}

char *forte_fgets(char *str, int count, void *file) {
  return fgets(str, count, static_cast<FILE *>(file));
}

int forte_fseek(void *file, long offset, int whence) {
  return fseek(static_cast<FILE *>(file), offset, whence);
}

long forte_ftell(void *file) {
  return ftell(static_cast<FILE *>(file));
}

int forte_feof(void *file) {
  return feof(static_cast<FILE *>(file));
}

size_t forte_fread(void *ptr, size_t itemsize, size_t nitems, void *file) {
  return fread(ptr, itemsize, nitems, static_cast<FILE *>(file));
}

size_t forte_fwrite(const void *ptr, size_t itemsize, size_t nitems, void *file) {
  return fwrite(ptr, itemsize, nitems, static_cast<FILE *>(file));
}

} // extern "C"
