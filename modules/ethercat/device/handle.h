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

#include "forte/io/mapper/io_handle.h"
#include "forte/arch/forte_sync.h"
#include "endian.h"
#include <span>
#include <string>

namespace forte::eclipse4diac::io::ethercat {

  class ECBusDeviceHandler;

  class ECDeviceHandle : public forte::io::IOHandle {
    public:
			friend class ECBusDeviceHandler;
			
      ECDeviceHandle(forte::io::IODeviceController *paController,
                    forte::io::IOMapper::Direction paDirection,
                    CIEC_ANY::EDataTypeID type,
                    uint8_t paOffset,
                    const std::string &paHandleId,
                    ECBusDeviceHandler &paDevice);
      ~ECDeviceHandle() override;

      const std::string &handleId() const {
        return mHandleId;
      }

      /** Domain byte offset for EtherCAT registration (EsiFileParser / ECDeviceModel). */
      unsigned int *ecDomainOffsetPtr() {
        return &mECDomainDataOffset;
      }

      void set(const CIEC_ANY &) override;
      void get(CIEC_ANY &) override;
      bool equal(std::span<const unsigned char> paOldBuffer);

    protected:
			static constexpr bool IS_LITTLE_ENDIAN = __BYTE_ORDER == __LITTLE_ENDIAN;	

			template<typename T>
			static T hostLeEndianSwap(T value) {
				if constexpr (!IS_LITTLE_ENDIAN) {
					if constexpr (sizeof(T) == 2) {
						return static_cast<T>(__builtin_bswap16(static_cast<uint16_t>(value)));
					} else if constexpr (sizeof(T) == 4) {
						return static_cast<T>(__builtin_bswap32(static_cast<uint32_t>(value)));
					} else if constexpr (sizeof(T) == 8) {
						return static_cast<T>(__builtin_bswap64(static_cast<uint64_t>(value)));
					}
				}

				return value;
			}

			template<typename T>
			static T hostToLittleEndian(T value) {
				return hostLeEndianSwap(value);
			}

			template<typename T>
			static T littleEndianToHost(T value) {
				return hostLeEndianSwap(value);
			}

			const CIEC_BYTE getByteValue(std::span<const unsigned char> paBuffer);
			const CIEC_WORD getWordValue(std::span<const unsigned char> paBuffer);
			const CIEC_DWORD getDWordValue(std::span<const unsigned char> paBuffer);
			const CIEC_LWORD getLWordValue(std::span<const unsigned char> paBuffer);
        
      virtual void reset() {}
      void onObserver(forte::io::IOObserver *paObserver) override;
      void dropObserver() override;
			void syncDomainData(uint8_t *paECDomainData);

      std::span<unsigned char> mBuffer;
      const uint8_t mOffset;

      unsigned int mECDomainDataOffset;
      ECBusDeviceHandler &mDevice;
      arch::CSyncObject &mUpdateMutex;

			size_t mByteLength;
      std::string mHandleId;
  };
}