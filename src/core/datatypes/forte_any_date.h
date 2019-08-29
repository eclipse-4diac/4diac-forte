/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN, nxtControl GmbH, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Martin Melik Merkumians, Alois Zoitl,
 *    Monika Wenger, Stansilav Meduna
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _ANY_DAT_H_
#define _ANY_DAT_H_

#include "forte_any_elementary.h"

/*!\ingroup COREDTS IIEC_ANY_DATE represents any date data types according to IEC 61131.
 */
class CIEC_ANY_DATE : public CIEC_ANY_ELEMENTARY{
  public:
    virtual ~CIEC_ANY_DATE(){
    }

    /*! Retrieve the current timezone
     *
     * Can be sed to adjust mktime()-related values
     *
     * @return Number of minutes west (negative) or east (positive), not taking DST into consideration
     */
    static TForteInt32 getTimeZoneOffset();

    /*! set the value of the data according to the given values
     *
     */
    bool setDateAndTime(struct tm &pa_rstTM, unsigned int pa_unMilliSec);

    /*! retrieve the time struct for the current value stored in this Data variable
     */
    struct tm *getTimeStruct() const;

    /*! Get the milliseconds part of the data value
     */
    unsigned int getMilliSeconds() const;

  protected:
    CIEC_ANY_DATE() {
    }

  private:
    static TForteInt32 sm_nTimeZoneOffset;
};

#endif /*_MANY_DAT_H_*/
