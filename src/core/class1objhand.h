/*******************************************************************************
  * Copyright (c) 2006 - 2015 ACIN, Profactor GmbH, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *   Alois Zoitl, Thomas Strasser, Gerhard Ebenhofer, Ingo Hegny,
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _CLASS1OBJHAND_H_
#define _CLASS1OBJHAND_H_

#include "stringdict.h"
#include "funcbloc.h"
#include "conn.h"
#include "fortelist.h"
#include "mgmcmdstruct.h"

/**\ingroup CORE
 *Class handling all management commands and handling created objects like, FBs, Resources, Connections,...
 *
 */
class C61499Class1ObjectHandler{
  public:
    C61499Class1ObjectHandler(CResource& pa_roHandlerOwner);

    virtual ~C61499Class1ObjectHandler();

    /*!\brief Execute the given management command
     *
     * Evaluates the m_sDestination parameter of the command if empty the resource tries
     * to execute the management command if not it tries to apply the mgm command to a
     * contained FB if possible.
     * \param pa_oCommand internal representation of the management command
     * \return response of the MGMCommand execution as defined in IEC 61499
     */
    virtual EMGMResponse executeMGMCommand(SManagementCMD &pa_oCommand);

    /*!\brief Create a new connection between source and destination
     *
     * @param pa_nSrcId   ID of source of the connection fb.output
     * @param pa_nDestId  ID of destination of the connection fb.input
     * @return response of the command execution as defined in IEC 61499
     */
    EMGMResponse deleteConnection(SManagementCMD &paCommand);

#ifdef FORTE_SUPPORT_QUERY_CMD
    EMGMResponse queryTypeList(SManagementCMD &paCommand, CTypeLib::CTypeEntry * pa_LibTypeListStart);
    EMGMResponse queryTypeVersion(TForteUInt32 pa_nSource, char* pa_sValue);

    /*!\brief Read a parameter value from a given FB
     *
     * @param pa_oCommand Structure including management-command and response
     * @return response of the command execution as defined in IEC 61499
     */
    EMGMResponse executeQueryReq(SManagementCMD &paCommand);

    /*! \brief query connections
     *
     * @param pa_oCommand Structure including management-command and response
     * @return response of the command execution as defined in IEC 61499
     */
    EMGMResponse executeQueryCon(SManagementCMD &paCommand);
#endif


    /*!\brief check if there exists an connection with given source identifier in this resource
     *
     * \param pa_nSrcId  source identifier of connection (FB.Output)
     * \return pointer to connection if connection does not exist 0
     */
    CConnection* getConnection(TConnectionID pa_nSrcId);

  protected:
    /** \brief Delete all FB instances
     *
     * @return e_RDY on success
     */
    EMGMResponse deleteWholeFBNetwork();

    /*! \brief Owner resource/device of the objecthandler
     */
    CResource& m_roHandlerOwner;

    /*! \brief Type for storing the list of FB objects.
     */

    typedef CSinglyLinkedList<CFunctionBlock*> TObjInstanceList;
    /*! \brief List of the current instantiated FBs in this 61499 Object Handler (may also contain Resources).
     *
     */
    TObjInstanceList m_lstpoObjectInstanceList;

    /*! \brief Type for storing the list of connection.
     */
    typedef CSinglyLinkedList<CConnection*> TConnectionList;

    /*! \brief List of the current connections in this 61499 Object Handler.
     */
    TConnectionList m_lstpoConnectionList;

  private:
    C61499Class1ObjectHandler();
};

#endif
