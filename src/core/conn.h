/*******************************************************************************
  * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny,
  *    Michael Hofmann
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _CONN_H_
#define _CONN_H_

#include "fortelist.h"
#include "mgmcmd.h"
#include "stringdict.h"

//forward declaration of a few classes to reduce includefile dependencies
class CFunctionBlock;

struct SConnectionPoint{
    CFunctionBlock *mFB;
    TPortId mPortId;

    SConnectionPoint(CFunctionBlock *paFB, TPortId paPortId) :
        mFB(paFB), mPortId(paPortId){
    }

    SConnectionPoint():mFB(0), mPortId(0) {
    }

    bool operator==(const SConnectionPoint & paRight) const{
      return ((mFB == paRight.mFB) && (mPortId == paRight.mPortId));
    }
};

/*!\ingroup CORE \brief Base class for handling a connection.
 */

class CConnection{
  public:
    CConnection(CFunctionBlock *paSrcFB, TPortId paSrcPortId);

    virtual ~CConnection(){
    }
    ;
    /*! \brief Establishes a connection.
     *
     * With this command the connection is created and the FBs are set to appropriate states.
     * As connections are stored at the function block outputs this mainly affects the destination of
     * the connection.
     *
     * A child needs to implement this function!
     *
     * \param paDstFB     the destination FB of this connection
     * \param paDestPortNameId the input port name to which this connection should be connected to
     * \return can be the following response:
     *     - e_RDY... creation went well.
     *     - e_NO_SUCH_OBJECT... The destination is not a valid input.
     *     - e_INVALID_STATE... The specified connection already exists.
     */
    virtual EMGMResponse connect(CFunctionBlock *paDstFB,
        CStringDictionary::TStringId paDstPortNameId) = 0;

    /*!\brief establish an event connection of a CFB to an event output of the CFB.
     *
     */
    virtual EMGMResponse connectToCFBInterface(CFunctionBlock *paDstFB,
        CStringDictionary::TStringId paDstPortNameId) = 0;

#ifndef FORTE_CLASS_0
    /*! \brief Disconnects the connection.
     *
     * With this command the connection is removed and the FBs are set to appropriate states.
     * This mainly effects the destination of the connection as the source point is the storage
     * for the connection object
     *
     * A child needs to implement this function!
     *
     * \param paDstFB     the destination FB of this connection
     * \param paDestPortNameId the input port name to from which this connection should be removed
     * \return can be the following response:
     *     - e_RDY... creation went well.
     *     - e_NO_SUCH_OBJECT... The destination is not a valid input.
     *     - e_INVALID_STATE... this connection is not connected to the destination
     */
    virtual EMGMResponse disconnect(CFunctionBlock *paDstFB,
        CStringDictionary::TStringId paDstPortNameId) = 0;
    #endif

    /*! \brief Check if there are destinations added to this connection
     *
     * \return TRUE if there is no destination in the connection.
     */
    bool isEmpty(void) const{
      return mDestinationIds.isEmpty();
    }

    bool isConnected() const{
      return !isEmpty();
    }

    /*! \brief Get the source string of the connection
     */
    const SConnectionPoint& getSourceId(void) const{
      return mSourceId;
    }

  protected:
    EMGMResponse addDestination(const SConnectionPoint &paDestPoint);
    #ifndef FORTE_CLASS_0
    EMGMResponse removeDestination(const SConnectionPoint &paDestPoint);
    #endif

    void setSource(CFunctionBlock *paSrcFB, TPortId paSrcPortId);

    //!Non const version
    SConnectionPoint& getSourceId(void){
      return mSourceId;
    }

    typedef CSinglyLinkedList<SConnectionPoint> TDestinationIdList;

    /*!\brief a list of destinations the connection is connected to.
     *
     * By storing a list of destinations an implicit support for fan-out is given.
     * The destination is represented as string id the same way as the sourceId
     */
    TDestinationIdList mDestinationIds;

    /*!\brief An identifier for the source of this connection
     *
     * The source is identified by a FB pointer and the port ID
     */
    SConnectionPoint mSourceId;

  private:

    //! Check if there is already a connection within this connection with the same dst.
    bool dstExists(const SConnectionPoint &paDestPoint);
};

#endif /*_CONN_H_*/
