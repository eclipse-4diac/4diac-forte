/*******************************************************************************
 * Copyright (c) 2026 Sichuan Qunyuan Technology Co., Ltd.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Sichuan Qunyuan Technology Co., Ltd. - initial API and implementation
 *******************************************************************************/

#pragma once

#include <cstdint>
#include <vector>
#include "ecrt.h"

namespace forte::eclipse4diac::io::ethercat {

  typedef enum {In, Out} SyncDir;

  struct PdoEntry {
    uint16_t mPdoIndex;
    uint16_t mIndex;
    uint8_t mSubIndex;
    uint8_t mBitLength;

    PdoEntry() = default;
    PdoEntry(uint16_t paPdoIndex, uint16_t paIndex, uint8_t paSubIndex, uint8_t paBitLength);
  };

  struct Pdo {
    uint16_t mIndex;
    SyncDir mSyncDir;

    Pdo() = default;
    Pdo(uint16_t paIndex, SyncDir paDir);
  };

  struct Sync {
    uint8_t mIndex;
    SyncDir mSyncDir;

    Sync() = default;
    Sync(uint8_t paIndex, SyncDir paDir);
  };

  struct EntryReg {
    uint16_t mAlias;
    uint16_t mPosition;
    uint32_t mVendorId;
    uint32_t mProductCode;
    uint16_t mIndex;
    uint8_t mSubIndex;
    unsigned int* mOffset;

    EntryReg() = default;
    EntryReg(uint16_t paAlias, uint16_t paPosition, uint32_t paVendorId, uint32_t paProductCode, uint16_t paIndex, uint8_t paSubIndex, unsigned int *paOffset);
  };

  struct ECDeviceModel {
    uint16_t mAlias;
    uint16_t mPosition;
    uint32_t mVendorId;
    uint32_t mProductCode;

    uint32_t mSlotIndexInc;
    uint32_t mSlotPdoInc;

    std::vector<Sync> mSyncList;
    std::vector<Pdo> mPdoList;
    std::vector<PdoEntry> mPdoEntryList;
    std::vector<EntryReg> mEntryRegList;

    ECDeviceModel() = default;
    ECDeviceModel(uint16_t paAlias, uint16_t paPosition, uint32_t paVendorId, uint32_t paProductCode, uint32_t paSlotIndexInc, uint32_t paSlotPdoInc);
    ~ECDeviceModel();

    void addSync(uint8_t paSyncIndex, SyncDir paDir);
    void addPdo(uint16_t paPdoIndex,  SyncDir paDir);
    void addPdoEntry(uint16_t paPdoIndex, uint16_t paEntryIndex, uint8_t paSubIndex, uint8_t paBitLength);
    void addEntryReg(uint16_t paIndex, uint8_t paSubIndex, unsigned int *paOffset);

    ec_sync_info_t *getSyncs();
    ec_pdo_entry_reg_t *getDomainRegs();

    void cleanupSync();
    void cleanupDomainReg();

    void getPdoSize(unsigned int &paRxPdoSize, unsigned int &paTxPdoSize);
    void getPdoEntrySize(uint16_t paPdoIndex, unsigned int &paPdoEntrySize);

  private:
    ec_sync_info_t *cachedSyncs = nullptr;
    unsigned int cachedRxPdoSize = 0;
    unsigned int cachedTxPdoSize = 0;

    ec_pdo_entry_reg_t *cachedDomainRegs = nullptr;
  };
}