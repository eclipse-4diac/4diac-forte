/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Rene Smodic, Thomas Strasser, Gerhard Ebenhofer,
 *   Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _DEV_MGR_H_
#define _DEV_MGR_H_

#include <funcbloc.h>
#include <mgmcmdstruct.h>
#include <commfb.h>
#include "IBootFileCallback.h"

class CDevice;

/*! \brief Implementation of the DEV_MGR FB.
 */
class DEV_MGR: public forte::com_infra::CCommFB, public IBootFileCallback {
  DECLARE_FIRMWARE_FB(DEV_MGR)

  public:
    /*!\brief Type for the response of MGM command messages
     *
     * TODO fully define all responses as defined in IEC 61499 inc. numbers.
     */
    static const char * const scm_sMGMResponseTexts[13];

    DEV_MGR(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);
    virtual ~DEV_MGR();

    bool executeCommand(char *pa_acDest, char *pa_acCommand);

  private:

    EMGMResponse parseAndExecuteMGMCommand(char *pa_acDest, char *pa_acCommand);

    static const CStringDictionary::TStringId scm_anDataInputNames[];
    static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
    static const CStringDictionary::TStringId scm_anDataOutputNames[];
    static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];

    static const TEventID scm_nEventINITID = 0;
    static const TEventID scm_nEventREQID = 1;
    static const TForteInt16 scm_anEIWithIndexes[];
    static const TDataIOID scm_anEIWith[];
    static const CStringDictionary::TStringId scm_anEventInputNames[];

    static const TEventID scm_nEventINITOID = 0;
    static const TEventID scm_nEventCNFID = 1;
    static const TForteInt16 scm_anEOWithIndexes[];
    static const TDataIOID scm_anEOWith[];
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    static const SFBInterfaceSpec scm_stFBInterfaceSpec;

    FORTE_FB_DATA_ARRAY(2, 3, 4, 0);

    //! The device the block is contained in
    CDevice &m_poDevice;

    void executeRQST(void);
    /*! \brief Parse the given request header to determine the ID and the requested command
     *
     * \param pa_acRequestString data of the request
     * \param pa_rstCommand the command structure for holding command information
     * \return pointer to the next part of the command zero on error
     */
    static char* parseRequest(char *pa_acRequestString, forte::core::SManagementCMD &pa_rstCommand);
    /*! \brief Parse the given request that is left after parsing the header to parse FB data
     *
     * \param pa_acRequestPartLeft  data of the request that has been left after parsing the header
     * \param pa_rstCommand the command structure for holding command information
     * \return true if the FB data could be parsed
     */
    static bool parseFBData(char *pa_acRequestPartLeft, forte::core::SManagementCMD &pa_rstCommand);
    /*! \brief Parse the given request that is left after parsing the header to parse FB or Adapter type
     *
     * \param pa_acRequestPartLeft  data of the request that has been left after parsing the header
     * \param pa_rstCommand the command structure for holding command information
     * \param pa_requestType the type that should be searched
     * \return true if the FB type could be parsed
     */
    static bool parseXType(char *pa_acRequestPartLeft, forte::core::SManagementCMD &pa_rstCommand, char *pa_requestType);
    /*! \brief Parse the given request that is left after parsing the header to parse connection data
     *
     * \param pa_acRequestPartLeft   data of the request that has been left after parsing the header
     * \param pa_rstCommand the command structure for holding command information
     * \return true if the connection data could be parsed
     */
    static bool parseConnectionData(char *pa_acRequestPartLeft, forte::core::SManagementCMD &pa_rstCommand);
    static bool parseWriteConnectionData(char *pa_acRequestPartLeft, forte::core::SManagementCMD &pa_rstCommand);


    static void parseCreateData(char *pa_acRequestPartLeft, forte::core::SManagementCMD &pa_rstCommand);
    static void parseDeleteData(char *pa_acRequestPartLeft, forte::core::SManagementCMD &pa_rstCommand);
    //! Check if an FB is given for a state change command (i.e., START, STOP, KILL, RESET)
    static void parseAdditionalStateCommandData(char *pa_acRequestPartLeft, forte::core::SManagementCMD &pa_rstCommand);
    static void parseReadData(char *pa_acRequestPartLeft, forte::core::SManagementCMD &pa_rstCommand);
    static void parseWriteData(char *pa_acRequestPartLeft, forte::core::SManagementCMD &pa_rstCommand);

#ifdef FORTE_SUPPORT_QUERY_CMD
    static void parseQueryData(char *pa_acRequestPartLeft, forte::core::SManagementCMD &pa_rstCommand);
    static bool parseTypeListData(char *pa_acRequestPartLeft, forte::core::SManagementCMD &pa_rstCommand);
#endif


    /*! \brief parse a hierarchical identifier list
     *
     * The identifiers are separated  by '.' and the end character for the list is '\"'
     *
     * @param paIdentifierStart pointer to the start of the identifier that will be parsed
     * @param paIdentifier identifier vector where to write the parsed identifiers to
     * @return number of bytes used from the character array or -1 if the identifier could not be parsed
     */
    static int parseIdentifier(char *paIdentifierStart, forte::core::TNameIdentifier &paIdentifier);
  
    virtual void executeEvent(int pa_nEIID);

#ifdef FORTE_SUPPORT_MONITORING
    static bool parseMonitoringData(char *pa_acRequestPartLeft, forte::core::SManagementCMD &pa_rstCommand);
    void generateMonitorResponse(EMGMResponse pa_eResp, forte::core::SManagementCMD &pa_stCMD);
#endif //FORTE_SUPPORT_MONITORING    virtual void executeEvent(int pa_nEIID);

    /*! \brief set the RESP output of the DEV_MGR according to the given response data
     *
     * \param pa_acID id of the response
     * \param pa_eResp qualifier of the response
     */
    void generateResponse(const char *pa_acID, EMGMResponse pa_eResp);
    /*! \brief set the RESP output of the DEV_MGR according to the given response data
     *
     * \param pa_acID id of the response
     * \param pa_eResp qualifier of the response
     * \param pa_stCMD the command type
     */
    void generateLongResponse(EMGMResponse pa_eResp, forte::core::SManagementCMD &pa_stCMD);
    void appedIdentifierName(CIEC_STRING& paDest, forte::core::TNameIdentifier &paIdentifier);

    CIEC_BOOL &QI() {
      return *static_cast<CIEC_BOOL*>(getDI(0));
    };

    CIEC_STRING &ID() {
      return *static_cast<CIEC_STRING*>(getDI(1));
    };

    CIEC_STRING &RESP() {
      return *static_cast<CIEC_STRING*>(getDI(2));
    };

    CIEC_BOOL &QO() {
      return *static_cast<CIEC_BOOL*>(getDO(0));
    };

    CIEC_STRING &STATUS() {
      return *static_cast<CIEC_STRING*>(getDO(1));
    };

    CIEC_STRING &DST() {
      return *static_cast<CIEC_STRING*>(getDO(2));
    };

    CIEC_STRING &RQST() {
      return *static_cast<CIEC_STRING*>(getDO(3));
    };

    forte::core::SManagementCMD m_stCommand;
  };

#endif /*DEV_MGR_H_*/
