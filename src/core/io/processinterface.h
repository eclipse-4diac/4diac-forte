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

#include <string>

namespace forte {
  namespace core {
    namespace io {

      class ProcessInterface : public CProcessInterfaceBase, public IOObserver {

        public:
          ProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId);
          ~ProcessInterface() override;

          bool onChange() override;

        protected:
          bool initialise(bool paIsInput);
          bool deinitialise();
          bool readPin() {
            return read(IN_X());
          }
          bool writePin() {
            return write(OUT_X());
          }
          bool readByte() {
            return read(IN_B());
          }
          bool writeByte() {
            return write(OUT_B());
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
          bool readLWord() {
            return read(IN_L());
          }
          bool writeLWord() {
            return write(OUT_L());
          }

          bool read();
          bool write();

          bool read(CIEC_ANY &);
          bool write(CIEC_ANY &);

          void onHandle(IOHandle *paHandle) override;
          void dropHandle() override;

        private:
          bool mIsListening;
          bool mIsReady;
          CSyncObject mSyncMutex;

          static const CIEC_STRING scmOK;
          static const CIEC_STRING scmWaitingForHandle;
          static const CIEC_STRING scmFailedToRegister;
          static const CIEC_STRING scmMappedWrongDirectionOutput;
          static const CIEC_STRING scmMappedWrongDirectionInput;
          static const CIEC_STRING scmMappedWrongDataType;
      };

    } //namespace IO
  } //namepsace core
} //namespace forte

typedef forte::core::io::ProcessInterface CProcessInterface;



#endif /* SRC_CORE_IO_PROCESSINTERFACE_H_ */
