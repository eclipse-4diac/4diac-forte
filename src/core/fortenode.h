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
 *    Martin Melik Merkumians, Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - updates naming
 *******************************************************************************/
#ifndef FORTENODE_H_
#define FORTENODE_H_

/*! \ingroup CORE\brief FORTE implementation of a Singly Linked List Node
 */


  /*!\brief container element of the singly linked list
   *
   * This class is the container element of the singly linked list.
   * It's nested in the singly linked list class for reuse in the template class
   */

template<typename T>
class CSinglyLinkedListNode {
private:
  /*!\brief contained object of the node element
   */
  T mNodeElement;
  /*!\brief pointer to the next node element
   */
  CSinglyLinkedListNode* mNextNode;
  CSinglyLinkedListNode();
public:
  CSinglyLinkedListNode(T paNodeElement, CSinglyLinkedListNode* paNextNode = 0) : mNodeElement(paNodeElement), mNextNode(paNextNode) {

  }

  ~CSinglyLinkedListNode()  {

  }

  T& getData() {
    return mNodeElement;
  }

  CSinglyLinkedListNode* getNext() const  {
    return mNextNode;
  }

  void setNext(CSinglyLinkedListNode* paNextNode)  {
    mNextNode = paNextNode;
  }

};

#endif /* FORTENODE_H_ */
