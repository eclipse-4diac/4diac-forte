/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#ifndef SRC_CORE_IO_PROCESSINTERFACE_H_
#define SRC_CORE_IO_PROCESSINTERFACE_H_

#include "mapper/io_mapper.h"
#include "mapper/io_observer.h"

#include <forte_sync.h>
#include "../../stdfblib/io/processinterfacebase.h"

namespace forte {
  namespace core {
    namespace io {

      class ProcessInterface : public CProcessInterfaceBase, public IOObserver {

        public:
          ProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId,
              TForteByte *paFBConnData, TForteByte *paFBVarsData);
          virtual ~ProcessInterface();

          bool onChange();

        protected:
          bool initialise(bool paIsInput);
          bool deinitialise();
          bool readPin() {
            return read(IN_X());
          }
          bool writePin() {
            return write(OUT_X());
          }
          bool readWord() {
            return read(IN_W());
          }
          bool writeWord() {
            return write(OUT_W());
          }
          bool readDWord() {
            return read(IN_D());
          }
          bool writeDWord() {
            return write(OUT_D());
          }

          bool read();
          bool write();

          bool read(CIEC_ANY &);
          bool write(CIEC_ANY &);

          virtual void onHandle(IOHandle *paHandle);
          virtual void dropHandle();

        private:
          bool mIsListening;
          bool mIsReady;
          CSyncObject mSyncMutex;

          static const char * const scmOK;
          static const char * const scmWaitingForHandle;
          static const char * const scmFailedToRegister;
          static const char * const scmMappedWrongDirectionOutput;
          static const char * const scmMappedWrongDirectionInput;
          static const char * const scmMappedWrongDataType;
      };

    } //namespace IO
  } //namepsace core
} //namespace forte

typedef forte::core::io::ProcessInterface CProcessInterface;



#endif /* SRC_CORE_IO_PROCESSINTERFACE_H_ */
