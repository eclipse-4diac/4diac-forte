#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"

#include "forte/datatypes/forte_any_int.h"
#include "forte/datatypes/forte_int.h"

using namespace forte::literals;

namespace forte::test {
  // This should now compile because setTINT16 sets mInt16, which getTINT16 reads.
  constinit forte::CIEC_INT::TValueType seventeen = static_cast<forte::CIEC_INT::TValueType>(17_INT);
} // namespace forte::test
