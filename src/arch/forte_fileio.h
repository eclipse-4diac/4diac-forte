/************************************************************************************
 Copyright (c) 2023 Dirk O. Kaar
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk O. Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#ifndef SRC_ARCH_FORTE_FILEIO_H_
#define SRC_ARCH_FORTE_FILEIO_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

char* forte_getenv(const char* env_var);

size_t forte_strnlen_s(const char *str, size_t strsz);

void* forte_fopen(const char* filename, const char* mode);

int forte_fclose(void* file);

int forte_fseek(void* file, long offset, int whence);

long forte_ftell(void* file);

int forte_feof(void* file);

size_t forte_fread(void* ptr, size_t itemsize, size_t nitems, void* file);

size_t forte_fwrite(const void *ptr, size_t itemsize, size_t nitems, void* file);

char* forte_fgets(char* str, int count, void* file);

ssize_t forte_getline(char** lineptr, size_t* n, void* file);

#ifdef __cplusplus
}
#endif

#endif /* SRC_ARCH_FORTE_FILEIO_H_ */
