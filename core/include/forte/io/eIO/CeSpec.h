/*******************************************************************************
 * Copyright (c) 2025 Maximilian Scharf
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Maximilian Scharf - Initial implementation.
 *******************************************************************************/

#pragma once

/* BUFFER_SIZE has to be a power of 2 */
#define BUFFER_SIZE_RISING_EDGE 8
#define BUFFER_SIZE_FALLING_EDGE 8
#define BUFFER_SIZE_UPPER_THRESHOLD 8
#define BUFFER_SIZE_LOWER_THRESHOLD 8
#define BUFFER_SIZE_BOUNDED_AREA 8
#define BUFFER_SIZE_GRADIENT 8

#include "forte/io/eIO/CeBuffer.h"
#include "forte/datatypes/forte_word.h"

#include "forte/io/mapper/io_handle.h"
#include "forte/io/processinterfacefb.h"

namespace forte::io {
  enum EeIOTypes {
    eIO_RISING_EDGE = 10,
    eIO_FALLING_EDGE = 20,
    eIO_UPPER_THRESHOLD = 30,
    eIO_LOWER_THRESHOLD = 40,
    eIO_BOUNDED_AREA = 50,
    eIO_GRADIENT = 60
  };

  class CeSpecBase {
    private:
      CProcessInterfaceFB *mEIO;

    protected:
      bool mTriggered;

      bool checkConditionTriggered(bool condition) {
        if (condition) {
          if (!mTriggered) {
            mTriggered = true;
            return true;
          }
        } else {
          mTriggered = false;
        }
        return false;
      }

    public:
      EeIOTypes mEIOType;
      virtual ~CeSpecBase() = default;
      virtual void trigger() = 0;
      virtual bool checkCondition() = 0;
      virtual void readToBuffer() = 0;
      virtual void readToBuffer(CIEC_ANY *) = 0;
      IOHandle *mHandle;

      CeSpecBase(EeIOTypes paType, IOHandle *paHandle, CProcessInterfaceFB *paEIOfb) :
          mEIO(paEIOfb),
          mEIOType(paType),
          mHandle(paHandle) {
      }

      CProcessInterfaceFB *getEIOfb() {
        return mEIO;
      }
  };

  template<typename T, std::size_t size>
  class CeSpec : public CeSpecBase {
    protected:
      CeBuffer<T, size> mBuffer;

    public:
      CeSpec(EeIOTypes paType, CProcessInterfaceFB *paEIOfb, IOHandle *paHandle) :
          CeSpecBase(paType, paHandle, paEIOfb) {
      }

      ~CeSpec() override = default;

      void trigger() override {
        if (getEIOfb()) {
          // DEVLOG_WARNING("[EventSpec] trigger!\r\n");
        } else {
          DEVLOG_WARNING("[EventSpec] CProcessInterfaceFB == nullptr !\r\n");
        }
      }

      void readToBuffer() override {
        T tempValue;
        mHandle->get(tempValue);
        mBuffer.push(tempValue);
      }

      void readToBuffer(CIEC_ANY *paValue) override {
        auto tempValue = static_cast<T *>(paValue);
        mBuffer.push(*tempValue);
      }

      bool checkCondition() override = 0;
  };

  class CeIO_RisingEdge : public CeSpec<CIEC_BOOL, BUFFER_SIZE_RISING_EDGE> {
    public:
      CeIO_RisingEdge(CProcessInterfaceFB *paEIOfb, IOHandle *paHandle) : CeSpec(eIO_RISING_EDGE, paEIOfb, paHandle) {
      }

      bool checkCondition() final {
        if (mHandle == nullptr) {
          DEVLOG_DEBUG("[EIO_RisingEdge] mHandle is nullptr\r\n");
          return false;
        }

        /* CRITERIA */
        /* ============================================ */
        mBuffer.makeSnapshot();
        auto orderedHistory = mBuffer.getSnapshot();
        auto curVal = orderedHistory[0];
        auto prevVal = orderedHistory[1];
        bool condition = curVal && !prevVal;
        /* ============================================ */

        return checkConditionTriggered(condition);
      }
  };

  class CeIO_FallingEdge : public CeSpec<CIEC_BOOL, BUFFER_SIZE_FALLING_EDGE> {
    public:
      CeIO_FallingEdge(CProcessInterfaceFB *paEIOfb, IOHandle *paHandle) : CeSpec(eIO_FALLING_EDGE, paEIOfb, paHandle) {
      }

      bool checkCondition() final {
        if (mHandle == nullptr) {
          DEVLOG_DEBUG("[EIO_FallingEdge] mHandle is nullptr\r\n");
          return false;
        }

        /* CRITERIA */
        /* ============================================ */
        mBuffer.makeSnapshot();
        auto orderedHistory = mBuffer.getSnapshot();
        auto curVal = orderedHistory[0];
        auto prevVal = orderedHistory[1];
        bool condition = !curVal && prevVal;
        /* ============================================ */

        return checkConditionTriggered(condition);
      }
  };

  class CeIO_UpperThreshold : public CeSpec<CIEC_WORD, BUFFER_SIZE_UPPER_THRESHOLD> {
    public:
      CeIO_UpperThreshold(CProcessInterfaceFB *paEIOfb, IOHandle *paHandle, uint32_t paThreshold) :
          CeSpec(eIO_UPPER_THRESHOLD, paEIOfb, paHandle),
          mUpperThreshold(paThreshold) {
      }

      bool checkCondition() final {
        if (mHandle == nullptr) {
          DEVLOG_DEBUG("[EIO_UPPER_THRESHOLD] mHandle is nullptr\r\n");
          return false;
        }

        /* CRITERIA */
        /* ============================================ */
        uint32_t curVal = mBuffer.getCurrentData();
        bool condition = curVal > mUpperThreshold;
        /* ============================================ */

        return checkConditionTriggered(condition);
      }

    private:
      uint32_t mUpperThreshold;
  };

  class CeIO_LowerThreshold : public CeSpec<CIEC_WORD, BUFFER_SIZE_LOWER_THRESHOLD> {
    public:
      CeIO_LowerThreshold(CProcessInterfaceFB *paEIOfb, IOHandle *paHandle, uint32_t paThreshold) :
          CeSpec(eIO_LOWER_THRESHOLD, paEIOfb, paHandle),
          mLowerThreshold(paThreshold) {
      }

      bool checkCondition() final {
        if (mHandle == nullptr) {
          DEVLOG_DEBUG("[EIO_LOWER_THRESHOLD] mHandle is nullptr\r\n");
          return false;
        }

        /* CRITERIA */
        /* ============================================ */
        uint32_t curVal = mBuffer.getCurrentData();
        bool condition = curVal < mLowerThreshold;
        /* ============================================ */

        return checkConditionTriggered(condition);
      }

    private:
      uint32_t mLowerThreshold;
  };

  class CeIO_BoundedArea : public CeSpec<CIEC_WORD, BUFFER_SIZE_BOUNDED_AREA> {
    public:
      CeIO_BoundedArea(CProcessInterfaceFB *paEIOfb,
                       IOHandle *paHandle,
                       uint32_t paBiggerThan,
                       uint32_t paSmallerThan) :
          CeSpec(eIO_BOUNDED_AREA, paEIOfb, paHandle),
          mBiggerThan(paBiggerThan),
          mSmallerThan(paSmallerThan) {
      }

      bool checkCondition() final {
        if (mHandle == nullptr) {
          DEVLOG_DEBUG("[EIO_BoundedArea] mHandle is nullptr\r\n");
          return false;
        }

        /* CRITERIA */
        /* ============================================ */
        uint32_t curVal = mBuffer.getCurrentData();
        bool condition = false;

        // checks if value leaves bounded area
        if (mBiggerThan > mSmallerThan) {
          condition = curVal > mBiggerThan || curVal < mSmallerThan;
        }

        // checks if value enters bounded area
        if (mBiggerThan < mSmallerThan) {
          condition = curVal > mBiggerThan && curVal < mSmallerThan;
        }
        /* ============================================ */

        return checkConditionTriggered(condition);
      }

    private:
      uint32_t mBiggerThan;
      uint32_t mSmallerThan;
  };

  class CeIO_Gradient : public CeSpec<CIEC_WORD, BUFFER_SIZE_GRADIENT> {
    public:
      CeIO_Gradient(CProcessInterfaceFB *paEIOfb, IOHandle *paHandle, uint32_t paDifference) :
          CeSpec(eIO_GRADIENT, paEIOfb, paHandle),
          mGradient(paDifference) {
      }

      bool checkCondition() final {
        if (mHandle == nullptr) {
          DEVLOG_DEBUG("[EIO_Gradient] mHandle is nullptr\r\n");
          return false;
        }

        /* CRITERIA */
        /* ============================================ */
        mBuffer.makeSnapshot();
        auto orderedHistory = mBuffer.getSnapshot();
        auto curVal = uint32_t(orderedHistory[0]);
        auto prevVal = uint32_t(orderedHistory[1]);
        bool condition = curVal > prevVal + mGradient || curVal < prevVal - mGradient;
        // /* ============================================ */

        return condition ? true : false;
      }

    private:
      uint32_t mGradient;
  };
} // namespace forte::io
