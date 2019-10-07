/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Gunnar Grabmaier, Thomas Strasser, Gerhard Ebenhofer,
 *    Martin Melik Merkumians, Ingo Hegny,
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _MGMCMD_H_
#define _MGMCMD_H_

/**  \ingroup CORE \defgroup MGMCommands Management Commands Internal Representation
 * \brief In this section the FORTE representation of the management commands is described.
 */
/*@{*/

/*!\brief Type for the management command (e.g. create, delete ...)
 *
 * The constants for the management commands are selected in the following way:
 *  - the lower 4 bit are the same as the corresponding command defined by the standard (see 61499-1: Table 6, p47).
 *  - the higher 4 bits are counted as they are listed in the Table 8, p48 in 61499-1.
 *
 */
enum EMGMCommandType{
  cg_nMGM_CMD_Create_Group = 0x00, //!< Identifier for all create commands

  cg_nMGM_CMD_Create_DataType = 0x10, //!< Create a new data type definition in the FORTE.
  cg_nMGM_CMD_Create_FBType = 0x20,   //!< Create a new FB type definition in the FORTE.
  cg_nMGM_CMD_Create_AdapterType = 0x50,   //!< Create a new Adapter type definition in the FORTE.

  /*! \brief Create a new FB or resource instance.
   *
   * When creating a FB instance the parameters of the SManagementCMD are defined as:
   *    - m_sDestination = "" for creating within the device or "resname" for creating within a resource
   *    - m_sFistParam = FB Instance name
   *    - m_sSecondParam = FB Type
   *    - m_sAdditionalParams not used
   */
  cg_nMGM_CMD_Create_FBInstance = 0x30,

  /*! \brief Create a new connection (can be event or data connection).
   *
   * When creating a connection the parameters of the SManagementCMD are defined as:
   *    - m_sDestination = "" for creating within the device or "resname" for creating within a resource
   *    - m_sFistParam = source of the connection given by: "fbname.output"
   *    - m_sSecondParam = destination of the connection given by: "fbname.intput"
   *    - m_sAdditionalParams not used
   */
  cg_nMGM_CMD_Create_Connection = 0x40,

  cg_nMGM_CMD_Delete_Group = 0x01,   //!<Identifier for all delete commands
  cg_nMGM_CMD_Delete_DataType = 0x11,
  cg_nMGM_CMD_Delete_FBType = 0x21,

  /*! \brief Delete a FB or resource instance.
   *
   * When deleting a FB instance the parameters of the SManagementCMD are defined as:
   *    - m_sDestination = "" for deleting within the device or "resname" for creating within a resource
   *    - m_sFistParam = FB Instance name
   *    - m_sSecondParam not used
   *    - m_sAdditionalParams not used
   */
  cg_nMGM_CMD_Delete_FBInstance = 0x31,

  /*! \brief Delete a connection (can be event or data connection).
   *
   * When deleting a connection the parameters of the SManagementCMD are defined as:
   *    - m_sDestination = "" for deleting within the device or "resname" for deleting within a resource
   *    - m_sFistParam = source of the connection given by: "fbname.output"
   *    - m_sSecondParam = destination of the connection given by: "fbname.intput"
   *    - m_sAdditionalParams not used
   */
  cg_nMGM_CMD_Delete_Connection = 0x41,

  /*! \brief Delete all FBs or resource instances.
   *
   * When deleting a FB instance the parameters of the SManagementCMD are defined as:
   *    - m_sDestination = "" for deleting within the device or "resname" for deleting within a resource
   *    - m_sFistParam not used
   *    - m_sSecondParam not used
   *    - m_sAdditionalParams not used
   */
  cg_nMGM_CMD_Delete_AllFBInstances = 0x61,

  /*! \brief start a FB, resource or the device.
   *
   * When starting a FB, resource or the device the parameters of the SManagementCMD are defined as:
   *    - m_sDestination = "" for starting within the device or "resname" for starting within a resource
   *    - m_sFirstParam = target to be started can be empty for starting the whole device or resource, or contain a name for the resource or FB to start.
   *    - m_sSecondParam = not used
   *    - m_sAdditionalParams not used
   */
  cg_nMGM_CMD_Start = 0x02,

  /*! \brief stop a FB, resource or the device.
   *
   * When stopping a FB, resource or the device the parameters of the SManagementCMD are defined as:
   *    - m_sDestination = "" for stopping within the device or "resname" for stopping within a resource
   *    - m_sFirstParam = target to be stopped can be empty for starting the whole device or resource, or contain a name for the resource or FB to stop.
   *    - m_sSecondParam = not used
   *    - m_sAdditionalParams not used
   */
  cg_nMGM_CMD_Stop = 0x03,

  /*! \brief Read values from FB in- and outputs or from resource inputs
   *
   * When reading a parameter value the parameters of the SManagementCMD are defined as:
   *    - m_sDestination = "" for creating within the device or "resname" for creating within a resource
   *    - m_sFistParam = Source qualifier for reading an FB's variable ("fbname.var") variable identifier may be input, output, or internal var
   *    - m_sSecondParam = CStringDictionary::scm_nInvalidStringId
   *    - m_sAdditionalParams the read value is stored here
   */
  cg_nMGM_CMD_Read = 0x04,

  /*! \brief Write value to a given data input or output (FB or Resource)
   *
   * When writing a parameter value the parameters of the SManagementCMD are defined as:
   *    - m_sDestination = "" for creating within the device or "resname" for creating within a resource
   *    - m_sFistParam = Destination qualifier for writing on an FB's variable ("fbname.var") variable identifier may be input, output, or internal var
   *    - m_sSecondParam = CStringDictionary::scm_nInvalidStringId
   *    - m_sAdditionalParams = the string converted value to be set
   */
  cg_nMGM_CMD_Write = 0x05,

  /*! \brief kill a FB, resource or the device.
   *
   * When killing a FB, resource or the device the parameters of the SManagementCMD are defined as:
   *    - m_sDestination = "" for killing within the device or "resname" for stopping within a resource
   *    - m_sFistParam = target to be killed can be empty for starting the whole device or resource, or contain a name for the resource or FB to stop.
   *    - m_sSecondParam = not used
   *    - m_sAdditionalParams not used
   */
  cg_nMGM_CMD_Kill = 0x06,

#ifdef FORTE_SUPPORT_QUERY_CMD
  cg_nMGM_CMD_Query_Group = 0x07,   //!< Identifier for all query commands

  /*! \brief query FBs
   *
   * The parameters of the SManagementCMD are defined as:
   *    - m_sDestination = "" to query within the device or "resname" for fetching within a resource
   *    - m_acFirstParam = CStringDictionary::scm_nInvalidStringId for all FBs or instance name
   *    - m_sSecondParam = CStringDictionary::scm_nInvalidStringId for all FB types or type name
   *    - m_sAdditionalParams  the response value of the query
   */
  cg_nMGM_CMD_QUERY_FB = 0x17,

  /*! \brief query connections
   *
   * The parameters of the SManagementCMD are defined as:
   *    - m_sDestination = "" to query within the device or "resname" for fetching within a resource
   *    - m_acFirstParam = CStringDictionary::scm_nInvalidStringId
   *    - m_sSecondParam = CStringDictionary::scm_nInvalidStringId
   *    - m_sAdditionalParams  the response value of the query - a list of event and data connections (comma separated list of end points to end points)
   */
  cg_nMGM_CMD_QUERY_Connection = 0x27,

  /*! \brief Read the existing fb types.
   *
   * When reading the fb types the parameters of the SManagementCMD are defined as:
   *    - m_sDestination = "" for reading within the device or "resname" for reading within a resource
   *    - m_sFistParam = not used
   *    - m_sSecondParam = not used
   *    - m_sAdditionalParams the read value is stored here
   */
  cg_nMGM_CMD_QUERY_FBTypes = 0x37,

  /*! \brief Read the existing data types.
   *
   * When reading the data types the parameters of the SManagementCMD are defined as:
   *    - m_sDestination = "" for reading within the device or "resname" for reading within a resource
   *    - m_sFistParam = not used
   *    - m_sSecondParam = not used
   *    - m_sAdditionalParams the read value is stored here
   */
  cg_nMGM_CMD_QUERY_DTTypes = 0x47,

  /*! \brief Read the existing data types.
   *
   * When reading the data types the parameters of the SManagementCMD are defined as:
   *    - m_sDestination = "" for reading within the device or "resname" for reading within a resource
   *    - m_sFirstParam = not used
   *    - m_sSecondParam = not used
   *    - m_sAdditionalParams the read value is stored here
   */
  cg_nMGM_CMD_QUERY_AdapterTypes = 0x57,

  /**
   * currently not implemented
   */
  cg_nMGM_CMD_QUERY_TypeVers = 0x67,

  /*! \brief Read the interface of a specific FB type.
   *
   * When reading the FB interface the parameters of the SManagementCMD are defined as:
   *    - m_sFirstParam = FB type name
   *    - m_sSecondParam = not used
   *    - m_sAdditionalParams the read value is stored here
   */
  cg_nMGM_CMD_QUERY_FBType = 0x77,

  /*! \brief Read the interface of a specific adapter type.
   *
   * When reading the adapter interface the parameters of the SManagementCMD are defined as:
   *    - m_sFirstParam = adapter type name
   *    - m_sSecondParam = not used
   *    - m_sAdditionalParams the read value is stored here
   */
  cg_nMGM_CMD_QUERY_AdapterType = 0x87,
#endif

  /*! \brief reset a FB, resource or the device.
   *
   * When reseting a FB, resource or the device the parameters of the SManagementCMD are defined as:
   *    - m_sDestination = "" for reseting within the device or "resname" for reseting within a resource
   *    - m_sFistParam = target to be reset can be empty for reseting the whole device or resource, or contain a name for the resource or FB to reset.
   *    - m_sSecondParam = not used
   *    - m_sAdditionalParams not used
   */
  cg_nMGM_CMD_Reset = 0x08,



#ifdef FORTE_SUPPORT_MONITORING
  cg_nMGM_CMD_Monitoring_Group = 0x0A,
  cg_nMGM_CMD_Monitoring_Add_Watch = 0x1A,
  cg_nMGM_CMD_Monitoring_Remove_Watch = 0x2A,
  cg_nMGM_CMD_Monitoring_Read_Watches = 0x3A,
  cg_nMGM_CMD_Monitoring_Force = 0x5A,
  cg_nMGM_CMD_Monitoring_ClearForce = 0x6A,
  cg_nMGM_CMD_Monitoring_Trigger_Event = 0x7A,
  cg_nMGM_CMD_Monitoring_Reset_Event_Count = 0x8A,
#endif // FORTE_SUPPORT_MONITORING


  /*! \brief invalid command: some of the data could not be parsed
     */
  cg_nMGM_CMD_INVALID = 0xFF
};


/*!\brief Type for the response of MGM commands
 *
 * TODO fully define all responses as defined in IEC 61499 inc. numbers.
 */
enum EMGMResponse {e_RDY, e_BAD_PARAMS, e_LOCAL_TERMINATION, e_SYSTEM_TERMINATION, e_NOT_READY, e_UNSUPPORTED_CMD,
  e_UNSUPPORTED_TYPE, e_NO_SUCH_OBJECT, e_INVALID_OBJECT, e_INVALID_OPERATION, e_INVALID_STATE,
  e_OVERFLOW, e_INVALID_DST};

/*@}*/
#endif /*MGMCMD_H_*/
