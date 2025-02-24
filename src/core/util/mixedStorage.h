/*********************************************************************
 * Copyright (c) 2022 Davor Cihlar
 *
 * This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License 2.0
 * which is available at https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Davor Cihlar
 *     - initial implementation
 **********************************************************************/

#ifndef _MIXED_STORAGE_H_
#define _MIXED_STORAGE_H_

#include <vector>


namespace forte::core::util {

/**
 * @brief This class can be used to store any kind of binary blob data
 *
 * Memory needs to be reserved when stable pointers to this storage are required.
 *
 * @note Data is not aligned when added (padding is never added)!
 */
class CMixedStorage {
  public:
    /// Clear storage.
    void clear() { mData.clear(); }
    /// Get storage size in bytes.
    auto size() { return mData.size(); }
    /// Reserve storage memory
    void reserve(std::size_t paSize) { mData.reserve(paSize); }
    /// Get pointer to memory
    void* data() { return reinterpret_cast<void*>(mData.data()); }
    /// Get pointer to memory
    const void* data() const { return reinterpret_cast<const void*>(mData.data()); }

    /**
     * @brief Write a block of data between pointers.
     * @tparam T element type
     * @param paBegin pointer to first block
     * @param paEnd pointer after last block
     * @return pointer to the written block
     */
    template<typename T>
    T* write(const T *paBegin, const T *paEnd) {
      std::size_t bytesSize = (paEnd - paBegin) * sizeof(T);
      return reinterpret_cast<T*>(write(paBegin, bytesSize));
    }
    /**
     * @brief Write a block of data bound by size.
     * @tparam T element type
     * @param paBegin pointer to first block
     * @param paSize number of elements to write
     * @return pointer to the written block
     */
    template<typename T>
    T* write(const T *paBegin, std::size_t paSize) {
      return reinterpret_cast<T*>(write(reinterpret_cast<const void*>(paBegin), paSize * sizeof(T)));
    }
    /**
     * @brief Write one data element.
     * @tparam T element type
     * @param paValue element to write
     * @return pointer to the written element
     */
    template<typename T>
    T* write(T paValue) {
      return reinterpret_cast<T*>(write(reinterpret_cast<void*>(&paValue), sizeof(paValue)));
    }

    /**
     * @brief Write a void block of data bound by size.
     * @param paBegin pointer to block
     * @param paSize size of block in bytes
     * @return pointer to the written block
     */
    void* write(const void *paBegin, std::size_t paSize);

    /**
     * @brief Gets storage ending pointer.
     * @tparam T type to automatically cast the returned pointer
     * @return ending pointer (past the last byte)
     */
    template<typename T=void>
    T* end() {
      return reinterpret_cast<T*>(mData.data() + mData.size());
    }
    /**
     * @brief Gets storage ending const pointer.
     * @tparam T type to automatically cast the returned pointer
     * @return ending pointer (past the last byte)
     */
    template<typename T=void>
    const T* end() const {
      return reinterpret_cast<const T*>(mData.data() + mData.size());
    }

  private:
    std::vector<char> mData;
};

}

#endif /*_MIXED_STORAGE_H_ */
