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
#ifndef FIXEDCAPVECTOR_H_
#define FIXEDCAPVECTOR_H_

#include "fortearray.h"

namespace forte {
  namespace core {
    namespace util {

      /*!\brief A vector having a fixed size array as data storage.
       *
       * Allows to push_back items, pop items, access the items, iterate over, and clear them.
       */
      template<typename T, size_t Capacity, typename TDataStorage = CArray<T, Capacity> >
      class CFixedCapazityVector{
        public:
          CFixedCapazityVector() :
              mNumElements(0){
          }

          void clear(){
            mNumElements = 0;
          }

          bool pushBack(const T & paVal){
            bool retVal = false;
            if(mNumElements < Capacity){
              mDataStorage[mNumElements] = paVal;
              mNumElements++;
              retVal = true;
            }
            return retVal;
          }

          void popBack(){
            if(0 < mNumElements){
              mNumElements--;
            }
          }

          T& front(){
            return mDataStorage[0];
          }

          T& back(){
            return mDataStorage[mNumElements - 1];
          }

          T& operator [](size_t paIndex){
            assert(paIndex < Capacity);
            return mDataStorage[paIndex];
          }

          bool isEmpty() const{
            return (0 == mNumElements);
          }

          size_t size() const {
            return mNumElements;
          }

          class CIterator{
            public:
              CIterator(CFixedCapazityVector &paVector, size_t paPos) : mVector(paVector), mPos(paPos){
              }

              CIterator& operator++(){
                if(mPos < mVector.size()){
                  mPos++;
                }
                return *this;
              }

              T& operator*(){
                return mVector[mPos];
              }

              bool operator==(CIterator const& rhs) const{
                return mPos == rhs.mPos;
              }

              bool operator!=(CIterator const& rhs) const{
                return !(*this == rhs);
              }

              bool isLastEntry(){
                return (mPos == (mVector.size() -1));
              }

            private:

              CFixedCapazityVector &mVector;
              size_t mPos;

          };

          const CIterator begin(){
            return CIterator(*this, 0);
          }

          const CIterator end(){
            return CIterator(*this, mNumElements);
          }

        private:
          TDataStorage mDataStorage;
          size_t mNumElements; //!< number of all elements currently in this container
      };

    } // namespace util
  } // namespace core
} // namespace forte

#endif // FIXEDCAPVECTOR_H_
