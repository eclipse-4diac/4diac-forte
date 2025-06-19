/*******************************************************************************
 * Copyright (c) 2025 Monika Wenger
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 *******************************************************************************/
#ifndef _IEXTEVHAN_H_
#define _IEXTEVHAN_H_

class IExternalEventHandler {
  public:
    virtual ~IExternalEventHandler() = default;

    /*!\brief Enables this event source
     *
     */
    virtual void enableHandler() = 0;
    /*!\brief Disable this event source
     */
    virtual void disableHandler() = 0;
    /*!\brief Sets the priority of the event source
     *
     * \param paPriority new priority of the event source
     */
    virtual void setPriority(int paPriority) = 0;
    /*!\brief Get the current priority of the event source
     *
     * \return current priority
     */
    virtual int getPriority() const = 0;

    virtual size_t getIdentifier() const = 0;
};

/*@}*/
/*@}*/
#endif /*IEXTEVHAN_H_*/
