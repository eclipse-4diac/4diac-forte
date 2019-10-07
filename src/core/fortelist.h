/*******************************************************************************
 * Copyright (c) 2009 - 2012, 2018 TU Vienna/ACIN, 2017 fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians, Alois Zoitl, Ingo Hegny,
 *      - initial implementation and rework communication infrastructure
 *    Stefan Profanter
 *      - list->begin changed to be const, and smaller bugfixes to support
 *        const as Type
 *     Martin Melik Merkumians - fixes naming, adds missing back functions for
 *       list specializations
 *******************************************************************************/
#include <fortenew.h>
#include "fortenode.h"
#include "forteiterator.h"

#ifndef _FORTELIST_H_
#define _FORTELIST_H_

/*! \ingroup CORE\brief FORTE implementation of a Singly Linked List
 */

template <typename T, typename Container = CSinglyLinkedListNode<T> >
class CSinglyLinkedList {
private:

  friend class CIterator<T,Container>;

  /*!\brief First element of the singly linked list
   */
  Container* mFirstNode;

  /*!\brief Last element of the singly linked list
   */
  Container* mLastNode;

public:
  CSinglyLinkedList();
  ~CSinglyLinkedList();

  /*!\brief Typedef for getting the right iterator for the list
   * */
  typedef CIterator<T,Container> Iterator;

  /*!\brief Add's the given object at the start of the singly linked list
   *
   * \param paElement the reference to the object to be added
   */
  void pushFront(T const& paElement);

  /*!\brief Add's the given object at the end of the singly linked list
   *
   * \param paElement the reference to the object to be added
   */
  void pushBack(T const& paElement);

  /*!\brief Deletes the first object of the singly linked list
   *
   */
  void popFront();

  /*!\brief Deletes all objects in the singly linked list
   */
  void clearAll();

  /*!\brief checks if the singly linked list is empty or not
   *
   * \return true if the list is empty, false if not
   */
  bool isEmpty() const {
    return (0 == mFirstNode);
  }

  /*!\brief returns an iterator with it's current position set to the start of the singly linked list
   *
   * \return iterator on position FirstNode
   */
  const Iterator begin() const {
    return Iterator(mFirstNode);
  }

  /*!\brief returns an iterator with it's current position set to 0
   *
   * \return iterator with current position 0
   */
  const Iterator end() const {
    return Iterator(0);
  }

  const Iterator back() const {
    return Iterator(mLastNode);
  }

  /*!\brief erases the next node element after the position of the iterator
   *
   * \param it  iterator set on the element before the element to delete
   * \return Iterator set on the new next node element
   */
  const Iterator eraseAfter(Iterator& it);

  /*! \brief erases the element according to the passed pointer
   *
   * @param paToDelete pointer to the element to be deleted
   */
  void erase(T const& paToDelete);

};

template <typename T, typename Container>
inline CSinglyLinkedList<T,Container>::CSinglyLinkedList() : mFirstNode(0), mLastNode(0) {
}

template <typename T, typename Container>
inline CSinglyLinkedList<T,Container>::~CSinglyLinkedList() {
  clearAll();
}

template <typename T, typename Container>
void CSinglyLinkedList<T,Container>::pushFront(T const& paElement) {
  Container* poNewNode = new Container(paElement, mFirstNode);
  mFirstNode = poNewNode;
  if(0 == mLastNode){
    mLastNode = poNewNode;
  }
}

template <typename T, typename Container>
void CSinglyLinkedList<T,Container>::pushBack(T const& paElement)  {
  Container* poNewNode = new Container(paElement);

  if(0 != mLastNode){
    mLastNode->setNext(poNewNode);
  }
  else{
    mFirstNode = poNewNode;
  }
  mLastNode = poNewNode;
}

template <typename T, typename Container>
void CSinglyLinkedList<T,Container>::popFront() {
  Container* pNodeToDelete = mFirstNode;
  mFirstNode = mFirstNode->getNext();
  if(0 == mFirstNode) {
    mLastNode = 0;
  }
  delete pNodeToDelete;
}

template <typename T, typename Container>
inline void CSinglyLinkedList<T,Container>::clearAll() {
  while(mFirstNode != 0)  {
    popFront();
  }
}

template <typename T, typename Container>
const CIterator<T,Container> CSinglyLinkedList<T,Container>::eraseAfter(Iterator& it) {
  Container* pNodeToDelete = (it.getPosition())->getNext();
  it.getPosition()->setNext(pNodeToDelete->getNext());
  if(0 == it.getPosition()->getNext()) {
    mLastNode = it.getPosition();
  }
  delete pNodeToDelete;
  return Iterator((it.getPosition())->getNext());
}

template <typename T, typename Container>
void CSinglyLinkedList<T,Container>::erase(T const& paToDelete){

    Iterator itRunner = begin();
    Iterator itRefNode = end();
    Iterator itEnd = end();

    while(itRunner != itEnd){
      if(*itRunner == paToDelete){
        if(itRefNode == itEnd){
          popFront();
        }
        else{
          eraseAfter(itRefNode);
        }
        break;
      }

      itRefNode = itRunner;
      ++itRunner;
    }
  }

/*!\brief explicit specialization for a void* list
 *
 */

template<>
class CSinglyLinkedList<void*, CSinglyLinkedListNode<void*> >  {
private:

  friend class CIterator<void*,CSinglyLinkedListNode<void*> >;

  /*!\brief First element of the singly linked list
   */
  CSinglyLinkedListNode<void*>* mFirstNode;

  /*!\brief Last element of the singly linked list
   */
  CSinglyLinkedListNode<void*>* mLastNode;

public:
  CSinglyLinkedList() : mFirstNode(0), mLastNode(0) {

  }
  ~CSinglyLinkedList()  {
    clearAll();
  }

  /*!\brief Typedef for getting the right iterator for the list
   * */
  typedef CIterator<void*,CSinglyLinkedListNode<void*> > Iterator;

  /*!\brief Add's the given object at the start of the singly linked list
   *
   * \param paElement the reference to the object to be added
   */
  void pushFront(const void* paElement)  {
    CSinglyLinkedListNode<void*>* poNewNode = new CSinglyLinkedListNode<void*>((void *)paElement, mFirstNode);
    mFirstNode = poNewNode;
    if(0 == mLastNode){
      mLastNode = poNewNode;
    }
  }

  /*!\brief Add's the given object at the end of the singly linked list
   *
   * \param pa_poElement the reference to the object to be added
   */
  void pushBack(void* const paElement) {
    CSinglyLinkedListNode<void*>* poNewNode = new CSinglyLinkedListNode<void*>(paElement);

    if(0 != mLastNode){
      mLastNode->setNext(poNewNode);
    }
    else{
      mFirstNode = poNewNode;
    }
    mLastNode = poNewNode;
  }

  /*!\brief Returns the element at first position without deleting it
   *
   */
  inline const void* peekFront() const {
      return (mFirstNode != 0) ? mFirstNode->getData() : 0;
  }

  /*!\brief Deletes the first object of the singly linked list
   *
   */
  inline void popFront()  {
    CSinglyLinkedListNode<void*>* pNodeToDelete = mFirstNode;
    mFirstNode = mFirstNode->getNext();
    if(0 == mFirstNode) {
      mLastNode = 0;
    }
    delete pNodeToDelete;
  }

  /*!\brief Deletes all objects in the singly linked list
   */
  inline void clearAll() {
    while(0 != mFirstNode)  {
      popFront();
    }
  }

  /*!\brief checks if the singly linked list is empty or not
   *
   * \return true if the list is empty, false if not
   */
  inline bool isEmpty() const {
    return (0 == mFirstNode);
  }

  /*!\brief returns an iterator with it's current position set to the start of the singly linked list
   *
   * \return iterator on position FirstNode
   */
  inline const Iterator begin() const {
    return Iterator(mFirstNode);
  }

  /*!\brief returns an iterator with it's current position set to 0
   *
   * \return iterator with current position 0
   */
  inline const Iterator end() const {
    return Iterator(0);
  }

  inline const Iterator back() const{
    return Iterator(mLastNode);
  }

  /*!\brief erases the next node element after the position of the iterator
   *
   * \param it  an iterator set on the element before the element to delete
   * \return Iterator set on the new next node element
   */
  const Iterator eraseAfter(Iterator& it) {
    CSinglyLinkedListNode<void*>* pNodeToDelete = (it.getPosition())->getNext();
    it.getPosition()->setNext(pNodeToDelete->getNext());
    if(0 == it.getPosition()->getNext()) {
      mLastNode = it.getPosition();
    }
    delete pNodeToDelete;
    return Iterator((it.getPosition())->getNext());
  }

  /*! \brief erases the element according to the passed pointer
   *
   * @param paToDelete pointer to the element to be deleted
   */
  void erase(void* paToDelete){

    Iterator itRunner = begin();
    Iterator itRefNode = end();
    Iterator itEnd = end();

    while(itRunner != itEnd){
      if(*itRunner == paToDelete){
        if(itRefNode == itEnd){
          popFront();
        }
        else{
          eraseAfter(itRefNode);
        }
        break;
      }

      itRefNode = itRunner;
      ++itRunner;
    }
  }

};

/*!\brief partial specialization for pointer lists
 */

template <typename T>
class CSinglyLinkedList<T*> {
private:
  friend class CIterator<T*,CSinglyLinkedListNode<void*> >;
  CSinglyLinkedList<void*> mList;

public:
  CSinglyLinkedList() {

  }

  ~CSinglyLinkedList()  {
    clearAll();
  }

  /*!\brief Typedef for getting the right iterator for the list
   * */
  typedef CIterator<T*,CSinglyLinkedListNode<void*> > Iterator;

  /*!\brief Add's the given object at the start of the singly linked list
   *
   * \param paElement the reference to the object to be added
   */
  inline void pushFront(T* const paElement)  {
    mList.pushFront(paElement);
  }

  /*!\brief Add's the given object at the end of the singly linked list
   *
   * \param paElement the reference to the object to be added
   */
  inline void pushBack(T* const paElement) {
    mList.pushBack(paElement);
  }

  /*!\brief Returns the element at first position without deleting it
   *
   */
  inline const T* peekFront() const {
    return (const T*) mList.peekFront();
  }

  /*!\brief Deletes the first object of the singly linked list
   *
   */
  inline void popFront()  {
    mList.popFront();
  }

  /*!\brief Deletes all objects in the singly linked list
   */
  inline void clearAll() {
    mList.clearAll();
  }

  /*!\brief checks if the singly linked list is empty or not
   *
   * \return true if the list is empty, false if not
   */
  inline bool isEmpty() const {
    return mList.isEmpty();
  }

  /*!\brief returns an iterator with it's current position set to the start of the singly linked list
   *
   * \return iterator on position FirstNode
   */
  inline const Iterator begin() const  {
    return Iterator((mList.begin()).getPosition());
  }

  /*!\brief returns an iterator with it's current position set to 0
   *
   * \return iterator with current position 0
   */
  inline const Iterator end() const {
    return Iterator((mList.end()).getPosition());
  }

  inline const Iterator back() const {
    return Iterator(mList.back().getPosition());
  }

  /*!\brief erases the next node element after the position of the iterator
   *
   * \param it  an iterator set on the element before the element to delete
   * \return Iterator set on the new next node element
   */
  const Iterator eraseAfter(Iterator& it) {
    CSinglyLinkedList<void*>::Iterator itBuff(it.getPosition());
    return Iterator((mList.eraseAfter(itBuff)).getPosition());
  }

  void erase(T* paToDelete){
    mList.erase(paToDelete);
  }

};


#endif /* CSINGLYLINKEDLIST_H_ */
