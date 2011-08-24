#define BOOST_TEST_MODULE TestDataObject
#include <boost/test/unit_test.hpp>
#include "src/dataobject.h"

#define TEST_EQUAL(a,b) BOOST_CHECK(a==b)

BOOST_AUTO_TEST_CASE (ReadWrite) {
  DataPointerFlags dp = create_data();
  data_set(dp, 10, 4);
  TEST_EQUAL (data_get (dp, 10), 4);
  data_set(dp, 100, 32);
  TEST_EQUAL (data_get(dp, 105), 0);
  TEST_EQUAL (data_get(dp, 100), 32);
  TEST_EQUAL (data_get(dp, 10), 4);
}

BOOST_AUTO_TEST_CASE (DataObjectData) {
  DataPointerFlags d1 = create_data();
  DataPointerFlags d2 = create_data();
  data_set_data(d1, 4, d2);
  data_set_data_r(d1, 8, d2);
  BOOST_CHECK (data_get_data(d1, 4) == d2);
}