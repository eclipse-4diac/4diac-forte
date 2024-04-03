/*******************************************************************************
 * Copyright (c) 2012 -2014 AIT, fortiss GmbH
 *               2024 Samator Indo Gas
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *   Ketut Kumajaya - Code refactoring from char* to std::string
 *******************************************************************************/
#ifndef OPCCONNECTIONHANDLER_H_
#define OPCCONNECTIONHANDLER_H_

#include "singlet.h"
#include "fortelist.h"
#include <comlayer.h>
#include <fortelist.h>

class COpcConnection;

class COpcConnectionHandler{
  DECLARE_SINGLETON(COpcConnectionHandler)
    ;
  public:
    /*! \brief returns existing or creates new OpcConnection
     *
     *  If a connection to the specified server already exist a pointer to this is returned. Otherwise a
     *  new connection is created. The returned connection includes the group with the specified settings.
     */
    COpcConnection* getOpcConnection(const std::string& paHost, const std::string& paServerName, forte::com_infra::CComLayer* paComCallback);

    void removeOpcConnection(const std::string& paHost, const std::string& paServerName, const std::string& paGroupName);

  private:
    COpcConnection* findOpcConnection(const std::string& paHost, const std::string& paServerName);
    void deleteOpcConnection(const std::string& paHost, const std::string& paServerName);

    typedef CSinglyLinkedList<COpcConnection*> TOpcConnectionList;
    TOpcConnectionList mOpcConnectionList;

};

#endif // OPCCONNECTIONHANDLER_H_
