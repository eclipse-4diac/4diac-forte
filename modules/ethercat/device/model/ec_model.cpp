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

#include "ec_model.h"
#include "forte/util/criticalregion.h"

namespace forte::eclipse4diac::io::ethercat {

  PdoEntry::PdoEntry(uint16_t paPdoIndex, uint16_t paIndex, uint8_t paSubIndex, uint8_t paBitLength) :
    mPdoIndex(paPdoIndex),mIndex(paIndex),mSubIndex(paSubIndex),mBitLength(paBitLength) {
  }

  Pdo::Pdo(uint16_t paIndex, SyncDir paSyncDir) :
    mIndex(paIndex), mSyncDir(paSyncDir) {
  }

  Sync::Sync(uint8_t paIndex, SyncDir paSyncDir) :
    mIndex(paIndex), mSyncDir(paSyncDir) {
  }

  EntryReg::EntryReg(uint16_t paIndex, uint8_t paSubIndex, uint8_t paSyncIndex, unsigned int paPdoPos, unsigned int paEntryPos) :
    mIndex(paIndex), mSubIndex(paSubIndex), mSyncIndex(paSyncIndex), mPdoPos(paPdoPos), mEntryPos(paEntryPos),
    mDomainOffset(0), mBitPosition(0), mOffsetValid(false), mHandleOffset(nullptr) {
  }

  ECDeviceModel::ECDeviceModel(uint16_t paAlias, uint16_t paPosition, uint32_t paVendorId, uint32_t paProductCode, uint32_t paSlotIndexInc, uint32_t paSlotPdoInc) :
    mAlias(paAlias), mPosition(paPosition), mVendorId(paVendorId), mProductCode(paProductCode), mSlotIndexInc(paSlotIndexInc), mSlotPdoInc(paSlotPdoInc),
    cachedSyncs(nullptr), cachedRxPdoSize(0), cachedTxPdoSize(0) {
  }

  ECDeviceModel::~ECDeviceModel() {
    cleanupSync();
  }

  void ECDeviceModel::addSync(uint8_t paIndex, SyncDir paSyncDir) {
    Sync sync {paIndex, paSyncDir};
    mSyncList.push_back(sync);
  }

  void ECDeviceModel::addPdo(uint16_t paIndex, SyncDir paSyncDir) {
    Pdo pdo {paIndex, paSyncDir};
    mPdoList.push_back(pdo);
  }

  void ECDeviceModel::addPdoEntry(uint16_t paPdoIndex, uint16_t paIndex, uint8_t paSubIndex, uint8_t paBitlength) {
    PdoEntry entry {paPdoIndex, paIndex, paSubIndex, paBitlength};
    mPdoEntryList.push_back(entry);
  }

  size_t ECDeviceModel::addEntryReg() {
    const PdoEntry &entry = mPdoEntryList.back();
    const Pdo &pdo = mPdoList.back();
    // Count the positions like getSyncs() builds the mapping, padding entries included.
    unsigned int pdoPos = 0;
    for (const Pdo &other : mPdoList) {
      if (other.mSyncDir == pdo.mSyncDir) {
        ++pdoPos;
      }
    }
    unsigned int entryPos = 0;
    getPdoEntrySize(pdo.mIndex, entryPos);
    const uint8_t syncIndex = (pdo.mSyncDir == SyncDir::Out) ? OUTPUT_SYNC_INDEX : INPUT_SYNC_INDEX;
    mEntryRegList.emplace_back(entry.mIndex, entry.mSubIndex, syncIndex, pdoPos - 1, entryPos - 1);
    return mEntryRegList.size() - 1;
  }

  unsigned int ECDeviceModel::syncBitLength(SyncDir paDir) const {
    unsigned int bitLength = 0;
    for (const PdoEntry &entry : mPdoEntryList) {
      for (const Pdo &pdo : mPdoList) {
        if (pdo.mIndex == entry.mPdoIndex) {
          if (pdo.mSyncDir == paDir) {
            bitLength += entry.mBitLength;
          }
          break;
        }
      }
    }
    return bitLength;
  }

  bool ECDeviceModel::hasEntryReg(size_t paRegIndex, bool paRequireDomainOffset) {
    util::CCriticalRegion criticalRegion(mMutex);
    return paRegIndex < mEntryRegList.size() && (!paRequireDomainOffset || mEntryRegList[paRegIndex].mOffsetValid);
  }

  bool ECDeviceModel::bindHandleOffset(size_t paRegIndex, unsigned int *paHandleOffset) {
    util::CCriticalRegion criticalRegion(mMutex);
    if (paRegIndex >= mEntryRegList.size() || paHandleOffset == nullptr) {
      return false;
    }
    EntryReg &reg = mEntryRegList[paRegIndex];
    if (reg.mHandleOffset != nullptr && reg.mHandleOffset != paHandleOffset) {
      return false;
    }
    reg.mHandleOffset = paHandleOffset;
    if (reg.mOffsetValid) {
      *paHandleOffset = reg.mDomainOffset;
    }
    return true;
  }

  void ECDeviceModel::unbindHandleOffset(const unsigned int *paHandleOffset) {
    util::CCriticalRegion criticalRegion(mMutex);
    for (EntryReg &reg : mEntryRegList) {
      if (reg.mHandleOffset == paHandleOffset) {
        reg.mHandleOffset = nullptr;
      }
    }
  }

  void ECDeviceModel::commitDomainOffsets() {
    util::CCriticalRegion criticalRegion(mMutex);
    for (EntryReg &reg : mEntryRegList) {
      reg.mOffsetValid = true;
      if (reg.mHandleOffset != nullptr) {
        *reg.mHandleOffset = reg.mDomainOffset;
      }
    }
  }

  void ECDeviceModel::cleanupSync() {
    if(cachedSyncs != nullptr) {
      if(cachedSyncs[0].pdos != nullptr) {
        for(unsigned int i = 0; i < cachedRxPdoSize; i++) {
          if(cachedSyncs[0].pdos[i].entries != nullptr) {
            delete[] cachedSyncs[0].pdos[i].entries;
          }
        }

        delete[] cachedSyncs[0].pdos;
      }

      if(cachedSyncs[1].pdos != nullptr) {
        for(unsigned int i = 0; i < cachedTxPdoSize; i++) {
          if(cachedSyncs[1].pdos[i].entries != nullptr) {
            delete[] cachedSyncs[1].pdos[i].entries;
          }
        }

        delete[] cachedSyncs[1].pdos;
      }

      delete[] cachedSyncs;
      cachedSyncs = nullptr;
      cachedRxPdoSize = 0;
      cachedTxPdoSize = 0;
    }
  }

  void ECDeviceModel::getPdoSize(unsigned int &paRxPdoSize, unsigned int &paTxPdoSize) {
    for(int i = 0; i < mPdoList.size(); i++) {
      if(mPdoList[i].mSyncDir == SyncDir::Out) {
        paRxPdoSize++;
      } else {
        paTxPdoSize++;
      }
    }
  }

  void ECDeviceModel::getPdoEntrySize(uint16_t paPdoIndex, unsigned int &paPdoEntrySize) {
    for(int i = 0; i < mPdoEntryList.size(); i++) {
      if(mPdoEntryList[i].mPdoIndex == paPdoIndex) {
        paPdoEntrySize++;
      }
    }
  }

  ec_sync_info_t* ECDeviceModel::getSyncs(){
    // Clean up previously cached syncs if any
    cleanupSync();
    
    ec_sync_info_t* syncInfos = new ec_sync_info_t[3];
    syncInfos[0] = {OUTPUT_SYNC_INDEX, EC_DIR_OUTPUT, 0, nullptr, EC_WD_DEFAULT};
    syncInfos[1] = {INPUT_SYNC_INDEX, EC_DIR_INPUT, 0, nullptr, EC_WD_DEFAULT};
    syncInfos[2] = {0xff};

    unsigned int rxPdoSize = 0;
    unsigned int txPdoSize = 0;
    getPdoSize(rxPdoSize, txPdoSize);
    
    // Cache the sizes for later cleanup
    cachedRxPdoSize = rxPdoSize;
    cachedTxPdoSize = txPdoSize;
   
    // Build RxPDO (OUTPUT direction - from controller to device)
    if(rxPdoSize > 0){
        ec_pdo_info_t* rxPdoes = new ec_pdo_info_t[rxPdoSize];
        int pdoIdx = 0;  // Use separate index for PDO array
        for(int i = 0; i < mPdoList.size(); i++){
            if(mPdoList[i].mSyncDir == SyncDir::Out){
                uint16_t pdoIndex = mPdoList[i].mIndex;
                unsigned int pdoEntrySize = 0;
                getPdoEntrySize(pdoIndex, pdoEntrySize);
                
                ec_pdo_entry_info_t* entries = new ec_pdo_entry_info_t[pdoEntrySize];
                int entryIdx = 0;
                for(int j = 0; j < mPdoEntryList.size(); j++){
                    if(mPdoEntryList[j].mPdoIndex == pdoIndex){
                        entries[entryIdx] = {mPdoEntryList[j].mIndex, mPdoEntryList[j].mSubIndex, mPdoEntryList[j].mBitLength};
                        entryIdx++;
                    }                 
                }
                rxPdoes[pdoIdx] = {mPdoList[i].mIndex, pdoEntrySize, entries};
                pdoIdx++;
            }     
        }
        syncInfos[0].n_pdos = rxPdoSize; 
        syncInfos[0].pdos = rxPdoes;
    }

    // Build TxPDO (INPUT direction - from device to controller)
    if(txPdoSize > 0){
        ec_pdo_info_t* txPdoes = new ec_pdo_info_t[txPdoSize];
        int pdoIdx = 0;  // Use separate index for PDO array
        for(int i = 0; i < mPdoList.size(); i++){
            if(mPdoList[i].mSyncDir == SyncDir::In){
                uint16_t pdoIndex = mPdoList[i].mIndex;
                unsigned int pdoEntrySize = 0;
                getPdoEntrySize(pdoIndex, pdoEntrySize);
                
                ec_pdo_entry_info_t* entries = new ec_pdo_entry_info_t[pdoEntrySize];
                int entryIdx = 0;
                for(int j = 0; j < mPdoEntryList.size(); j++){
                    if(mPdoEntryList[j].mPdoIndex == pdoIndex){
                        entries[entryIdx] = {mPdoEntryList[j].mIndex, mPdoEntryList[j].mSubIndex, mPdoEntryList[j].mBitLength};
                        entryIdx++;
                    }                 
                }
                txPdoes[pdoIdx] = {mPdoList[i].mIndex, pdoEntrySize, entries};
                pdoIdx++;
            }
        }
        syncInfos[1].n_pdos = txPdoSize;
        syncInfos[1].pdos = txPdoes;
    }

    // Cache the pointer for later cleanup
    cachedSyncs = syncInfos;
    return syncInfos;
  }
  
}