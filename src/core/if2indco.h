/*******************************************************************************
 * Copyright (c) 2006 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Gunnar Grabmaier, Alois Zoitl, Thomas Strasser, Ingo Hegny, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _IF2INDCO_H_
#define _IF2INDCO_H_

#include "dataconn.h"

/*! \ingroup CORE\brief class for handling a interface to internal data connection.
 */

class CFunctionBlock;

class CInterface2InternalDataConnection : public CDataConnection{
  public:
    CInterface2InternalDataConnection();
    virtual ~CInterface2InternalDataConnection();

    /*! \brief Clones interface data value (CFB data input) to internal FB data input.
     */
    void cloneInputInterfaceValue(void);

    void setSource(CFunctionBlock *paSrcFB, TPortId paSrcPortId);

  protected:

  private:

};

typedef CInterface2InternalDataConnection *TCInterface2InternalDataConnectionPtr;

#endif /*IF2INDCO_H_*/
