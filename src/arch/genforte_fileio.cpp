/************************************************************************************
 Copyright (c) 2023 Dirk O. Kaar
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk O. Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#include "forte_fileio.h"
#include <cstdio>
#include <cstdlib>
#include <string.h>

char* forte_getenv(const char* env_var) {
  return getenv(env_var);
}

size_t forte_strnlen_s(const char *str, size_t strsz) {
  if (nullptr == str) {
    return 0;
  }
  return strnlen(str, strsz);
}

void* forte_fopen(const char* filename, const char* mode) {
 return fopen(filename, mode);
}

int forte_fclose(void* file) {
  return fclose(static_cast<FILE*>(file));
}

char* forte_fgets(char* str, int count, void* file) {
  return fgets(str, count, static_cast<FILE*>(file));
}

int forte_fseek(void* file, long offset, int whence) {
  return fseek(static_cast<FILE*>(file), offset, whence);
}

long forte_ftell(void* file) {
  return ftell(static_cast<FILE*>(file));
}

int forte_feof(void* file) {
  return feof(static_cast<FILE*>(file));
}

size_t forte_fread(void* ptr, size_t itemsize, size_t nitems, void* file) {
  return fread(ptr, itemsize, nitems, static_cast<FILE*>(file));
}

size_t forte_fwrite(const void *ptr, size_t itemsize, size_t nitems, void* file) {
  return fwrite(ptr, itemsize, nitems, static_cast<FILE*>(file));
}

