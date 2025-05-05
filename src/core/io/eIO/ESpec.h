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
#define BUFFER_SIZE_RISING_EDGE        8
#define BUFFER_SIZE_FALLING_EDGE       8
#define BUFFER_SIZE_UPPER_THRESHOLD    8
#define BUFFER_SIZE_LOWER_THRESHOLD    8
#define BUFFER_SIZE_BOUNDED_AREA       8
#define BUFFER_SIZE_GRADIENT           8

#include "EBuffer.h"
#include "forte_word.h"

#include <core/io/mapper/io_handle.h>
#include <core/io/processinterfacefb.h>


using namespace forte::core::io;

enum eIOTypes {
  eIO_RISING_EDGE                   = 10,
  eIO_FALLING_EDGE                  = 20,
  eIO_UPPER_THRESHOLD               = 30,
  eIO_LOWER_THRESHOLD               = 40,
  eIO_BOUNDED_AREA                  = 50,
  eIO_GRADIENT                      = 60
};

class ESpecBase {
  private:
    CProcessInterfaceFB *eIO;

  protected:
    bool triggered;

    bool checkConditionTriggered(bool condition) {
      if (condition) {
        if (!triggered) {
          triggered = true;
          return true;
        }
      } else {
        triggered = false;
      }
      return false;
    }

  public:
    eIOTypes eIOType;
    virtual ~ESpecBase() = default;
    virtual void trigger() = 0;
    virtual bool checkCondition() = 0;
    virtual void readToBuffer() = 0;
    virtual void readToBuffer(CIEC_ANY *) = 0;
    IOHandle *mHandle;

    ESpecBase(eIOTypes type, IOHandle *handle, CProcessInterfaceFB *eIOfb) : eIOType(type), mHandle(handle), eIO(eIOfb) {}

    CProcessInterfaceFB *getEIOfb() {
        return eIO;
      }
};

template <typename T, std::size_t size>
class ESpec : public ESpecBase {
  protected:
    EBuffer<T, size> buffer;
    T triggerValue;

  public:
    ESpec(eIOTypes type, CProcessInterfaceFB *eIOfb, IOHandle *handle)
        : ESpecBase(type, handle, eIOfb) {}

    ~ESpec() = default;

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
      buffer.push(tempValue);
    }

    void readToBuffer(CIEC_ANY *paValue) override {
      auto tempValue = static_cast<T *>(paValue);
      buffer.push(*tempValue);
    }

    bool checkCondition() override = 0;
};

class EIO_RisingEdge : public ESpec<CIEC_BOOL, BUFFER_SIZE_RISING_EDGE> {
  public:
    EIO_RisingEdge(CProcessInterfaceFB *eIOfb, IOHandle *handle)
        : ESpec(eIO_RISING_EDGE, eIOfb, handle) {}

    bool checkCondition() final {
      if (mHandle == nullptr) {
        DEVLOG_DEBUG("[EIO_RisingEdge] mHandle is nullptr\r\n");
        return false;
      }

      /* CRITERIA */
      /* ============================================ */
      buffer.makeSnapshot();
      auto orderedHistory = buffer.getSnapshot();
      auto curVal = orderedHistory[0];
      auto prevVal = orderedHistory[1];
      bool condition = curVal && !prevVal;
      /* ============================================ */

      return checkConditionTriggered(condition);
    }
};

class EIO_FallingEdge : public ESpec<CIEC_BOOL, BUFFER_SIZE_FALLING_EDGE> {
  public:
    EIO_FallingEdge(CProcessInterfaceFB *eIOfb, IOHandle *handle)
        : ESpec(eIO_FALLING_EDGE, eIOfb, handle) {}

    bool checkCondition() final {
      if (mHandle == nullptr) {
        DEVLOG_DEBUG("[EIO_FallingEdge] mHandle is nullptr\r\n");
        return false;
      }

      /* CRITERIA */
      /* ============================================ */
      buffer.makeSnapshot();
      auto orderedHistory = buffer.getSnapshot();
      auto curVal = orderedHistory[0];
      auto prevVal = orderedHistory[1];
      bool condition = !curVal && prevVal;
      /* ============================================ */

      return checkConditionTriggered(condition);
    }
};

class EIO_UpperThreshold : public ESpec<CIEC_WORD, BUFFER_SIZE_UPPER_THRESHOLD> {
  public:
    EIO_UpperThreshold(CProcessInterfaceFB *eIOfb, IOHandle *handle, uint32_t threshold)
        : ESpec(eIO_UPPER_THRESHOLD, eIOfb, handle),
        upper_threshold(threshold){}

    bool checkCondition() final {
      if (mHandle == nullptr) {
        DEVLOG_DEBUG("[EIO_UPPER_THRESHOLD] mHandle is nullptr\r\n");
        return false;
      }

      /* CRITERIA */
      /* ============================================ */
      uint32_t curVal = buffer.getCurrentData();
      bool condition = curVal > upper_threshold;
      /* ============================================ */

      return checkConditionTriggered(condition);
    }

  private:
    uint32_t upper_threshold;
};

class EIO_LowerThreshold : public ESpec<CIEC_WORD, BUFFER_SIZE_LOWER_THRESHOLD> {
  public:
    EIO_LowerThreshold(CProcessInterfaceFB *eIOfb, IOHandle *handle, uint32_t threshold)
        : ESpec(eIO_LOWER_THRESHOLD, eIOfb, handle),
        lower_threshold(threshold){}

    bool checkCondition() final {
      if (mHandle == nullptr) {
        DEVLOG_DEBUG("[EIO_LOWER_THRESHOLD] mHandle is nullptr\r\n");
        return false;
      }

      /* CRITERIA */
      /* ============================================ */
      uint32_t curVal = buffer.getCurrentData();
      bool condition = curVal < lower_threshold;
      /* ============================================ */

      return checkConditionTriggered(condition);
    }

  private:
    uint32_t lower_threshold;
};

class EIO_BoundedArea : public ESpec<CIEC_WORD, BUFFER_SIZE_BOUNDED_AREA> {
  public:
    EIO_BoundedArea(CProcessInterfaceFB *eIOfb, IOHandle *handle, uint32_t biggerThan, uint32_t smallerThan)
        : ESpec(eIO_BOUNDED_AREA, eIOfb, handle),
        biggerThan(biggerThan), smallerThan(smallerThan) {}

    bool checkCondition() final {
      if (mHandle == nullptr) {
        DEVLOG_DEBUG("[EIO_BoundedArea] mHandle is nullptr\r\n");
        return false;
      }

      /* CRITERIA */
      /* ============================================ */
      uint32_t curVal = buffer.getCurrentData();
      bool condition = false;

      // checks if value leaves bounded area
      if (biggerThan > smallerThan)
        condition = curVal > biggerThan || curVal < smallerThan;

      // checks if value enters bounded area
      if (biggerThan < smallerThan)
        condition = curVal > biggerThan && curVal < smallerThan;

      // DEVLOG_DEBUG("curVal: %d, lower_threshold: %d, upper_threshold: %d, condition: %d\r\n", curVal, biggerThan, smallerThan, condition);
      // DEVLOG_DEBUG("curVal: %d \r\n", curVal);
      /* ============================================ */

      return checkConditionTriggered(condition);
    }

  private:
    uint32_t biggerThan;
    uint32_t smallerThan;

};

class EIO_Gradient : public ESpec<CIEC_WORD, BUFFER_SIZE_GRADIENT> {
  public:
    EIO_Gradient(CProcessInterfaceFB *eIOfb, IOHandle *handle, uint32_t difference)
        : ESpec(eIO_GRADIENT, eIOfb, handle),
        gradient(difference){}

    bool checkCondition() final {
      if (mHandle == nullptr) {
        DEVLOG_DEBUG("[EIO_Gradient] mHandle is nullptr\r\n");
        return false;
      }

      /* CRITERIA */
      /* ============================================ */
      buffer.makeSnapshot();
      auto orderedHistory = buffer.getSnapshot();
      auto curVal = uint32_t(orderedHistory[0]);
      auto prevVal = uint32_t(orderedHistory[1]);
      bool condition = curVal > prevVal + gradient || curVal < prevVal - gradient;
      // DEVLOG_DEBUG("curVal: %d, prevVal: %d, gradient: %d, condition: %d\r\n", curVal, prevVal, gradient, condition);
      // /* ============================================ */

      return condition ? true : false;
    }

  private:
    uint32_t gradient;
};
