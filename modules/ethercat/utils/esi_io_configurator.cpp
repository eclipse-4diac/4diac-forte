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

#include "esi_io_configurator.h"

#include "../device/ec_device.h"
#include "../device/ec_module.h"
#include "../handler/bus.h"
#include "../types/ECDevice.h"
#include "../types/ECModule.h"
#include "esi_xml_utils.h"
#include "forte/util/criticalregion.h"
#include "forte/util/devlog.h"

#include <string>
#include <tinyxml.h>
#include <vector>

namespace forte::eclipse4diac::io::ethercat {

  namespace {

    const char *const scmBusAlreadyStarted = "EtherCAT bus already started, process data cannot be changed";
    const char *const scmParentNotConfigured = "Parent EtherCAT device is not configured";

    struct EsiPdoEntry {
      uint16_t mIndex;
      uint8_t mSubIndex;
      uint8_t mBitLength;
    };

    struct EsiPdo {
      uint16_t mIndex;
      SyncDir mDir;
      std::vector<EsiPdoEntry> mEntries;
    };

    void applyVendorModulePdoEntryIndexCorrection(const std::string &paVendorName, uint16_t &paEntryIndex) {
      if (paVendorName == "SiChuan Odot Automation System Co.,Ltd.") {
        paEntryIndex = static_cast<uint16_t>(paEntryIndex + 1U);
      } else if (paVendorName == "Inovance") {
        paEntryIndex = static_cast<uint16_t>(paEntryIndex - 1U);
      }
    }

    const char *childText(const TiXmlElement *paElement, const char *paName) {
      const TiXmlElement *child = paElement->FirstChildElement(paName);
      return (child != nullptr) ? child->GetText() : nullptr;
    }

    /** IgH treats index 0 as a gap in the PDO mapping and as the end of a registration list. */
    bool isPadding(const EsiPdoEntry &paEntry) {
      return paEntry.mIndex == 0;
    }

    /** Selects PDOs like the IDE's ESI import, so that the n-th non-padding entry of a direction belongs to IN_n/OUT_n. */
    void readPdos(const TiXmlElement *paElement,
                  const char *paPdoType,
                  SyncDir paDir,
                  bool paModule,
                  std::vector<EsiPdo> &paPdos) {
      const uint32_t moduleSm = (paDir == SyncDir::Out) ? 2U : 3U;
      for (const TiXmlElement *pdoElement = paElement->FirstChildElement(paPdoType); pdoElement != nullptr;
           pdoElement = pdoElement->NextSiblingElement(paPdoType)) {
        const char *smText = pdoElement->Attribute("Sm");
        if (smText == nullptr) {
          continue;
        }
        const uint32_t sm = parseEcNumber(smText);
        if (paModule ? (sm != moduleSm) : (sm != 2U && sm != 3U)) {
          continue;
        }

        EsiPdo pdo{static_cast<uint16_t>(parseEcNumber(childText(pdoElement, "Index"))), paDir, {}};
        for (const TiXmlElement *entry = pdoElement->FirstChildElement("Entry"); entry != nullptr;
             entry = entry->NextSiblingElement("Entry")) {
          pdo.mEntries.push_back({static_cast<uint16_t>(parseEcNumber(childText(entry, "Index"))),
                                  static_cast<uint8_t>(parseEcNumber(childText(entry, "SubIndex"))),
                                  static_cast<uint8_t>(parseEcNumber(childText(entry, "BitLen")))});
        }
        paPdos.push_back(std::move(pdo));
      }
    }

    /** Inputs (TxPdo) come first, matching the order of the FB's IN_x and OUT_x pins. */
    std::vector<EsiPdo> readDefaultPdos(const TiXmlElement *paElement, bool paModule) {
      std::vector<EsiPdo> pdos;
      readPdos(paElement, "TxPdo", SyncDir::In, paModule, pdos);
      readPdos(paElement, "RxPdo", SyncDir::Out, paModule, pdos);
      return pdos;
    }

    bool checkMappingCount(const std::vector<EsiPdo> &paPdos, size_t paNumIn, size_t paNumOut, std::string &paErrMsg) {
      size_t numIn = 0;
      size_t numOut = 0;
      for (const EsiPdo &pdo : paPdos) {
        for (const EsiPdoEntry &entry : pdo.mEntries) {
          if (!isPadding(entry)) {
            ++((pdo.mDir == SyncDir::In) ? numIn : numOut);
          }
        }
      }
      if (numIn != paNumIn || numOut != paNumOut) {
        paErrMsg = "ESI defines " + std::to_string(numIn) + " input and " + std::to_string(numOut) +
                   " output PDO entries, but the FB has " + std::to_string(paNumIn) + " and " +
                   std::to_string(paNumOut);
        return false;
      }
      return true;
    }

    bool isMappableBitLength(uint8_t paBitLength) {
      return paBitLength == 8 || paBitLength == 16 || paBitLength == 32 || paBitLength == 64;
    }

    /** The caller must hold the model mutex. */
    void addProcessData(ECDeviceModel &paModel, ECBusDeviceHandler &paHandler, const std::vector<EsiPdo> &paPdos) {
      // getSyncs() puts all outputs into one SM and all inputs into another, so this is the bit offset inside the SM.
      unsigned int syncBitOffset[2] = {paModel.syncBitLength(SyncDir::In), paModel.syncBitLength(SyncDir::Out)};
      uint16_t imageSize[2] = {0, 0};
      std::vector<ECBusDeviceHandler::ProcessDataEntry> entries;

      for (const EsiPdo &pdo : paPdos) {
        const size_t dir = (pdo.mDir == SyncDir::In) ? 0 : 1;
        paModel.addPdo(pdo.mIndex, pdo.mDir);
        for (const EsiPdoEntry &entry : pdo.mEntries) {
          paModel.addPdoEntry(pdo.mIndex, entry.mIndex, entry.mSubIndex, entry.mBitLength);
          if (!isPadding(entry)) {
            const size_t regIndex = paModel.addEntryReg();
            const bool mappable = (syncBitOffset[dir] % 8 == 0) && isMappableBitLength(entry.mBitLength);
            entries.push_back({(pdo.mDir == SyncDir::In) ? forte::io::IOMapper::In : forte::io::IOMapper::Out,
                               entry.mIndex,
                               entry.mSubIndex,
                               entry.mBitLength,
                               imageSize[dir],
                               mappable,
                               regIndex});
            if (mappable) {
              imageSize[dir] = static_cast<uint16_t>(imageSize[dir] + entry.mBitLength / 8);
            }
          }
          syncBitOffset[dir] += entry.mBitLength;
        }
      }

      paHandler.initBuffer(imageSize[1], imageSize[0]);
      paHandler.mProcessDataEntries = std::move(entries);
      paHandler.mProcessDataConfigured = true;
    }

    template<typename FB>
    void mapProcessDataEntries(ECBusDeviceHandler &paHandler, FB &paFB) {
      if (!paHandler.mProcessDataConfigured) {
        return;
      }
      const std::vector<ECBusDeviceHandler::ProcessDataEntry> &entries = paHandler.mProcessDataEntries;
      if (entries.size() != paFB.numInMappings() + paFB.numOutMappings()) {
        DEVLOG_ERROR("ethercat[EsiIoConfigurator]: Device %zu has %zu PDO entries, but the FB has %zu pins.\n",
                     paHandler.index(), entries.size(), paFB.numInMappings() + paFB.numOutMappings());
        return;
      }

      for (size_t i = 0; i < entries.size(); ++i) {
        const auto *idVar = static_cast<const CIEC_WSTRING *>(paFB.mappingDi(static_cast<TPortId>(i)));
        const char *idText = idVar->getValue();
        if (nullptr == idText || idText[0] == '\0') {
          continue;
        }

        const ECBusDeviceHandler::ProcessDataEntry &entry = entries[i];
        if (!entry.mMappable) {
          DEVLOG_ERROR("ethercat[EsiIoConfigurator]: PDO entry 0x%04X:%02X (%u bit) of device %zu is not byte aligned "
                       "or has no matching IO type, '%s' is not mapped.\n",
                       static_cast<unsigned int>(entry.mIndex), static_cast<unsigned int>(entry.mSubIndex),
                       static_cast<unsigned int>(entry.mBitLength), paHandler.index(), idText);
          continue;
        }

        std::string handleId{idText};
        ECBusHandler::HandleDescriptor desc(handleId,
                                            entry.mDirection,
                                            paHandler.index(),
                                            entry.mImageOffset,
                                            static_cast<uint8_t>(entry.mBitLength / 8),
                                            entry.mIndex,
                                            entry.mSubIndex,
                                            entry.mEntryRegIndex);
        paFB.registerMappedHandle(desc);
      }
    }

  } // namespace

  EsiIoConfigurator::EsiIoConfigurator(EsiCatalog &paCatalog) : mCatalog(paCatalog) {}

  bool EsiIoConfigurator::configureDevice(uint32_t paProductCode,
                                          ECDeviceHandler *paDeviceHandler,
                                          FORTE_ECDevice &paDevice,
                                          std::string &paErrMsg) {
    TiXmlElement *deviceElement = mCatalog.resolveDevice(paProductCode, paErrMsg);
    if (nullptr == deviceElement) {
      return false;
    }

    const std::vector<EsiPdo> pdos = readDefaultPdos(deviceElement, false);
    if (!checkMappingCount(pdos, paDevice.numInMappings(), paDevice.numOutMappings(), paErrMsg)) {
      DEVLOG_ERROR("ethercat[EsiIoConfigurator]: Device %zu (ProductCode 0x%08X): %s.\n",
                   paDeviceHandler->index(), paProductCode, paErrMsg.c_str());
      return false;
    }

    if (paDeviceHandler->mProcessDataConfigured) {
      return true;
    }

    ECDeviceModel &model = paDeviceHandler->mECDeviceModel;
    util::CCriticalRegion criticalRegion(model.mMutex);
    if (model.mFrozen) {
      paErrMsg = scmBusAlreadyStarted;
      return false;
    }

    if (paDeviceHandler->mDeviceType == ECBusDeviceHandler::DeviceType::ECCoupler) {
      if (const TiXmlElement *slots = deviceElement->FirstChildElement("Slots")) {
        model.mSlotIndexInc = parseEcNumber(slots->Attribute("SlotIndexIncrement"));
        model.mSlotPdoInc = parseEcNumber(slots->Attribute("SlotPdoIncrement"));
      }
    }

    uint8_t smIndex = 0;
    for (const TiXmlElement *sm = deviceElement->FirstChildElement("Sm"); sm != nullptr;
         sm = sm->NextSiblingElement("Sm"), ++smIndex) {
      const char *smText = sm->GetText();
      if (nullptr == smText) {
        continue;
      }
      const std::string smType{smText};
      if (smType == "Outputs" || smType == "Inputs") {
        model.addSync(smIndex, smType == "Outputs" ? SyncDir::Out : SyncDir::In);
      }
    }

    addProcessData(model, *paDeviceHandler, pdos);
    return true;
  }

  bool EsiIoConfigurator::configureModule(uint32_t paModuleIdent,
                                          ECDeviceHandler *paDeviceHandler,
                                          ECModuleHandler *paModuleHandler,
                                          FORTE_ECModule &paModule,
                                          std::string &paErrMsg) {
    TiXmlElement *moduleElement = mCatalog.resolveModule(paModuleIdent, paErrMsg);
    if (nullptr == moduleElement) {
      return false;
    }

    std::vector<EsiPdo> pdos = readDefaultPdos(moduleElement, true);
    if (!checkMappingCount(pdos, paModule.numInMappings(), paModule.numOutMappings(), paErrMsg)) {
      DEVLOG_ERROR("ethercat[EsiIoConfigurator]: Module %zu (ModuleIdent 0x%08X): %s.\n",
                   paModuleHandler->index(), paModuleIdent, paErrMsg.c_str());
      return false;
    }

    if (paModuleHandler->mProcessDataConfigured) {
      return true;
    }
    if (!paDeviceHandler->mProcessDataConfigured) {
      paErrMsg = scmParentNotConfigured;
      return false;
    }

    ECDeviceModel &model = paDeviceHandler->mECDeviceModel;
    util::CCriticalRegion criticalRegion(model.mMutex);
    if (model.mFrozen) {
      paErrMsg = scmBusAlreadyStarted;
      return false;
    }

    const std::string &vendorName = mCatalog.vendorForModule(paModuleIdent);
    const uint32_t slot = paModuleHandler->slot();
    for (EsiPdo &pdo : pdos) {
      pdo.mIndex = static_cast<uint16_t>(pdo.mIndex + model.mSlotPdoInc * slot);
      for (EsiPdoEntry &entry : pdo.mEntries) {
        if (isPadding(entry)) {
          continue;
        }
        applyVendorModulePdoEntryIndexCorrection(vendorName, entry.mIndex);
        entry.mIndex = static_cast<uint16_t>(entry.mIndex + model.mSlotIndexInc * slot);
      }
    }

    addProcessData(model, *paModuleHandler, pdos);
    return true;
  }

  void EsiIoConfigurator::mapDeviceIOHandles(ECDeviceHandler *paDeviceHandler, FORTE_ECDevice &paDevice) {
    mapProcessDataEntries(*paDeviceHandler, paDevice);
  }

  void EsiIoConfigurator::mapModuleIOHandles(ECModuleHandler *paModuleHandler, FORTE_ECModule &paModule) {
    mapProcessDataEntries(*paModuleHandler, paModule);
  }

} // namespace forte::eclipse4diac::io::ethercat
