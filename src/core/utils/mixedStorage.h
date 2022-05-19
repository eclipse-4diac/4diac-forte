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
 */
class CMixedStorage {
  public:
    void clear() { m_vData.clear(); }
    auto size() { return m_vData.size(); }
    void reserve(std::size_t pa_unSize) { m_vData.reserve(pa_unSize); }
    void* data() { return reinterpret_cast<void*>(m_vData.data()); }
    const void* data() const { return reinterpret_cast<const void*>(m_vData.data()); }

    template<typename T>
    T* write(const T *pa_pBegin, const T *pa_pEnd) {
      std::size_t bytesSize = (pa_pEnd - pa_pBegin) * sizeof(T);
      return reinterpret_cast<T*>(write(pa_pBegin, bytesSize));
    }
    template<typename T>
    T* write(const T *pa_pBegin, std::size_t pa_unSize) {
      return reinterpret_cast<T*>(write(reinterpret_cast<const void*>(pa_pBegin), pa_unSize * sizeof(T)));
    }
    template<typename T>
    T* write(T pa_nValue) {
      return reinterpret_cast<T*>(write(reinterpret_cast<void*>(&pa_nValue), sizeof(pa_nValue)));
    }

    void* write(const void *pa_pBegin, std::size_t pa_unSize);
    
    template<typename T=void>
    T* end() {
      return reinterpret_cast<T*>(&*m_vData.end());
    }
    template<typename T=void>
    const T* end() const {
      return reinterpret_cast<const T*>(&*m_vData.end());
    }

  private:
    std::vector<char> m_vData;
};

}

#endif /*_MIXED_STORAGE_H_ */
