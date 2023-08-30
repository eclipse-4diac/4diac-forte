/*******************************************************************************
 * Copyright (c) 2005, 2023 Profactor GmbH, ACIN, fortiss GmbH,
 *                          Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
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

/*! \ingroup CORE \brief Class for handling a data connection.
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
    *   \param paValue pointer to FB data output
    *   \return Can be the following response:
    *     - TRUE ... write successful
    *     - FALSE ... no such data connection exists
    */
    void writeData(const CIEC_ANY& paValue) {
      if(mValue){
        mValue->setValue(paValue.unwrap());
      }
    };

    /*! \brief Read connection data value.
    *
    *   Read data value from connection data variable to FB data input.
    *   \param paValue pointer to FB data input
    */
    void readData(CIEC_ANY& paValue) const {
      if(mValue){
        paValue.setValue(mValue->unwrap());
      }
    }

    /*! \brief Set class member variable mValue.
     *
     *   Set class member variable mValue.
     *   \param paValue ... pointer to data variable
     */
    virtual void setValue(CIEC_ANY *paValue) {
      mValue = paValue;
    }

    /*! \brief Get class member variable mValue.
    *
    *   Get class member variable mValue.
    *   \return pointer to class member variable mValue
    */
    CIEC_ANY* getValue() {
      return mValue;
    }

  protected:
    /*! \brief check if the the given data points are compatible so that a connection can be established
     *
     * @param paSrcDataPoint  data point of the connection's source (if 0 than it is a any data type)
     * @param paDstDataPoint  data point of the connection's destination (if 0 than it is a any data type)
     * @return true if a connection between the given end points is valid
     */
    static bool canBeConnected(const CIEC_ANY *paSrcDataPoint, const CIEC_ANY *paDstDataPoint);

    virtual EMGMResponse establishDataConnection(CFunctionBlock *paDstFB, TPortId paDstPortId, CIEC_ANY *paDstDataPoint);

    void handleAnySrcPortConnection(const CIEC_ANY &paDstDataPoint);

    /*! \brief Value for storing the current data of the connection
     */
    CIEC_ANY *mValue;
};

typedef CDataConnection *TDataConnectionPtr;

#endif /*_DATACONN_H_*/
