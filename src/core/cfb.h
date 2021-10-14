/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl, Gunnar Grabmaier, Gerhard Ebenhofer,
 *    Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _CFB_H_
#define _CFB_H_

#include "funcbloc.h"

class CInterface2InternalDataConnection;

struct SCFB_FBConnectionData {
    CStringDictionary::TStringId m_nSrcId;
    int m_nSrcFBNum;
    CStringDictionary::TStringId m_nDstId;
    int m_nDstFBNum;
};

/*! \ingroup CORE
 * \brief Connection data for a fanned out connection of a composite's FB network
 *
 * As fanned out connections are attached together they need different data than
 * the first connection.
 */
struct SCFB_FBFannedOutConnectionData {
    int m_nConnectionNum; //!<the connection this fanned out connection should be attached to.
    CStringDictionary::TStringId m_nDstId;
    int m_nDstFBNum;
};

struct SCFB_FBParameter {
    unsigned int m_nFBNum;
    CStringDictionary::TStringId m_nDINameID; //!< the data input id of the input param to set
    const char * m_acParamValue;
};

struct SCFB_FBNData {
    unsigned int m_nNumFBs;
    const SCFB_FBInstanceData * m_pstFBInstances;
    //TODO add FB parameters
    unsigned int m_nNumEventConnections;
    const SCFB_FBConnectionData * m_pstEventConnections;
    unsigned int m_nNumFannedOutEventConnections;
    const SCFB_FBFannedOutConnectionData * m_pstFannedOutEventConnections;
    unsigned int m_nNumDataConnections;
    const SCFB_FBConnectionData * m_pstDataConnections;
    unsigned int m_nNumFannedOutDataConnections;
    const SCFB_FBFannedOutConnectionData * m_pstFannedOutDataConnections;
    unsigned int m_nNumParams;
    const SCFB_FBParameter * m_pstParams;
};

//These two defines are here for legacy reasons. They allow that old CFBs can still be complied
/*! Generate a connection port id for a given fb-nameid / port-nameid combination
 *  This template will generate a constant and will therefore be very efficient
 */
#define GENERATE_CONNECTION_PORT_ID_2_ARG(FBNameId, PortNameId) (PortNameId)

/*! Generate a connection port id for a connection with a single source id e.g. interface connections
 *  This template will generate a constant and will therefore be very efficient
 */
#define GENERATE_CONNECTION_PORT_ID_1_ARG(PortNameId)\
GENERATE_CONNECTION_PORT_ID_2_ARG(CStringDictionary::scm_nInvalidStringId, PortNameId)

/*!\ingroup CORE
 * \brief Class for handling firmware composite function blocks.
 */
class CCompositeFB: public CFunctionBlock {

  public:
    /*! \brief Indicator that the given FB id is an adapter.
     *
     *This will allow for 65535 FBs within an CFB, which should be sufficient.
     */
    static const TForteInt32 scm_nAdapterMarker = 0x10000;

    /*! \brief constant for retrieving an FB Num from an FBId marked with an Adapter Marker
     *
     * needs to correspond to scm_nAdapterMarker
     */
    static const TForteInt32 scm_nAdapterFBRange = 0xFFFF;

    /*!\brief The main constructor for a composite function block.
     *
     * The interface of the constructor has been extended from the basic FB's interface with the following values:
     * \param pa_poSrcRes         pointer to including resource
     * \param pa_pstInterfaceSpec pointer to interface specification
     * \param pa_nInstanceNameId  StringId of instance name
     * \param pa_cpoFBNData       const pointer to description of internal structure of FB (FBs, Connections, ...)
     * \param pa_acFBData         ByteArray for FB-specific data (DI, DO, int. Vars, ...)
     */
    CCompositeFB(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec,
        const CStringDictionary::TStringId pa_nInstanceNameId,
        const SCFB_FBNData * const pa_cpoFBNData, TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData);
    virtual ~CCompositeFB();

    /*!\brief Connects specific data input of a CFB with a specific data connection.
     *
     * \param pa_unDINameId    StringId of input name.
     * \param pa_poDataCon     Pointer to the data connection the CFB should be connected to.
     * \return TRUE on success, FALSE if data output not exists or output is already connected.
     */
    virtual bool connectDI(TPortId paDIPortId, CDataConnection *paDataCon);

    virtual bool configureGenericDO(TPortId paDOPortId, const CIEC_ANY &paRefValue);
    //! implement improved version of get Var for CFBs, allowing to read and write to internal elements
    virtual CIEC_ANY* getVar(CStringDictionary::TStringId *paNameList,
        unsigned int paNameListSize);

    virtual EMGMResponse changeFBExecutionState(EMGMCommandType pa_unCommand);

#ifdef FORTE_SUPPORT_MONITORING
    virtual CFunctionBlock *getFB(forte::core::TNameIdentifier::CIterator &paNameListIt);
#endif

  private:
    virtual void executeEvent(int pa_nEIID);

    void createInternalFBs();
    void createEventConnections();
    void prepareIf2InEventCons();
    void establishConnection(CConnection *paCon, CFunctionBlock *paDstFb, CStringDictionary::TStringId paDstNameId);
    void createDataConnections();
    void prepareIf2InDataCons();
    void setParams();
    void sendInternal2InterfaceOutputEvent(int pa_nEOID);

    //!Acquire the functionblock for a given function block number this may be a contained fb, an adapter, or the composite itself.
    CFunctionBlock *getFunctionBlock(int pa_nFBNum);

    CInterface2InternalDataConnection *mIf2InDConns;
    CDataConnection **m_apoIn2IfDConns;

    const SCFB_FBNData * const cm_cpoFBNData;

    CEventConnection **m_apoEventConnections;
    CDataConnection **m_apoDataConnections;

    //!Array storing the holding the connections to be used in the execute event for triggering the internal FBs
    CEventConnection **mInterface2InternalEventCons;

#ifdef FORTE_FMU
    friend class fmuInstance;
#endif //FORTE_FMU
};

#define COMPOSITE_FUNCTION_BLOCK_CTOR(fbclass) \
 fbclass(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : \
 CCompositeFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, &scm_stFBNData, m_anFBConnData, m_anFBVarsData)

#endif /*_CFB_H_*/
