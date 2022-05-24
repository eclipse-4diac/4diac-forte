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
    void clear() { m_vData.clear(); }
    /// Get storage size in bytes.
    auto size() { return m_vData.size(); }
    /// Reserve storage memory
    void reserve(std::size_t pa_unSize) { m_vData.reserve(pa_unSize); }
    /// Get pointer to memory
    void* data() { return reinterpret_cast<void*>(m_vData.data()); }
    /// Get pointer to memory
    const void* data() const { return reinterpret_cast<const void*>(m_vData.data()); }

    /**
     * @brief Write a block of data between pointers.
     * @tparam T element type
     * @param pa_pBegin pointer to first block
     * @param pa_pEnd pointer after last block
     * @return pointer to the written block
     */
    template<typename T>
    T* write(const T *pa_pBegin, const T *pa_pEnd) {
      std::size_t bytesSize = (pa_pEnd - pa_pBegin) * sizeof(T);
      return reinterpret_cast<T*>(write(pa_pBegin, bytesSize));
    }
    /**
     * @brief Write a block of data bound by size.
     * @tparam T element type
     * @param pa_pBegin pointer to first block
     * @param pa_unSize number of elements to write
     * @return pointer to the written block
     */
    template<typename T>
    T* write(const T *pa_pBegin, std::size_t pa_unSize) {
      return reinterpret_cast<T*>(write(reinterpret_cast<const void*>(pa_pBegin), pa_unSize * sizeof(T)));
    }
    /**
     * @brief Write one data element.
     * @tparam T element type
     * @param pa_xValue element to write
     * @return pointer to the written element
     */
    template<typename T>
    T* write(T pa_xValue) {
      return reinterpret_cast<T*>(write(reinterpret_cast<void*>(&pa_xValue), sizeof(pa_xValue)));
    }

    /**
     * @brief Write a void block of data bound by size.
     * @param pa_pBegin pointer to block
     * @param pa_unSize size of block in bytes
     * @return pointer to the written block
     */
    void* write(const void *pa_pBegin, std::size_t pa_unSize);

    /**
     * @brief Gets storage ending pointer.
     * @tparam T type to automatically cast the returned pointer
     * @return ending pointer (past the last byte)
     */
    template<typename T=void>
    T* end() {
      return reinterpret_cast<T*>(&*m_vData.end());
    }
    /**
     * @brief Gets storage ending const pointer.
     * @tparam T type to automatically cast the returned pointer
     * @return ending pointer (past the last byte)
     */
    template<typename T=void>
    const T* end() const {
      return reinterpret_cast<const T*>(&*m_vData.end());
    }

  private:
    std::vector<char> m_vData;
};

}

#endif /*_MIXED_STORAGE_H_ */
