/*******************************************************************************
 * Copyright (c) 2026 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/

#pragma once

#include <optional>
#include <span>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "forte/datatypes/forte_any_variant.h"
#include "forte/com/factory.h"

namespace forte::com::test {
  using namespace forte::literals;

  template<typename T, typename TDerived>
  class TestChannelFixtureBase : public ComChannel<T> {
    public:
      static inline int smCreated = 0;
      static inline int smDestroyed = 0;
      static inline int smOpenCalls = 0;
      static inline int smCloseCalls = 0;
      static inline int smSendCalls = 0;
      static inline int smPollCalls = 0;
      static inline int smSentCallbacks = 0;
      static inline int smReceiveCallbacks = 0;
      static inline int smNextIdentity = 0;
      static inline ComResult smNextOpenResult = ComResult::Ok;
      static inline ComResult smNextSendResult = ComResult::Ok;
      static inline TDerived *smLastChannel = nullptr;
      static inline std::size_t smLastMinReceiveSize = 0;
      static inline std::size_t smLastMaxReceiveSize = 0;
      static inline std::string smLastSentData;
      static inline std::vector<CIEC_ANY_VARIANT> smLastSentValues;
      static inline ComResult smLastReceiveResult = ComResult::Ok;
      static inline ComResult smSetMaxReceiveSizeResult = ComResult::Ok;

      explicit TestChannelFixtureBase(ComObserver<T> &paObserver) :
          ComChannel<T>(paObserver),
          mIdentity(++smNextIdentity) {
        ++smCreated;
      }

      ~TestChannelFixtureBase() override {
        ++smDestroyed;
      }

      ComResult open(const std::string_view paConfigString, std::span<const ComChannelDescriptor>) override {
        ++smOpenCalls;
        mConfigString.assign(paConfigString);
        smLastMinReceiveSize = this->getMinReceiveSize();
        smLastMaxReceiveSize = this->getMaxReceiveSize();
        const ComResult result = smNextOpenResult;
        mIsOpen = result == ComResult::Ok;
        return result;
      }

      ComResult poll() override {
        ++smPollCalls;
        if (mSendCompletionResult.has_value()) {
          const ComResult result = *mSendCompletionResult;
          mSendCompletionResult.reset();
          this->getObserver().sent(result, *this);
          ++smSentCallbacks;
          return ComResult::Async;
        }
        if (mReceiveCompletionData.has_value()) {
          const T data = *mReceiveCompletionData;
          mReceiveCompletionData.reset();
          deliverReceive(data);
          return ComResult::Async;
        }
        return ComResult::Ok;
      }

      ComResult close() override {
        ++smCloseCalls;
        mIsOpen = false;
        return ComResult::Ok;
      }

      ComResult send(T paData) override {
        ++smSendCalls;
        if constexpr (std::is_same_v<T, ComBuffer>) {
          if (paData.size()) {
            smLastSentData.assign(reinterpret_cast<const char *>(paData.data()), paData.size());
          } else {
            smLastSentData.clear();
          }
        } else if constexpr (std::is_same_v<T, std::span<CIEC_ANY_VARIANT>>) {
          smLastSentValues.assign(paData.begin(), paData.end());
        }
        return smNextSendResult;
      }

      [[nodiscard]] bool isOpen() const override {
        return mIsOpen;
      }

      ComResult setMaxReceiveSize(const std::size_t paSize) override {
        if (smSetMaxReceiveSizeResult != ComResult::Ok) {
          return smSetMaxReceiveSizeResult;
        }
        return ComChannel<T>::setMaxReceiveSize(paSize);
      }

      [[nodiscard]] int identity() const {
        return mIdentity;
      }

      [[nodiscard]] std::string_view configString() const {
        return mConfigString;
      }

      void completeSend(const ComResult paResult) {
        mSendCompletionResult = paResult;
      }

      void completeReceive(T paData) {
        mReceiveCompletionData = paData;
      }

      ComResult deliverReceive(T paData) {
        ++smReceiveCallbacks;
        smLastReceiveResult = this->getObserver().receive(paData, *this);
        return smLastReceiveResult;
      }

      static void reset() {
        smCreated = 0;
        smDestroyed = 0;
        smOpenCalls = 0;
        smCloseCalls = 0;
        smSendCalls = 0;
        smPollCalls = 0;
        smSentCallbacks = 0;
        smReceiveCallbacks = 0;
        smNextIdentity = 0;
        smNextOpenResult = ComResult::Ok;
        smNextSendResult = ComResult::Ok;
        smLastChannel = nullptr;
        smLastMinReceiveSize = 0;
        smLastMaxReceiveSize = 0;
        smLastSentData.clear();
        smLastSentValues.clear();
        smLastReceiveResult = ComResult::Ok;
        smSetMaxReceiveSizeResult = ComResult::Ok;
      }

    protected:
      bool mIsOpen = false;
      std::string mConfigString;

    private:
      int mIdentity;
      std::optional<ComResult> mSendCompletionResult;
      std::optional<T> mReceiveCompletionData;
  };

  class TestChannel : public TestChannelFixtureBase<std::span<CIEC_ANY_VARIANT>, TestChannel> {
    public:
      explicit TestChannel(ComObserver<std::span<CIEC_ANY_VARIANT>> &paObserver) : TestChannelFixtureBase(paObserver) {
        smLastChannel = this;
      }

      [[nodiscard]] bool supports(const std::span<const ComChannelDescriptor> paDescriptors) const override {
        return supportsChannel("test-channel"_STRID, paDescriptors);
      }
  };

  class AlternateTestChannel final : public TestChannel {
    public:
      explicit AlternateTestChannel(ComObserver<std::span<CIEC_ANY_VARIANT>> &paObserver) : TestChannel(paObserver) {
      }

      [[nodiscard]] bool supports(const std::span<const ComChannelDescriptor> paDescriptors) const override {
        return supportsChannel("test-alternate-channel"_STRID, paDescriptors);
      }
  };

  class TestBufferChannel final : public TestChannelFixtureBase<ComBuffer, TestBufferChannel> {
    public:
      explicit TestBufferChannel(ComObserver<ComBuffer> &paObserver) : TestChannelFixtureBase(paObserver) {
        smLastChannel = this;
      }

      [[nodiscard]] bool supports(std::span<const ComChannelDescriptor> paDescriptors) const override {
        return supportsChannel("test-buffer-channel"_STRID, paDescriptors);
      }
  };

  template<typename T>
  class TestObserverFixtureBase : public ComObserver<T> {
    public:
      ComResult receive(T paData, ComChannel<T> &paChannel) override {
        ++mReceiveCalls;
        mLastReceiveSize = paData.size();
        mLastReceiveChannel = &paChannel;
        return mReceiveResult;
      }

      void sent(const ComResult paResult, ComChannel<T> &paChannel) override {
        ++mSentCalls;
        mLastSentResult = paResult;
        mLastSentChannel = &paChannel;
      }

      int mReceiveCalls = 0;
      int mSentCalls = 0;
      std::size_t mLastReceiveSize = 0;
      ComResult mReceiveResult = ComResult::Ok;
      ComResult mLastSentResult = ComResult::Ok;
      ComChannel<T> *mLastReceiveChannel = nullptr;
      ComChannel<T> *mLastSentChannel = nullptr;
  };

  class TestVariantObserver final : public TestObserverFixtureBase<std::span<CIEC_ANY_VARIANT>> {
    public:
      ComResult receive(std::span<CIEC_ANY_VARIANT> paData,
                        ComChannel<std::span<CIEC_ANY_VARIANT>> &paChannel) override {
        const ComResult result = TestObserverFixtureBase::receive(paData, paChannel);
        if (paData.size() == 1) {
          mLastReceived = paData[0];
        } else {
          mLastReceived.reset();
        }
        return result;
      }

      std::optional<std::span<CIEC_ANY_VARIANT>> getReceiveBuffer() override {
        return std::span<CIEC_ANY_VARIANT>{};
      }

      std::optional<std::span<CIEC_ANY_VARIANT>> getSendBuffer() override {
        return std::span<CIEC_ANY_VARIANT>{};
      }

      std::optional<CIEC_ANY_VARIANT> mLastReceived;
  };

  class TestBufferObserver final : public TestObserverFixtureBase<ComBuffer> {};
} // namespace forte::com::test
