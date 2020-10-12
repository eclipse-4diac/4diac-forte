/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef FORTEARRAY_H_
#define FORTEARRAY_H_

#include <datatype.h>
#include <assert.h>

namespace forte {
  namespace core {
    namespace util {


      /*!\brief Simple array class allowing to implement more save arrays.
       *
       * This class is aligned to the std::array.
       * Consider to use std:array when stl support gets better on all supported platforms.
       */
      template<typename T, size_t Capacity>
      class CArray{
        public:
          T& operator [](size_t paIndex){
            assert(paIndex < Capacity);
            return mData[paIndex];
          }

          size_t getSize() const {
            return Capacity;
          }

        private:
          T mData[Capacity];
      };

    } // namespace util
  } // namespace core
}  // namespace forte

#endif /* FORTEARRAY_H_ */
