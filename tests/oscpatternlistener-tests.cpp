#include "UnitTest++.h"

#include "oscpatternlistener.h"
#include "pattern.h"


TEST(OscPatternListener)
{
  Pattern pattern(4, 16);
  OscPatternListener listener("7770", &pattern);
  lo_address address = lo_address_new(NULL, "7770");

  CHECK_EQUAL(false, pattern.isSet(1, 2));

  int success = lo_send(address, "/pattern/set", "ii", 1, 2);
  CHECK(success != -1);

  usleep(50 * 1000);
  CHECK_EQUAL(true, pattern.isSet(1, 2));

  success = lo_send(address, "/pattern/clear", "ii", 1, 2);
  CHECK(success != -1);

  usleep(50 * 1000);
  CHECK_EQUAL(false, pattern.isSet(1, 2));
}
