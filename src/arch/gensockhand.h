/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _GENSOCKHAND_H_
#define _GENSOCKHAND_H_


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
    typedef typename THandler::TFileDescriptor TSocketDescriptor;

    static const TSocketDescriptor scm_nInvalidSocketDescriptor = THandler::scm_nInvalidFileDescriptor;
  private:
    //as this class is just a place holder for the others we don't want to have an instance of it
    CGenericIPComSocketHandler();
    CGenericIPComSocketHandler(const CGenericIPComSocketHandler&);
    CGenericIPComSocketHandler& operator =(const CGenericIPComSocketHandler &);
    ~CGenericIPComSocketHandler();
};


#endif /* _GENSOCKHAND_H_ */
