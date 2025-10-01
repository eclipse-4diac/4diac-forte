/*******************************************************************************
 * Copyright (c) 2025 Jose Cabral
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral- initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "resourceReplayer.h"

#include "forte/resource.h"
#include "../../../../core/src/resource_internal.h"
#include "../../../../core/src/trace/reader/utils.h"
#include "../../../../core/src/ecetFake.h"

#include <iostream>

namespace forte::iec61499::system {
  CResourceReplayer::CResourceReplayer(CResource &paResource, std::vector<trace::EventMessage> paExternalEvents) :
      mResource{paResource},
      mEcet{*dynamic_cast<CFakeEventExecutionThread *>(mResource.getResourceEventExecution())},
      mExternalEvents{std::move(paExternalEvents)} {

    mEcet.takeExternalControl();

    // similar implentation as in CFunctionBlock::receiveInputEvent,
    // If the FB type is one that does not interest us (i.e. not a Service Function Block),
    // we don't do anything and just pass through to the original CFunctionBlock::receiveInputEvent
    // Otherwise, we read the inputs and trace the event, but don't trigger the event itself, meaning
    // that we absorv the event
    // capturing "this" into the lambda created some issues for some reason
    auto processOneEvent = [validTypes = trace::reader::utils::getServiceFunctionBlockTypes(mResource),
                            &ecet = this->mEcet](TEventEntry paEvent) {
      // pass through non interesting events
      if (auto type = StringId::lookup(paEvent.getFB().getFBTypeName()); validTypes.find(type) == validTypes.end()) {

        paEvent.getFB().receiveInputEvent(paEvent.getPortId(), &ecet);
        return;
      }

      if (CFunctionBlock::E_FBStates::Running != paEvent.getFB().getState()) {
        return;
      }

      if (paEvent.getPortId() >= paEvent.getFB().getFBInterfaceSpec().getNumEIs()) {
        return;
      }

      paEvent.getFB().readInputData(paEvent.getPortId());
      paEvent.getFB().traceInputEvent(paEvent.getPortId());
    };

    mEcet.setRemoteCallbackForEventTriggering(processOneEvent);
    mReleaseEcet = [&ecet = mEcet]() { ecet.removeExternalControl(); };
  }

  CResourceReplayer::~CResourceReplayer() {
    if (mReleaseEcet) {
      mReleaseEcet();
    }
  }

  CResourceReplayer::CResourceReplayer(CResourceReplayer &&paOther) :
      mResource{paOther.mResource},
      mEcet{paOther.mEcet},
      mStepperIndex{paOther.mStepperIndex},
      mExternalEvents{std::move(paOther.mExternalEvents)},
      mReleaseEcet(std::move(paOther.mReleaseEcet)) {
    paOther.mReleaseEcet = nullptr;
  }

  std::vector<trace::EventMessage> CResourceReplayer::reproduceAll() {

    while (reproduceNextEvent() != std::nullopt)
      ;

    return getGeneratedEvents();
  }

  std::optional<TEventEntry> CResourceReplayer::reproduceNextEvent() {

    // For each of the external events we received as input (with its event counter X), we will advance the ecet
    // as long as the event counter is less than X, and then trigger the external event X
    while (mStepperIndex < mExternalEvents.size()) {

      auto payload = mExternalEvents[mStepperIndex].getPayload<trace::FBOutputEventPayload>();

      auto simulateExternalOutputEvent = [this](TEventEntry paEvent, const std::vector<std::string> &paOutputs) {
        // copy output data to FB
        for (size_t i = 0; i < paOutputs.size(); i++) {
          paEvent.getFB().getDO(i)->fromString(paOutputs[i].c_str());
        }

        // the following will trace and add possible new events to the queue
        paEvent.getFB().sendOutputEvent(paEvent.getPortId(), &mEcet);
      };

      while (mEcet.getEventCounter() < payload->mEventCounter) {
        auto toReturn = mEcet.getNextEvent();
        mEcet.triggerNextEvent();
        return toReturn;
      }

      auto functionBlock = trace::reader::utils::getFB(&mResource, payload->getInstanceName());

      if (functionBlock == nullptr) {
        std::cout << "Could not find the FB " << payload->getInstanceName() << " -> aborting..." << std::endl;
        std::abort();
      }

      simulateExternalOutputEvent(TEventEntry(*functionBlock, payload->mEventId), payload->mOutputs);
      mStepperIndex++;
      // we don't return yet, since simulating an external input only add events to the queue
      // we exit the loop only after mEcet.triggerNextEvent() is executed
    }

    while (mEcet.hasEvent()) {
      auto toReturn = mEcet.getNextEvent();
      mEcet.triggerNextEvent();
      return toReturn;
    }

    return std::nullopt;
  }

  std::vector<trace::EventMessage> CResourceReplayer::getGeneratedEvents() {
    return std::visit(
        [this](auto &&paTracer) -> std::vector<trace::EventMessage> {
          using T = std::decay_t<decltype(paTracer)>;
          if constexpr (std::is_same_v<T, trace::CInternalTracer> == true) {
            return paTracer.getEvents();
          }
          return {};
        },
        mResource.getInternal().getTracer().getTracerVariant());
  }
} // namespace forte::iec61499::system
