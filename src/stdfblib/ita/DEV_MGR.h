/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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

    DEV_MGR(CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);
    virtual ~DEV_MGR();

    bool executeCommand(char *paDest, char *paCommand);

  private:

    EMGMResponse parseAndExecuteMGMCommand(char *paDest, char *paCommand);

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
     * \param paRequestString data of the request
     * \param paCommand the command structure for holding command information
     * \return pointer to the next part of the command zero on error
     */
    static char* parseRequest(char *paRequestString, forte::core::SManagementCMD &paCommand);
    /*! \brief Parse the given request that is left after parsing the header to parse FB data
     *
     * \param paRequestPartLeft  data of the request that has been left after parsing the header
     * \param paCommand the command structure for holding command information
     * \return true if the FB data could be parsed
     */
    static bool parseFBData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand);
    /*! \brief Parse the given request that is left after parsing the header to parse FB or Adapter type
     *
     * \param paRequestPartLeft  data of the request that has been left after parsing the header
     * \param paCommand the command structure for holding command information
     * \param pa_requestType the type that should be searched
     * \return true if the FB type could be parsed
     */
    static bool parseXType(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand, const char *paRequestType);
    /*! \brief Parse the given request that is left after parsing the header to parse connection data
     *
     * \param paRequestPartLeft   data of the request that has been left after parsing the header
     * \param paCommand the command structure for holding command information
     * \return true if the connection data could be parsed
     */
    static bool parseConnectionData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand);
    static bool parseWriteConnectionData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand);


    static void parseCreateData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand);
    static void parseDeleteData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand);
    //! Check if an FB is given for a state change command (i.e., START, STOP, KILL, RESET)
    static void parseAdditionalStateCommandData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand);
    static void parseReadData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand);
    static void parseWriteData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand);

#ifdef FORTE_SUPPORT_QUERY_CMD
    static void parseQueryData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand);
    static bool parseTypeListData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand);
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
    static bool parseMonitoringData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand);
    void generateMonitorResponse(EMGMResponse paResp, forte::core::SManagementCMD &paCMD);
#endif //FORTE_SUPPORT_MONITORING

    /*! \brief set the RESP output of the DEV_MGR according to the given response data
     *
     * \param pa_acID id of the response
     * \param pa_eResp qualifier of the response
     */
    void generateResponse(const char *paID, EMGMResponse paResp);
    /*! \brief set the RESP output of the DEV_MGR according to the given response data
     *
     * \param pa_acID id of the response
     * \param pa_eResp qualifier of the response
     * \param paCMD the command type
     */
    void generateLongResponse(EMGMResponse paResp, forte::core::SManagementCMD &paCMD);
    static void appendIdentifierName(CIEC_STRING& paDest, forte::core::TNameIdentifier& paIdentifier);

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

    forte::core::SManagementCMD mCommand;
  };

#endif /*DEV_MGR_H_*/
