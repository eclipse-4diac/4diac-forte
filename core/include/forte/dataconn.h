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

#pragma once

#include <memory>
#include "forte/datatypes/forte_any.h"
#include "forte/conn.h"

namespace forte {
  /*! \ingroup CORE \brief Class for handling a data connection.
   */
  class CDataConnection : public CConnection {
    public:
      CDataConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId);

      EMGMResponse connect(CFunctionBlock &paDstFB, std::span<const StringId> paDstPortNameId) override;

      EMGMResponse connectToCFBInterface(CFunctionBlock &paDstFB, std::span<const StringId> paDstPortNameId) override;

      EMGMResponse disconnect(CFunctionBlock &paDstFB, std::span<const StringId> paDstPortNameId) override;

      Wrapper getDelegatingConnection(std::span<const StringId> paSrcNameList) override;

      void handleAnySrcPortConnection(const CIEC_ANY &paDstDataPoint);

      void getSourcePortName(TNameIdentifier &paResult) const override;

      /*! \brief Write connection data value.
       *
       *   Write data value from FB data output to connection data variable.
       *   \param paValue pointer to FB data output
       *   \return Can be the following response:
       *     - TRUE ... write successful
       *     - FALSE ... no such data connection exists
       */
      virtual void writeData(const CIEC_ANY &paValue) = 0;

      /*! \brief Read connection data value.
       *
       *   Read data value from connection data variable to FB data input.
       *   \param paValue pointer to FB data input
       */
      virtual void readData(CIEC_ANY &paValue) const = 0;

      /*! \brief Get class member variable mValue.
       *
       *   Get class member variable mValue.
       *   \return pointer to class member variable mValue
       */
      virtual CIEC_ANY &getValue() = 0;

      [[nodiscard]] virtual bool isGathering() const {
        return false;
      }

      /*!
       * \brief Get a member connection for the given name
       * @param paMemberName The member name
       * @return a member connection for the name, nullptr if there is no such member connection,
       *          or this if the name list was empty
       */
      virtual CDataConnection *getMemberConnection(const std::span<const StringId> paMemberName) {
        if (paMemberName.empty()) {
          return this;
        }
        return nullptr;
      }

    protected:
      /*! \brief check if the the given data points are compatible so that a connection can be established
       *
       * @param paSrcDataPoint  data point of the connection's source
       * @param paDstDataPoint  data point of the connection's destination
       * @return true if a connection between the given end points is valid
       */
      static bool canBeConnected(const CIEC_ANY &paSrcDataPoint, const CIEC_ANY &paDstDataPoint);

      virtual EMGMResponse
      establishDataConnection(CFunctionBlock &paDstFB, TPortId paDstPortId, const CIEC_ANY &paDstDataPoint);

    private:
      EMGMResponse establishGatheringConnection(CFunctionBlock &paDstFB,
                                                TPortId paDstPortId,
                                                CIEC_ANY &paDstDataPoint,
                                                std::span<const StringId> paDstPortNameId);
  };

  template<typename T>
  class COutDataConnection final : public CDataConnection {
    public:
      COutDataConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId, const T &paValue) :
          CDataConnection(paSrcFB, paSrcPortId),
          mValue(paValue) {
      }

      void writeData(const T &paValue) {
        mValue = paValue;
      }

      void writeData(const CIEC_ANY &paValue) override {
        mValue.setValue(paValue.unwrap());
      }

      void readData(CIEC_ANY &paValue) const override {
        paValue.setValue(mValue.unwrap());
      }

      T &getValue() override {
        return mValue;
      }

    private:
      T mValue;
  };

  class CGenDataConnection final : public CDataConnection {
    public:
      CGenDataConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId, CIEC_ANY &paValue) :
          CDataConnection(paSrcFB, paSrcPortId),
          mValue(paValue.clone(nullptr)) {
      }

      void writeData(const CIEC_ANY &paValue) override {
        mValue->setValue(paValue.unwrap());
      }

      void readData(CIEC_ANY &paValue) const override {
        paValue.setValue(mValue->unwrap());
      }

      CIEC_ANY &getValue() override {
        return *mValue;
      }

    private:
      std::unique_ptr<CIEC_ANY> mValue;
  };
} // namespace forte
