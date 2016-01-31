/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef OPENSCADASERVERHANDLER_H_
#define OPENSCADASERVERHANDLER_H_

#include <extevhan.h>
#include <singlet.h>
#include <thread.h>
#include <comlayer.h>

#include <eclipsescada/sfp_server.h>



struct TOpenSCADAItem{
    struct sfp_item *m_pstItem;
    struct sfp_variant m_stValueBuf;  //FIXME check if the sfp_item's variant could directly be used
};


class CEclipseSCADASFPHandler : public CExternalEventHandler, public CThread{
  DECLARE_SINGLETON(CEclipseSCADASFPHandler)
    ;

  public:

    struct sfp_item *registerDataPoint(const char *paDataPointId, const char* paDataPointComment);
    void unregisterDataPoint(struct sfp_item *patItem);

    void registerWriteCallBack(struct sfp_item *patItem, forte::com_infra::CComLayer *paLayer);

    static void updateDataPoint(struct sfp_item *patItem, const CIEC_ANY &paDataPoint);
    static bool readBackDataPoint(const struct sfp_variant *paValue, CIEC_ANY &paDataPoint);

    /* functions needed for the external event handler interface */
    void enableHandler(void);
    void disableHandler(void);
    void setPriority(int pa_nPriority);
    int getPriority(void) const;

  protected:

  private:
    static void handleCellWrite(struct sfp_item * pa_pstItem, struct sfp_variant *pa_pstValue, int32_t pa_nOperationID,
        struct sfp_strategy * pa_pstStrategy,
        void (*handle_result)(struct sfp_strategy* strategy, int32_t operation_id, struct sfp_error_information* error),
        void * pa_pvCtx);

    static const uint16_t scm_unServerPort = 9999;

    virtual void run();

    struct sfp_registry *mItemRegistry;
    struct sfp_server *mEclipseSCADAServer;

};

#endif /* OPENSCADASERVERHANDLER_H_ */
