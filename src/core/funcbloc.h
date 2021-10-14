/*******************************************************************************
 * Copyright (c) 2005 - 2018 ACIN, Profactor GmbH, nxtControl GmbH, fortiss GmbH,
 *                           Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Thomas Strasser, Gunnar Grabmaier, Smodic Rene,
 *    Gerhard Ebenhofer, Michael Hofmann, Martin Melik Merkumians, Ingo Hegny,
 *    Stanislav Meduna, Patrick Smejkal,
 *      - initial implementation and rework communication infrastructure
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *******************************************************************************/
#ifndef _FUNCBLOC_H_
#define _FUNCBLOC_H_

#include <forte_config.h>
#include "mgmcmd.h"
#include "event.h"
#include "dataconn.h"
#include "eventconn.h"
#include "stringdict.h"
#include "../arch/devlog.h"
#include "iec61131_functions.h"
#include <stringlist.h>

class CEventChainExecutionThread;
class CAdapter;
class CTimerHandler;

typedef CFunctionBlock *TFunctionBlockPtr;

#ifdef FORTE_SUPPORT_MONITORING
#include "mgmcmdstruct.h"
namespace forte {
  namespace core {
    class CMonitoringHandler;
  }
}
#endif //FORTE_SUPPORT_MONITORING

#ifndef FORTE_FB_DATA_ARRAY  //with this check we can overwrite this define in a platform specific file (e.g., config.h)
/*! Define that adds the data array to a SIFB, simple FB or CFB
 * May be overwritten by a platform specific version that adapts for example some alignment requirements
 */
#define FORTE_FB_DATA_ARRAY(a_nNumEOs, a_nNumDIs, a_nNumDOs, a_nNumAdapters) \
  union{ \
    TForteByte m_anFBConnData[genFBConnDataSizeTemplate<a_nNumEOs, a_nNumDIs, a_nNumDOs>::value]; \
    void *m_apoDummyConnPointerAlign; \
  };\
  union{ \
    TForteByte m_anFBVarsData[genFBVarsDataSizeTemplate<a_nNumDIs, a_nNumDOs, a_nNumAdapters>::value]; \
    CIEC_ANY::TLargestUIntValueType m_uDummyDataAlign; \
  };
#endif

typedef CAdapter *TAdapterPtr;

typedef TForteUInt8 TDataIOID; //!< \ingroup CORE Type for holding an data In- or output ID (max value 254)

/*!\ingroup CORE\brief Structure to hold all data of adapters instantiated in the function block.
 */
struct SAdapterInstanceDef {
    CStringDictionary::TStringId m_nAdapterTypeNameID; //!< Adapter type name
    CStringDictionary::TStringId m_nAdapterNameID; //!< Adapter instance name
    bool m_bIsPlug; //!< Flag for distinction of adapter nature (plug/socket)
};

/*!
 * \brief Instance and type name of to be created FBs. Used in CFBs FBNs and BFB/SFBs internal FBs
 */
struct SCFB_FBInstanceData {
    CStringDictionary::TStringId m_nFBInstanceNameId;
    CStringDictionary::TStringId m_nFBTypeNameId;
};

/*!\ingroup CORE\brief Structure to hold all the data for specifying a function block interface.
 */
struct SFBInterfaceSpec {
    TForteUInt8 m_nNumEIs; //!< Number of event inputs (max 254)
    const CStringDictionary::TStringId *m_aunEINames; //!< List of the event input names
    const TDataIOID *m_anEIWith; //!< Input WITH reference list. This list contains an array of input data ids. For each input event the associated data inputs are listed. The start for each input event is specified in the m_anEIWithIndexes field. The end is defined trough the value 255.
    const TForteInt16 *m_anEIWithIndexes; //!< Index list for each input event. This list gives for each input event an entry in the m_anEIWith. Input events are numbered starting from 0. if the input event has no assciated data inputs -1 is the entry at this event inputs postion.
    TForteUInt8 m_nNumEOs; //!< Number of event outputs (max 254)
    const CStringDictionary::TStringId *m_aunEONames; //!< List of the event output names
    const TDataIOID *m_anEOWith; //!< Output WITH reference list. This list contains an array of output data ids. For each output event the associated data outputs are listed. The start for each output event is specified in the m_anEOWithIndexes field. The end is defined trough the value 255.
    const TForteInt16 *m_anEOWithIndexes; //!< Index list for each output event. This list gives for each output event an entry in the m_anEOWith. Output events are numbered starting from 0. if the output event has no assciated data outputs -1 is the entry at this event outputs postion. Additionally at the postion m_nNumEOs in this list an index to an own list in the m_anEOWith list is stored specifying all output data port that are not associated with any output event. That values will be updated on every FB invocation.
    TForteUInt8 m_nNumDIs; //!< Number of data inputs (max 254)
    const CStringDictionary::TStringId *m_aunDINames; //!< List of the data input names
    const CStringDictionary::TStringId *m_aunDIDataTypeNames; //!< List of the data type names for the data inputs
    TForteUInt8 m_nNumDOs; //!< Number of data outputs (max 254)
    const CStringDictionary::TStringId *m_aunDONames; //!< List of the data output names
    const CStringDictionary::TStringId *m_aunDODataTypeNames; //!< List of the data type names for the data outputs
    TForteUInt8 m_nNumAdapters; //!< Number of Adapters
    const SAdapterInstanceDef *m_pstAdapterInstanceDefinition; //!< List of adapter instances
};

/*!\ingroup CORE\brief Base class for all function blocks.
 */
class CFunctionBlock {
  public:
    const static TDataIOID scmWithListDelimiter = cg_unInvalidPortId; //!< value identifying the end of a with list

    /*!\brief Possible states of a runable object.
     *
     */
    enum E_FBStates {
      e_RUNNING = 0, // in the most critical execution path of FORTE we are checking for this enum value it is faster if this is the zero entry
      e_IDLE,
      e_STOPPED,
      e_KILLED
    };

    /*! \brief Indicator that the given EventID is an included adapter's eventID.
     *
     * This will allow 255 Input and Output Events.
     *
     * EventIDs > scm_nMaxInterfaceEvents: highByte indicates (AdapterID+1)
     */
    static const TForteUInt16 scm_nMaxInterfaceEvents = 0x00FF;

    virtual ~CFunctionBlock();

    /*!\brief Get the resource the function block is contained in.
     */
    CResource& getResource(void) {
      return *m_poResource;
    }

    CResource* getResourcePtr(void) {
      return m_poResource;
    }

    /*!\brief Get the timer of the device wher the FB is contained.
     */
    CTimerHandler& getTimer(void);

    /*!\brief Returns the type of this FB instance
     */
    virtual CStringDictionary::TStringId getFBTypeId(void) const = 0;
    /*!\brief Get the ID of a specific event input of the FB.
     *
     * \param pa_unEINameId   StringId to the event input name.
     * \return The ID of the event input or cg_nInvalidEventID.
     */
    TEventID getEIID(CStringDictionary::TStringId pa_unEINameId) const {
      return static_cast<TEventID>(getPortId(pa_unEINameId, m_pstInterfaceSpec->m_nNumEIs, m_pstInterfaceSpec->m_aunEINames));
    }

    /*!\brief Get the ID of a specific event output of the FB.
     *
     * \param pa_unEONameId string id to the event output name.
     * \return The ID of the event output or cg_nInvalidEventID.
     */
    TEventID getEOID(CStringDictionary::TStringId pa_unEONameId) const {
      return static_cast<TEventID>(getPortId(pa_unEONameId, m_pstInterfaceSpec->m_nNumEOs, m_pstInterfaceSpec->m_aunEONames));
    }

    CEventConnection* getEOConnection(CStringDictionary::TStringId paEONameId) const;

    /*!\brief Connects specific data input of a FB with a specific data connection.
     *
     * If the connection pointer is 0 then it is equal to a disconnect. Furthermore, if their is already a connection
     * to the given data port the connection attempt will be rejected. However if the further connection attempt
     * is done with the same connection object then it is a reconfiguration attempt for any data inputs. This is necessary
     * when during the first connection the data type of the connection was not clear.
     *
     * \param pa_unDINameId StringID of the data input name.
     * \param pa_poDataCon Pointer to the data connection the FB should be connected to.
     * \return TRUE on success, FALSE if data output not exists or output is already connected.
     */
    virtual bool connectDI(TPortId paDIPortId, CDataConnection *pa_poDataCon);

    /*! \brief Gets the index of the m_acDINames array of a specific data output of a FB
     *
     * \param pa_unDINameId  StringId of the data input name.
     * \return Returns index of the Data Input Array of a FB
     */
    TPortId getDIID(CStringDictionary::TStringId pa_unDINameId) const {
      return getPortId(pa_unDINameId, m_pstInterfaceSpec->m_nNumDIs, m_pstInterfaceSpec->m_aunDINames);
    }

    /*!\brief Get the pointer to a data input of the FB.
     *
     * \param pa_unDINameId ID of the data input name.
     * \return Pointer to the data input or 0. If 0 is returned DataInput is ANY
     */
    CIEC_ANY* getDataInput(CStringDictionary::TStringId pa_unDINameId) const;

    /*!\brief get the pointer to a data input using the portId as identifier
     */
    CIEC_ANY* getDIFromPortId(TPortId paDIPortId) const;

    /*! \brief Gets the index of the m_acDONames array of a specific data output of a FB
     * \param pa_unDONameId  StringId of the data input name.
     * \return Returns index of the Data Output Array of a FB
     */
    TPortId getDOID(CStringDictionary::TStringId pa_unDONameId) const {
      return getPortId(pa_unDONameId, m_pstInterfaceSpec->m_nNumDOs, m_pstInterfaceSpec->m_aunDONames);
    }

    /*!\brief get the pointer to a data output using the portId as identifier
     */
    CIEC_ANY* getDOFromPortId(TPortId paDOPortId) const;

    CDataConnection* getDOConnection(CStringDictionary::TStringId paDONameId) const;

    CDataConnection* getDIConnection(CStringDictionary::TStringId paDINameId) const;

    /*!\brief if the data output is of generic type (i.e, ANY) this function allows an data connection to configure
     * the DO with the specific type coming from the other end of the connection
     */
    virtual bool configureGenericDO(TPortId paDOPortId, const CIEC_ANY &paRefValue);

    /*!\brief Get the pointer to a data output of the FB.
     *
     * \param pa_unDONameId StringID of the data output name.
     * \return Pointer to the data output or 0. If 0 is returned DataOutput is ANY
     */
    CIEC_ANY* getDataOutput(CStringDictionary::TStringId pa_unDONameId) const;

    /*!\brief Get the pointer to a variable of the FB.
     *
     * @param paNameList array of the name hierarchy the requested variable
     * @param paNameListSize length of the array
     * \return Pointer to the variable or 0.
     *  The variable may be input, output or in the case of a BFB an internal var.
     */
    virtual CIEC_ANY* getVar(CStringDictionary::TStringId *paNameList, unsigned int paNameListSize);

    /*!\brief Get the pointer to the adapter instance of the FB.
     *
     * \param pa_uAdapterNameId  StringId of the adapter name.
     * \return Pointer to the data output or 0.
     */
    CAdapter* getAdapter(CStringDictionary::TStringId paAdapterNameId) const;

    TPortId getAdapterPortId(CStringDictionary::TStringId paAdapterNameId) const;

    /*!\brief Function that handles incoming events.
     *
     * \param paEIID ID of the input event that has occurred.
     * \param paExecEnv Event chain execution environment the FB will be executed in (used for adding output events).
     */
    void receiveInputEvent(size_t paEIID, CEventChainExecutionThread *paExecEnv);

    /*!\brief Configuration interface used by the typelib to parameterize generic function blocks.
     *
     * \param pa_acConfigString  A string containing the needed configuration data.
     * \return TRUE if configuration went ok, else FALSE.
     *
     */
    virtual bool configureFB(const char *pa_acConfigString);

    void setupFBInterface(const SFBInterfaceSpec *pa_pstInterfaceSpec, TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData);

    const SFBInterfaceSpec* getFBInterfaceSpec() const {
      return m_pstInterfaceSpec;
    }

    virtual EMGMResponse changeFBExecutionState(EMGMCommandType pa_unCommand);

    /*!\brief Get/set the instance name
     */
    CStringDictionary::TStringId getInstanceNameId(void) const {
      return m_nFBInstanceName;
    }
    ;

    const char* getInstanceName(void) const {
      return CStringDictionary::getInstance().get(m_nFBInstanceName);
    }

    void setInstanceNameId(const CStringDictionary::TStringId pa_nInstanceNameId) {
      m_nFBInstanceName = pa_nInstanceNameId;
    }

    /*!\brief Get information if the runable object is deletable by a management command.
     *
     */
    bool getDeletable(void) const {
      return m_bDeletable;
    }
    ;
    /*!\brief Set attribute to enable/disable the runable object deletion by a management command.
     *
     */
    void setDeletable(const bool &pa_bDelAble) {
      m_bDeletable = pa_bDelAble;
    }
    ;
    /*!\brief Return if the runable object is allowed to be deleted now.
     *
     * This is more complex then the simple deleteable flag as the current state has to be incorporated.
     * According to IEC 61499-1 Figure 24 an FB is deleteable if it is not in the Running state
     * \return true if currently all conditions are met to be deleteable
     */
    bool isCurrentlyDeleteable(void) const {
      return ((m_bDeletable) && (m_enFBState != e_RUNNING));
    }

    /*!\brief return the current execution state of the managed object
     */
    E_FBStates getState(void) const {
      return m_enFBState;
    }

    template<unsigned int ta_nNumEOs, unsigned int ta_nNumDIs, unsigned int ta_nNumDOs>
    struct genFBConnDataSizeTemplate {
        enum {
          value = (sizeof(CEventConnection) * ta_nNumEOs + sizeof(TDataConnectionPtr) * ta_nNumDIs + sizeof(CDataConnection) * ta_nNumDOs)
        };
    };

    static size_t genFBConnDataSize(unsigned int pa_nNumEOs, unsigned int pa_nNumDIs, unsigned int pa_nNumDOs) {
      return (sizeof(CEventConnection) * pa_nNumEOs + sizeof(TDataConnectionPtr) * pa_nNumDIs + sizeof(CDataConnection) * pa_nNumDOs);
    }

    template<unsigned int ta_nNumDIs, unsigned int ta_nNumDOs, unsigned int ta_nNumAdapters = 0>
    struct genFBVarsDataSizeTemplate {
        enum {
          value = (sizeof(CIEC_ANY) * ta_nNumDIs + sizeof(CIEC_ANY) * ta_nNumDOs + sizeof(TAdapterPtr) * ta_nNumAdapters)
        };
    };

    static size_t genFBVarsDataSize(unsigned int pa_nNumDIs, unsigned int pa_nNumDOs, unsigned int pa_nNumAdapters = 0) {
      return (sizeof(CIEC_ANY) * pa_nNumDIs + sizeof(CIEC_ANY) * pa_nNumDOs + sizeof(TAdapterPtr) * pa_nNumAdapters);

    }

    /*! \brief Get the data input with given number
     *
     * Attention this function will not perform any range checks on the pa_nDINum parameter!
     * @param pa_nDINum number of the data input starting with 0
     * @return pointer to the data input
     */
    TIEC_ANYPtr getDI(unsigned int pa_nDINum) const {
      return m_aoDIs + pa_nDINum;
    }

    /*! \brief Get the data output with given number
     *
     * Attention this function will not perform any range checks on the pa_nDONum parameter!
     * @param pa_nDONum number of the data output starting with 0
     * @return pointer to the data output
     */
    CIEC_ANY* getDO(unsigned int pa_nDONum) const {
      return m_aoDOs + pa_nDONum;
    }

#ifdef FORTE_SUPPORT_MONITORING
    TForteUInt32 &getEIMonitorData(TEventID paEIID);

    TForteUInt32 &getEOMonitorData(TEventID paEOID);

    /*!\brief get any internal FB referenced by the iterator to the name list
     *
     * This allows that also adapters and the internals of a CFB can be monitored.
     */
    virtual CFunctionBlock *getFB(forte::core::TNameIdentifier::CIterator &paNameListIt);

#endif //FORTE_SUPPORT_MONITORING
  protected:

    /*!\brief The main constructor for a function block.
     *
     * \param pa_poSrcRes          pointer to the resource this function block is contained in (mainly necessary for management functions and service interfaces)
     * \param pa_pstInterfaceSpec  const pointer to the interface spec
     * \param pa_nInstanceNameId   string id
     * \param pa_acFBConnData      Byte-array for fb-specific connection data. It will need space for the event output connections,
     *                             data input connections, and data output connections, in that order. The space requirements are:
     *                               sizeof(TEventConnectionPtr) * Number of Event outputs +
     *                               sizeof(TDataConnectionPtr)  * Number of Data inputs +
     *                               sizeof(TDataConnectionPtr)  * Number of Data outputs
     * \param pa_acFBVarsData      Byte-array for fb-specific variable data. It will need space for the data inputs, data outputs, and adapters in that order.
     *                             The space requirements are:
     *                               sizeof(CIEC_ANY)) * Number of Data inputs +
     *                               sizeof(CIEC_ANY)) * Number of Data outputs +
     *                               sizeof(TAdapterPtr) * ta_nNumAdapters
     */
    CFunctionBlock(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec, const CStringDictionary::TStringId pa_nInstanceNameId,
        TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData);

    static TPortId getPortId(CStringDictionary::TStringId pa_unPortNameId, TPortId pa_unMaxPortNames, const CStringDictionary::TStringId *pa_aunPortNames);

    /*!\brief Function to send an output event of the FB.
     *
     * \param pa_nEO Event output ID where event should be fired.
     */
    void sendOutputEvent(size_t paEO);

    /*!\brief Function to send an output event via the adapter.
     *
     * \param pa_nAdapterID ID of Adapter in current FBs adapter list.
     * \param pa_nEID Event ID where event should be fired.
     */
    void sendAdapterEvent(size_t paAdapterID, size_t paEID) const;

    void setupAdapters(const SFBInterfaceSpec *pa_pstInterfaceSpec, TForteByte *pa_acFBData);

    CEventConnection* getEOConUnchecked(TPortId paEONum) const {
      return (mEOConns + paEONum);
    }

    /*! \brief Get the data output connection with given number
     *
     * Attention this function will not perform any range checks on the pa_nDONum parameter!
     * @param pa_nDONum number of the data output starting with 0
     * @return pointer to the data output connection
     */
    CDataConnection* getDOConUnchecked(TPortId paDONum) {
      return mDOConns + paDONum;
    }

    /*!\brief Function to create an data type instance of given type
     *
     * @param pa_panDataTypeIds pointer to the data type ids. If the datatype
     *        is an Array to more values are taken from the array. If the given
     *        type is Any 0 is returned as necessary for maintaining the FB's interface.
     *        The functions puts the pointer in the datatype array to the next data point's id.
     * @param pa_acDataBuf pointer to the data buffer which should be used by the data type to create
     * @return on success... pointer to the datatype instance
     *         on error... 0
     */
    static CIEC_ANY* createDataPoint(const CStringDictionary::TStringId **pa_panDataTypeIds, TForteByte *pa_acDataBuf);

    void freeAllData();

    const SFBInterfaceSpec *m_pstInterfaceSpec; //!< Pointer to the interface specification
    CEventConnection *mEOConns; //!< A list of event connections pointers storing for each event output the event connection. If the output event is not connected the pointer is 0.
    TDataConnectionPtr *m_apoDIConns; //!< A list of data connections pointers storing for each data input the data connection. If the data input is not connected the pointer is 0.
    CDataConnection *mDOConns; //!< A list of data connections pointers storing for each data output the data connection. If the data output is not connected the pointer is 0.
    CEventChainExecutionThread *m_poInvokingExecEnv; //!< A pointer to the execution thread that invoked the FB. This value is stored here to reduce function parameters and reduce therefore stack usage.
    CAdapter **m_apoAdapters; //!< A list of pointers to the adapters. This allows to implement a general getAdapter().
    TFunctionBlockPtr *mInternalFBs;

  private:
    /*!\brief Function providing the functionality of the FB (e.g. execute ECC for basic FBs).
     *
     * \param pa_nEIID Event input ID where event occurred.
     */
    virtual void executeEvent(int pa_nEIID) = 0;

    /*!\brief Set the initial values of data inputs, outputs, and internal vars.
     *
     * Functionblocks which need to specify special initial values for their
     * data inputs, outputs, or internal vars other than 0 need to implement this
     * function. The function will be invoked on entering the IDLE state (i.e.,
     * on creation and on RESET).
     */
    virtual void setInitialValues() {
    }

    //!declared but undefined copy constructor as we don't want FBs to be directly copied.
    CFunctionBlock(const CFunctionBlock&);

    void configureGenericDI(TPortId paDIPortId, const CIEC_ANY *paRefValue);

    CResource *m_poResource; //!< A pointer to the resource containing the function block.
    CIEC_ANY *m_aoDIs; //!< A list of pointers to the data inputs. This allows to implement a general getDataInput()
    CIEC_ANY *m_aoDOs; //!< A list of pointers to the data outputs. This allows to implement a general getDataOutput()

#ifdef FORTE_SUPPORT_MONITORING
    void setupEventMonitoringData();

    // monitoring stuff
    TForteUInt32 *mEOMonitorCount;
    TForteUInt32 *mEIMonitorCount;
#endif

    //! the instance name of the object
    CStringDictionary::TStringId m_nFBInstanceName;

    /*!\brief Current state of the runnable object.
     *
     */
    E_FBStates m_enFBState;

    /*!\brief Attribute defines if runnable object can be deleted by a management command.
     *
     * Default value is set to true.
     * If the runnable object is declared in a device or resource specification it must be set to false.
     */
    bool m_bDeletable;

    //FIXME remove these friends
    friend class CAdapter;

#ifdef FORTE_SUPPORT_MONITORING
    friend class forte::core::CMonitoringHandler;
#endif //FORTE_SUPPORT_MONITORING

#ifdef FORTE_FMU
    friend class fmuInstance;
#endif //FORTE_FMU
};

template<>
struct CFunctionBlock::genFBConnDataSizeTemplate<0, 0, 0> {
    enum {
      value = 1
    };
};

template<>
struct CFunctionBlock::genFBVarsDataSizeTemplate<0, 0, 0> {
    enum {
      value = 1
    };
};

#define FUNCTION_BLOCK_CTOR(fbclass) \
 fbclass(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : \
 CFunctionBlock( pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData)

#define FUNCTION_BLOCK_CTOR_WITH_BASE_CLASS(fbclass, fbBaseClass) \
 fbclass(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : \
 fbBaseClass( pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData)


#ifdef OPTIONAL
#undef OPTIONAL
#endif

#endif /*_FUNCBLOC_H_*/
