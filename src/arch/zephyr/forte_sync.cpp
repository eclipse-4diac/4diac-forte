/************************************************************************************
 Copyright (c) 2023 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#include "forte_sync.h"

CZephyrSyncObject::CZephyrSyncObject() {
  mutex = static_cast<struct k_mutex*>(k_malloc(sizeof(struct k_mutex)));
  k_mutex_init(mutex);
}

CZephyrSyncObject::~CZephyrSyncObject() {
  k_free(mutex);
}
