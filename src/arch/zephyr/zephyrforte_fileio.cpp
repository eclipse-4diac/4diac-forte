/************************************************************************************
 Copyright (c) 2023 Dirk O. Kaar
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk O. Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#include "fortenew.h"
#include "../forte_fileio.h"
#include <cstring>
#include <zephyr/fs/fs.h>

char* forte_getenv(const char*) {
  return nullptr;
}

size_t forte_strnlen_s(const char *str, size_t strsz) {
  if (nullptr == str) {
    return 0;
  }
  for (size_t i = 0; i < strsz; ++i) {
    if (str[i] == '\0') {
      return i;
    }
  }
  return strsz;
}

void* forte_fopen(const char* filename, const char* mode) {
  auto file = new fs_file_t;
  fs_file_t_init(file);
  fs_mode_t flags = 0;
  for (size_t i = 0; i < strlen(mode); ++i) {
    switch (mode[i]) {
    case 'r': flags |= FS_O_READ; break;
    case 'w': flags |= (FS_O_WRITE | FS_O_CREATE); break;
    case 'a': flags |= (FS_O_WRITE | FS_O_APPEND); break;
    case '+': flags |= FS_O_RDWR; break;
    }
  }
  if (0 == fs_open(file, filename, flags)) {
    return file;
  }
  delete file;
  return nullptr;
}

int forte_fclose(void* file) {
  int res = -1;
  if (static_cast<fs_file_t*>(file)) {
    res = fs_close(static_cast<fs_file_t*>(file));
    delete static_cast<fs_file_t*>(file);
  }
  return res;
}

int forte_fseek(void* file, long offset, int whence) {
  int fs_whence;
  switch (whence) {
  case SEEK_CUR: fs_whence = FS_SEEK_CUR; break;
  case SEEK_END: fs_whence = FS_SEEK_END; break;
  case SEEK_SET:
  default:
    fs_whence = FS_SEEK_SET;
    break;
  }
  return fs_seek(static_cast<fs_file_t*>(file), offset, fs_whence) == 0 ? 0 : -1;
}

long forte_ftell(void* file) {
  return fs_tell(static_cast<fs_file_t*>(file));
}

int forte_feof(void* file) {
  auto pos = fs_tell(static_cast<fs_file_t*>(file));
  if (pos < 0) {
    return 0;
  }
  if (fs_seek(static_cast<fs_file_t*>(file), 0, FS_SEEK_END) != 0) {
    return 0;
  }
  auto end = fs_tell(static_cast<fs_file_t*>(file));
  if (pos >= end) {
    return 1;
  }
  fs_seek(static_cast<fs_file_t*>(file), pos, FS_SEEK_SET);
  return 0;
}

size_t forte_fread(void* ptr, size_t itemsize, size_t nitems, void* file) {
  auto res = fs_read(static_cast<fs_file_t*>(file), ptr, itemsize * nitems);
  return (res < 0) ? 0 : res / itemsize;
}

size_t forte_fwrite(const void *ptr, size_t itemsize, size_t nitems, void* file) {
  auto res = fs_write(static_cast<fs_file_t*>(file), ptr, itemsize * nitems);
  return (res < 0) ? 0 : res / itemsize;
}

namespace {
  int forte_fpositions(off_t* cur, off_t* end, void* file) {
    if ((*cur = fs_tell(static_cast<fs_file_t*>(file))) < 0) {
      return *cur;
    }
    if (int res = fs_seek(static_cast<fs_file_t*>(file), 0, FS_SEEK_END); res != 0) {
      return res;
    }
    if ((*end = fs_tell(static_cast<fs_file_t*>(file))) < 0) {
      return *end;
    }
    if (int res = fs_seek(static_cast<fs_file_t*>(file), *cur, FS_SEEK_SET); res != 0) {
      return res;
    }
    return 0;
  }
}

char* forte_fgets(char* str, int count, void* file) {
  if (count <= 0) {
    return nullptr;
  }
  off_t fbegin;
  off_t fend;
  if (forte_fpositions(&fbegin, &fend, file) != 0) {
    return nullptr;
  }
  const auto rem = fend - fbegin;
  count = (rem < count) ? rem : count - 1;
  ssize_t len = 0;
  const auto read = fs_read(static_cast<fs_file_t*>(file), str, count);
  while (true) {
    if (len >= read || str[len++] == '\n') {
      str[len] = '\0';
      break;
    }
  }
  if (read <= 0) {
    return nullptr;
  }
  // Rewind to start of next line.
  const off_t rewind = len - read;
  if (fs_seek(static_cast<fs_file_t*>(file), rewind, FS_SEEK_CUR) != 0) {
    return nullptr;
  }
  return str;
}

ssize_t forte_getline(char** lineptr, size_t* n, void* file) {
  if (*lineptr == nullptr) {
    *n = 100;
    *lineptr = static_cast<char*>(malloc(*n));
    if (*lineptr == nullptr) {
      return - 1;
    }
  }
  off_t fbegin;
  off_t fend;
  if (forte_fpositions(&fbegin, &fend, file) != 0) {
    return -1;
  }
  ssize_t len = 0;
  ssize_t read;
  // The text line buffer in lineptr may grow dynamically, the outer loop
  // repeats until an EOL or EOF occurs, the inner loop takes care of
  // reallocating the line buffer as needed.
  for (bool finished = false; finished == false; ) {
    // Set count to remaining available chars in the file or
    // in the current line buffer (excluding terminating zero).
    const auto rem = fend - fbegin - len;
    const auto count = (static_cast<size_t>(rem) < *n) ? rem : *n - 1;
    read = fs_read(static_cast<fs_file_t*>(file), *lineptr + len, count);
    if (read <= 0) {
      return -1;
    }
    // Update last read count to total read count in this unfinished line.
    read += len;
    while (true) {
      if ((*lineptr)[len] == '\n') {
        finished = true;
        (*lineptr)[++len] = '\0';
        break;
      // No EOL yet, but the line buffer is exhausted.
      } else if (static_cast<size_t>(++len) == *n) {
        *lineptr = static_cast<char*>(realloc(*lineptr, *n + 100));
        if (*lineptr != nullptr) {
          *n += 100;
        } else {
          finished = true;
          len = -1;
        }
        break;
      }
    }
  }
  // Rewind to start of next line.
  const off_t rewind = len > 0 ? len - read : -read;
  if (fs_seek(static_cast<fs_file_t*>(file), rewind, FS_SEEK_CUR) != 0) {
    return -1;
  }
  return len;
}

