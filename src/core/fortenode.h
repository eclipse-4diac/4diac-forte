/*******************************************************************************
  * Copyright (c) 2010 ACIN
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Martin Melik Merkumians, Alois Zoitl
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef FORTENODE_H_
#define FORTENODE_H_

/*! \ingroup CORE\brief FORTE implementation of a Singly Linked List Node
 */


  /*!\brief container element of the singly linked list
   *
   * This struct is the container element of the singly linked list.
   * It's nested in the singly linked list class for reuse in the template class
   */

template<typename T>
class CSinglyLinkedListNode {
private:
  /*!\brief contained object of the node element
   */
  T m_oNodeElement;
  /*!\brief pointer to the next node element
   */
  CSinglyLinkedListNode* m_poNextNode;
  CSinglyLinkedListNode();
public:
  CSinglyLinkedListNode(T pa_oNodeElement, CSinglyLinkedListNode* pa_poNextNode = 0) : m_oNodeElement(pa_oNodeElement), m_poNextNode(pa_poNextNode) {

  }

  ~CSinglyLinkedListNode()  {

  }

  T& getData() {
    return m_oNodeElement;
  }

  CSinglyLinkedListNode* getNext() const  {
    return m_poNextNode;
  }

  void setNext(CSinglyLinkedListNode* pa_poNextNode)  {
    m_poNextNode = pa_poNextNode;
  }

};

#endif /* FORTENODE_H_ */
