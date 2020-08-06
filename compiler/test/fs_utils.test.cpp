#define BOOST_TEST_MODULE FSUtilsTests

#include <boost/test/included/unit_test.hpp>
#include <filesystem>

#include "fs_utils.hpp"

BOOST_AUTO_TEST_SUITE(FSUtilsTestSuite)

BOOST_AUTO_TEST_CASE(getJackFilesInDirectory_tests) {
  std::vector<fs::path> jackFiles{"test/Main.jack"};

  BOOST_TEST(getJackFilesInDirectory("test/") == jackFiles);
}

BOOST_AUTO_TEST_SUITE_END()