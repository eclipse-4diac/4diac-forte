/*******************************************************************************
 * Copyright (c) 2026 HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Franz Höpfinger - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <boost/test/unit_test.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

#ifdef FORTE_FILEIO_RAMDISK
#include "forte/arch/forte_ramdisk.h"
#include "forte/arch/forte_fileio.h"

namespace forte::arch::test {

  BOOST_AUTO_TEST_SUITE(RAMDISK_function_test)

  BOOST_AUTO_TEST_CASE(ramdisk_load_and_read_test) {
    // Create a temporary file with known content
    const char *testContent = "Line1;Content1\nLine2;Content2\nLine3;Content3\n";
    const char *tempFileName = "/tmp/forte_ramdisk_test_file.txt";

    std::ofstream outFile(tempFileName);
    outFile << testContent;
    outFile.close();

    // Load file into RAMDISK
    int result = forte_ramdisk_load(tempFileName);
    BOOST_CHECK_EQUAL(result, 0);

    // Open via RAMDISK (should use fmemopen internally)
    void *file = forte_fopen(tempFileName, "r");
    BOOST_CHECK(file != nullptr);

    if (file != nullptr) {
      char buffer[256];

      // Read first line
      char *line1 = forte_fgets(buffer, sizeof(buffer), file);
      BOOST_CHECK(line1 != nullptr);
      BOOST_CHECK(strcmp(buffer, "Line1;Content1\n") == 0);

      // Read second line
      char *line2 = forte_fgets(buffer, sizeof(buffer), file);
      BOOST_CHECK(line2 != nullptr);
      BOOST_CHECK(strcmp(buffer, "Line2;Content2\n") == 0);

      // Read third line
      char *line3 = forte_fgets(buffer, sizeof(buffer), file);
      BOOST_CHECK(line3 != nullptr);
      BOOST_CHECK(strcmp(buffer, "Line3;Content3\n") == 0);

      // Close should free the buffer for read mode
      int closeResult = forte_fclose(file);
      BOOST_CHECK_EQUAL(closeResult, 0);
    }

    // Clean up temporary file
    std::remove(tempFileName);
  }

  BOOST_AUTO_TEST_CASE(ramdisk_fallback_to_real_file_test) {
    // Test that files NOT in RAMDISK still work via normal fopen
    const char *testContent = "FallbackTest;Data\n";
    const char *tempFileName = "/tmp/forte_ramdisk_fallback_test.txt";

    std::ofstream outFile(tempFileName);
    outFile << testContent;
    outFile.close();

    // Do NOT load into RAMDISK - should fallback to real file
    void *file = forte_fopen(tempFileName, "r");
    BOOST_CHECK(file != nullptr);

    if (file != nullptr) {
      char buffer[256];
      char *line = forte_fgets(buffer, sizeof(buffer), file);
      BOOST_CHECK(line != nullptr);
      BOOST_CHECK(strcmp(buffer, "FallbackTest;Data\n") == 0);

      int closeResult = forte_fclose(file);
      BOOST_CHECK_EQUAL(closeResult, 0);
    }

    // Clean up temporary file
    std::remove(tempFileName);
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::arch::test

#else // FORTE_FILEIO_RAMDISK not defined
// Provide an empty test suite so the file compiles even without RAMDISK
namespace forte::arch::test {
  BOOST_AUTO_TEST_SUITE(RAMDISK_function_test)
  BOOST_AUTO_TEST_CASE(dummy_test) {
    BOOST_CHECK(true); // Always passes when RAMDISK is not available
  }
  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::arch::test
#endif // FORTE_FILEIO_RAMDISK
