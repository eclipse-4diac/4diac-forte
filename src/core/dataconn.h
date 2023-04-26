/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 *               2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl, Rene Smodic, Gunnar Grabmaier, Ingo Hegny,
 *    Martin Melik Merkumians, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Jobst - move allocation of data value to FB
 *******************************************************************************/
#ifndef _DATACONN_H_
#define _DATACONN_H_

#include "./datatypes/forte_any.h"
#include "conn.h"

/*! \ingroup CORE\brief Class for handling a data connection.
 */
class CDataConnection : public CConnection {

  public:
    CDataConnection(CFunctionBlock *paSrcFB, TPortId paSrcPortId, CIEC_ANY *paValue);

    EMGMResponse connect(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId) override;

    EMGMResponse connectToCFBInterface(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId) override;

    EMGMResponse disconnect(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId) override;

/*! \brief Write connection data value.
 *
 *   Write data value from FB data output to connection data variable.
 *   \param pa_poValue pointer to FB data output
 *   \return Can be the following response:
 *     - TRUE ... write successful
 *     - FALSE ... no such data connection exists
 */
    void writeData(const CIEC_ANY *pa_poValue){
      if(m_poValue){
        m_poValue->setValue(pa_poValue->unwrap());
      }
    };

/*! \brief Read connection data value.
 *
 *   Read data value from connection data variable to FB data input.
 *   \param pa_poValue pointer to FB data input
 */
    void readData(CIEC_ANY *pa_poValue) const {
      if(m_poValue){
        pa_poValue->setValue(m_poValue->unwrap());
      }
    }

/*! \brief Set class member variable m_poValue.
 *
 *   Set class member variable m_poValue.
 *   \param pa_poValue ... pointer to data variable
 */
    void setValue(CIEC_ANY *pa_poValue) {
      m_poValue = pa_poValue;
    }
/*! \brief Get class member variable m_poValue.
 *
 *   Get class member variable m_poValue.
 *   \return pointer to class member variable m_poValue
 */
    CIEC_ANY* getValue() {
      return m_poValue;
    }

  protected:

    /*! \brief check if the the given data points are compatible so that a connection can be established
     *
     * @param pa_poSrcDataPoint  data point of the connection's source (if 0 than it is a any data type)
     * @param pa_poDstDataPoint  data point of the connection's destination (if 0 than it is a any data type)
     * @return true if a connection between the given end points is valid
     */
    static bool canBeConnected(const CIEC_ANY *pa_poSrcDataPoint, const CIEC_ANY *pa_poDstDataPoint);

    /*! \brief Value for storing the current data of the connection
     */
    CIEC_ANY *m_poValue;
  private:

    void handleAnySrcPortConnection(const CIEC_ANY &paDstDataPoint);

    EMGMResponse establishDataConnection(CFunctionBlock *paDstFB, TPortId paDstPortId, CIEC_ANY *paDstDataPoint);

};

typedef CDataConnection *TDataConnectionPtr;

#endif /*_DATACONN_H_*/
