/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_STDFBLIB_ITA_FORTEBOOTFILELOADER_H_
#define SRC_STDFBLIB_ITA_FORTEBOOTFILELOADER_H_

#include <stdio.h>
#include <stdlib.h>

class CIEC_STRING;
class IBootFileCallback;

enum LoadBootResult {
  LOAD_RESULT_OK,
  MISSING_COLON,
  FILE_NOT_OPENED,
  EXTERNAL_ERROR,
};

class ForteBootFileLoader {
  public:

    /**
     * Constructor which uses the the default values for the boot file location
     * @param paCallback Object to be called for each command
     */
    explicit ForteBootFileLoader(IBootFileCallback &paCallback);

    ~ForteBootFileLoader();

    LoadBootResult loadBootFile();

    bool isOpen() const {
      return (0 != mBootfile);
    }

    bool needsExit() const {
      return mNeedsExit;
    }

  private:
    FILE *mBootfile;
    IBootFileCallback &mCallback; //for now with one callback is enough for all cases
    bool mNeedsExit;

    bool openBootFile();
    bool readLine(CIEC_STRING &line);
    bool hasCommandEnded(const CIEC_STRING &line) const;
};

#endif /* SRC_STDFBLIB_ITA_FORTEBOOTFILELOADER_H_ */
