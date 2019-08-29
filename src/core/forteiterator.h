/*******************************************************************************
 * Copyright (c) 2010, 2018 TU Vienna/ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians, Alois Zoitl,
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - updates naming
 *******************************************************************************/
#ifndef FORTEITERATOR_H_
#define FORTEITERATOR_H_

/*! \ingroup CORE\brief FORTE implementation of a Singly Linked List Iterator
 */


  /*!\brief Iterator class of the Singly linked list
   *
   * This is the iterator class of this singly linked list, with minimum functionality needed for read and write access.
   * It's nested in the singly linked list template class for reuse
   *
   * T ... Data Type
   * C ... Node Class i.e. CSinglyLinkedListNode
   */

template<typename T, typename Container>
class CIterator  {
public:
  explicit CIterator(Container* paNode) : mCurrentPosition(paNode)  {

  }

  ~CIterator() {
  }

  /*!\brief returns the pointer of the current node element
   */

  Container* getPosition() const {
    return mCurrentPosition;
  }

  /*!\brief moves iterator to the next element
   */
  CIterator& operator++()  {
    mCurrentPosition = mCurrentPosition->getNext();
    return *this;
  }

  /*!\brief returns reference to contained object
   *
   * \return reference of the contained object of type T
   */
  T& operator*()  {
    return mCurrentPosition->getData();
  }

  /*!\brief const version of dereference operator for const iterators
   */
  const T& operator*() const {
    return mCurrentPosition->getData();
  }

  /*!\brief Returns a pointer to the contained object
   * \return A Pointer to the contained object of type T
   */
  T* operator->() {
    return &(**this);
  }

  /*!\brief equality operator for the iterator
   *
   * \return true if position of iterators match, false if otherwise
   */
  bool operator==(CIterator const& rhs) const {
    return mCurrentPosition == rhs.mCurrentPosition;
  }

  /*!\brief inequality operator of the iterator
   *
   * \return false if positions of the iterators match, true if otherwise
   */
  bool operator!=(CIterator const& rhs) const {
    return !(*this == rhs);
  }

private:
  CIterator();
  Container* mCurrentPosition;
};

template<typename T, typename Container>
// cppcheck-suppress noConstructor
class CIterator<T*, Container>  {
public:
  CIterator(Container* paNode) : mCurrentPosition(paNode)  {

  }

  ~CIterator() {

  }

  /*!\brief returns the pointer of the current node element
   */

  Container* getPosition() const  {
    return mCurrentPosition;
  }

  /*!\brief moves iterator to the next element
   */
  CIterator& operator++()  {
    mCurrentPosition = mCurrentPosition->getNext();
    return *this;
  }

  /*!\brief returns reference to contained object
   *
   * \return reference of the contained object of type T
   */
  T* operator*()  {
    return static_cast<T*>(mCurrentPosition->getData());
  }


  /*!\brief const version of dereference operator for const iterators
   */
  const T* operator*() const {
    return static_cast<T*>(mCurrentPosition->getData());
  }


  /*!\brief Returns a pointer to the contained object
   * \return A Pointer to the contained object of type T
   */
  T* operator->() {
    return static_cast<T*>(mCurrentPosition->getData());
  }

  /*!\brief equality operator for the iterator
   *
   * \return true if position of iterators match, false if otherwise
   */
  bool operator==(CIterator const& rhs) const  {
    return mCurrentPosition == rhs.mCurrentPosition;
  }

  /*!\brief inequality operator of the iterator
   *
   * \return false if positions of the iterators match, true if otherwise
   */
  bool operator!=(CIterator const& rhs) const {
    return !(*this == rhs);
  }

private:
  CIterator();
  Container* mCurrentPosition;
};

#endif /* FORTEITERATOR_H_ */
