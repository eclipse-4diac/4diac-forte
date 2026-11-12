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
 *   Zijun Tang - initial API and implementation
 *******************************************************************************/

#pragma once

#include <cstdint>
#include <vector>
#include "ecrt.h"
#include "forte/arch/forte_sync.h"

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

  /** getSyncs() puts all outputs into one sync manager and all inputs into another. */
  constexpr uint8_t OUTPUT_SYNC_INDEX = 2;
  constexpr uint8_t INPUT_SYNC_INDEX = 3;

  /** A PDO entry is registered by its position in the mapping, because entry indices need not be unique. */
  struct EntryReg {
    uint16_t mIndex;
    uint8_t mSubIndex;
    uint8_t mSyncIndex;
    unsigned int mPdoPos;
    unsigned int mEntryPos;
    unsigned int mDomainOffset;
    unsigned int mBitPosition;
    bool mOffsetValid;
    unsigned int *mHandleOffset;

    EntryReg(uint16_t paIndex, uint8_t paSubIndex, uint8_t paSyncIndex, unsigned int paPdoPos, unsigned int paEntryPos);
  };

  struct ECDeviceModel {
    uint16_t mAlias;
    uint16_t mPosition;
    uint32_t mVendorId;
    uint32_t mProductCode;

    uint32_t mSlotIndexInc = 0;
    uint32_t mSlotPdoInc = 0;

    std::vector<Sync> mSyncList;
    std::vector<Pdo> mPdoList;
    std::vector<PdoEntry> mPdoEntryList;
    std::vector<EntryReg> mEntryRegList;

    /** Guards the lists and the handle bindings, which are used by both the FB thread and the bus thread. */
    arch::CSyncObject mMutex;
    /** Set when the bus hands the model to the EtherCAT master; the lists must not grow afterwards. */
    bool mFrozen = false;

    ECDeviceModel() = default;
    ECDeviceModel(uint16_t paAlias, uint16_t paPosition, uint32_t paVendorId, uint32_t paProductCode, uint32_t paSlotIndexInc, uint32_t paSlotPdoInc);
    ~ECDeviceModel();

    // The caller must hold mMutex for the following functions.
    void addSync(uint8_t paSyncIndex, SyncDir paDir);
    void addPdo(uint16_t paPdoIndex,  SyncDir paDir);
    void addPdoEntry(uint16_t paPdoIndex, uint16_t paEntryIndex, uint8_t paSubIndex, uint8_t paBitLength);
    /** Registers the last added PDO entry and returns its index in mEntryRegList. */
    size_t addEntryReg();
    unsigned int syncBitLength(SyncDir paDir) const;
    ec_sync_info_t *getSyncs();

    // The following functions lock mMutex themselves.
    bool hasEntryReg(size_t paRegIndex, bool paRequireDomainOffset);
    /** Fails if the entry is not registered or already bound to another handle. */
    bool bindHandleOffset(size_t paRegIndex, unsigned int *paHandleOffset);
    void unbindHandleOffset(const unsigned int *paHandleOffset);
    void commitDomainOffsets();

    void cleanupSync();

    void getPdoSize(unsigned int &paRxPdoSize, unsigned int &paTxPdoSize);
    void getPdoEntrySize(uint16_t paPdoIndex, unsigned int &paPdoEntrySize);

  private:
    ec_sync_info_t *cachedSyncs = nullptr;
    unsigned int cachedRxPdoSize = 0;
    unsigned int cachedTxPdoSize = 0;
  };
}