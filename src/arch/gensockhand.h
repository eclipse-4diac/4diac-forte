/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _GENSOCKHAND_H_
#define _GENSOCKHAND_H_

#ifndef FORTE_SOCKET_TYPE
#define FORTE_SOCKET_TYPE int
#endif

#ifndef FORTE_INVALID_SOCKET
#define FORTE_INVALID_SOCKET -1
#endif

/*!\brief Template class allowing to easily setup IP communication socket handlers.
 *
 * There are two template parameters:
 *  - THandler  an external event handler which allows Comlayers to register with a descriptor and get notifications on received messages
 *  - TSocektCreator a class providing methods for creating and deleting tcp and udp sockets
 */
template <typename THandler, typename TSocketCreator>
class CGenericIPComSocketHandler : public THandler, public TSocketCreator{
  public:
    //! Type needed by the class CIPCommLayer
    typedef FORTE_SOCKET_TYPE TSocketDescriptor;

    static const TSocketDescriptor scmInvalidSocketDescriptor = FORTE_INVALID_SOCKET;
  private:
    //as this class is just a place holder for the others we don't want to have an instance of it
    CGenericIPComSocketHandler();
    CGenericIPComSocketHandler(const CGenericIPComSocketHandler&);
    CGenericIPComSocketHandler& operator =(const CGenericIPComSocketHandler &);
    ~CGenericIPComSocketHandler();
};


#endif /* _GENSOCKHAND_H_ */
