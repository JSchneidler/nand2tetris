#define BOOST_TEST_MODULE FSUtilsTests

#include <boost/test/included/unit_test.hpp>
#include <filesystem>

#include "fs_utils.hpp"

BOOST_AUTO_TEST_SUITE(FSUtilsTestSuite)

BOOST_AUTO_TEST_CASE(getJackFilesInDirectory_tests)
{
  FSUtils::JackFiles expectedJackFiles{"test/Main.jack", "test/TokenizerTest.jack"};

  BOOST_TEST(FSUtils::getJackFilesInDirectory("test/") == expectedJackFiles);
}

BOOST_AUTO_TEST_SUITE_END()