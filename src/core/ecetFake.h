/*******************************************************************************
 * Copyright (c) 2024 
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _CORE_FAKE_ECET_H_
#define _CORE_FAKE_ECET_H_

#include "ecet.h"

#include <functional>
#include <optional>
#include <set>

/**
 * @brief An ecet which can be fully controlled from outside the class
 * The ecet has two modes:
 * - Normal: 
 *    - The thread processing of events runs as usual
 *    - External events are added to the queue of external events as usual
 *    - manual methods are disabled: triggerNextEvent, removeFromBack
 * 
 * - Remote:
 *   - The thread processing events is suspended. Events can be triggered only via triggerNextEvent
 *   - External events are discarded
 *   - manual methods are enabled: triggerNextEvent, removeFromBack
 * 
 * When a breakpoint is hit, the usual behaviour is to set all ecets from the device in remote mode, 
 * but since this class does not have a reference to the device, a callback needs to be passed which will
 * be called when a breakpoint is hit. 
 * The caller should then take care of setting all ecets to remote mode.
 * 
 */
class CFakeEventExecutionThread : public CEventChainExecutionThread{
  public:

    /**
     * @brief Callback type for processing events 
     */
    typedef std::function<void(TEventEntry)> HandleEvent;


    CFakeEventExecutionThread();
    ~CFakeEventExecutionThread() override = default;

    /**
     * @brief Let the ecet run freely and not controlled from outside anymore
     */
    void removeExternalControl();

    /**
     * @brief Enable control from outside the class. Events are triggered only when requested 
     */
    void takeExternalControl();

    /**
     * @brief Set the callback that will be called for each event that is triggered. By default the callback does nothing
     * This ecet enters in remote mode, and the callback offers the change for extra work to be done by the caller.
     * 
     * @param paCallback Callback to be used
     */
    void setRemoteCallbackForEventTriggering(HandleEvent paCallback);

    /**
     * @brief Triggers the next event in the list if there's any
     * 
     * How the event trigger is handled can be configured via a callback. 
     */
    void triggerNextEvent();

    /**
     * @brief Starts a new chain event. In remote mode, this doesn't do anything 
     * 
     * @param paEventToAdd event starting the chain 
     */
    void startEventChain(TEventEntry paEventToAdd) override;

    /**
     * @brief Insert a new event at the front of the list of events
     * 
     * @param paEvent event to be inserted
     */
    void insertFront(TEventEntry paEvent);

    /**
     * @brief Remove events from the back of the list of events
     * 
     * @param paNumberOfItemsToRemove number of events to remove from the back of the list
     */
    void removeFromBack(size_t paNumberOfItemsToRemove);

    /**
     * @brief Get a copy of next event to be triggered if any
     * 
     * @return The next event to be triggered, std::nullopt if there isn't any 
     */
    std::optional<TEventEntry> getNextEvent();

    /**
     * @brief Add a new breakpoint. If the breakpoint already exists, nothing happens

     * @param paBreakpoint breakpoint to add
     */
    void addBreakpoint(TEventEntry paBreakpoint);

    /**
     * @brief Remove breakpoint. If the breakpoint does not exist, nothing happen

     * @param paBreakpoint breakpoint to remove
     */
    void removeBreakpoint(TEventEntry paBreakpoint);

    /**
     * @brief Set the callback that will be called when a breakpoint is hit.
     * 
     * @param paCallback Callback to be used
     */
    void setBreakpointHitCallback(HandleEvent paCallback);

private:

    void run() override;
    bool mIsRemoteEnabled{true};
    HandleEvent mProcessEventCallback;
    HandleEvent mBreakpointWasHit{[](TEventEntry){}};

    struct EventEntryComparator {
      bool operator() (const TEventEntry& paFirst, const TEventEntry& paSecond) const {
        if(paFirst.mFB < paSecond.mFB){
          return true;
        } else if (paFirst.mFB > paSecond.mFB){
          return false;
        } 
        return paFirst.mPortId < paSecond.mPortId;
      }
    };

    std::set<TEventEntry, EventEntryComparator> mBreakpoints;
};

#endif /*_CORE_FAKE_ECET_H_*/